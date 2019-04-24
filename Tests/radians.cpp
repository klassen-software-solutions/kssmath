//
//  radians.cpp
//  unittest
//
//  Created by Steven W. Klassen on 2013-01-11.
//  Copyright (c) 2013 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include "ksstest.hpp"

#include <kss/math/radians.hpp>

using namespace std;
using namespace kss::math::geom;
using namespace kss::test;


static TestSuite ts("geom::radians", {
    make_pair("toRadians", [] {
        KSS_ASSERT(toRadians(90.F) == 1.57079625F);
        KSS_ASSERT(toRadians(90.) == 1.5707963267948966);
        KSS_ASSERT(toRadians(90.L) == 1.57079632679489661926L);
    }),
    make_pair("toDegrees", [] {
        KSS_ASSERT(toDegrees(2.F) == 114.591568F);
        KSS_ASSERT(toDegrees(2.) == 114.59155902616465);
        KSS_ASSERT(toDegrees(2.L) == 114.591559026164641753L);
    })
});
