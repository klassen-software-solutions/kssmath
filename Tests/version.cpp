//
//  version.cpp
//  unittest
//
//  Copyright © 2018 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include <kss/math/version.hpp>
#include "ksstest.hpp"

using namespace std;
using namespace kss::test;


static TestSuite ts("version", {
    make_pair("version", [] {
        KSS_ASSERT(!kss::math::version().empty());
        KSS_ASSERT(!kss::math::license().empty());
    })
});
