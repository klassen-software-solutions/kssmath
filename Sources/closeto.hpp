//
//  closeto.hpp
//  kssutil
//
//  Created by Steven W. Klassen on 2013-01-11.
//  Copyright (c) 2013 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#ifndef kssmath_closeto_hpp
#define kssmath_closeto_hpp

#include <cmath>
#include <cstdlib>
#include <limits>

namespace kss { namespace math {

    /*!
     Returns true if the values x and y are within epsilon of each other.
     */
    template <class T>
    bool closeTo(const T& x,
                 const T& y,
                 const T& epsilon = std::numeric_limits<T>::epsilon()) noexcept
    {
        T diff = (y <= x ? x - y : y - x);
        return (diff <= epsilon);
    }

    // Specializations to use abs and fabs which are (presumably) more efficient.
    template<>
    inline bool closeTo(const int &x, const int &y, const int& epsilon) noexcept {
        return std::abs(x-y) <= epsilon;
    }

    template<>
    inline bool closeTo(const long& x, const long& y, const long& epsilon) noexcept {
        return std::abs(x-y) <= epsilon;
    }

    template<>
    inline bool closeTo(const float& x, const float& y, const float& epsilon) noexcept {
        return std::fabs(x-y) <= epsilon;
    }

    template<>
    inline bool closeTo(const double& x, const double& y, const double& epsilon) noexcept {
        return std::fabs(x-y) <= epsilon;
    }

    template<>
    inline bool closeTo(const long double& x, const long double& y, const long double& epsilon) noexcept {
        return std::fabs(x-y) <= epsilon;
    }
}}

#endif
