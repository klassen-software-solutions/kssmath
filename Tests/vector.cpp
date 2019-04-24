//
//  vector.cpp
//  unittest
//
//  Created by Steven W. Klassen on 2019-04-04.
//  Copyright © 2019 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include "ksstest.hpp"

#include <kss/math/vector.hpp>

#include <algorithm>
#include <array>
#include <iostream>
#include <limits>
#include <valarray>
#include <vector>

using namespace std;
using namespace kss::math::la;
using namespace kss::test;

namespace {
    // "Identical" test vectors of each underlying implementation.
    static const array<int, 5> arVec = { 1, 2, 3, 4, 5 };

    static int arPtr[] = { 1, 2, 3, 4, 5, 6 };
    static const VectorPtr<int, 5> ptrVec(arPtr);

    static vector<int> vec = { 1, 2, 3, 4, 5, 6, 7 };
    static const VectorVec<int, 5> vecVec(vec);

    static valarray<int> va1 = { 1, 2, 3, 4, 5 };
    static valarray<int> va2 = { 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
    static slice va2Slice = slice(0, 5, 2);
    static const VectorVA<int, 5> va1Vec(va1);
    static const VectorVA<int, 5> va2Vec(va2, va2Slice);

    template <class Vector>
    void testAddition(const Vector& inputVec) {
        // operator +=(Vector, Value)
        {
            const auto answer = array<int, 5>{ 3, 4, 5, 6, 7 };

            KSS_ASSERT(isTrue([&] {
                auto v = arVec;
                v += 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                int ar[] = { 1, 2, 3, 4, 5 };
                VectorPtr<int, 5> v(ar);
                v += 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto vec1 = vec;
                VectorVec<int, 5> v(vec1);
                v += 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va1;
                VectorVA<int, 5> v(va);
                v += 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va2;
                VectorVA<int, 5> v(va, va2Slice);
                v += 2;
                auto ans2 = va2;
                ans2[va2Slice] = valarray<int>(answer.data(), answer.size());
                return (v == answer && equal(begin(va), end(va), begin(ans2)));
            }));
        }

        // operator +=(Vector, Vector)
        {
            const auto rhs = array<int, 5>{ 1, -1, 2, -2, 3 };
            const auto answer = array<int, 5>{ 2, 1, 5, 2, 8 };

            KSS_ASSERT(isTrue([&] {
                auto v = arVec;
                v += rhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto vec1 = vec;
                VectorVec<int, 5> v(vec1);
                v += rhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va1;
                VectorVA<int, 5> v(va);
                v += rhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va2;
                VectorVA<int, 5> v(va, va2Slice);
                v += rhs;
                auto ans2 = va2;
                ans2[va2Slice] = valarray<int>(answer.data(), answer.size());
                return (v == answer && equal(begin(va), end(va), begin(ans2)));
            }));
        }

        // operator +(Vector, Vector>
        {
            const auto lhs = array<int, 5>{ 1, -1, 2, -2, 3 };
            const auto answer = array<int, 5>{ 2, 1, 5, 2, 8 };

            KSS_ASSERT(isTrue([&] {
                auto v = arVec + lhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto v = vecVec + lhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto v = va1Vec + lhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto v = va2Vec + lhs;
                return v == answer;
            }));
        }
    }

    template <class Vector>
    void testSubtraction(const Vector& inputVec) {
        // operator -=(Vector, Value)
        {
            const auto answer = array<int, 5>{ -1, 0, 1, 2, 3 };

            KSS_ASSERT(isTrue([&] {
                auto v = arVec;
                v -= 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                int ar[] = { 1, 2, 3, 4, 5 };
                VectorPtr<int, 5> v(ar);
                v -= 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto vec1 = vec;
                VectorVec<int, 5> v(vec1);
                v -= 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va1;
                VectorVA<int, 5> v(va);
                v -= 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va2;
                VectorVA<int, 5> v(va, va2Slice);
                v -= 2;
                auto ans2 = va2;
                ans2[va2Slice] = valarray<int>(answer.data(), answer.size());
                return (v == answer && equal(begin(va), end(va), begin(ans2)));
            }));
        }

        // operator +=(Vector, Vector)
        {
            const auto rhs = array<int, 5>{ 1, -1, 2, -2, 3 };
            const auto answer = array<int, 5>{ 0, 3, 1, 6, 2 };

            KSS_ASSERT(isTrue([&] {
                auto v = arVec;
                v -= rhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto vec1 = vec;
                VectorVec<int, 5> v(vec1);
                v -= rhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va1;
                VectorVA<int, 5> v(va);
                v -= rhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va2;
                VectorVA<int, 5> v(va, va2Slice);
                v -= rhs;
                auto ans2 = va2;
                ans2[va2Slice] = valarray<int>(answer.data(), answer.size());
                return (v == answer && equal(begin(va), end(va), begin(ans2)));
            }));
        }

        // operator +(Vector, Vector>
        {
            const auto lhs = array<int, 5>{ 1, -1, 2, -2, 3 };
            const auto answer = array<int, 5>{ 0, 3, 1, 6, 2 };

            KSS_ASSERT(isTrue([&] {
                auto v = arVec - lhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto v = vecVec - lhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto v = va1Vec - lhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto v = va2Vec - lhs;
                return v == answer;
            }));
        }
    }

    template <class Vector>
    void testMultiplication(const Vector& inputVec) {
        // operator *=(Vector, Value)
        {
            const auto answer = array<int, 5>{ 2, 4, 6, 8, 10 };

            KSS_ASSERT(isTrue([&] {
                auto v = arVec;
                v *= 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                int ar[] = { 1, 2, 3, 4, 5 };
                VectorPtr<int, 5> v(ar);
                v *= 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto vec1 = vec;
                VectorVec<int, 5> v(vec1);
                v *= 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va1;
                VectorVA<int, 5> v(va);
                v *= 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va2;
                VectorVA<int, 5> v(va, va2Slice);
                v *= 2;
                auto ans2 = va2;
                ans2[va2Slice] = valarray<int>(answer.data(), answer.size());
                return (v == answer && equal(begin(va), end(va), begin(ans2)));
            }));
        }

        // operator *=(Vector, Vector)
        {
            const auto rhs = array<int, 5>{ 1, -1, 2, -2, 3 };
            const auto answer = array<int, 5>{ 1, -2, 6, -8, 15 };

            KSS_ASSERT(isTrue([&] {
                auto v = arVec;
                v *= rhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto vec1 = vec;
                VectorVec<int, 5> v(vec1);
                v *= rhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va1;
                VectorVA<int, 5> v(va);
                v *= rhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va2;
                VectorVA<int, 5> v(va, va2Slice);
                v *= rhs;
                auto ans2 = va2;
                ans2[va2Slice] = valarray<int>(answer.data(), answer.size());
                return (v == answer && equal(begin(va), end(va), begin(ans2)));
            }));
        }

        // operator *(Vector, Vector>
        {
            const auto lhs = array<int, 5>{ 1, -1, 2, -2, 3 };
            const auto answer = array<int, 5>{ 1, -2, 6, -8, 15 };

            KSS_ASSERT(isTrue([&] {
                auto v = arVec * lhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto v = vecVec * lhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto v = va1Vec * lhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto v = va2Vec * lhs;
                return v == answer;
            }));
        }
    }

    template <class Vector>
    void testDivision(const Vector& inputVec) {
        // operator /=(Vector, Value)
        {
            const auto answer = array<int, 5>{ 0, 1, 1, 2, 2 };

            KSS_ASSERT(isTrue([&] {
                auto v = arVec;
                v /= 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                int ar[] = { 1, 2, 3, 4, 5 };
                VectorPtr<int, 5> v(ar);
                v /= 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto vec1 = vec;
                VectorVec<int, 5> v(vec1);
                v /= 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va1;
                VectorVA<int, 5> v(va);
                v /= 2;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va2;
                VectorVA<int, 5> v(va, va2Slice);
                v /= 2;
                auto ans2 = va2;
                ans2[va2Slice] = valarray<int>(answer.data(), answer.size());
                return (v == answer && equal(begin(va), end(va), begin(ans2)));
            }));
        }

        // operator /=(Vector, Vector)
        {
            const auto rhs = array<int, 5>{ 1, -1, 2, -2, 3 };
            const auto answer = array<int, 5>{ 1, -2, 1, -2, 1 };

            KSS_ASSERT(isTrue([&] {
                auto v = arVec;
                v /= rhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto vec1 = vec;
                VectorVec<int, 5> v(vec1);
                v /= rhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va1;
                VectorVA<int, 5> v(va);
                v /= rhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto va = va2;
                VectorVA<int, 5> v(va, va2Slice);
                v /= rhs;
                auto ans2 = va2;
                ans2[va2Slice] = valarray<int>(answer.data(), answer.size());
                return (v == answer && equal(begin(va), end(va), begin(ans2)));
            }));
        }

        // operator *(Vector, Vector>
        {
            const auto lhs = array<int, 5>{ 1, -1, 2, -2, 3 };
            const auto answer = array<int, 5>{ 1, -2, 1, -2, 1 };

            KSS_ASSERT(isTrue([&] {
                auto v = arVec / lhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto v = vecVec / lhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto v = va1Vec / lhs;
                return v == answer;
            }));

            KSS_ASSERT(isTrue([&] {
                auto v = va2Vec / lhs;
                return v == answer;
            }));
        }
    }

    template <class Vector>
    void testSum(const Vector& a) {
        // Note the conversion to int drops all the decimal points.
        KSS_ASSERT(isEqualTo<int>(21, [&] {
            return sum<int>(a);
        }));
        KSS_ASSERT(isTrue([&] {
            const auto s = sum<long double>(a);
            return (abs(s - 22.31) < 1e-6);
        }));
    }
}

static TestSuite ts("la::vector", {
    make_pair("equality", [] {
        KSS_ASSERT(toString(arVec) == "(1,2,3,4,5)");

        KSS_ASSERT(arVec == ptrVec);
        KSS_ASSERT(arVec == vecVec);
        KSS_ASSERT(arVec == va1Vec);
        KSS_ASSERT(arVec == va2Vec);

        array<int, 5> veca = { 1, -2, 3, 4, 5 };
        KSS_ASSERT(veca != arVec);
        KSS_ASSERT(veca != ptrVec);
        KSS_ASSERT(veca != vecVec);
        KSS_ASSERT(veca != va1Vec);
        KSS_ASSERT(veca != va2Vec);
    }),
    make_pair("addition", [] {
        testAddition(arVec);
        testAddition(ptrVec);
        testAddition(vecVec);
        testAddition(va1Vec);
        testAddition(va2Vec);
    }),
    make_pair("subtraction", [] {
        testSubtraction(arVec);
        testSubtraction(ptrVec);
        testSubtraction(vecVec);
        testSubtraction(va1Vec);
        testSubtraction(va2Vec);
    }),
    make_pair("multiplication", [] {
        testMultiplication(arVec);
        testMultiplication(ptrVec);
        testMultiplication(vecVec);
        testMultiplication(va1Vec);
        testMultiplication(va2Vec);
    }),
    make_pair("division", [] {
        testDivision(arVec);
        testDivision(ptrVec);
        testDivision(vecVec);
        testDivision(va1Vec);
        testDivision(va2Vec);
    }),
    make_pair("sum", [] {
        array<float, 6> a { 1, 2.2, 3.9, 4.01, 5.2, 6 };
        testSum(a);
        testSum(VectorPtr<float, 6>(a.data()));

        vector<float> vec(a.begin(), a.end());
        testSum(VectorVec<float, 6>(vec));

        valarray<float> va1(a.data(), 6);
        testSum(VectorVA<float, 6>(va1));

        valarray<float> va2 { 1, 2.2, 3.9, 4.01, 5.2, 6, 1, 2.2, 3.9, 4.01, 5.2, 6 };
        testSum(VectorVA<float, 6>(va2, slice(6, 6, 1)));
    }),
    make_pair("dotProduct", [] {
        KSS_ASSERT(isEqualTo<long double>(16, [] {
            array<double, 4> a { 1.0, 2.0, 3.0, 2.0 };
            valarray<double> b { 4.0, -5.0, 6.0, 2.0 };
            return dotProduct<long double>(a, VectorVA<double, 4>(b));
        }));
    }),
    make_pair("norm", [] {
        KSS_ASSERT(isEqualTo<long double>(4, [] {
            array<double, 5> a { 1.0, 2.0, 3.0, 1.0, 1.0 };
            return norm<long double>(a);
        }));
    })
});
