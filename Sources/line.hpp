//
//  line.hpp
//  kssmath
//
//  Created by Steven W. Klassen on 2015-01-04.
//  Copyright (c) 2015 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#ifndef kssmath_line_hpp
#define kssmath_line_hpp

#include "closeto.hpp"
#include "point.hpp"
#include "vautil.hpp"
#include "vector.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <stdexcept>
#include <type_traits>
#include <valarray>

namespace kss { namespace math { namespace geom {

    /*!
     This exception is thrown when there is no intersection between two lines.
     */
    class NoIntersection : public std::runtime_error {
    public:
        NoIntersection() : std::runtime_error("the lines do not intersect") {}
    };


    /*!
     Defines a multi-dimension line (well, actually a segment). The line is
     defined by two points a() and b().
     */
    template <class Number, unsigned Dim>
    class Line {
    public:
        /*!
         The dimension of the line.
         */
        static constexpr unsigned dimension = Dim;

        /*!
         Type definitions.
         */
        using point_t = Point<Number, Dim>;
        using value_t = Number;

        /*!
         Construct a line. The default line will be a zero-length line between
         the origin of the dimension and itself.
         */
        Line() = default;
        Line(const point_t& aa, const point_t& bb) : _a(aa), _b(bb) {}
        Line(point_t&& aa, point_t&& bb) : _a(std::move(aa)), _b(std::move(bb)) {}
        Line(const Line&) = default;
        Line(Line&&) = default;
        ~Line() noexcept {}

        Line& operator=(const Line&) = default;
        Line& operator=(Line&&) = default;

        /*!
         Set or return the end points.
         */
        const point_t&  a() const noexcept  { return _a; }
        point_t&        a() noexcept        { return _a; }
        const point_t&  b() const noexcept  { return _b; }
        point_t&        b() noexcept        { return _b; }

        /*!
         A line is considered equal if its endpoints (ordered) are equal.
         */
        bool operator==(const Line& l) const noexcept {
            if (&l == this) {
                return true;
            }
            return (_a == l._a && _b == l._b);

        }
        inline bool operator!=(const Line& l) const noexcept {
            return !operator==(l);
        }

    private:
        point_t _a = point_t::origin;
        point_t _b = point_t::origin;
    };

    /*!
     Returns the length of a line. This is the distance between the line's endpoints.
     Note that Result should be at least as precise as Number
     */
    template <class Result, class Number, unsigned Dim>
    inline Result length(const Line<Number, Dim>& l) {
        return distance<Result>(l.a(), l.b());
    }

    /*!
     Returns the midpoint of a segment. Note that Result should have at least
     the precision of Number.
     */
    template <class Result, class Number, unsigned Dim>
    Point<Result, Dim> midpoint(const Line<Number, Dim>& l) {
        Point<Result, Dim> m;
        for (unsigned i = 0; i < Dim; ++i) {
            m[i] = (Result(l.a()[i]) + Result(l.b()[i])) / Result(2);
        }
        return m;
    }

    namespace _private {
        template <class Result, class Number, size_t Dim>
        std::valarray<Result> makeValArrayFromDiffs(const std::array<Number, Dim>& a,
                                                    const std::array<Number, Dim>& b) noexcept
        {
            auto va = kss::math::makeValArray<Result>(a.begin(), a.end());
            for (unsigned i = 0; i < Dim; ++i) {
                va[i] -= Result(b[i]);
            }
            return va;
        }
    }

    /*!
     Returns the distance between a point and a line. This is the distance between
     the point and the closest point on the line. Note that this is the full line
     defined by the two points and is not restricted to the segment itself.

     Note that Result should have at least as much precision as Number.

     The algorithm used is based on a discussion of the core geometry found at
     http://programmers.stackexchange.com.
     */
    template <class Result, class Number, unsigned Dim>
    Result distance(const Line<Number, Dim>& l, const Point<Number, Dim>& p) {
        using kss::math::la::dotProduct;
        using kss::math::la::norm;
        using vector_t = kss::math::la::Vector<Result, Dim>;

        auto pa = _private::makeValArrayFromDiffs<Result>(p.values(), l.a().values());
        auto ba = _private::makeValArrayFromDiffs<Result>(l.b().values(), l.a().values());
        auto t = (dotProduct<Result>(vector_t(pa), vector_t(ba))
                  / dotProduct<Result>(vector_t(ba), vector_t(ba)));
        std::valarray<Result> va = pa - t * ba;
        return norm<Result>(vector_t(va));
    }

    template <class Result, class Number, unsigned Dim>
    inline Result distance(const Point<Number, Dim>& p, const Line<Number, Dim>& l) {
        return distance<Result>(l, p);
    }

    namespace _private {
        template <class Number>
        Point<Number, 2> intersectUsingArrays(const std::array<Number, 2>& a,
                                              const std::array<Number, 2>& b,
                                              const std::array<Number, 2>& c,
                                              const std::array<Number, 2>& d)
        {
            using kss::math::la::operator-;
            using kss::math::la::operator+;
            using kss::math::la::operator*=;

            // preconditions
            assert(a.size() == 2);
            assert(b.size() == 2);
            assert(c.size() == 2);
            assert(d.size() == 2);

            Number denom = a[0]*(d[1]-c[1]) + b[0]*(c[1]-d[1]) + d[0]*(b[1]-a[1]) + c[0]*(a[1]-b[1]);
            if (closeTo<Number>(denom, 0.)) {
                throw NoIntersection();
            }

            Number s = (a[0]*(d[1]-c[1]) + c[0]*(a[1]-d[1]) + d[0]*(c[1]-a[1])) / denom;
            auto tmp = b-a;
            tmp *= s;
            return Point<Number, 2>(a + tmp);
        }

        template <class Result, class Number, unsigned Dim>
        std::array<Result, Dim> makeArrayFromArray(const std::array<Number, Dim>& a) {
            std::array<Result, Dim> newArray;
            std::copy(a.begin(), a.end(), newArray.begin());
            return newArray;
        }
    }

    /*!
     Returns the point of intersection between two lines. Note that
     this is the point of intersection between lines, not just segments. If you
     are interested in segment intersection you will need to call this and then
     check that the point is within the bounds of the line segments.

     This is based on an algorithm and sample code described in <em>Computational
     Geometry in C_ by O'Rourke</em>.

     @throws NoIntersection if the lines do not intersect. This implies they are
        parallel. Note that we interpret overlapping lines (i.e. two segments that
        actually are on the same line) as parallel and hence not intersecting.
     */
    template <class Result, class Number>
    std::enable_if_t<!std::is_same<Result, Number>::value, Point<Result, 2>>
    intersection(const Line<Number, 2>& l1, const Line<Number, 2>& l2) {
        auto a = _private::makeArrayFromArray<Result, Number, 2>(l1.a().values());
        auto b = _private::makeArrayFromArray<Result, Number, 2>(l1.b().values());
        auto c = _private::makeArrayFromArray<Result, Number, 2>(l2.a().values());
        auto d = _private::makeArrayFromArray<Result, Number, 2>(l2.b().values());
        return _private::intersectUsingArrays(a, b, c, d);
    }

    template <class Number>
    inline Point<Number, 2> intersection(const Line<Number, 2>& l1, const Line<Number, 2>& l2) {
        return _private::intersectUsingArrays(l1.a().values(),
                                              l1.b().values(),
                                              l2.a().values(),
                                              l2.b().values());
    }
}}}
#endif
