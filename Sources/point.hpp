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

#include <algorithm>
#include <array>

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
        explicit Point(Number val = Number()) {
            _values.fill(val);
        }

        Point(const Point&) = default;
        Point(Point&&) = default;
        Point& operator=(Point&&) = default;
        ~Point() = default;

        Point& operator=(const Point& p) {
            if (this != &p) {
                _values = p._values;
            }
            return *this;
        }

        explicit Point(const std::array<Number, Dim>& a) {
            _values = a;
        }

        explicit Point(std::array<Number, Dim>&& a) {
            _values = std::move(a);
        }

        explicit Point(std::initializer_list<Number> il) {
            if (il.size() != Dim) {
                throw std::invalid_argument("initializer list must have "
                                            + std::to_string(Dim)
                                            + " values");
            }
            std::copy_n(il.begin(), Dim, _values.begin());
        }

        explicit Point(const Number* ptr) {
            if (!ptr) {
                throw std::invalid_argument("ptr must not be null");
            }
            std::copy_n(ptr, Dim, _values.begin());
        }

        /*!
         Return an individual value within the point. If i > Dim the result is undefined.
         @param i the index of the value
         */
        Number& operator[](size_t i) noexcept                { return _values[i]; }
        const Number& operator[](size_t i) const noexcept    { return _values[i]; }

        /*!
         Returns a reference to the underlying array.
         */
        std::array<Number, Dim>&        values() noexcept       { return _values; }
        const std::array<Number, Dim>&  values() const noexcept { return _values; }

        /*!
         Determine the equality of two points. They will be equal if and only if
         all their values are equal.
         */
        inline bool operator==(const Point& p) const noexcept {
            return (_values == p._values);
        }

        inline bool operator!=(const Point& p) const noexcept {
            return _values != p._values;
        }

    private:
        std::array<Number, Dim> _values;
    };

    template<class Number, unsigned Dim>
    const Point<Number, Dim> Point<Number, Dim>::origin;

    /*!
     Returns the distance between two points. For good results you need that
     the precision of Result is at least as good as the precision of Number.

     @throws std::overflow_error if the values are too large to compute
     */
    template <class Result, class Number, unsigned Dim>
    inline Result distance(const Point<Number, Dim>& a, const Point<Number, Dim>& b) {
        const auto diffs = kss::math::la::operator-(b.values(), a.values());
        return kss::math::la::norm<Result>(diffs);
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
