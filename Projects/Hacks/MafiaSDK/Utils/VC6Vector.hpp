/*
    Copyright (c) 1995 by P.J. Plauger. ALL RIGHTS RESERVED.
    Derived from software copyright (c) 1994 Hewlett-Packard Company.
    Consult your license regarding permissions and restrictions.

    Memory-layout-compatible reimplementation of the VC6 <vector> that Mafia (2002) was
    compiled with: { _A allocator; iterator _First, _Last, _End; }. The modern MSVC
    std::vector has a different internal layout, so any game struct holding a real
    std::vector member (C_mission::m_vActors, C_program::m_vInstructions, etc.) needs this
    type instead to read/write it correctly. Ported from reMafia's vector.h (same author,
    MafiaOrbitCam/Vendors/reMafia), which itself carries this same upstream attribution.
*/

#ifndef _VC6_VECTOR_H_
#define _VC6_VECTOR_H_

#include <climits>
#include <memory>
#include <stdexcept>
#include <xutility>

namespace MafiaSDK
{
#pragma pack(push, 8)
#pragma warning(push)
#pragma warning(disable : 4018)
#pragma warning(disable : 4996)

    template<class _Ty, class _A = std::allocator<_Ty> >
    class vc6_vector
    {
    public:
        typedef vc6_vector<_Ty, _A> _Myt;
        typedef _A allocator_type;
        typedef typename _A::size_type size_type;
        typedef typename _A::difference_type difference_type;
        typedef typename _A::pointer _Tptr;
        typedef typename _A::const_pointer _Ctptr;
        typedef typename _A::reference reference;
        typedef typename _A::const_reference const_reference;
        typedef typename _A::value_type value_type;
        typedef _Tptr iterator;
        typedef _Ctptr const_iterator;
        explicit vc6_vector(const _A& _Al = _A())
            : allocator(_Al), _First(0), _Last(0), _End(0) {}
        explicit vc6_vector(size_type _N, const _Ty& _V = _Ty(),
            const _A& _Al = _A())
            : allocator(_Al)
            {_First = allocator.allocate(_N, (void *)0);
            _Ufill(_First, _N, _V);
            _Last = _First + _N;
            _End = _Last; }
        vc6_vector(const _Myt& _X)
            : allocator(_X.allocator)
            {_First = allocator.allocate(_X.size(), (void *)0);
            _Last = _Ucopy(_X.begin(), _X.end(), _First);
            _End = _Last; }
        typedef const_iterator _It;
        vc6_vector(_It _F, _It _L, const _A& _Al = _A())
            : allocator(_Al), _First(0), _Last(0), _End(0)
            {insert(begin(), _F, _L); }
        ~vc6_vector()
            {
                _Destroy(_First, _Last);
                allocator.deallocate(_First, _End - _First);
                _First = 0, _Last = 0, _End = 0;
            }
        _Myt& operator=(const _Myt& _X)
            {if (this == &_X)
                ;
            else if (_X.size() <= size())
                {iterator _S = std::copy(_X.begin(), _X.end(), _First);
                _Destroy(_S, _Last);
                _Last = _First + _X.size(); }
            else if (_X.size() <= capacity())
                {const_iterator _S = _X.begin() + size();
                std::copy(_X.begin(), _S, _First);
                _Ucopy(_S, _X.end(), _Last);
                _Last = _First + _X.size(); }
            else
                {_Destroy(_First, _Last);
                allocator.deallocate(_First, _End - _First);
                _First = allocator.allocate(_X.size(), (void *)0);
                _Last = _Ucopy(_X.begin(), _X.end(),
                    _First);
                _End = _Last; }
            return (*this); }
        void reserve(size_type _N)
            {if (capacity() < _N)
                {iterator _S = allocator.allocate(_N, (void *)0);
                _Ucopy(_First, _Last, _S);
                _Destroy(_First, _Last);
                allocator.deallocate(_First, _End - _First);
                _End = _S + _N;
                _Last = _S + size();
                _First = _S; }}
        size_type capacity() const
            {return (_First == 0 ? 0 : _End - _First); }
        iterator begin()
            {return (_First); }
        const_iterator begin() const
            {return ((const_iterator)_First); }
        iterator end()
            {return (_Last); }
        const_iterator end() const
            {return ((const_iterator)_Last); }
        void resize(size_type _N, const _Ty& _X = _Ty())
            {if (size() < _N)
                insert(end(), _N - size(), _X);
            else if (_N < size())
                erase(begin() + _N, end()); }
        size_type size() const
            {return (_First == 0 ? 0 : _Last - _First); }
        size_type max_size() const
            {return (allocator.max_size()); }
        bool empty() const
            {return (size() == 0); }
        _A get_allocator() const
            {return (allocator); }
        const_reference at(size_type _P) const
            {if (size() <= _P)
                _Xran();
            return (*(begin() + _P)); }
        reference at(size_type _P)
            {if (size() <= _P)
                _Xran();
            return (*(begin() + _P)); }
        const_reference operator[](size_type _P) const
            {return (*(begin() + _P)); }
        reference operator[](size_type _P)
            {return (*(begin() + _P)); }
        reference front()
            {return (*begin()); }
        const_reference front() const
            {return (*begin()); }
        reference back()
            {return (*(end() - 1)); }
        const_reference back() const
            {return (*(end() - 1)); }
        void push_back(const _Ty& _X)
            {insert(end(), _X); }
        void pop_back()
            {erase(end() - 1); }
        void assign(_It _F, _It _L)
            {erase(begin(), end());
            insert(begin(), _F, _L); }
        void assign(size_type _N, const _Ty& _X = _Ty())
            {erase(begin(), end());
            insert(begin(), _N, _X); }
        iterator insert(iterator _P, const _Ty& _X = _Ty())
            {size_type _O = _P - begin();
            insert(_P, 1, _X);
            return (begin() + _O); }
        void insert(iterator _P, size_type _M, const _Ty& _X)
            {if (_End - _Last < _M)
                {size_type _N = size() + (_M < size() ? size() : _M);
                iterator _S = allocator.allocate(_N, (void *)0);
                iterator _Q = _Ucopy(_First, _P, _S);
                _Ufill(_Q, _M, _X);
                _Ucopy(_P, _Last, _Q + _M);
                _Destroy(_First, _Last);
                allocator.deallocate(_First, _End - _First);
                _End = _S + _N;
                _Last = _S + size() + _M;
                _First = _S; }
            else if (_Last - _P < _M)
                {_Ucopy(_P, _Last, _P + _M);
                _Ufill(_Last, _M - (_Last - _P), _X);
                std::fill(_P, _Last, _X);
                _Last += _M; }
            else if (0 < _M)
                {_Ucopy(_Last - _M, _Last, _Last);
                std::copy_backward(_P, _Last - _M, _Last);
                std::fill(_P, _P + _M, _X);
                _Last += _M; }}
        void insert(iterator _P, _It _F, _It _L)
            {size_type _M = 0;
            std::_Distance(_F, _L, _M);
            if (_End - _Last < _M)
                {size_type _N = size() + (_M < size() ? size() : _M);
                iterator _S = allocator.allocate(_N, (void *)0);
                iterator _Q = _Ucopy(_First, _P, _S);
                _Q = _Ucopy(_F, _L, _Q);
                _Ucopy(_P, _Last, _Q);
                _Destroy(_First, _Last);
                allocator.deallocate(_First, _End - _First);
                _End = _S + _N;
                _Last = _S + size() + _M;
                _First = _S; }
            else if (_Last - _P < _M)
                {_Ucopy(_P, _Last, _P + _M);
                _Ucopy(_F + (_Last - _P), _L, _Last);
                std::copy(_F, _F + (_Last - _P), _P);
                _Last += _M; }
            else if (0 < _M)
                {_Ucopy(_Last - _M, _Last, _Last);
                std::copy_backward(_P, _Last - _M, _Last);
                std::copy(_F, _L, _P);
                _Last += _M; }}
        iterator erase(iterator _P)
            {std::copy(_P + 1, end(), _P);
            _Destroy(_Last - 1, _Last);
            --_Last;
            return (_P); }
        iterator erase(iterator _F, iterator _L)
            {iterator _S = std::copy(_L, end(), _F);
            _Destroy(_S, end());
            _Last = _S;
            return (_F); }
        void clear()
            {erase(begin(), end()); }
        bool _Eq(const _Myt& _X) const
            {return (size() == _X.size()
                && std::equal(begin(), end(), _X.begin())); }
        bool _Lt(const _Myt& _X) const
            {return (std::lexicographical_compare(begin(), end(),
                _X.begin(), _X.end())); }
        void swap(_Myt& _X)
            {if (allocator == _X.allocator)
                {std::swap(_First, _X._First);
                std::swap(_Last, _X._Last);
                std::swap(_End, _X._End); }
            else
                {_Myt _Ts = *this; *this = _X, _X = _Ts; }}
        friend void swap(_Myt& _X, _Myt& _Y)
            {_X.swap(_Y); }
    protected:
        void _Destroy(iterator _F, iterator _L)
            {for (; _F != _L; ++_F)
                allocator.destroy(_F); }
        iterator _Ucopy(const_iterator _F, const_iterator _L,
            iterator _P)
            {for (; _F != _L; ++_P, ++_F)
                allocator.construct(_P, *_F);
            return (_P); }
        void _Ufill(iterator _F, size_type _N, const _Ty &_X)
            {for (; 0 < _N; --_N, ++_F)
                allocator.construct(_F, _X); }
        void _Xran() const
            {throw std::out_of_range("invalid vc6_vector<T> subscript"); }
        _A allocator;
        iterator _First, _Last, _End;
    };

