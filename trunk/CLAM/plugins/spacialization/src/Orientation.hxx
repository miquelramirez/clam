#ifndef Orientation_hxx 
#define Orientation_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>
#include <iomanip>

namespace CLAM
{

/**
 Represents spherical orientation of a point
 from a given reference frame expressed as the
 azimuth and the elevation angles in degrees.

 @see AmbisonicsConventions
 @ingroup SpatialAudio
*/
struct Orientation
{
	double azimuth;
	double elevation;
private:
	double eradians;
	double aradians;
	double ce;
	double se;
	double ca;
	double sa;
public:

	Orientation(double anAzimuth=0.0, double anElevation=0.0)
		: azimuth(anAzimuth)
		, elevation(anElevation)
		, eradians(M_PI*elevation/180.)
		, aradians(M_PI*azimuth/180.)
		, ce(std::cos(eradians))
		, se(std::sin(eradians))
		, ca(std::cos(aradians))
		, sa(std::sin(aradians))
	{}
	/**
	 Converts the orientation values for azimuth and elevation to be
	 within the ranges according @ref AmbisonicsConventions.
	*/
	void normalize()
	{
		elevation += 90.;
		elevation = _module(elevation, 360.);
		if (elevation>180.+1e-13)
		{
			azimuth += 180;
			elevation = 360 - elevation;
		}
		azimuth = _module(azimuth, 360.);
		elevation -= 90;
	}
	/**
	 Set the orientation to the one of the vector (x,y,z)
	 For vectors of module less than 1e-12 elevation is set to z.
	*/
	void toPoint(double x, double y, double z)
	{
		azimuth = 180./M_PI*std::atan2(y,x);
		double module = std::sqrt(x*x+y*y+z*z);
		if (module<1e-12)
			elevation = 180./M_PI*z;
		else
			elevation = 180./M_PI*std::asin(z/module);
		normalize();
	}
	/**
	 Computes the chord size of the projections of the orientations
	 into a unit sphere.
	 Comparing chord distaces is equivalent to comparing angles,
	 or comparing surface distances
	*/
	double chordDistance(const Orientation & other)
	{
		double dz = se-other.se;
		double dy = ce*sa-other.ce*other.sa;
		double dx = ce*ca-other.ce*other.ca;
		return dx*dx + dy*dy + dz*dz;
		// TODO: Use the optimized form
		return 2 -2*(se*other.se + ce*other.ce * (sa*other.sa + ca*other.ca));
	}

	bool operator!=(const Orientation & other) const
	{
		if (fabs(azimuth-other.azimuth)>1e-13) return true;
		if (fabs(elevation-other.elevation)>1e-13) return true;
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
		return std::fmod(input, factor);
	}
};


} // namespace
#endif // Orientation_hxx
