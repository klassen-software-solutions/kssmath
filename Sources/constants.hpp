//
//  constants.hpp
//  kssmath
//
//  Created by Steven W. Klassen on 2017-03-20.
//  Copyright © 2017 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#ifndef kssmath_constants_hpp
#define kssmath_constants_hpp

namespace kss { namespace math {

    /*!
     Define a number of physical constants to the best accuracy available for
     a floating point precision.
     */
    template<class Float>
    class Constants {
    public:
        static constexpr Float PI = 0;
    };

    template<> class Constants<float> {
    public:
        static constexpr float PI = 3.1415926F;
    };

    template<> class Constants<double> {
    public:
        static constexpr double PI = 3.1415926535897932;
    };

    template<> class Constants<long double> {
    public:
        static constexpr long double PI = 3.14159265358979323846L;
    };
}}

#endif
