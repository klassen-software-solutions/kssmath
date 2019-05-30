//
//  vector.cpp
//  unittest
//
//  Created by Steven W. Klassen on 2019-04-04.
//  Copyright © 2019 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include "ksstest.hpp"

#include <kss/math/closeto.hpp>
#include <kss/math/vector.hpp>

#include <algorithm>
#include <array>
#include <iostream>

using namespace std;
using namespace kss::math::la;
using namespace kss::test;
using kss::math::closeTo;

namespace {
    template <class T>
    bool equal(const valarray<T>& a, const valarray<T>& b) {
        return (a.size() == b.size()
                && std::equal(begin(a), end(a), begin(b)));
    }

    template <class T>
    void basicTests() {
        valarray<T> va1 = { 1, 2, 3, 4, 5 };
        valarray<T> va2 = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
        Vector<T, 5> vec1(va1);
        Vector<T, 5> vec2(va2, slice(0, 5, 2));

        KSS_ASSERT(vec1.size() == 5);
        KSS_ASSERT(vec2.size() == 5);
        for (size_t i = 0; i < 5; ++i) {
            KSS_ASSERT(vec1[i] == T(i+1));
            KSS_ASSERT(vec2[i] == T(i+1));
        }

        vec1[2] += T(2);
        vec2[2] = vec1[2];
        KSS_ASSERT(equal(vec1, vec2));
        KSS_ASSERT(equal(va1, valarray<T> { 1, 2, 5, 4, 5 }));
        KSS_ASSERT(equal(va2, valarray<T> { 1, 1, 2, 2, 5, 3, 4, 4, 5, 5 }));
    }

    template <class T>
    void equalityTests() {
        valarray<T> va1 = { 1, 2, 3, 4, 5 };
        valarray<T> va2 = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
        Vector<T, 5> vec1(va1);
        Vector<T, 5> vec2(va2, slice(0, 5, 2));
        array<T, 5> ar = { 1, 2, 3, 4, 5 };

        KSS_ASSERT(vec1 == vec1);
        KSS_ASSERT(vec1 == vec2);
        KSS_ASSERT(vec1 == ar);
        KSS_ASSERT(vec2 == vec2);
        KSS_ASSERT(vec2 == ar);
        KSS_ASSERT(ar == ar);
        KSS_ASSERT(equal(vec1, vec1));
        KSS_ASSERT(equal(vec1, vec2));
        KSS_ASSERT(equal(vec1, ar));
        KSS_ASSERT(equal(vec2, vec2));
        KSS_ASSERT(equal(vec2, ar));
        KSS_ASSERT(kss::math::la::equal(ar, ar));

        ar[1] = 0;
        KSS_ASSERT(vec1 != ar);
        KSS_ASSERT(vec2 != ar);
        KSS_ASSERT(!equal(vec1, ar));
        KSS_ASSERT(!equal(vec2, ar));
    }

    template <class T>
    void additionTests() {
        valarray<T> va1 = { 1, 2, 3, 4, 5 };
        valarray<T> va2 = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
        Vector<T, 5> vec1(va1);
        Vector<T, 5> vec2(va2, slice(0, 5, 2));
        array<T, 5> ar = { 1, 2, 3, 4, 5 };
        array<T, 5> ar2 = { 0, 1, 0, -1, 0 };

        array<T, 5> ans = { 2, 4, 6, 8, 10 };
        KSS_ASSERT((vec1 + vec2) == ans);
        KSS_ASSERT((vec1 + ar) == ans);
        KSS_ASSERT((ar + vec2) == ans);

        ans = { 1, 3, 3, 3, 5 };
        KSS_ASSERT((vec1 + ar2) == ans);
        KSS_ASSERT((vec1 + ar2) == ans);
        KSS_ASSERT((ar + ar2) == ans);

        ans = { 3, 4, 5, 6, 7 };
        KSS_ASSERT((vec1 += T(2)) == ans);
        KSS_ASSERT((vec2 += T(2)) == ans);
        KSS_ASSERT((ar += T(2)) == ans);

        ans = { 3, 5, 5, 5, 7 };
        KSS_ASSERT((vec1 += ar2) == ans);
        KSS_ASSERT((vec2 += ar2) == ans);
        KSS_ASSERT((ar += ar2) == ans);
    }

