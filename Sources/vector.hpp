//
//  vector.hpp
//  kssmath
//
//  Created by Steven W. Klassen on 2019-04-04.
//  Copyright © 2019 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#ifndef kssmath_vector_hpp
#define kssmath_vector_hpp

#include <algorithm>
#include <array>
#include <iterator>
#include <stdexcept>
#include <valarray>

namespace kss { namespace math { namespace la {

    /** \file
     Definition of the VectorLike types.
     A Vector in our terminology is an ordered array-like object that supports the
     following methods:
     - VectorLike& operator=(const VectorLike&);
     - constexpr size_t size() noexcept;
     - reference operator[] (size_t n);
     - const_reference operator[] (size_t n);
     - iterator begin();
     - iterator end();
     - const_iterator begin() const;
     - const_iterator end() const;

     The std::array class already fits this description. std::vector comes close
     except that its size() method is not a constexpr. (Some - but not all - of the
     VectorLike methods will work with it anyway.) We provide an additional
     implementation based on a reference to an underlying std::valarray combined
     with a slice.

     We then provide a number of methods that may operator on VectorLike objects.
     */

    namespace _private {
        inline bool isValidSlice(size_t arrayLen, const std::slice& s) noexcept {
            return (s.start() + ((s.size()-1) * s.stride()) <= arrayLen);
        }

        template <class T>
        struct AddRelOps {
            inline bool operator!=(const T& t) const noexcept {
                const T* self = static_cast<const T*>(this);
                return !(*self == t);
            }

            inline bool operator<=(const T& t) const noexcept {
                const T* self = static_cast<const T*>(this);
                return (*self < t || *self == t);
            }

            inline bool operator>(const T& t) const noexcept {
                const T* self = static_cast<const T*>(this);
                return (!(*self == t) && !(*self < t));
            }

            inline bool operator>=(const T& t) const noexcept {
                const T* self = static_cast<const T*>(this);
                return !(*self < t);
            }
        };

        template <class VectorLike, class T>
        class VectorIterator : public AddRelOps<VectorIterator<VectorLike, T>>,
        public std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, T*, T&>
        {
            public:
            VectorIterator() = default;

            explicit VectorIterator(VectorLike* container, bool isEnd = false)
            : _container(container), _pos(isEnd ? container->size() : 0)
            {}

            VectorIterator(const VectorIterator&) = default;
            ~VectorIterator() = default;
            VectorIterator& operator=(const VectorIterator& it) noexcept = default;

            bool operator==(const VectorIterator& rhs) const noexcept {
                return (_container == rhs._container && _pos == rhs._pos);
            }
            bool operator<(const VectorIterator& rhs) const noexcept {
                return (_pos < rhs._pos);
            }

            T& operator*()                      { return (*_container)[_pos]; }
            const T& operator*() const          { return (*_container)[_pos]; }
            T* operator->()                     { return &(*_container)[_pos]; }
            const T* operator->() const         { return &(*_container)[_pos]; }
            T& operator[](size_t i)             { return (*_container)[_pos+i]; }
            const T& operator[](size_t i) const { return (*_container)[_pos+i]; }

            // Pointer arithmetic. For efficiency reasons we do not check if the resulting
            // position is valid.
            VectorIterator& operator++() noexcept {
                ++_pos;
                return *this;
            }
            VectorIterator operator++(int) noexcept {
                VectorIterator tmp(*this);
                operator++();
                return tmp;
            }
            VectorIterator& operator+=(ptrdiff_t n) noexcept {
                (n >= 0 ? _pos += size_t(n) : _pos -= size_t(-n));
                return *this;
            }
            VectorIterator operator+(ptrdiff_t n) const noexcept {
                VectorIterator tmp(*this);
                tmp += n;
                return tmp;
            }

            VectorIterator& operator--() noexcept {
                --_pos;
                return *this;
            }
            VectorIterator& operator-=(ptrdiff_t n) noexcept {
                (n >= 0 ? _pos -= size_t(n) : _pos += size_t(-n));
                return *this;
            }
            VectorIterator operator-(ptrdiff_t n) const noexcept {
                VectorIterator tmp(*this);
                tmp -= n;
                return tmp;
            }
            ptrdiff_t operator-(const VectorIterator& rhs) const noexcept {
                return ptrdiff_t(_pos >= rhs._pos ? _pos - rhs._pos : -(rhs._pos - _pos));
            }

            /*!
             Swap with another iterator.
             */
            void swap(VectorIterator& b) noexcept {
                if (this != &b) {
                    std::swap(_container, b._container);
                    std::swap(_pos, b._pos);
                }
            }

            private:
            VectorLike* _container = nullptr;
            size_t      _pos = 0;
        };

