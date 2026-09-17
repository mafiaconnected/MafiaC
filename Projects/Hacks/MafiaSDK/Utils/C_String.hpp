/*----------------------------------------------------------------\
Copyright (c) Lonely Cat Games. All rights reserved.
Abstract:
 C_String class - reference-counting string class, minimizing memory copying
 as possible, keeping reference to other string after copying.
 Safe access - making its own copy on non-constant access.
 Keeping valid NULL-terminated "C" string even if not initialized.
 Allowing wild-card compare with other string.

 This is the LS3D engine's own internal string type (portable C++ reimplementation,
 not a set of address-calling stubs) - some engine APIs may expect/return one of these
 rather than a plain `const char*`. Named C_String (not C_string) to avoid colliding
 with Galactic's own CString on case-insensitive filesystems/IDEs.
 Ported from reMafia's C_str.hpp (same author, MafiaOrbitCam/Vendors/reMafia).
|\----------------------------------------------------------------*/

#ifndef _MAFIASDK_C_STRING_H_
#define _MAFIASDK_C_STRING_H_

#include <ctype.h>
#include <string.h>
#include <cassert>

namespace MafiaSDK
{
    class C_String
    {
        // String base - concrete string data. This class' size is variable, depending on
        // the size of the string it contains.
#pragma pack(push, 1)
        class C_rep
        {
            uint32_t ref;
            uint32_t size; // size without terminating null character
            char data[1];

        public:
            inline C_rep(const void* cp, size_t len)
                : ref(1), size((uint32_t)len)
            {
                memcpy(data, cp, len);
                *(uint8_t*)(data + len) = 0;
            }

            inline C_rep(size_t len) : ref(1), size((uint32_t)len) { }

            inline C_rep(const void* cp1, const void* cp2, size_t l1, size_t l2)
                : ref(1), size((uint32_t)(l1 + l2))
            {
                memcpy(data, cp1, l1);
                memcpy(data + l1, cp2, l2);
                *(uint8_t*)(data + size) = 0;
            }

            void DeleteThis()
            {
                this->~C_rep();
                delete[](uint8_t*) this;
            }

            inline void AddRef() { ++ref; }
            inline uint32_t Release() { return --ref; }
            inline uint32_t Count() const { return ref; }
            inline char* GetData() { return data; }
            inline const char* GetData() const { return data; }
            inline size_t Size() const { return size; }
        };
#pragma pack(pop)

        C_rep* rep;

        inline void* AllocRep(size_t sz) const { return new uint8_t[sizeof(C_rep) + sz]; }

        inline void ReleaseRep()
        {
            if (rep && !rep->Release())
                rep->DeleteThis();
        }

        inline void ReplaceRep(C_rep* r)
        {
            ReleaseRep();
            rep = r;
        }

        // Make sure this string is the only owner of its actual string data.
        void MakeUniqueCopy()
        {
            if (rep->Count() != 1)
            {
                C_rep* new_rep = new(AllocRep(rep->Size())) C_rep(rep->GetData(), rep->Size());
                ReplaceRep(new_rep);
            }
        }

    public:
        inline C_String() : rep(NULL) { }

        inline C_String(const char* cp)
        {
            if (!cp)
                rep = NULL;
            else
            {
                size_t len = strlen(cp);
                rep = new(AllocRep(len)) C_rep(cp, len);
            }
        }

        inline C_String(const C_String& s)
        {
            rep = s.rep;
            if (rep) rep->AddRef();
        }

        inline ~C_String() { ReleaseRep(); }

        // Pointer to a C string - always a valid NULL-terminated string, even if empty.
        // Valid until any other operation is done on the contents of this string.
        inline operator const char* () const
        {
            if (!rep) return (const char*)&rep;
            return rep->GetData();
        }

        C_String& operator=(const C_String& s)
        {
            if (s.rep) s.rep->AddRef();
            ReplaceRep(s.rep);
            return (*this);
        }

        C_String& operator=(const char* cp)
        {
            C_rep* new_rep;
            if (!cp)
                new_rep = NULL;
            else
            {
                size_t len = strlen(cp);
                new_rep = new(AllocRep(len)) C_rep(cp, len);
            }
            ReplaceRep(new_rep);
            return (*this);
        }

