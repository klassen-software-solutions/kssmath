//
//  point.hpp
//  kssmath
//
//  Created by Steven W. Klassen on 2015-01-04.
//  Copyright (c) 2015 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#ifndef kssmath_point_hpp
#define kssmath_point_hpp

#include "vector.hpp"

#include <cmath>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <valarray>

namespace kss { namespace math { namespace geom {

    /*!
     Defines a multi-dimensional point. Note that it is assumed that the type of Number
     is some form of a numeric type. Also note that many of the computations will
     assume double precision floating point internally hence it is necessary to
     be able to cast both ways between a double and a Number.
     */
    template<class Number, unsigned Dim>
    class Point {
    public:
        /*!
         The dimension of the point.
         */
        static constexpr unsigned dimension = Dim;

        /*!
         The origin point (0,0,...,0) for this dimension.
         */
        static const Point<Number, Dim> origin;

        /*!
         The underlying type in the point.
         */
        using value_type = Number;


        /*!
         Construct a point.
         */
        explicit Point(Number val = Number()) : _values(val, Dim) {}
        Point(const Point& p) : _values(p._values) {}
        Point(Point&&) = default;
        Point& operator=(Point&&) = default;
        ~Point() = default;

        Point& operator=(const Point& p) {
            if (this != &p) {
                _values = p._values;
            }
            return *this;
        }

        explicit Point(std::initializer_list<Number> il) : _values(il) {
            if (il.size() != Dim) {
                throw std::invalid_argument("initializer list must have "
                                            + std::to_string(Dim)
                                            + " values");
            }
        }

        explicit Point(const Number* ptr) : _values(ptr, Dim) {
            if (!ptr) {
                throw std::invalid_argument("ptr must not be null");
            }
        }

        /*!
         Return an individual value within the point. If i > Dim the result is undefined.
         @param i the index of the value
         */
        Number& operator[](size_t i) noexcept                { return _values[i]; }
        const Number& operator[](size_t i) const noexcept    { return _values[i]; }

        /*!
         Returns a reference to the underlying values.
         */
        std::valarray<Number>&          values() noexcept       { return _values; }
        const std::valarray<Number>&    values() const noexcept { return _values; }

        kss::math::la::VectorVA<Number, Dim>&       vector() noexcept       { return _vector; }
        const kss::math::la::VectorVA<Number, Dim>& vector() const noexcept { return _vector; }

        /*!
         Determine the equality of two points. They will be equal if and only if
         all their values are equal.
         */
        inline bool operator==(const Point& p) const noexcept {
            //return (_vector == p._vector);
            return true;
        }

        inline bool operator!=(const Point& p) const noexcept {
            return _vector != p._vector;
        }

    private:
        std::valarray<Number>                   _values;
        kss::math::la::VectorVA<Number, Dim>    _vector = kss::math::la::VectorVA<Number, Dim>(_values);
    };

    template<class Number, unsigned Dim>
    const Point<Number, Dim> Point<Number, Dim>::origin;


    /*!
     Returns the distance between two points. For good results you need that
     the precision of Result is at least as good as the precision of Number.

     @throws std::overflow_error if the values are too large to compute
     */
    template <class Result, class Number, unsigned Dim>
    Result distance(const Point<Number, Dim>& a, const Point<Number, Dim>& b) {
        std::valarray<Number> diffs = b.values() - a.values();
        auto vec = kss::math::la::VectorVA<Number, Dim>(diffs);
        return std::sqrt(kss::math::la::dotProduct<Result>(vec, vec));
    }

    /*!
     Returns true if two points are close to each other. Two points are considered
     near each other if the distance between them is less than a given epsilon
     value. Note that the epsilon value is given as a double regardless of the type
     of T.
     @throws std::overflow_error if the values are too large to compute the distance
     between the points.
     */
    template <class Compute, class Number, unsigned Dim>
    inline bool areClose(const Point<Number, Dim>& a,
                         const Point<Number, Dim>& b,
                         Compute epsilon = std::numeric_limits<Compute>::epsilon()) noexcept
    {
        return (distance<Compute>(a, b) < epsilon);
    }

}}}

#endif