        template <class VectorLike, class T>
        inline VectorIterator<VectorLike, T> operator+(ptrdiff_t n,
                                                       const VectorIterator<VectorLike, T>& c) noexcept
        {
            return (c + n);
        }

        template <class VectorLike, class T>
        class ConstVectorIterator : public AddRelOps<ConstVectorIterator<VectorLike, T>>,
            public std::iterator<std::random_access_iterator_tag, T, ptrdiff_t, const T*, const T&>
        {
        public:
            ConstVectorIterator() = default;

            explicit ConstVectorIterator(const VectorLike* container, bool isEnd = false)
            : _container(container), _pos(isEnd ? container->size() : 0)
            {}

            ConstVectorIterator(const ConstVectorIterator&) = default;
            ~ConstVectorIterator() = default;
            ConstVectorIterator& operator=(const ConstVectorIterator& it) = default;

            bool operator==(const ConstVectorIterator& rhs) const noexcept {
                return (_container == rhs._container && _pos == rhs._pos);
            }
            bool operator<(const ConstVectorIterator& rhs) const noexcept {
                return (_pos < rhs._pos);
            }

            const T& operator*() const          { return (*_container)[_pos]; }
            const T* operator->() const         { return &(*_container)[_pos]; }
            const T& operator[](size_t i) const { return (*_container)[_pos+i]; }

            ConstVectorIterator& operator++() noexcept {
                ++_pos;
                return *this;
            }
            ConstVectorIterator operator++(int) noexcept {
                ConstVectorIterator tmp(*this);
                operator++();
                return tmp;
            }
            ConstVectorIterator& operator+=(ptrdiff_t n) noexcept {
                (n >= 0 ? _pos += size_t(n) : _pos -= size_t(-n));
                return *this;
            }
            ConstVectorIterator operator+(ptrdiff_t n) const noexcept {
                ConstVectorIterator tmp(*this);
                tmp += n;
                return tmp;
            }

            ConstVectorIterator& operator--() noexcept {
                --_pos;
                return *this;
            }
            ConstVectorIterator operator--(int) noexcept {
                ConstVectorIterator tmp(_pos);
                operator--();
                return tmp;
            }
            ConstVectorIterator& operator-=(ptrdiff_t n) noexcept {
                (n >= 0 ? _pos -= size_t(n) : _pos += size_t(-n));
                return *this;
            }
            ConstVectorIterator operator-(ptrdiff_t n) const noexcept {
                ConstVectorIterator tmp(*this);
                tmp -= n;
                return tmp;
            }
            ptrdiff_t operator-(const ConstVectorIterator& rhs) const noexcept {
                return ptrdiff_t(_pos >= rhs._pos ? _pos - rhs._pos : -(rhs._pos - _pos));
            }

            /*!
             Swap with another iterator.
             */
            void swap(ConstVectorIterator& b) noexcept {
                if (this != &b) {
                    std::swap(_container, b._container);
                    std::swap(_pos, b._pos);
                }
            }

        private:
            const VectorLike*   _container = nullptr;
            size_t              _pos = 0;
        };

        template <class VectorLike, class T>
        ConstVectorIterator<VectorLike, T> operator+(ptrdiff_t n,
                                                     const ConstVectorIterator<VectorLike, T>& c) noexcept
        {
            return (c + n);
        }
    }


