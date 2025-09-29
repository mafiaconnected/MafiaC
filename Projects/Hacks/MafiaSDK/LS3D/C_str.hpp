#ifndef __C_string_HPP
#define __C_string_HPP
#pragma once

/*----------------------------------------------------------------\
Copyright (c) Lonely Cat Games  All rights reserved.
Abstract:
 C_string class - reference-counting string class, minimizing memory copying
 as possible, keeping reference to other string after copying.
 Safe access - making its own copy on non-constant access.
 Keeping valid NULL-terminated "C" string even if not initialized.
 Allowing wild-card compare with other string.
 Support for single-uint8_t and double-uint8_t contents.
 Usable for shared memory data.

 C_fstr - inherited from C_string, having additional constructor for
 creating string from formatted string and variable-length arguments.

Revision 17 feb 2003 by Michal : added C_xstr (type-safe formatted string, replacement for C_fstr).

|\----------------------------------------------------------------*/
#include <ctype.h>
#include <exception> //can't use C_except, because it uses C_string
#include <new.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include "common.h"

#define CFSTRING_MAX_FMT_BUFFER 1024 // size of formatting buffer, in characters

#pragma warning(disable : 4996)
#pragma warning(disable : 4244) // int to float
#pragma warning(disable : 4800) // int to bool
#pragma warning(disable : 4838) // narrowing conversion

//----------------------------
// String - keeping reference to string data.
class C_string {

//----------------------------
// String base - concrete string data. This class' size is
// variable, depending on size of string it contains.
#pragma pack(push, 1)
    class C_rep {
        uint32_t ref;
		uint32_t size; // size without terminating null character
        char data[1];

    public:
        inline C_rep(const void *cp, size_t len)
        : ref(1), size(len) {
            memcpy(data, cp, len);
            *(uint8_t*)(data + len) = 0;
        }

        inline C_rep(size_t len) : ref(1), size(len) {
        }

        inline C_rep(const void *cp1, const void *cp2, size_t l1, size_t l2)
        : ref(1), size(l1 + l2)  {
            memcpy(data, cp1, l1);
            memcpy(data + l1, cp2, l2);
            *(uint8_t *)(data + size) = 0;
        }

        void DeleteThis() {
            this->~C_rep();
            delete[](uint8_t *) this;
        }

        inline void AddRef() {
            ++ref;
        }

        inline uint32_t Release() {
            return --ref;
        }

        inline uint32_t Count() const {
            return ref;
        }

        inline char *GetData() {
            return data;
        }

        inline const char *GetData() const {
            return data;
        }

        inline size_t Size() const {
            return size;
        }
    };
#pragma pack(pop)

    C_rep *rep;

    inline void *AllocRep(size_t sz) const {
        return new uint8_t[sizeof(C_rep) + sz];
    }
    inline void ReleaseRep() {
        if(rep && !rep->Release()) {
            // virtual deletion function of rep is called, so it is freed in those module,
            //  which allocated the string
            rep->DeleteThis();
        }
    }
    inline void ReplaceRep(C_rep *r) {
        ReleaseRep();
        rep = r;
    }

    //----------------------------
    // Make sure this string is the only owner of actual string data.
    void MakeUniqueCopy() {
        if(rep->Count() != 1) {
            // use placement new for proper allocation and initialization of string data.
            C_rep *new_rep = new(AllocRep(rep->Size()))
            C_rep(rep->GetData(), rep->Size());
            ReplaceRep(new_rep);
        }
    }
 
    public:
    //----------------------------
    // Empty constructor.
    inline C_string() : rep(NULL) {
    }

    //----------------------------
    // Constructor from C string.
    inline C_string(const char *cp) {
        if(!cp)
            rep = NULL;
        else {
            size_t len = strlen(cp);
            rep        = new(AllocRep(len)) C_rep(cp, len);
        }
    }
    //----------------------------
    // Copy constructor.
    inline C_string(const C_string &s) {
        rep = s.rep;
        if(rep) rep->AddRef();
    }

    //----------------------------
    // Destructor.
    inline ~C_string() {
        ReleaseRep();
    }

    //----------------------------
    // Getting pointer to C string. This is always valid NULL-terminated string,
    // even if the string is empty. It is valid until any other operation is
    // done on contents of string.
    inline operator const char *() const {
        if(!rep) return (const char *)&rep;
        return rep->GetData();
    }

    //----------------------------
    // Assignment operator - fast copy of reference from given string.
    C_string &operator=(const C_string &s) {
        if(s.rep) s.rep->AddRef();
        ReplaceRep(s.rep);
        return (*this);
    }

    //----------------------------
    // Assignment from C string.
    C_string &operator=(const char *cp) {
        C_rep *new_rep;
        if(!cp)
            new_rep = NULL;
        else {
            size_t len = strlen(cp);
            new_rep    = new((AllocRep(len))) C_rep(cp, len);
        }
        ReplaceRep(new_rep);
        return (*this);
    }

    //----------------------------
    // Assign raw data to string. Data may be anything, no '\0' character at the end is searched.
    // If 'cp' is NULL, the string allocates uninitialized buffer for holding the string,
    // which may be filled further by array-access functions.
    C_string &Assign(const char *cp, uint32_t size) {
        C_rep *new_rep;
        if(!cp)
            new_rep = new((AllocRep(size))) C_rep(size);
        else
            new_rep = new((AllocRep(size))) C_rep(cp, size);
        ReplaceRep(new_rep);
        return (*this);
    }

