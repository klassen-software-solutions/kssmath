//
//  geospatial.cpp
//  unittest
//
//  Created by Steven W. Klassen on 2017-03-20.
//  Copyright © 2017 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include "ksstest.hpp"

#include <kss/math/geospatial.hpp>

#include <iostream>
#include <vector>

using namespace std;
using namespace kss::math::geos;
using namespace kss::test;


static TestSuite ts("geos::geospatial", {
    make_pair("Point", [] {
        KSS_ASSERT(isTrue([] {
            Point p;
            return (p.latitude() == 0
                    && p.longitude() == 0
                    && string(p) == "(0,0)"
                    && p.gis() == "POINT(0 0)"
                    && p.dms() == "0º 0' 0\"N, 0º 0' 0\"E");
        }));

        KSS_ASSERT(isTrue([] {
            Point p(40, 110);
            return (p.latitude() == 40
                    && p.longitude() == 110
                    && string(p) == ("(40,110)")
                    && p.gis() == "POINT(110 40)"
                    && p.dms() == "40º 0' 0\"N, 110º 0' 0\"E");
        }));

        const Point ans(40, 110);
        KSS_ASSERT(isTrue([&] {
            Point p = ans;
            return (p == ans);
        }));

        KSS_ASSERT(isTrue([&] {
            Point p("(40,110)");
            return (p == ans);
        }));

        KSS_ASSERT(isTrue([&] {
            Point p("POINT(110 40)");
            return (p == ans);
        }));

        KSS_ASSERT(isTrue([&] {
            Point p(-12.12835, -1.00238);
            p.latitude(40);
            p.longitude(110);
            return (p == ans);
        }));

        KSS_ASSERT(isTrue([&] {
            Point p(-12.12835, -1.00238);
            return (p.dms() == "12º 7' 42.05999999999746564\"S, 1º 0' 8.568000000000175476\"W");
        }));

        KSS_ASSERT(throwsException<out_of_range>([] { Point p(92, 100); }));
        KSS_ASSERT(throwsException<out_of_range>([] { Point p(-92, 100); }));
        KSS_ASSERT(throwsException<out_of_range>([] { Point p(80, 181); }));
        KSS_ASSERT(throwsException<out_of_range>([] { Point p(80, -181); }));

        KSS_ASSERT(throwsException<invalid_argument>([] { Point p("invalid string"); }));
        KSS_ASSERT(throwsException<invalid_argument>([] { Point p("(xxx, 100)"); }));
        KSS_ASSERT(throwsException<invalid_argument>([] { Point p("(10, xxx)"); }));
        KSS_ASSERT(throwsException<invalid_argument>([] { Point p("POINT(xxx 10)"); }));
        KSS_ASSERT(throwsException<invalid_argument>([] { Point p("POINT(100 xxx)"); }));
        KSS_ASSERT(throwsException<out_of_range>([] { Point p("(92, 100)"); }));
        KSS_ASSERT(throwsException<out_of_range>([] { Point p("POINT(-181 80)"); }));

        Point p;
        KSS_ASSERT(throwsException<out_of_range>([&] { p.latitude(91); }));
        KSS_ASSERT(throwsException<out_of_range>([&] { p.latitude(-91); }));
        KSS_ASSERT(throwsException<out_of_range>([&] { p.longitude(185); }));
        KSS_ASSERT(throwsException<out_of_range>([&] { p.longitude(-186); }));
    }),
    make_pair("Point computations", [] {
        Point p1(51.06707497, -1.32007599);
        Point p2(51.09430508, -1.31192207);
        Point p3(51.36283147, -0.4553318);
        Point p4(-1, 1);
        KSS_ASSERT(isCloseTo<double>(3081., 0.5, [&] { return distance(p1, p2, 6371000); }));
        KSS_ASSERT(isCloseTo<double>(68624., 0.5, [&] { return distance(p1, p3, 6371000); }));
        KSS_ASSERT(isCloseTo<double>(5793754., 0.5, [&] { return distance(p1, p4, 6371000); }));

        KSS_ASSERT(areClose(intermediatePoint(p1, p2, 0.5, 6371000.0), Point(51.0806901, -1.3160002)));
        KSS_ASSERT(areClose(intermediatePoint(p1, p3, 0.5, 6371000.0), Point(51.2157498, -0.8890926)));
        KSS_ASSERT(areClose(intermediatePoint(p1, p4, 0.5, 6371000.0), Point(25.0378061, 0.1046237)));

        KSS_ASSERT(distance(p1, p1) == 0.0);
        KSS_ASSERT(intermediatePoint(p1, p1, 0.2) == p1);
    }),
    make_pair("Path computations", [] {
        vector<Point> path({
            Point(51.06707497,    -1.32007599),
            Point(51.09430508,    -1.31192207),
            Point(51.10206677,    -1.30926132),
            Point(51.11133597,    -1.30376816),
            Point(51.12981493,    -1.29261017),
            Point(51.15906713,    -1.27510071),
            Point(51.16440941,    -1.27057314),
            Point(51.16897072,    -1.26606703),
            Point(51.17439257,    -1.26235485),
            Point(51.17875111,    -1.26089573),
            Point(51.1833917,    -1.26044512),
            Point(51.19727033,    -1.25793457),
            Point(51.20141159,    -1.25669003),
            Point(51.20630532,    -1.25347137),
            Point(51.21110444,    -1.24845028),
            Point(51.22457158,    -1.23325825),
            Point(51.22821321,    -1.2274003),
            Point(51.23103494,    -1.22038364),
            Point(51.23596583,    -1.20326042),
            Point(51.24346193,    -1.1776185),
            Point(51.24968088,    -1.16356373),
            Point(51.26363353,    -1.13167763),
            Point(51.2659966,    -1.12247229),
            Point(51.26682901,    -1.11629248),
            Point(51.26728549,    -1.10906124),
            Point(51.26823871,    -1.09052181),
            Point(51.26885628,    -1.08522177),
            Point(51.27070895,    -1.07013702),
            Point(51.27350122,    -1.03683472),
            Point(51.27572955,    -1.00917578),
            Point(51.2779175,    -0.98243952),
            Point(51.28095094,    -0.9509182),
            Point(51.28305811,    -0.9267354),
            Point(51.28511151,    -0.90499878),
            Point(51.2883055,    -0.86051702),
            Point(51.29023789,    -0.83661318),
            Point(51.29708113,    -0.7534647),
            Point(51.29795323,    -0.74908733),
            Point(51.2988924,    -0.7400322),
            Point(51.30125366,    -0.71535587),
            Point(51.29863749,    -0.68475723),
            Point(51.30220618,    -0.65746307),
            Point(51.30380261,    -0.63246489),
            Point(51.30645873,    -0.60542822),
            Point(51.3103219,    -0.58150291),
            Point(51.31150225,    -0.57603121),
            Point(51.31317883,    -0.57062387),
            Point(51.32475227,    -0.54195642),
            Point(51.34771616,    -0.4855442),
            Point(51.36283147,    -0.4553318)
        });

        KSS_ASSERT(isCloseTo<double>(76399.0, 0.5, [&] { return pathLength(path, 6371000); }));
        KSS_ASSERT(areClose(pathIntermediatePoint(path, 0.25), Point(51.2267572, -1.2297426)));
    })
});
