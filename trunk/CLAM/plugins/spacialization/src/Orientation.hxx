#ifndef Orientation_hxx 
#define Orientation_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>
#include <iomanip>
#define PI 3.14159265

namespace CLAM
{

/**
 Represents spherical orientation of a point
 from a given reference frame expressed as the
 azimuth and the elevation angles in degrees.

 @see AmbisonicsConventions
*/
struct Orientation
{
	double azimuth;
	double elevation;

	Orientation(double anAzimuth=0.0, double anElevation=0.0)
		: azimuth(anAzimuth)
		, elevation(anElevation)
	{}
	/**
	 Converts the orientation values for azimuth and elevation to be
	 within the ranges according AmbisonicsConventions.
	*/
	void normalize()
	{
		elevation += 90;
		elevation = _module(elevation, 360);
		if (elevation>180)
		{
			azimuth += 180;
			elevation = 360 - elevation;
		}
		azimuth = _module(azimuth, 360.);
		elevation -= 90;
	}
	void toPoint(double x, double y, double z)
	{
		azimuth = 180./M_PI*std::atan2(y,x);
		elevation = 180./M_PI*std::asin(z/std::sqrt(x*x+y*y+z*z));
		normalize();
	}

	bool operator!=(const Orientation & other) const
	{
		if (fabs(azimuth-other.azimuth)>1e-14) return true;
		if (fabs(elevation-other.elevation)>1e-14) return true;
		return false;
	}
	operator std::string() const
	{
		std::ostringstream os;
		os << *this;
		return os.str();
	}
	friend std::ostream & operator<<(std::ostream & os, const Orientation & orientation)
	{
		return os
			<< std::setprecision(20)
			<< orientation.azimuth
			<< " "
			<< orientation.elevation
			;
	}

private:
	double _module(double input, double factor)
	{
		while (input<0) input+=factor;
		return fmod(input, factor);
	}
};


} // namespace
#endif // Orientation_hxx
