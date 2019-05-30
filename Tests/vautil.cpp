//
//  vautil.cpp
//  unittest
//
//  Created by Steven W. Klassen on 2019-04-24.
//  Copyright © 2019 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include "ksstest.hpp"

#include <kss/math/vautil.hpp>

using namespace std;
using namespace kss::math;
using namespace kss::test;

namespace {
    template <class T>
    void makeFromIterator() {
        int ar[] = { 1, 2, 3, 4, 5 };
        auto var = makeValArray<T>(ar, ar+5);
        KSS_ASSERT(var.size() == 5);
        for (size_t i = 0; i < var.size(); ++i) {
            KSS_ASSERT(var[i] == T(ar[i]));
        }
    }

    template <class T>
    void makeFromValarray() {
        valarray<int> ar(5);
        for (int i = 0; i < 5; ++i) {
            ar[i] = i+1;
        }

        auto var = makeValArray<T>(ar);
        KSS_ASSERT(var.size() == 5);
        for (size_t i = 0; i < var.size(); ++i) {
            KSS_ASSERT(var[i] == T(ar[i]));
        }
    }

    template <class T>
    void testMakeValArray() {
        makeFromIterator<T>();
        makeFromValarray<T>();
    }
}

static TestSuite ts("::vautil", {
    make_pair("makeValArray<float>", testMakeValArray<float>),
    make_pair("makeValArray<double>", testMakeValArray<double>),
    make_pair("makeValArray<long double>", testMakeValArray<long double>)
});
