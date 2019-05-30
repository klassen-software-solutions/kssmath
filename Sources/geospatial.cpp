//
//  geospatial.cpp
//  kssmath
//
//  Created by Steven W. Klassen on 2017-03-17.
//  Copyright © 2017 Klassen Software Solutions. All rights reserved.
//  Licensing follows the MIT License.
//

#include "_contract.hpp"
#include "constants.hpp"
#include "geospatial.hpp"
#include "radians.hpp"

#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;
using namespace kss::math::geos;
using kss::math::Constants;
using kss::math::geom::toDegrees;
using kss::math::geom::toRadians;
namespace contract = kss::math::_private::contract;


// MARK: internal utilities
namespace {
    static constexpr double MIN_DIAMETER_OF_THE_EARTH_IN_M = 6370000.0;

    void set_precision(ostringstream& oss) noexcept {
        // See http://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html for a description
        // of why the "+2" should be used.
        oss.precision(numeric_limits<double>::max_digits10 + 2);
    }

    struct dms_representation {
        unsigned degrees;
        unsigned minutes;
        double seconds;
        char direction;
    };

    void to_dms(double val, dms_representation& dms, char positiveDirection, char negativeDirection) noexcept {
        dms.direction = (val >= 0. ? positiveDirection : negativeDirection);
        val = abs(val);
        dms.degrees = unsigned(val);
        dms.minutes = unsigned((val - dms.degrees) * 60.);
        dms.seconds = (val - dms.degrees - dms.minutes/60.) * 3600.;
    }

    ostringstream& write_dms(ostringstream& oss, const dms_representation& dms) noexcept {
        oss << dms.degrees << "º " << dms.minutes << "' " << dms.seconds << "\"" << dms.direction;
        return oss;
    }

    inline constexpr double squared(double d) noexcept {
        return (d*d);
    }

    inline double normalize_longitude(double lon) noexcept {
        return fmod(lon+540., 360.) - 180.;
    }

    inline bool isValid(const Point& p) noexcept {
        return (p.latitude() >= -90 && p.latitude() <= 90
                && p.longitude() >= -180 && p.longitude() <= 180);
    }

    bool kss_starts_with(const char* str, const char* prefix) {
        assert(str != nullptr);
        assert(prefix != nullptr);

        /* All strings - even empty ones - start with an empty prefix. */
        size_t plen = strlen(prefix);
        if (!plen) {
            return true;
        }

        /* Empty strings, or strings shorter than the prefix, cannot start with the prefix. */
        size_t slen = strlen(str);
        if (slen < plen) {
            return false;
        }

        /* Compare the start of the string to see if it matches the prefix. */
        return (strncmp(str, prefix, plen) == 0);
    }

    inline bool startsWith(const string &str, const string &prefix) noexcept {
        return kss_starts_with(str.c_str(), prefix.c_str());
    }

    inline void throwCouldNotParse(const string& s) {
        throw invalid_argument("could not parse " + s + " as a Point");
    }

    void parseInternalStringFormat(Point& p, const string& s) {
        if (s.empty()) {
            throwCouldNotParse(s);
        }

        const auto len = s.size();
        if (s[0] != '(' || s[len-1] != ')') {
            throwCouldNotParse(s);
        }

        const auto commaPos = s.find(',', 1);
        if (commaPos == string::npos) {
            throwCouldNotParse(s);
        }

        const auto latStr = s.substr(1, commaPos-1);
        const auto lngStr = s.substr(commaPos+1, len-commaPos-2);
        p.latitude(stod(latStr));
        p.longitude(stod(lngStr));
    }

    void parseGisStringFormat(Point& p, const string& s) {
        // the following assertions have already been checked at this point
        assert(!s.empty());
        assert(startsWith(s, "POINT("));

        const auto len = s.size();
        if (s[len-1] != ')') {
            throwCouldNotParse(s);
        }

        const size_t prefixLen = strlen("POINT(");
        const auto spacePos = s.find(' ', prefixLen);
        if (spacePos == string::npos) {
            throwCouldNotParse(s);
        }

        const auto latStr = s.substr(spacePos+1, len-spacePos-2);
        const auto lngStr = s.substr(prefixLen, spacePos-prefixLen);
        p.latitude(stod(latStr));
        p.longitude(stod(lngStr));
    }
}

// MARK: point implementation

Point::Point(const std::string& s) {
    contract::parameters({
        KSS_EXPR(!s.empty())
    });

    if (startsWith(s, "POINT(")) {
        parseGisStringFormat(*this, s);
    }
    else {
        parseInternalStringFormat(*this, s);
    }

    contract::postconditions({
        KSS_EXPR(isValid(*this))
    });
}

Point::Point(double lat, double lng) {
    latitude(lat);
    longitude(lng);

    contract::postconditions({
        KSS_EXPR(isValid(*this))
    });
}