    /*!
     A Vector is a vector in the mathematical sense as opposed to an std::vector.
     It it designed to contain a fixed sized array of numerical values. Internally
     it consists of a reference to a backing valarray, plus a slice that defines
     the elements of the vector.
     */
    template <class T, size_t N>
    class Vector {
        public:
        using value_type = T;
        using size_type = size_t;
        using reference = T&;
        using const_reference = const T&;
        using iterator = _private::VectorIterator<Vector, T>;
        using const_iterator = _private::ConstVectorIterator<Vector, T>;

        /*!
         Wrap an existing std::valarray to act as a Vector<N>.
         @throws std::invalid_argument if v.size() is not at least N.
         */
        explicit Vector(std::valarray<T>& v) : va(v), s(std::slice(0, N, 1)) {
            if (v.size() < N) {
                throw std::invalid_argument("valarray must have a size of at least N");
            }
        }

        /*!
         Wrap an existing std::valarray to act as a Vector<N>.
         @throws std::invalid_argument if v.size() is not at least N.
         */
        explicit Vector(std::valarray<T>& v, const std::slice& sl) : va(v), s(sl) {
            if (sl.size() != N) {
                throw std::invalid_argument("slice must be of size N");
            }
            if (!_private::isValidSlice(v.size(), sl)) {
                throw std::invalid_argument("valarray must be large enough to contain the slice");
            }
        }

        ~Vector() = default;
        Vector(Vector&&) = default;
        Vector& operator=(Vector&&) = default;
        Vector(const Vector& v) = default;
        Vector& operator=(const Vector&) = default;

        constexpr size_t size() const noexcept              { return N; }
        reference operator[](size_t n) noexcept             { return va[index(n)]; }
        const_reference operator[](size_t n) const noexcept { return va[index(n)]; }

        iterator begin() noexcept               { return iterator(this); }
        iterator end() noexcept                 { return iterator(this, true); }
        const_iterator begin() const noexcept   { return const_iterator(this); }
        const_iterator end() const noexcept     { return const_iterator(this, true); }

    private:
        std::valarray<T>&   va;
        std::slice          s;

        // Convert an index in the slice to the underlying index of the valarray.
        inline size_t index(size_t n) const noexcept {
            return s.start() + (n * s.stride());
        }
    };

    // MARK: Operations on generic VectorLike objects.

    template <template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    inline bool equal(const Vec1<T, N>& a, const Vec2<T, N>& b) noexcept {
        return std::equal(a.begin(), a.end(), b.begin());
    }

    template <template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    inline bool operator==(const Vec1<T, N>& a, const Vec2<T, N>& b) noexcept {
        return std::equal(a.begin(), a.end(), b.begin());
    }

    template <template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    inline bool operator!=(const Vec1<T, N>& a, const Vec2<T, N>& b) noexcept {
        return !std::equal(a.begin(), a.end(), b.begin());
    }

    // Arithmetic operators on vectors - addition

    template <template<class, size_t> class Vector, class T, size_t N>
    Vector<T, N>& operator+=(Vector<T, N>& v, const T& t) noexcept {
        for (size_t i = 0; i < N; ++i) {
            v[i] += t;
        }
        return v;
    }

    template <template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    Vec1<T, N>& operator+=(Vec1<T, N>& v1, const Vec2<T, N>& v2) noexcept {
        for (size_t i = 0; i < N; ++i) {
            v1[i] += v2[i];
        }
        return v1;
    }

    template <template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    std::array<T, N> operator+(const Vec1<T, N>& a, const Vec2<T, N>& b) noexcept {
        std::array<T, N> ret;
        for (size_t i = 0; i < N; ++i) {
            ret[i] = a[i] + b[i];
        }
        return ret;
    }

    // Arithmetic operators on vectors - subtraction

    template <template<class, size_t> class Vector, class T, size_t N>
    Vector<T, N>& operator-=(Vector<T, N>& v, const T& t) noexcept {
        for (size_t i = 0; i < N; ++i) {
            v[i] -= t;
        }
        return v;
    }

