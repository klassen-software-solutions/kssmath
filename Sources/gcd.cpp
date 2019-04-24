//
//  gcd.cpp
//  KSSCore
//
//  Created by Steven W. Klassen on 2015-01-05.
//  Copyright (c) 2015 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include "gcd.hpp"

#include <cassert>


using namespace std;
using namespace kss::math;

namespace {
    template <class UNSIGNED>
    UNSIGNED _gcd(UNSIGNED u, UNSIGNED v) noexcept {
        // Trivial first test.
        if (!u && !v) return UNSIGNED(0);

        // B1 Find power of 2
        UNSIGNED k(0);
        while (!(u % UNSIGNED(2)) && !(v % UNSIGNED(2))) {
            ++k;
            u >>= 1;
            v >>= 1;
        }

        // B2 Initialize
        assert((u % UNSIGNED(2)) || (v % UNSIGNED(2))); // at least one of u and v is odd
        UNSIGNED t;
        bool isNegative = false;
        if (u % UNSIGNED(2)) {
            t = v;
            isNegative = true;
            goto b4;
        }
        else {
            t = u;
        }

        // B3 Halve t
    b3:
        assert(t != UNSIGNED(0) && !(t % UNSIGNED(2)));  // t is even and non-zero
        t >>= 1;

        // B4 Is t even?
    b4:
        if (!(t % UNSIGNED(2))) goto b3;

        // B5 Reset max(u,v)
        if (! isNegative) u = t;
        else v = t;

        // B6 Subtract
        if (u >= v) {
            t = u - v;
            isNegative = false;
        }
        else {
            t = v - u;
            isNegative = true;
        }
        if (t != UNSIGNED(0)) goto b3;

        // Compute the answer u * 2^k
        return UNSIGNED(u << k);
    }
}


unsigned short kss::math::num::gcd(unsigned short u, unsigned short v) noexcept {
    return _gcd<unsigned short>(u, v);
}

unsigned int kss::math::num::gcd(unsigned int u, unsigned int v) noexcept {
    return _gcd<unsigned int>(u, v);
}

unsigned long kss::math::num::gcd(unsigned long u, unsigned long v) noexcept {
    return _gcd<unsigned long>(u, v);
}

unsigned long long kss::math::num::gcd(unsigned long long u, unsigned long long v) noexcept {
    return _gcd<unsigned long long>(u, v);
}

