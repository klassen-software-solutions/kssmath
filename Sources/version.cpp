//
//  version.cpp
//  kssmath
//
//  Copyright © 2018 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include "_license_internal.h"
#include "_version_internal.h"
#include "version.hpp"

using namespace std;

string kss::math::version() noexcept {
    return versionText;
}

string kss::math::license() noexcept {
    return licenseText;
}