    template <class T>
    void subtractionTests() {
        valarray<T> va1 = { 1, 2, 3, 4, 5 };
        valarray<T> va2 = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
        Vector<T, 5> vec1(va1);
        Vector<T, 5> vec2(va2, slice(0, 5, 2));
        array<T, 5> ar = { 1, 2, 3, 4, 5 };
        array<T, 5> ar2 = { 0, 1, 0, -1, 0 };

        array<T, 5> ans = { 0, 0, 0, 0, 0 };
        KSS_ASSERT((vec1 - vec2) == ans);
        KSS_ASSERT((vec1 - ar) == ans);
        KSS_ASSERT((ar - vec2) == ans);

        ans = { 1, 1, 3, 5, 5 };
        KSS_ASSERT((vec1 - ar2) == ans);
        KSS_ASSERT((vec1 - ar2) == ans);
        KSS_ASSERT((ar - ar2) == ans);

        ans = { -1, 0, 1, 2, 3 };
        KSS_ASSERT((vec1 -= T(2)) == ans);
        KSS_ASSERT((vec2 -= T(2)) == ans);
        KSS_ASSERT((ar -= T(2)) == ans);

        ans = { -1, -1, 1, 3, 3 };
        KSS_ASSERT((vec1 -= ar2) == ans);
        KSS_ASSERT((vec2 -= ar2) == ans);
        KSS_ASSERT((ar -= ar2) == ans);
    }

    template <class T>
    void multiplicationTests() {
        valarray<T> va1 = { 1, 2, 3, 4, 5 };
        valarray<T> va2 = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
        Vector<T, 5> vec1(va1);
        Vector<T, 5> vec2(va2, slice(0, 5, 2));
        array<T, 5> ar = { 1, 2, 3, 4, 5 };
        array<T, 5> ar2 = { 0, 1, 0, -1, 0 };

        array<T, 5> ans = { 1, 4, 9, 16, 25 };
        KSS_ASSERT((vec1 * vec2) == ans);
        KSS_ASSERT((vec1 * ar) == ans);
        KSS_ASSERT((ar * vec2) == ans);

        ans = { 0, 2, 0, -4, 0 };
        KSS_ASSERT((vec1 * ar2) == ans);
        KSS_ASSERT((vec1 * ar2) == ans);
        KSS_ASSERT((ar * ar2) == ans);

        ans = { 2, 4, 6, 8, 10 };
        KSS_ASSERT((vec1 *= T(2)) == ans);
        KSS_ASSERT((vec2 *= T(2)) == ans);
        KSS_ASSERT((ar *= T(2)) == ans);

        ans = { 0, 4, 0, -8, 0 };
        KSS_ASSERT((vec1 *= ar2) == ans);
        KSS_ASSERT((vec2 *= ar2) == ans);
        KSS_ASSERT((ar *= ar2) == ans);
    }

    template <class T>
    void divisionTests() {
        valarray<T> va1 = { 2, 2, 4, 4, 6 };
        valarray<T> va2 = { 2, 2, 2, 2, 4, 4, 4, 4, 6, 6 };
        Vector<T, 5> vec1(va1);
        Vector<T, 5> vec2(va2, slice(0, 5, 2));
        array<T, 5> ar = { 2, 2, 4, 4, 6 };
        array<T, 5> ar2 = { 1, -1, 2, -2, 1 };

        array<T, 5> ans = { 1, 1, 1, 1, 1 };
        KSS_ASSERT((vec1 / vec2) == ans);
        KSS_ASSERT((vec1 / ar) == ans);
        KSS_ASSERT((ar / vec2) == ans);

        ans = { 2, -2, 2, -2, 6 };
        KSS_ASSERT((vec1 / ar2) == ans);
        KSS_ASSERT((vec1 / ar2) == ans);
        KSS_ASSERT((ar / ar2) == ans);

        ans = { 1, 1, 2, 2, 3 };
        KSS_ASSERT((vec1 /= T(2)) == ans);
        KSS_ASSERT((vec2 /= T(2)) == ans);
        KSS_ASSERT((ar /= T(2)) == ans);

        ans = { 1, -1, 1, -1, 3 };
        KSS_ASSERT((vec1 /= ar2) == ans);
        KSS_ASSERT((vec2 /= ar2) == ans);
        KSS_ASSERT((ar /= ar2) == ans);
    }

