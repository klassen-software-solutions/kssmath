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

#include <array>
#include <cmath>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <valarray>
#include <vector>

namespace kss { namespace math { namespace la {

    namespace _private {
        inline bool isValidSlice(size_t arrayLen, const std::slice& s) noexcept {
            return (s.start() + ((s.size()-1) * s.stride()) <= arrayLen);
        }
    }

    // MARK: Vector types

    /** \file
     Definition of the Vector types.
     A Vector in our terminology is an ordered array-like object that supports the
     following methods:
        - Vector& operator=(const Vector&);
        - constexpr size_t size() noexcept;
        - reference operator[] (size_t n);
        - const_reference operator[] (size_t n);

     The std::array class already fits this description. We provide wrappers to allow
     raw pointers, std::vector, and std::valarray combined with an std::slice to
     also work as a Vector.

     We then provide a number of methods that may operator on Vectors.
     */


    /*!
     VectorPtr wraps a raw T* to allow it to be treated as a Vector, provided that it
     contains at least N values. If not, the result of any Vector operations will be
     undefined and will probably cause a memory overwrite.
     */
    template <class T, size_t N>
    class VectorPtr {
    public:
        using value_type = T;
        using size_type = size_t;
        using reference = T&;
        using const_reference = const T&;

        /*!
         Wrap an existing std::vector to act as a Vector<N>.
         @throws std::invalid_argument if p is null
         */
        explicit VectorPtr(T* p) : ptr(p) {
            if (!p) {
                throw std::invalid_argument("the pointer must not be null");
            }
        }

        ~VectorPtr() = default;

        // Move is allowed, copy is not.
        VectorPtr(VectorPtr&&) = default;
        VectorPtr& operator=(VectorPtr&&) = default;
        VectorPtr(const VectorPtr&) = delete;
        VectorPtr& operator=(const VectorPtr&) = delete;

        constexpr size_t size() const noexcept                      { return N; }
        inline reference operator[](size_t n) noexcept              { return ptr[n]; }
        inline const_reference operator[](size_t n) const noexcept  { return ptr[n]; }

    private:
        T* ptr = nullptr;
    };


    /*!
     VectorVec wraps an std::vector to allow it to be treated as a Vector, provided that
     it is at least of size N. If the underlying vector is resized less than N, the
     result of any Vector operations will be undefined and will probably cause a memory
     overwrite.
     */
    template <class T, size_t N>
    class VectorVec : public VectorPtr<T, N> {
    public:
        /*!
         Wrap an existing std::vector to act as a Vector<N>.
         @throws std::invalid_argument if v.size() is not at least N.
         */
        explicit VectorVec(std::vector<T>& v) : VectorPtr<T,N>(v.data()) {
            if (v.size() < N) {
                throw std::invalid_argument("vector must have a size of at least N");
            }
        }

        ~VectorVec() = default;

        // Move is allowed, copy is not.
        VectorVec(VectorVec&&) = default;
        VectorVec& operator=(VectorVec&&) = default;
        VectorVec(const VectorVec&) = delete;
        VectorVec& operator=(const VectorVec&) = delete;
    };


    /*!
     VectorVA wrap an std::valarray to allow it to be treated as a Vector, provided that
     it is at least of size N. If the underlying valarray is resized less than N, the result
     of any Vector operations will be undefined and will probably cause a memory overwrite.

     If it is constructed with a slice, then only that portion of the valarray will be
     considered as the vector. Otherwise the first N items will be used.
     */
    template <class T, size_t N>
    class VectorVA {
    public:
        using value_type = T;
        using size_type = size_t;
        using reference = T&;
        using const_reference = const T&;

        /*!
         Wrap an existing std::valarray to act as a Vector<N>.
         @throws std::invalid_argument if v.size() is not at least N.
         */
        explicit VectorVA(std::valarray<T>& v) : va(v), s(std::slice(0, N, 1)) {
            if (v.size() < N) {
                throw std::invalid_argument("valarray must have a size of at least N");
            }
        }

        /*!
         Wrap an existing std::valarray to act as a Vector<N>.
         @throws std::invalid_argument if v.size() is not at least N.
         */
        explicit VectorVA(std::valarray<T>& v, const std::slice& sl) : va(v), s(sl) {
            if (sl.size() != N) {
                throw std::invalid_argument("slice must be of size N");
            }
            if (!_private::isValidSlice(v.size(), sl)) {
                throw std::invalid_argument("valarray must be large enough to contain the slice");
            }
        }

        ~VectorVA() = default;

        // Move is allowed, copy is not.
        VectorVA(VectorVA&&) = default;
        VectorVA& operator=(VectorVA&&) = default;
        VectorVA(const VectorVA&) = delete;
        VectorVA& operator=(const VectorVA&) = delete;

        constexpr size_t size() const noexcept              { return N; }
        reference operator[](size_t n) noexcept             { return va[index(n)]; }
        const_reference operator[](size_t n) const noexcept { return va[index(n)]; }

    private:
        std::valarray<T>&   va;
        std::slice          s;

        // Convert an index in the slice to the underlying index of the valarray.
        inline size_t index(size_t n) const noexcept {
            return s.start() + (n * s.stride());
        }
    };

    // MARK: Vector operations

    /*!
     Return true if two vectors have the same elements.
     */
    template <template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    bool operator==(const Vec1<T, N>& v1, const Vec2<T, N>& v2) noexcept {
        for (size_t i = 0; i < N; ++i) {
            if (v1[i] != v2[i]) {
                return false;
            }
        }
        return true;
    }

    /*!
     Returns true if two vectors have at least one element that differs.
     */
    template <template<class, size_t> class Vec1, template<class, size_t> class Vec2, class T, size_t N>
    inline bool operator!=(const Vec1<T, N>& v1, const Vec2<T, N>& v2) noexcept {
        return !operator==(v1, v2);
    }

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
    std::array<T, N> operator+(const Vec1<T, N>& v1, const Vec2<T, N>& v2) noexcept {
        std::array<T, N> a;
        for (size_t i = 0; i < N; ++i) {
            a[i] = v1[i] + v2[i];
        }
        return a;
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
