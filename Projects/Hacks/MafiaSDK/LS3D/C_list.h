//----------------------------
// Multi-platform mobile library
// (c) Lonely Cat Games
//----------------------------

#ifndef __C_LIST_HPP
#define __C_LIST_HPP

#include "common.h"
#include <cmath>
#include <cstdlib>
#include <assert.h>
#include <memory.h>

template <class T>
class C_list {
    enum { VECTOR_SIZE_GROW = 512 };
    uint8_t* array;
    uint32_t res_size, used_size, grow_size;

    void _Construct(void* ptr) {
        new (ptr) T;
    }
    
    void _Destruct(void* ptr) {
        ((T*)ptr)->~T();
    }
    
    void _Copy(void* dst, const void* src) {
        *(T*)dst = *(T*)src;
    }

    //----------------------------

    void _Clear() {
        for (int i = used_size; i--;) _Destruct(array + i * sizeof(T));
        used_size = 0;
        _Reserve(res_size);
    }

    //----------------------------

    void _CopyVector(const C_list& v) {
        if (this == &v)
            return;

        _Clear();
        
        int i = v.used_size;
        _Reserve(i);
        used_size = i;
        while (i--) {
            int offs = i * sizeof(T);
            _Construct(array + offs);
            _Copy(array + offs, v.array + offs);
        }
    }

    //----------------------------

    void* _End() {
        return array + used_size * sizeof(T);
    }

    //----------------------------

    void _Insert(uint32_t dst_i, const void* src, uint32_t num) {
        if (!num)
            return;
        assert(dst_i <= (uint32_t)used_size);
        _Reserve(used_size + num);

        used_size += num;
        for (int i = used_size - num - dst_i; i--;) 
            memcpy(_At(dst_i + num + i), _At(dst_i + i));

        uint8_t* dst = (uint8_t*)_At(dst_i);
        for (uint32_t i = 0; i < num; i++, dst += sizeof(T), src = (uint8_t*)src + sizeof(T)) {
            _Construct(dst);
            _Copy(dst, src);
        }
    }

    //----------------------------

    void _PushBack(const void* val) {
        if (used_size == res_size) {
            res_size       = max(1ul, res_size + grow_size);
            uint8_t* new_a = new uint8_t[res_size * sizeof(T)];
            memcpy(new_a, array, used_size * sizeof(T));
            delete[] array;
            array = new_a;
        }

        void* ptr = _End();
        _Construct(ptr);
        _Copy(ptr, val);
        ++used_size;

    }
    //----------------------------

    void _PopBack() {
        assert(used_size);
        --used_size;
        _Destruct(_End());

    }

    //----------------------------

    void _Reserve(uint32_t s) {
        s = max(s, max(res_size, used_size));
        if (res_size != s) {
            res_size       = s;
            uint8_t* new_a = new uint8_t[res_size * sizeof(T)];
            memcpy(new_a, array, used_size * sizeof(T));
            delete[] array;
            array = new_a;
        }
    }

    //----------------------------

    void* _At(uint32_t i) {
        assert(i <= used_size);
        return array + i * sizeof(T);
    }
    
    //----------------------------

    void _Erase(uint32_t indx, uint32_t num) {
        if (!num)
            return;
        assert(indx + num <= (uint32_t)used_size);
        uint8_t *bp = (uint8_t*)_At(indx), *base = bp;

        for (uint32_t i = 0; i < num; i++, bp += sizeof(T)) _Destruct(bp);

        memcpy(base, bp, (used_size - indx - num) * sizeof(T));
        used_size -= num;
    }

    //----------------------------

    void _Resize(uint32_t n, const void* val) {
        if (n < used_size) {
            uint8_t* ptr = (uint8_t*)_End();
            for (int i = used_size - n; i--;) {
                ptr -= sizeof(T);
                _Destruct(ptr);
            }
            used_size = n;
        } else {
            _Reserve(n);
            while (used_size < n) _PushBack(val);
        }
    }

    //----------------------------

public:
    inline C_list() 
        : res_size(0)
        , used_size(0)
        , grow_size(0)
        , array(NULL) {
        //assert(grow_size);
        grow_size = VECTOR_SIZE_GROW / sizeof(T);
    }

    inline C_list(int n, const T& val = T()) {
        resize(n, val);
    }

    inline C_list(const C_list<T>& v) {
        operator =(v);
    }

    inline ~C_list() {
        _Clear();
        delete[] array;
    }

    inline T* data() {
        return (T*)array; 
    }

    inline C_list<T>& operator=(const C_list<T>& v) {
        _CopyVector(v);
        return *this;
    }
    
    // iterators
    typedef T* iterator;
    typedef const T* const_iterator;

    inline iterator begin() {
        return (T*)array;
    }
    
    inline const_iterator begin() const {
        return (const T*)array;
    }
    
    inline iterator end() {
        return (iterator)_End();
    }

    inline const_iterator end() const {
        return (const_iterator) const_cast<C_list<T>*>(this)->_End();
    }

    inline T& operator[](int i) {
        return *(T*)_At(i);
    }
    inline const T& operator[](int i) const {
        return *(const T*)const_cast<C_list<T>*>(this)->_At(i);
    }

    inline T& front() {
        return *(T*)array;
    }

    inline const T& front() const {
        return *(const T*)array;
    }

    inline T& back() {
        return end()[-1];
    }
    
    inline const T& back() const {
        return end()[-1];
    }

    inline int capacity() {
        return res_size;
    }
    
    inline void clear() {
        _Clear();
    }
    
    inline bool empty() const {
        return !used_size;
    }

    inline void erase(iterator first, iterator last) {
        _Erase(first - begin(), last - first, sizeof(T));
    }

    inline void erase(iterator pos) {
        _Erase(pos - begin(), 1);
    }

    inline void insert(iterator pos, const T& val) {
        _Insert(pos - begin(), &val, 1);
    }

    inline void insert(iterator pos, const_iterator first, const_iterator last) {
        _Insert(pos - begin(), first, last - first, sizeof(T));
    }

    // int max_size() const{ return -1; }
    
    inline T& push_back(const T& val) {
        _PushBack(&val);
        return back();
    }
    
    inline void pop_back() {
        _PopBack();
    }

    inline T& push_front(const T& val) {
        _Insert(0, &val, 1);
        return front();
    }

    inline void pop_front() {
        erase(begin());
    }

    inline void reserve(int s) {
        _Reserve(s);
    }

    void resize(int n, const T& val = T()) {
        _Resize(n, &val);
    }

    void assign(int n, const T& val = T()) {
        clear();
        resize(n, val);
    }

    void assign(const_iterator first, const_iterator last) {
        clear();
        _Insert(0, first, last - first);
    }

    inline uint32_t size() const {
        return used_size;
    }

    inline void remove_index(uint32_t i) {
        _Erase(i, 1);
    }

    inline T& insert_index(uint32_t i, const T& val = T()) {
        _Insert(i, &val, 1);
        return operator[](i);
    }
};
////----------------------------

#endif