//
//  gcd.hpp
//  unittest
//
//  Created by Steven W. Klassen on 2013-01-11.
//  Copyright (c) 2013 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include "ksstest.hpp"

#include <kss/math/gcd.hpp>

#include <iostream>

using namespace std;
using namespace kss::math::num;
using namespace kss::test;

namespace {
    template <class UNSIGNED>
    void test_gcd() {
        KSS_ASSERT(gcd(UNSIGNED(54), UNSIGNED(24)) == 6);
        KSS_ASSERT(gcd(UNSIGNED(3), UNSIGNED(5)) == 1);
        KSS_ASSERT(gcd(UNSIGNED(12), UNSIGNED(60)) == 12);
        KSS_ASSERT(gcd(UNSIGNED(12), UNSIGNED(90)) == 6);
    }
}

static TestSuite ts("num::gcd", {
    make_pair("gcd", [] {
        // test greatest_common_divisor
        test_gcd<unsigned short>();
        test_gcd<unsigned int>();
        test_gcd<unsigned long>();
        test_gcd<unsigned long long>();
        test_gcd<uint16_t>();
        test_gcd<uint32_t>();
        test_gcd<uint64_t>();
        KSS_ASSERT(gcd(2837844U, 283734U) == 18U);
    })
});
