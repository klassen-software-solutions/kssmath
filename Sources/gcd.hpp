//
//  gcd.hpp
//  kssmath
//
//  Created by Steven W. Klassen on 2015-01-04.
//  Copyright (c) 2015 Klassen Software Solutions. All rights reserved.
//

#ifndef kssmath_gcd_hpp
#define kssmath_gcd_hpp

namespace kss { namespace math { namespace numerical {

    /*!
      Greatest common divisor. This will return the gcd of the two arguments.

      This method computes the gcd using the binary gcd algorithm
      described in <i>The Art of Computer Programming, Volume 2,
      Seminumerical Algorithms</i>, by Knuth, ISBN 0-201-03822-6.
     */
    unsigned short gcd(unsigned short u, unsigned short v) noexcept;
    unsigned int gcd(unsigned int u, unsigned int v) noexcept;
    unsigned long gcd(unsigned long u, unsigned long v) noexcept;
    unsigned long long gcd(unsigned long long u, unsigned long long v) noexcept;

}}}

#endif