    template<class _Ty, class _A> inline
        bool operator==(const vc6_vector<_Ty, _A>& _X,
            const vc6_vector<_Ty, _A>& _Y)
        {return (_X._Eq(_Y)); }
    template<class _Ty, class _A> inline
        bool operator!=(const vc6_vector<_Ty, _A>& _X,
            const vc6_vector<_Ty, _A>& _Y)
        {return (!(_X == _Y)); }
    template<class _Ty, class _A> inline
        bool operator<(const vc6_vector<_Ty, _A>& _X,
            const vc6_vector<_Ty, _A>& _Y)
        {return (_X._Lt(_Y)); }
    template<class _Ty, class _A> inline
        bool operator>(const vc6_vector<_Ty, _A>& _X,
            const vc6_vector<_Ty, _A>& _Y)
        {return (_Y < _X); }
    template<class _Ty, class _A> inline
        bool operator<=(const vc6_vector<_Ty, _A>& _X,
            const vc6_vector<_Ty, _A>& _Y)
        {return (!(_Y < _X)); }
    template<class _Ty, class _A> inline
        bool operator>=(const vc6_vector<_Ty, _A>& _X,
            const vc6_vector<_Ty, _A>& _Y)
        {return (!(_X < _Y)); }

#pragma warning(pop)
#pragma pack(pop)
};

#endif
