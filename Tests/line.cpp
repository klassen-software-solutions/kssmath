//
//  line.cpp
//  unittest
//
//  Created by Steven W. Klassen on 2014-07-02.
//  Copyright (c) 2014 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include "ksstest.hpp"

#include <kss/math/line.hpp>
#include <kss/math/point.hpp>

using namespace std;
using namespace kss::math::geom;
using namespace kss::test;

namespace {
    template <class T, class computeT, unsigned Dim>
    void test_line_instance() {
        using point_t = Point<T, Dim>;
        using line_t = Line<T, Dim>;

        {
            T raw_values1[] = { 1, 2, 3, 4 };
            T raw_values2[] = { 4, 5, 3, 1 };
            point_t p1(raw_values1);
            point_t p2(raw_values2);

            // Test construction.
            line_t l;
            KSS_ASSERT(l.a() == point_t::origin && l.b() == point_t::origin);

            line_t l2(p1, p2);
            KSS_ASSERT(l2.a() == p1 && l2.b() == p2);

            line_t l3(l2);
            KSS_ASSERT(l3.a() == p1 && l3.b() == p2);

            // Test setting the end points.
            l.a() = l2.b();
            l.b() = l2.a();
            KSS_ASSERT(l.a() == p2 && l.b() == p1);

            // Test equality.
            KSS_ASSERT(l2 == l3);
            KSS_ASSERT(l2 == l2);
            KSS_ASSERT(l2 != l);

            // Test assignment.
            l2 = l;
            KSS_ASSERT(l2.a() == p2 && l2.b() == p1);

            // Test move semantics.
            point_t p1p = p1;
            point_t p2p = p2;
            line_t l2p = l2;
            line_t l4(std::move(p1p), std::move(p2p));
            KSS_ASSERT(l4.a() == p1 && l4.b() == p2);
            line_t l5(std::move(l2p));
            KSS_ASSERT(l5 == l2);
        }
        {
            long double lengths[] = { 0.L, 0.L, 1.L, sqrt(5.L), sqrt(14.L) };
            static constexpr double tol = 1.e-5;

            // Length
            KSS_ASSERT(isCloseTo<T>(lengths[Dim], tol, [] {
                T raw_values[] { 0, 1, 2, 3 };
                point_t p(raw_values);
                line_t l(point_t::origin, p);
                return length<computeT>(l);
            }));

            // Midpoint.
            KSS_ASSERT(isTrue([] {
                T raw_values1[] = { 1, 2, 3, 4 };
                T raw_values2[] = { 5, 6, 7, 8 };
                computeT raw_values3[] = { 3, 4, 5, 6 };
                line_t l(point_t((const T*)raw_values1), point_t((const T*)raw_values2));
                Point<computeT, Dim> p(raw_values3);
                return (midpoint<computeT>(l) == p);
            }));

            if (Dim == 2) {
                KSS_ASSERT(isTrue([] {
                    Line<T, 2> l(Point<T, 2>({ 3, 9 }), Point<T, 2>({ -1, 5 }));
                    Point<computeT, 2> p { 1, 7 };
                    return (midpoint<computeT>(l) == p);
                }));
            }
            else if (Dim == 3) {
                KSS_ASSERT(isTrue([] {
                    Line<T, 3> l(Point<T, 3>({3, -2, 5}), Point<T, 3>({3, 1, 7}));
                    Point<computeT, 3> p { 3, -0.5, 6 };
                    return (midpoint<computeT>(l) == p);
                }));
            }

            // Distance
            if (Dim == 2) {
                // Test case checking against a result from a python code.
                long double dist = 1.41421356237L;
                point_t p { 2, 0 };
                line_t l(point_t({ 1, 1 }), point_t({ 2, 2 }));
                KSS_ASSERT(isCloseTo<T>(dist, tol, [&] {
                    return distance<computeT>(l, p);
                }));
                KSS_ASSERT(isCloseTo<T>(dist, tol, [&] {
                    return distance<computeT>(p, l);
                }));
            }
            {
                // Test case using a simple plane.
                long double dist = 100.L;
                T raw_values1[] { 100, 0, 0, 0 };
                T raw_values2[] { 100, 100, 0, 0 };
                T raw_values3[] { 0, 100, 0, 0 };
                point_t p(raw_values3);
                line_t l(point_t((const T*)raw_values1), point_t((const T*)raw_values2));
                KSS_ASSERT(isCloseTo<T>(dist, tol, [&] {
                    return distance<computeT>(l, p);
                }));
                KSS_ASSERT(isCloseTo<T>(dist, tol, [&] {
                    return distance<computeT>(p, l);
                }));

                // Simple shift of the above test case. Should change the computations (i.e.
                // fewer are zero) but the resulting distance should be the same.
                T shift(15);
                kss::math::la::operator+=(p.values(), shift);
                kss::math::la::operator+=(l.a().values(), shift);
                kss::math::la::operator+=(l.b().values(), shift);

                KSS_ASSERT(isCloseTo<T>(dist, tol, [&] {
                    return distance<computeT>(l, p);
                }));
                KSS_ASSERT(isCloseTo<T>(dist, tol, [&] {
                    return distance<computeT>(p, l);
                }));
            }
        }
    }

