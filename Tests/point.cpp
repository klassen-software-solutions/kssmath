//
//  point.cpp
//  unittest
//
//  Created by Steven W. Klassen on 2014-07-02.
//  Copyright (c) 2014 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include "ksstest.hpp"

#include <kss/math/point.hpp>

#include <cmath>
#include <iostream>

using namespace std;
using namespace kss::math::geom;
using namespace kss::test;


namespace {
    template <class T, unsigned DIM>
    void test_point_instance() {
        using point_t = Point<T,DIM>;

        {
            long double distanceFromOrigin[] = { 0.L, 0.L, 1.L, sqrt(5.L), sqrt(14.L) };
            T rawvalues[] = { 0, 1, 2, 3 };

            point_t p;
            for (unsigned i = 0; i < DIM; ++i) { KSS_ASSERT(p[i] == T()); }

            point_t p2(p);
            for (unsigned i = 0; i < DIM; ++i) { KSS_ASSERT(p2[i] == T()); }

            point_t p3((const T*)rawvalues);
            for (unsigned i = 0; i < DIM; ++i) { KSS_ASSERT(p3[i] == rawvalues[i]); }

            KSS_ASSERT(p == p);
            KSS_ASSERT(p == p2);
            KSS_ASSERT(p != p3);
            KSS_ASSERT(p2 != p3);

            p = p3;
            KSS_ASSERT(p == p);
            KSS_ASSERT(p != p2);
            KSS_ASSERT(p == p3);

            KSS_ASSERT(isCloseTo<long double>(distanceFromOrigin[DIM], [&] {
                return distance<long double>(p, p2);
            }));
            KSS_ASSERT(distance<long double>(p, p3) == 0.);

            KSS_ASSERT(p.dimension == DIM);
            KSS_ASSERT(p.dimension == p2.dimension);
            KSS_ASSERT(isTrue([&] {
                return p2 == Point<T,DIM>::origin;
            }));
            KSS_ASSERT((p != Point<T,DIM>::origin));
        }
        {
            // Initializer list tests.
            if (DIM == 3) {
                point_t p { 1, 2, 3 };
                KSS_ASSERT(p[0] == 1 && p[1] == 2 && p[2] == 3);
                KSS_ASSERT(throwsException<invalid_argument>([] {
                    point_t p2 { 1, 2, 3, 4};
                }));
                KSS_ASSERT(throwsException<invalid_argument>([] {
                    point_t p3 { 1, 2 };
                }));
            }
        }
        KSS_ASSERT(isTrue([] {
            // Move semantics.
            T rawvalues[] = { 0, 1, 2, 3 };
            point_t p(rawvalues);
            point_t p1(rawvalues);
            point_t p2(std::move(p1));
            return (p2 == p);
        }));
        {
            // Close to computations.
            T rawvalues1[] = { 0, 1, 2, 3 };
            T rawvalues2[] = { 1, 1, 2, 3 };
            T rawvalues3[] = { 10, 1, 2, 3 };
            point_t p1(rawvalues1);
            point_t p2(rawvalues2);
            point_t p3(rawvalues3);
            KSS_ASSERT(areClose<T>(p1, p1, 2));
            KSS_ASSERT(areClose<T>(p1, p2, 2));
            KSS_ASSERT(!areClose<T>(p1, p3, 2));
        }
    }
}



static TestSuite ts("geom::point", {
    make_pair("Point<int, 2>", test_point_instance<int, 2>),
    make_pair("Point<int, 3>", test_point_instance<int, 3>),
    make_pair("Point<int, 4>", test_point_instance<int, 4>),
    make_pair("Point<float, 2>", test_point_instance<float, 2>),
    make_pair("Point<float, 3>", test_point_instance<float, 3>),
    make_pair("Point<float, 4>", test_point_instance<float, 4>),
    make_pair("Point<double, 2>", test_point_instance<double, 2>),
    make_pair("Point<double, 3>", test_point_instance<double, 3>),
    make_pair("Point<double, 4>", test_point_instance<double, 4>),
    make_pair("Point<long double, 2>", test_point_instance<long double, 2>),
    make_pair("Point<long double, 3>", test_point_instance<long double, 3>),
    make_pair("Point<long double, 4>", test_point_instance<long double, 4>)
});
