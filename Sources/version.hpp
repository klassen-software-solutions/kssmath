//
//  version.hpp
//  kssmath
//
//  Copyright © 2018 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#ifndef kssmath_version_hpp
#define kssmath_version_hpp

#include <string>

namespace kss { namespace math {

    /*!
     Returns a string of the form x.y.z<optional tags> that describes the version
     of this library.
     */
    std::string version() noexcept;

    /*!
     Returns the text of the software license.
     */
    std::string license() noexcept;

}}

#endif