    template <class T, class computeT>
    void test_line_intersection() {
        using point_t = Point<T, 2>;
        using line_t = Line<T, 2>;

        T rawvalues1[] { 100, 0, 0, 0 };
        T rawvalues2[] { 100, 100, 0, 0 };
        T rawvalues3[] { 50, 50, 0, 0 };
        T rawvalues4[] { 150, 50, 0, 0 };
        computeT rawvalues5[] { 100, 50, 0, 0 };
        T rawvalues6[] { 0, 100, 0, 0 };
        line_t l1(point_t((const T*)rawvalues1), point_t((const T*)rawvalues2));
        line_t l2(point_t((const T*)rawvalues3), point_t((const T*)rawvalues4));
        line_t l3(point_t::origin, point_t((const T*)rawvalues6));
        Point<computeT, 2> p(rawvalues5);
        KSS_ASSERT(areClose<computeT>(intersection<computeT>(l1, l2), p));
        KSS_ASSERT(throwsException<NoIntersection>([&] {
            intersection<computeT>(l1, l1);
        }));
        KSS_ASSERT(throwsException<NoIntersection>([&] {
            intersection<computeT>(l1, l3);
        }));
    }
}


static TestSuite ts("geom::line", {
    make_pair("line<int/double, 2>", test_line_instance<int, double, 2>),
    make_pair("line<int/double, 3>", test_line_instance<int, double, 3>),
    make_pair("line<int/double, 4>", test_line_instance<int, double, 4>),
    make_pair("line<float/double, 2>", test_line_instance<float, double, 2>),
    make_pair("line<float/double, 3>", test_line_instance<float, double, 3>),
    make_pair("line<float/double, 4>", test_line_instance<float, double, 4>),
    make_pair("line<double/double, 2>", test_line_instance<double, double, 2>),
    make_pair("line<double/double, 3>", test_line_instance<double, double, 3>),
    make_pair("line<double/double, 4>", test_line_instance<double, double, 4>),
    make_pair("line<long double/long double, 2>", test_line_instance<long double, long double, 2>),
    make_pair("line<long double/long double, 3>", test_line_instance<long double, long double, 3>),
    make_pair("line<long double/long double, 4>", test_line_instance<long double, long double, 4>),
    make_pair("intersection<int/double>", test_line_intersection<int, double>),
    make_pair("intersection<int/double>", test_line_intersection<float, double>),
    make_pair("intersection<int/double>", test_line_intersection<double, double>),
    make_pair("intersection<int/double>", test_line_intersection<long double, double>)
});