void Point::latitude(double lat) {
    if (lat < -90 || lat > 90) {
        throw out_of_range("latitude must be in the range [-90,90]");
    }
    operator[](1) = lat;
}

void Point::longitude(double lng) {
    if (lng < -180 || lng > 180) {
        throw out_of_range("longitude must be in the range [-180,180]");
    }
    operator[](0) = lng;
}

Point::operator string() const noexcept {
    contract::preconditions({
        KSS_EXPR(isValid(*this))
    });

    ostringstream oss;
    set_precision(oss);
    oss << "(" << latitude() << "," << longitude() << ")";
    return oss.str();
}

string Point::gis() const noexcept {
    contract::preconditions({
        KSS_EXPR(isValid(*this))
    });

    ostringstream oss;
    set_precision(oss);
    oss << "POINT(" << longitude() << " " << latitude() << ")";
    return oss.str();
}

string Point::dms() const noexcept {
    contract::preconditions({
        KSS_EXPR(isValid(*this))
    });

    ostringstream oss;
    set_precision(oss);

    dms_representation dmsr;
    to_dms(latitude(), dmsr, 'N', 'S');
    write_dms(oss, dmsr);

    oss << ", ";
    to_dms(longitude(), dmsr, 'E', 'W');
    write_dms(oss, dmsr);
    return oss.str();
}


// MARK: point computations

bool kss::math::geos::areClose(const Point& p1, const Point& p2, double epsilon, double diameterOfTheEarth) {
    contract::parameters({
        KSS_EXPR(epsilon > 0.)
    });

    return (distance(p1, p2, diameterOfTheEarth) <= epsilon);
}

// Compute the distance between two points using the haversine formula. This is based on the
// description found at http://www.movable-type.co.uk/scripts/latlong.html.
double kss::math::geos::distance(const Point& p1, const Point& p2, double diameterOfTheEarth) {
    contract::parameters({
        KSS_EXPR(diameterOfTheEarth > MIN_DIAMETER_OF_THE_EARTH_IN_M)
    });
    contract::preconditions({
        KSS_EXPR(isValid(p1)),
        KSS_EXPR(isValid(p2))
    });

    const double lat1 = p1.latitude();
    const double lon1 = p1.longitude();
    const double lat2 = p2.latitude();
    const double lon2 = p2.longitude();

    const double R = diameterOfTheEarth;
    const double phi1 = toRadians(lat1);
    const double phi2 = toRadians(lat2);
    const double deltaPhi = toRadians(lat2 - lat1);
    const double deltaLambda = toRadians(lon2 - lon1);

    const double a = squared(sin(deltaPhi/2.)) + (cos(phi1) * cos(phi2)) * squared(sin(deltaLambda/2.));
    const double c = 2. * atan2(sqrt(a), sqrt(1-a));
    const double d = R * c;

    // Distance should never be more than 1/2 the circumference of the earth.
    contract::conditions({
        KSS_EXPR(d <= (diameterOfTheEarth * Constants<double>::PI / 2))
    });
    return d;
}

// Compute the intermediate between two points. This is based on the description found
// at http://www.movable-type.co.uk/scripts/latlong.html.
Point kss::math::geos::intermediatePoint(const Point& p1, const Point& p2,
                                         double fractionalDistance,
                                         double diameterOfTheEarth)
{
    contract::parameters({
        KSS_EXPR(fractionalDistance >= 0 && fractionalDistance <= 1),
        KSS_EXPR(diameterOfTheEarth > MIN_DIAMETER_OF_THE_EARTH_IN_M)
    });
    contract::preconditions({
        KSS_EXPR(isValid(p1)),
        KSS_EXPR(isValid(p2))
    });

    // If the points are very close to each other, then all intermediate points will be equal
    // and we just return p1.
    if (areClose(p1, p2, (numeric_limits<double>::epsilon()*2), diameterOfTheEarth)) {
        return p1;
    }

    const double f = fractionalDistance;
    const double R = diameterOfTheEarth;
    const double d = distance(p1, p2, diameterOfTheEarth);
    const double phi1 = toRadians(p1.latitude());
    const double lambda1 = toRadians(p1.longitude());
    const double phi2 = toRadians(p2.latitude());
    const double lambda2 = toRadians(p2.longitude());

    const double delta = d / R;
    const double a = sin((1-f)*delta) / sin(delta);
    const double b = sin(f*delta) / sin(delta);
    const double x = (a * cos(phi1) * cos(lambda1)) + (b * cos(phi2) * cos(lambda2));
    const double y = (a * cos(phi1) * sin(lambda1)) + (b * cos(phi2) * sin(lambda2));
    const double z = (a * sin(phi1)) + (b * sin(phi2));
    const double phii = atan2(z, sqrt(squared(x) + squared(y)));
    const double lambdai = atan2(y, x);

    return Point(toDegrees(phii), normalize_longitude(toDegrees(lambdai)));
}
