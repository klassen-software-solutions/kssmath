//
//  minimum.hpp
//  kssmath
//
//  Created by Steven W. Klassen on 2015-01-04.
//  Copyright (c) 2015 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#ifndef kssmath_minimum_hpp
#define kssmath_minimum_hpp

#include <functional>
#include <limits>


namespace kss { namespace math { namespace num {

    /*!
     This exception is thrown when an iterative seminumerical algorithm does not
     appear to be converging to a solution.
     */
    class NoConvergence : public std::runtime_error {
    public:
        NoConvergence() : std::runtime_error("the algorithm does not appear to be conversing") {}
    };

    /*!
     Minimum value function. This is done using Brent's method based on the description
     and sample code found in _NUMERICAL RECIPES (FORTRAN)_, ISBN 0 521 38330 7.
     
     Note that if fbx is not provided (i.e. is the numeric max) then it will be computed
     automatically by calling fn(bx). If fn is an expensive function, then it is
     probable that the caller has already computed fn(bx) hence we give them the
     option of passing it in. Also the tolerance (tol1) is specified as a desired
     fixed tolerance and not a percentage of the x values as is done in the
     Numerical Recipes code.
     
     If the supplied function fn wants to halt execution for any reason, it should
     throw an exception which will be passed along from this function.
     
     @throws std::invalid_argument if ax, bx, and cx are not in increasing order
     @throws std::no_convergence if the algorithm does not appear to be converging to a solution
     */
    float minimum_value(float ax, float bx, float cx, float& xmin,
                        std::function<float(float)>& fn,
                        float tol1 = std::numeric_limits<float>::epsilon(),
                        float fbx = std::numeric_limits<float>::max());
    double minimum_value(double ax, double bx, double cx, double& xmin,
                         std::function<double(double)>& fn,
                         double tol1 = std::numeric_limits<double>::epsilon(),
                         double fbx = std::numeric_limits<double>::max());
    long double minimum_value(long double ax, long double bx, long double cx, long double& xmin,
                              std::function<long double(long double)>& fn,
                              long double tol1 = std::numeric_limits<long double>::epsilon(),
                              long double fbx = std::numeric_limits<long double>::max());
    
    
    /**
     * Maximum function value. This is really a minimum value function but with
     * the function negated.
     */
    template <class Float>
    Float maximum_value(Float ax, Float bx, Float cx, Float& xmin,
                        std::function<Float(Float)>& fn,
                        Float tol = std::numeric_limits<Float>::epsilon(),
                        Float fbx = std::numeric_limits<Float>::max())
    {
        std::function<Float(Float)> negfn([&](Float x) { return -fn(x); });
        return -minimum_value(ax, bx, cx, xmin, negfn, tol, fbx);
    }
    
}}}

#endif
