//
//  radians.hpp
//  kssmath
//
//  Created by Steven W. Klassen on 2013-01-11.
//  Copyright (c) 2013 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#ifndef kssmath_radians_hpp
#define kssmath_radians_hpp

#include "constants.hpp"

namespace kss { namespace math { namespace geom {

    /*!
     Convert from degrees to radians.
     */
    template <class Float>
    Float toRadians(Float deg) noexcept {
        return (deg * Constants<Float>::PI) / Float(180);
    }

    /*!
     Convert from radians to degrees.
     */
    template <class Float>
    Float toDegrees(Float rad) noexcept {
        return (rad * Float(180)) / Constants<Float>::PI;
    }

}}}

#endif
