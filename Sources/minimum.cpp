//
//  minimum.cpp
//  kssmath
//
//  Created by Steven W. Klassen on 2015-01-05.
//  Copyright (c) 2015 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include <cmath>
#include <stdexcept>
#include "minimum.h"
#include "utility.h"
#include "_noinstall/compatibility.h"

using namespace std;
using namespace kss::math;


namespace {
    template <class FLOAT>
    FLOAT _minimum_value(FLOAT ax, FLOAT bx, FLOAT cx, FLOAT& xmin,
                        function<FLOAT(FLOAT)>& fn, FLOAT tol1, FLOAT fbx)
    {
        // Sanity checks on our initial values.
        if (ax >= bx || bx >= cx)
            throw std::invalid_argument("ax, bx, and cx must be in increasing order");

        const size_t itmax = 100;
        const FLOAT cgold = FLOAT(1.) - (std::sqrt(FLOAT(5.)) - FLOAT(1.)) / FLOAT(2.);

        FLOAT a = ax;
        FLOAT b = cx;
        FLOAT v = bx;
        FLOAT w = v;
        FLOAT x = v;
        FLOAT e = 0.;
        FLOAT fx = (fbx == std::numeric_limits<FLOAT>::max() ? fn(x) : fbx);
        FLOAT fv = fx;
        FLOAT fw = fx;
        FLOAT d = 0.;
        FLOAT tol2 = FLOAT(2.) * tol1;

        // Iterate towards the solution. If we reach itmax we will give up and throw
        // a no_convergence error.
        for (size_t i = 0; i < itmax; ++i) {

            // Check to see if we have found our convergence.
            FLOAT xm = FLOAT(0.5) * (a+b);
            if (std::fabs(x-xm) <= (tol2 - 0.5 * (b-a))) {
                xmin = x;
                return fx;
            }

            // If not we do the curve fit as described in Brent's algorithm.
            if (std::fabs(e) > tol1) {
                FLOAT r = (x-w) * (fx-fv);
                FLOAT q = (x-v) * (fx-fw);
                FLOAT p = (x-v) * q-(x-w)*r;
                q = FLOAT(2.) * (q-r);
                if (q > 0.) {
                    p = -p;
                }
                q = std::fabs(q);
                if ((std::fabs(p) >= std::fabs(0.5*q*e)) || (p <= (q*(a-x))) || (p >= q*(b-x))) {
                    // Curve fit will not work so revert back to the golden algorithm.
                    e = (x >= xm ? a-x : b-x);
                    d = cgold*e;
                }
                else {
                    e = d;
                    d = p / q;
                    FLOAT u = x + d;
                    if (((u-a) < tol2) || ((b-u) < tol2)) {
                        d = std::copysign(tol1, xm-x);
                    }
                }
            }
            else {  // Need to use the golden algorithm
                e = (x >= xm ? a-x : b-x);
                d = cgold*e;
            }

            // Compute the function for the next iteration.
            FLOAT u = (std::fabs(d) >= tol1 ? x+d : x+std::copysign(tol1, d));
            FLOAT fu = fn(u);
            if (fu <= fx) {
                if (u >= x) {
                    a = x;
                }
                else {
                    b = x;
                }

                v = w;
                fv = fw;
                w = x;
                fw = fx;
                x = u;
                fx = fu;
            }
            else {
                if (u < x) {
                    a = u;
                }
                else {
                    b = u;
                }

                if ((fu <= fw) || (w == x)) {
                    v = w;
                    fv = fw;
                    w = u;
                    fw = fu;
                }
                else if ((fu <= fv) || (v == x) || (v == w)) {
                    v = u;
                    fv = fu;
                }
            }
        }

        // If we get here then we did not converge. This really should never occur
        // with this algorithm, unless perhaps we get something that cycles back
        // and forth.
        throw no_convergence();
    }
}

float kss::math::minimum_value(float ax, float bx, float cx, float& xmin,
                               std::function<float(float)>& fn, float tol1, float fbx)
{
    return _minimum_value<float>(ax, bx, cx, xmin, fn, tol1, fbx);
}

double kss::math::minimum_value(double ax, double bx, double cx, double& xmin,
                                std::function<double(double)>& fn, double tol1, double fbx)
{
    return _minimum_value<double>(ax, bx, cx, xmin, fn, tol1, fbx);
}

long double kss::math::minimum_value(long double ax, long double bx, long double cx, long double& xmin,
                                     std::function<long double(long double)>& fn,
                                     long double tol1, long double fbx)
{
    return _minimum_value<long double>(ax, bx, cx, xmin, fn, tol1, fbx);
}

