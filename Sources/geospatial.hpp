//
//  geospatial.hpp
//  kssmath
//
//  Created by Steven W. Klassen on 2017-03-17.
//  Copyright © 2017 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#ifndef kssmath_geospatial_hpp
#define kssmath_geospatial_hpp

#include "point.hpp"

#include <stdexcept>
#include <string>


namespace kss { namespace math { namespace geos {

    /*!
     A geospatial point in our definition is a two-dimensional, double precision
     point representing a latitude and a longitude.

     We implement this as a 2D double precision point where the first value (x axis) is
     the longitude and the second (y axis) is the latitude. In addition the possible
     values are limited to the following:
     -90 <= latitude <= +90
     -180 <= longitude <= +180

     */
    class Point : public kss::math::geom::Point<double, 2> {
    public:
        Point() = default;
        Point(const Point&) = default;
        Point(Point&&) = default;
        ~Point() = default;

        Point& operator=(const Point&) = default;
        Point& operator=(Point&&) = default;

        /*!
         Construct a point from a textual description.

         Note that the string format can accept "(<latitude>, <longitude>)" or
         the GIS format of "POINT(<longitude> <latitude>)". If the string cannot be
         parsed in one of these two formats a std::invalid_argument exception will
         be thrown.
         */
        explicit Point(const std::string& s);

        /*!
         Construct a point from a raw latitude and longitude.
         @throws out_of_range if lat or lng are not in the valid range
         */
        Point(double lat, double lng);

        double latitude() const noexcept    { return operator[](1); }
        double longitude() const noexcept   { return operator[](0); }
        void latitude(double lat);
        void longitude(double lng);

        /*!
         Cast to a string. Used mostly for debugging. The format of this
         will be (<latitude>, <longitude>).
         */
        operator std::string() const noexcept;

        /*!
         Write out as a GIS string. The GIS string will be of the format
         "POINT(<longitude> <latitude>)" and should be suitable for use with
         PostGIS or other similar GIS DBs.
         */
        std::string gis() const noexcept;

        /*!
         Write out a string showing that latitude,longitude in the degree-minutes-seconds
         format.
         */
        std::string dms() const noexcept;
    };

    /*!
     The default value used for the diameter of the earth, measured in metres.
     This is the same as the default value used in PostGIS.
     */
    static constexpr double DEFAULT_DIAMETER_OF_THE_EARTH_IN_M = 6370986.0;


    /*!
     Returns true if the two points are close to each other. "Close to" is defined
     as the distance between them being less than the given epsilon. (The default
     value for epsilon being within one metre.)
     @throws invalid_argument if epsilon <= 0.
     */
    bool areClose(const Point& p1, const Point& p2,
                  double epsilon = 1.,
                  double diameterOfTheEarth = DEFAULT_DIAMETER_OF_THE_EARTH_IN_M);

    /*!
     Compute the distance between two points. This uses the haversine formula
     assuming a spherical earth with the given diameter. The default diameter
     is the one used by default in PostGIS.

     @return the distance along the sphere of the two points, returned in metres.
     */
    double distance(const Point& p1, const Point& p2,
                    double diameterOfTheEarth = DEFAULT_DIAMETER_OF_THE_EARTH_IN_M);

    /*!
     Determine an intermediate point as a fraction of the distance between two points.
     Note that fractionalDistance must be in the range [0,1] where 0 would return p1
     and 1 would return p2.

     @return the intermediate point
     @throws invalid_argument if fractionalDistance is not in the range [0,1].
     */
    Point intermediatePoint(const Point& p1, const Point& p2, double fractionalDistance,
                            double diameterOfTheEarth = DEFAULT_DIAMETER_OF_THE_EARTH_IN_M);


    /*!
     Compute the length of a path defined by an ordered set of points. Note that
     Path must be a container type that supports forward iterators and contains
     only point objects.

     @return the distance along the path, measured in metres.
     */
    template <class Path>
    double pathLength(const Path& path,
                      double diameterOfTheEarth = DEFAULT_DIAMETER_OF_THE_EARTH_IN_M)
    {
        double currentLength = 0.0;
        auto it = path.begin();
        auto endIt = path.end();
        if (it != endIt) {
            Point previousPoint = *it;
            ++it;
            while (it != endIt) {
                Point p = *it;
                currentLength += distance(previousPoint, p, diameterOfTheEarth);
                previousPoint = p;
                ++it;
            }
        }
        return currentLength;
    }

    /*!
     Compute the intermediate point found at the fractional distance along the path.
     Note that fractionalDistance must be in the range [0,1] where 0 returns the
     first point of the path and 1 returns the last.

     @return the intermediate point
     @throws invalid_argument if fractionalDistance is not in the range [0,1].
     @throws invalid_argument if the path contains no points.
     */
    template <class Path>
    Point pathIntermediatePoint(const Path& path, double fractionalDistance,
                                double diameterOfTheEarth = DEFAULT_DIAMETER_OF_THE_EARTH_IN_M)
    {
        if (fractionalDistance < 0.0 || fractionalDistance > 1.0) {
            throw std::invalid_argument("fractionalDistance must be in the range [0,1]");
        }
        if (path.empty()) {
            throw std::invalid_argument("cannot dertermine an intermediate point on an empty path");
        }

        auto it = path.begin();
        auto endIt = path.end();
        Point previousPoint = *it;
        ++it;
        if (it == endIt) {      // A path with a single point will simply return that point.
            return previousPoint;
        }

        // We now know there are at least two points. If fractionalDistance is
        // either 0 or 1, we can simply return the proper endpoint.
        if (fractionalDistance == 0.0) {
            return previousPoint;
        }
        if (fractionalDistance == 1.0) {
            return path.back();
        }

        // Find the two points that bracket our desired distance.
        const double fullLengthOfPath = pathLength(path, diameterOfTheEarth);
        double lengthToIntermediatePoint = fullLengthOfPath * fractionalDistance;
        while (it != endIt) {
            Point p = *it;
            double dist = distance(previousPoint, p, diameterOfTheEarth);
            lengthToIntermediatePoint -= dist;
            if (lengthToIntermediatePoint == 0.0) {
                return p;
            }
            else if (lengthToIntermediatePoint < 0.0) {
                double fraction = (lengthToIntermediatePoint + dist) / dist;
                return intermediatePoint(previousPoint, p, fraction, diameterOfTheEarth);
            }
            else {
                previousPoint = p;
                ++it;
            }
        }

        // It should not be possible to get here.
        throw std::runtime_error("It should not be possible to reach this point.");
    }
}}}

#endif
