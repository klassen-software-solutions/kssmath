//
//  closeto.cpp
//  unittest
//
//  Created by Steven W. Klassen on 2013-01-11.
//  Copyright (c) 2013 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include "ksstest.hpp"

#include <kss/math/closeto.hpp>

using namespace std;
using namespace kss::math;
using namespace kss::test;


namespace {
    template <class T>
    void test_close_to(const T& arg_type) {
        KSS_ASSERT(closeTo(T(1), T(3), T(5)));
        KSS_ASSERT(!closeTo(T(1), T(300), T(5)));
        
        KSS_ASSERT(closeTo(T(1), T(1)));
        KSS_ASSERT(!closeTo(T(1), T(2)));
    }

    // Data type for which <=, = and - are defined but numeric_limits is not.
    class myint {
    public:
        explicit myint(int i = 0) { _i = i; }
        myint& operator=(const myint& i) { _i = i._i; return *this; }
        bool operator<=(const myint& i) const { return _i <= i._i; }
        myint operator-(const myint& i) const { return myint(_i - i._i); }
        
    private:
        int _i;
    };
}

static TestSuite ts("::closeto", {
    make_pair("closeTo", [] {
        test_close_to(unsigned());
        test_close_to(int());
        test_close_to((long int)0);
        test_close_to(float());
        test_close_to(double());
        test_close_to((long double)0);
        test_close_to(myint(0));
    })
});