    //----------------------------
    // Concanetate string with another C string and store m_Result in this string.
    C_string &operator+=(const char *cp) {
        if(!rep) return operator=(cp);
        size_t len = rep->Size(), len1 = strlen(cp);
        C_rep *new_rep =
        new((AllocRep(len + len1))) C_rep(rep->GetData(), cp, len, len1);
        ReplaceRep(new_rep);
        return (*this);
    }

    //----------------------------
    // Concanetate string with another string and store m_Result in this string.
    C_string &operator+=(const C_string &s) {
        if(!rep) return operator=(s);
        assert(!s.rep);
        size_t len = Size(), len1 = s.Size();
        if(IsWide()) {
            len *= 2;
            len1 *= 2;
        }
        C_rep *new_rep = new((AllocRep(len + len1)))
        C_rep(rep->GetData(), s.rep->GetData(), len, len1);
        ReplaceRep(new_rep);
        return (*this);
    }

    //----------------------------
    // Add this string with another string and return m_Result as a string.
    C_string operator+(const C_string &s) const {
        if(!rep) return s;
        assert(!s.rep);
        size_t l1 = Size(), l2 = s.Size();
        if(!l2) return *this;
        C_string ret;
        ret.rep = new((AllocRep(l1 + l2)))
        C_rep(rep->GetData(), s.rep->GetData(), l1, l2);
        return ret;
    }

    //----------------------------
    // Add this string with another C string and return m_Result as a string.
    C_string operator+(const char *cp) const {
        if(!rep) return cp;
        size_t l1 = Size(), l2 = strlen(cp);
        C_string ret;
        ret.rep = new((AllocRep(l1 + l2))) C_rep(rep->GetData(), cp, l1, l2);
        return ret;
    }
   
    //----------------------------
    // Get const character reference inside of string on particular position.
    inline const char &operator[](uint32_t pos) const {
        assert(pos < (Size() + 1));
        if(!rep) return *(char *)&rep;
        return rep->GetData()[pos];
    }

    inline const char &operator[](int pos) const {
        return operator[](uint32_t(pos));
    }
   
    //----------------------------
    // Get non-const character reference inside of string on particular position.
    // If contents of string is shared among multiple strings, it is made unique.
    char &operator[](uint32_t pos) {
        assert(pos < (Size() + 1));
        if(!rep) return *(char *)&rep;
        MakeUniqueCopy(); 
        return rep->GetData()[pos];
    }
    char &operator[](int pos) {
        return operator[](uint32_t(pos));
    }
    
    //----------------------------
    // Compare this string with another C string for equality. Slow, depends on sizes of both strings.
    inline bool operator==(const char *cp) const {
        return !cp ? (!Size()) : !strcmp(*this, cp);
    }
    inline bool operator!=(const char *cp) const {
        return !cp ? Size() : strcmp(*this, cp);
    }

    //----------------------------
    // Compare this string with another string for equality. Optimizations based on sizes of strings.
    bool operator==(const C_string &s) const {
        if(rep == s.rep) return true;
        if(Size() != s.Size()) return false;
        return !memcmp((const char *)*this, (const char *)s, Size());
    }
    inline bool operator!=(const C_string &s) const {
        return (!operator==(s));
    }

    //----------------------------
    // Compare this string with another C strings.
    inline bool operator<(const char *s) const {
        return (strcmp(*this, s) < 0);
    }
    
    //----------------------------
    // Get size of string data, without terminating NULL character.
    inline uint32_t Size() const {
        if(!rep) return 0;
        size_t sz = rep->Size();
        return (uint32_t)sz;
    }

    //----------------------------
    // Compare this string with another wild-char string. Following wildcards are recognized:
    //    * - ignore the contents of string to the end
    //    ? - ignore character on current position
    bool Match(const C_string &s) const {
        // uint32_t l1 = Size(), l2 = s.Size();
        // if(!l1 || !l2) return (l1==l2);
        for(const char *cp1 = operator const char *(), *cp2 = s;; ++cp1, ++cp2) {
            switch(*cp2) {
            case 0: return !(*cp1);
            case '*': return true;
            case '?':
                if(!*cp1) return false;
                break;
            default:
                if(*cp1 != *cp2) return false;
            }
        }
    }

    //----------------------------
    // Wildcard compare, but ignore case.
    bool Matchi(const C_string &s) const {
        // uint32_t l1 = Size(), l2 = s.Size();
        // if(!l1 || !l2) return (l1==l2);
        for(const char *cp1 = operator const char *(), *cp2 = s;; ++cp1, ++cp2) {
            switch(*cp2) {
            case 0: return !(*cp1);
            case '*': return true;
            case '?':
                if(!*cp1) return false;
                break;
            default:
                if(tolower(*cp1) != tolower(*cp2)) return false;
            }
        }
    }

    //----------------------------
    // Check if string is in wide-char format.
    bool IsWide() const {
        return false;
    }

    //----------------------------
    // Convert all characters to lower case.
    void ToLower() {
        uint32_t l = Size();
        if(!l) return;
        MakeUniqueCopy();
        char *buf = rep->GetData();
        while(l--) buf[l] = (char)tolower(buf[l]);
    }

    //----------------------------
    // Convert all characters to upper case.
    void ToUpper() {
        uint32_t l = Size();
        if(!l) return;
        MakeUniqueCopy();
        char *buf = rep->GetData();
        while(l--) buf[l] = (char)toupper(buf[l]);
    }
};

#endif