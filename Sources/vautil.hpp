//
//  vautil.hpp
//  kssmath
//
//  Created by Steven W. Klassen on 2019-04-20.
//  Copyright © 2019 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#ifndef kssmath_vautil_hpp
#define kssmath_vautil_hpp

#include <cassert>
#include <valarray>

namespace kss { namespace math {

    /*!
     Construct a valarray from another container, potentially casting member types.
     */
    template <class Result, class RandomAccessIterator>
    std::valarray<Result> makeValArray(RandomAccessIterator first, RandomAccessIterator last) {
        // preconditions
        const auto n = last - first;
        assert(n >= 0);

        std::valarray<Result> va(n);
        size_t i = 0;
        for (auto it = first; it != last; ++it, ++i) {
            va[i] = Result(*it);
        }

        // postconditions
        assert(va.size() == n);
        return va;
    }

    /*!
     Construct a valarray from another valarray, presumably changing element types.
     (Otherwise you may as well use an assignment operator.)
     */
    template <class Result, class Source>
    std::valarray<Result> makeValArray(const std::valarray<Source>& v) {
        const size_t len = v.size();
        std::valarray<Result> va(len);
        for (size_t i = 0; i < v.size(); ++i) {
            va[i] = Result(v[i]);
        }

        // postconditions
        assert(va.size() == v.size());
        return va;
    }
}}

#endif