    template <class T>
    void sumTests() {
        valarray<T> va1 = { 2, 2, 4, 4, 6 };
        valarray<T> va2 = { 2, 2, 2, 2, 4, 4, 4, 4, 6, 6 };
        Vector<T, 5> vec1(va1);
        Vector<T, 5> vec2(va2, slice(0, 5, 2));
        array<T, 5> ar = { 2, 2, 4, 4, 6 };

        KSS_ASSERT(sum<double>(vec1) == 18.0);
        KSS_ASSERT(sum<double>(vec2) == 18.0);
        KSS_ASSERT(sum<double>(ar) == 18.0);
    }

    template <class T>
    void dotProductTests() {
        valarray<T> va1 = { 1, 2, 3, 4, 5 };
        valarray<T> va2 = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
        Vector<T, 5> vec1(va1);
        Vector<T, 5> vec2(va2, slice(0, 5, 2));
        array<T, 5> ar = { 1, 2, 3, 4, 5 };
        array<T, 5> ar2 = { 3, 1, 0, -1, -2 };

        KSS_ASSERT(dotProduct<double>(vec1, ar2) == -9.0);
        KSS_ASSERT(dotProduct<double>(vec2, ar2) == -9.0);
        KSS_ASSERT(dotProduct<double>(ar, ar2) == -9.0);
        KSS_ASSERT(dotProduct<double>(ar2, vec1) == -9.0);
        KSS_ASSERT(dotProduct<double>(ar2, vec2) == -9.0);
        KSS_ASSERT(dotProduct<double>(ar2, ar) == -9.0);
    }

    template <class T>
    void normTests() {
        valarray<T> va1 = { 1, 2, 3, 4, 5 };
        valarray<T> va2 = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
        Vector<T, 5> vec1(va1);
        Vector<T, 5> vec2(va2, slice(0, 5, 2));
        array<T, 5> ar = { 1, 2, 3, 4, 5 };

        KSS_ASSERT(closeTo<double>(norm<double>(vec1), 7.4162, 0.001));
        KSS_ASSERT(closeTo<double>(norm<double>(vec2), 7.4162, 0.001));
        KSS_ASSERT(closeTo<double>(norm<double>(ar), 7.4162, 0.001));
    }
}

static TestSuite ts("la::vector", {
    make_pair("basic", [] {
        basicTests<int>();
        basicTests<float>();
        basicTests<double>();
        basicTests<long double>();
    }),
    make_pair("equality", [] {
        equalityTests<int>();
        equalityTests<float>();
        equalityTests<double>();
        equalityTests<long double>();
    }),
    make_pair("addition", [] {
        additionTests<int>();
        additionTests<float>();
        additionTests<double>();
        additionTests<long double>();
    }),
    make_pair("subtraction", [] {
        subtractionTests<int>();
        subtractionTests<float>();
        subtractionTests<double>();
        subtractionTests<long double>();
    }),
    make_pair("multiplication", [] {
        multiplicationTests<int>();
        multiplicationTests<float>();
        multiplicationTests<double>();
        multiplicationTests<long double>();
    }),
    make_pair("division", [] {
        divisionTests<int>();
        divisionTests<float>();
        divisionTests<double>();
        divisionTests<long double>();
    }),
    make_pair("sum", [] {
        sumTests<int>();
        sumTests<float>();
        sumTests<double>();
        sumTests<long double>();
    }),
    make_pair("dotProduct", [] {
        dotProductTests<int>();
        dotProductTests<float>();
        dotProductTests<double>();
        dotProductTests<long double>();
    }),
    make_pair("norm", [] {
        normTests<int>();
        normTests<float>();
        normTests<double>();
        normTests<long double>();
    })
});