    template <template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    Vec1<T, N>& operator-=(Vec1<T, N>& v1, const Vec2<T, N>& v2) noexcept {
        for (size_t i = 0; i < N; ++i) {
            v1[i] -= v2[i];
        }
        return v1;
    }

    template <template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    std::array<T, N> operator-(const Vec1<T, N>& v1, const Vec2<T, N>& v2) noexcept {
        std::array<T, N> a;
        for (size_t i = 0; i < N; ++i) {
            a[i] = v1[i] - v2[i];
        }
        return a;
    }

    // Arithmetic operators on vectors - multiplication

    template <template<class, size_t> class Vector, class T, size_t N>
    Vector<T, N>& operator*=(Vector<T, N>& v, const T& t) noexcept {
        for (size_t i = 0; i < N; ++i) {
            v[i] *= t;
        }
        return v;
    }

    template <template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    Vec1<T, N>& operator*=(Vec1<T, N>& v1, const Vec2<T, N>& v2) noexcept {
        for (size_t i = 0; i < N; ++i) {
            v1[i] *= v2[i];
        }
        return v1;
    }

    template <template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    std::array<T, N> operator*(const Vec1<T, N>& v1, const Vec2<T, N>& v2) noexcept {
        std::array<T, N> a;
        for (size_t i = 0; i < N; ++i) {
            a[i] = v1[i] * v2[i];
        }
        return a;
    }

    // Arithmetic operators on vectors - division

    template <template<class, size_t> class Vector, class T, size_t N>
    Vector<T, N>& operator/=(Vector<T, N>& v, const T& t) noexcept {
        for (size_t i = 0; i < N; ++i) {
            v[i] /= t;
        }
        return v;
    }

    template <template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    Vec1<T, N>& operator/=(Vec1<T, N>& v1, const Vec2<T, N>& v2) noexcept {
        for (size_t i = 0; i < N; ++i) {
            v1[i] /= v2[i];
        }
        return v1;
    }

    template <template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    std::array<T, N> operator/(const Vec1<T, N>& v1, const Vec2<T, N>& v2) noexcept {
        std::array<T, N> a;
        for (size_t i = 0; i < N; ++i) {
            a[i] = v1[i] / v2[i];
        }
        return a;
    }

    // MARK: Additional Vector related methods

    /*!
     Returns a string representation of a vector. This is intended primarily for debugging
     purposes.
     */
    template <template<class, size_t> class Vector, class T, size_t N>
    std::string toString(const Vector<T, N>& v) {
        std::ostringstream str;
        str << "(";
        if (N > 0) {
            str << v[0];
            for (size_t i = 1; i < N; ++i) {
                str << ',' << v[i];
            }
        }
        str << ")";
        return str.str();
    }

    /*!
     Perform a sum of the values of a given Vector, but accumulating to another, presumably
     larger, type.
     */
    template <class Result, template<class, size_t> class Vector, class T, size_t N>
    Result sum(const Vector<T, N>& v, const Result& = Result()) {
        Result s(0);
        for (size_t i = 0; i < N; ++i) {
            s += Result(v[i]);
        }
        return s;
    }

    /*!
     Compute the dot product of two vectors. Note that Result is the type used both for
     the internal computations and for the result. It should typically not be an integral
     type and should have at least as much precision and range as T.
     */
    template <class Result, template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    Result dotProduct(const Vec1<T, N>& v1, const Vec2<T, N>& v2, const Result& = Result()) {
        const auto prod = v1 * v2;
        return sum<Result>(prod);
    }

    /*!
     Compute the norm of a vector. (Also sometimes called the distance or the
     magnitude of the vector.) This is simply the square root of the dot product
     of the vector with itself. Note that Result should typically have at least
     as great a precision as Number.
     */
    template <class Result, template<class, size_t> class Vector, class T, size_t N>
    Result norm(const Vector<T, N>& v, const Result& = Result()) {
        return std::sqrt(dotProduct<Result>(v, v));
    }
}}}

#endif
