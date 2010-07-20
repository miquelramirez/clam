#ifndef Geometry_hxx
#define Geometry_hxx

#include <cmath>

struct CartesianCoords;
struct SphericalCoords;

inline float circularRotation (float number, float min, float max)
{
	if (max<=min) return number; // Error?
	while (number>max) number -= (max-min);
	while (number<min) number += (max-min);
	return number;
}

inline float mapRange(float number, float fromMin, float fromMax, float toMin, float toMax)
{
	return (number-fromMin)*(toMax-toMin)/(fromMax-fromMin);
}

inline float limitRange(float number, float min, float max)
{
	if (number<min) return min;
	if (number>max) return max;
	return number;
}

struct SphericalCoords
{
	float a;
	float e;
	float d;
	void canonicalize()
	{
		e = circularRotation(e, -180., +180.);
		if (e>+90.)
		{
			e=+180.-e;
			a+=180.;
		}
		if (e<-90.)
		{
			e=-180.-e;
			a+=180.;
		}
		a = circularRotation(a, -180., +180.);
	}
	SphericalCoords(float _a, float _e, float _d)
		: a(_a)
		, e(_e)
		, d(_d)
		{}
	inline SphericalCoords(const CartesianCoords & cartesian);
private:
};

struct CartesianCoords
{
	CartesianCoords(float _x, float _y, float _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{}
	CartesianCoords(const CartesianCoords & from, const CartesianCoords & to)
		: x(to.x-from.x)
		, y(to.y-from.y)
		, z(to.z-from.z)
	{}
	inline CartesianCoords(const SphericalCoords & spherical);
	float x;
	float y;
	float z;
};

SphericalCoords::SphericalCoords(const CartesianCoords & cartesian)
{
	const float & x = cartesian.x;
	const float & y = cartesian.y;
	const float & z = cartesian.z;
	d = std::sqrt(x*x+y*y+z*z);
	if (d<.00001)
	{
		a = e = 0;
		return;
	}
	a = std::atan2(y,x) * 180 / M_PI;
	e = std::asin(z/d) * 180 / M_PI;
}

CartesianCoords::CartesianCoords(const SphericalCoords & spherical)
{
	float a = spherical.a * M_PI / 180;
	float e = spherical.e * M_PI / 180;
	const float & d = spherical.d;
	double ce = std::cos(e);
	double ca = std::cos(a);
	double se = std::sin(e);
	double sa = std::sin(a);
	x = d * ce * ca;
	y = d * ce * sa;
	z = d * se;
}






#endif//Geometry_hxx