        // Assign raw data to the string (no terminating '\0' is searched for in `cp`).
        C_String& Assign(const char* cp, uint32_t size)
        {
            C_rep* new_rep;
            if (!cp)
                new_rep = new(AllocRep(size)) C_rep(size);
            else
                new_rep = new(AllocRep(size)) C_rep(cp, size);
            ReplaceRep(new_rep);
            return (*this);
        }

        C_String& operator+=(const char* cp)
        {
            if (!rep) return operator=(cp);
            size_t len = rep->Size(), len1 = strlen(cp);
            C_rep* new_rep = new(AllocRep(len + len1)) C_rep(rep->GetData(), cp, len, len1);
            ReplaceRep(new_rep);
            return (*this);
        }

        C_String& operator+=(const C_String& s)
        {
            if (!rep) return operator=(s);
            assert(s.rep);
            size_t len = Size(), len1 = s.Size();
            C_rep* new_rep = new(AllocRep(len + len1)) C_rep(rep->GetData(), s.rep->GetData(), len, len1);
            ReplaceRep(new_rep);
            return (*this);
        }

        C_String operator+(const C_String& s) const
        {
            if (!rep) return s;
            size_t l1 = Size(), l2 = s.Size();
            if (!l2) return *this;
            C_String ret;
            ret.rep = new(ret.AllocRep(l1 + l2)) C_rep(rep->GetData(), s.rep->GetData(), l1, l2);
            return ret;
        }

        C_String operator+(const char* cp) const
        {
            if (!rep) return cp;
            size_t l1 = Size(), l2 = strlen(cp);
            C_String ret;
            ret.rep = new(ret.AllocRep(l1 + l2)) C_rep(rep->GetData(), cp, l1, l2);
            return ret;
        }

        inline const char& operator[](uint32_t pos) const
        {
            assert(pos < (Size() + 1));
            if (!rep) return *(char*)&rep;
            return rep->GetData()[pos];
        }

        inline const char& operator[](int pos) const { return operator[]((uint32_t)pos); }

        // If contents of the string are shared among multiple strings, it is made unique first.
        char& operator[](uint32_t pos)
        {
            assert(pos < (Size() + 1));
            if (!rep) return *(char*)&rep;
            MakeUniqueCopy();
            return rep->GetData()[pos];
        }

        char& operator[](int pos) { return operator[]((uint32_t)pos); }

        inline bool operator==(const char* cp) const { return !cp ? (!Size()) : !strcmp(*this, cp); }
        inline bool operator!=(const char* cp) const { return !cp ? Size() != 0 : strcmp(*this, cp) != 0; }

        bool operator==(const C_String& s) const
        {
            if (rep == s.rep) return true;
            if (Size() != s.Size()) return false;
            return !memcmp((const char*)*this, (const char*)s, Size());
        }

        inline bool operator!=(const C_String& s) const { return (!operator==(s)); }
        inline bool operator<(const char* s) const { return (strcmp(*this, s) < 0); }

        // Size of the string's data, without the terminating NULL character.
        inline uint32_t Size() const
        {
            if (!rep) return 0;
            return (uint32_t)rep->Size();
        }

        // Wild-card compare: '*' ignores the rest of the string, '?' ignores one character.
        bool Match(const C_String& s) const
        {
            for (const char* cp1 = operator const char* (), *cp2 = s;; ++cp1, ++cp2)
            {
                switch (*cp2)
                {
                case 0: return !(*cp1);
                case '*': return true;
                case '?':
                    if (!*cp1) return false;
                    break;
                default:
                    if (*cp1 != *cp2) return false;
                }
            }
        }

        // Same as Match(), but case-insensitive.
        bool Matchi(const C_String& s) const
        {
            for (const char* cp1 = operator const char* (), *cp2 = s;; ++cp1, ++cp2)
            {
                switch (*cp2)
                {
                case 0: return !(*cp1);
                case '*': return true;
                case '?':
                    if (!*cp1) return false;
                    break;
                default:
                    if (tolower(*cp1) != tolower(*cp2)) return false;
                }
            }
        }

        void ToLower()
        {
            uint32_t l = Size();
            if (!l) return;
            MakeUniqueCopy();
            char* buf = rep->GetData();
            while (l--) buf[l] = (char)tolower(buf[l]);
        }

        void ToUpper()
        {
            uint32_t l = Size();
            if (!l) return;
            MakeUniqueCopy();
            char* buf = rep->GetData();
            while (l--) buf[l] = (char)toupper(buf[l]);
        }
    };
};

#endif
