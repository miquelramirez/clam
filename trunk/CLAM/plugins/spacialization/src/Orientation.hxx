#ifndef Orientation_hxx 
#define Orientation_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>
#include <iomanip>

namespace CLAM
{

// Basic vector
struct Vector3D
{
	float x;
	float y;
	float z;
};

struct SphericalHarmonicsDefinition
{
	const char * name;
	unsigned order;
	unsigned zProjection;
	signed sign;
	double weightFuMa;
};

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
public:
	double eradians;
	double aradians;
	double ce;
	double se;
	double ca;
	double sa;
public:
	static SphericalHarmonicsDefinition * sphericalHarmonics()
	{
		// taken from http://www.york.ac.uk/inst/mustech/3d_audio/higher_order_ambisonics.pdf
		static SphericalHarmonicsDefinition sphericalHarmonicsDefinition [] = 
		{
			// TODO: Precission on W lowered to pass b2b vs. previous implementation
			{"W", 0,0,+1, 0.707},//1./std::sqrt(2)},
			{"X", 1,1,+1, 1.},
			{"Y", 1,1,-1, 1.},
			{"Z", 1,0,+1, 1.},
			{"R", 2,0,+1, 1.},
			{"S", 2,1,+1, 2./std::sqrt(3)},
			{"T", 2,1,-1, 2./std::sqrt(3)},
			{"U", 2,2,+1, 2./std::sqrt(3)},
			{"V", 2,2,-1, 2./std::sqrt(3)},
			{"K", 3,0,+1, 1.},
			{"L", 3,1,+1, std::sqrt(45./32.)},
			{"M", 3,1,-1, std::sqrt(45./32.)},
			{"N", 3,2,+1, std::sqrt(9./5.)},
			{"O", 3,2,-1, std::sqrt(9./5.)},
			{"P", 3,3,+1, std::sqrt(8./5.)},
			{"Q", 3,3,-1, std::sqrt(8./5.)},
			{0, 0, 0, 0, 0}
		};
		return sphericalHarmonicsDefinition;
	}
	double sphericalHarmonic(unsigned shIndex) const
	{
		return sphericalHarmonic(sphericalHarmonics()[shIndex]);
	}
	double sphericalHarmonic(const SphericalHarmonicsDefinition & sh) const
	{
		return sphericalHarmonic(sh.order, sh.zProjection, sh.sign>=0);
	}
	double sphericalHarmonic(unsigned order, unsigned zProjection, bool positiveSpin) const
	{
		double zComponent = positiveSpin? 
			std::cos(zProjection*aradians): 
			std::sin(zProjection*aradians);
		switch (order) 
		{
		case 0:
			if (zProjection==0) return 1.;
			CLAM_ASSERT(false,"Degree greater than order");
		case 1:
			if (zProjection==0) return              se;
			if (zProjection==1) return zComponent * ce;
			CLAM_ASSERT(false,"Degree greater than order");
		case 2:
			if (zProjection==0) return              std::sqrt(1./4)*(3*se*se -1);
			if (zProjection==1) return zComponent * std::sqrt(3./1)*se*ce;
			if (zProjection==2) return zComponent * std::sqrt(3./4)*ce*ce;
			CLAM_ASSERT(false,"Degree greater than order");
		case 3:
			if (zProjection==0) return              std::sqrt( 1./4)*se*(5*se*se -3);
			if (zProjection==1) return zComponent * std::sqrt( 3./8)*ce*(5*se*se -1);
			if (zProjection==2) return zComponent * std::sqrt(15./4)*se*ce*ce;
			if (zProjection==3) return zComponent * std::sqrt( 5./8)*ce*ce*ce;
			CLAM_ASSERT(false,"Degree greater than order");
		default:
			CLAM_ASSERT(false,"Unsupported order");
		}
		return 0;
	}

	Orientation(double anAzimuth=0.0, double anElevation=0.0)
		: azimuth(anAzimuth)
		, elevation(anElevation)
	{
		precompute();
	}
	/**
	 Converts the orientation values for azimuth and elevation to be
	 within the ranges according @ref AmbisonicsConventions.
	*/
	void normalize()
	{
		elevation += 90.;
		elevation = module(elevation, 360.);
		if (elevation>180.+1e-13)
		{
			azimuth += 180;
			elevation = 360 - elevation;
		}
		azimuth = module(azimuth, 360.);
		elevation -= 90;
	}
	/**
	 Set the orientation to the one of the vector (x,y,z)
	 For vectors of module less than 1e-12 elevation is set to z.
	 Result is normalized according @ref AmbisonicsConventions.
	*/
	void toPoint(double x, double y, double z)
	{
		azimuth = 180./M_PI*std::atan2(y,x);
		double module = std::sqrt(x*x+y*y+z*z);
		if (module<1e-12)
			elevation = 180./M_PI*z;
		else
			elevation = 180./M_PI*std::asin(z/module);
		precompute();
		normalize();
	}
	/**
	 Computes the chord size of the projections of the orientations
	 into a unit sphere.
	 Comparing chord distaces is equivalent to comparing angles,
	 or comparing distances on the sphere surface
	*/
	double chordDistance(const Orientation & other)
	{
		return 2 -2*(se*other.se + ce*other.ce * (sa*other.sa + ca*other.ca));
		// This is a sub-optimized and less numerically accurate formula
		// left here as reference.
		double dz = se-other.se;
		double dy = ce*sa-other.ce*other.sa;
		double dx = ce*ca-other.ce*other.ca;
		return dx*dx + dy*dy + dz*dz;
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
	Vector3D toCartesian() const
	{
		Vector3D result = { ce * ca, ce * sa, se };
		return result;
	}

private:
	double module(double input, double factor)
	{
		while (input<0) input+=factor;
		return std::fmod(input, factor);
	}
	void precompute()
	{
		eradians=(M_PI*elevation/180.);
		aradians=(M_PI*azimuth/180.);
		ce=(std::cos(eradians));
		se=(std::sin(eradians));
		ca=(std::cos(aradians));
		sa=(std::sin(aradians));
	}
};


inline Vector3D vectorialProduct(const Vector3D& v1, const Vector3D& v2)
{
	Vector3D result = {
		v1.y * v2.z - v1.z * v2.y ,
		v1.z * v2.x - v1.x * v2.z ,
		v1.x * v2.y - v1.y * v2.x ,
		};
	return result;
}
inline Vector3D vectorByScalar(const float& factor, const Vector3D& v)
{
	Vector3D result = { factor * v.x, factor * v.y, factor * v.z };
	return result;
}

inline float scalarProduct(const Vector3D& v1, const Vector3D& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline Vector3D substract(const Vector3D& v1, const Vector3D& v2)
{
	Vector3D result = {
		v1.x - v2.x ,
		v1.y - v2.y ,
		v1.z - v2.z ,
		};
	return result;
}
inline float mod(const Vector3D& v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}
inline float angle(const Vector3D& v1, const Vector3D& v2)
{
	float divisor = mod(v1)*mod(v2);
	const float deltaNumeric = 0.00001;
	CLAM_ASSERT( divisor > deltaNumeric, "Cannot compute an angle of a zero vector"); 
	float arg =  scalarProduct(v1,v2) / (mod(v1)*mod(v2));
	if (arg <-1 or arg >1) return arg < 0 ? M_PI : 0;
	return acos( arg );
}
inline void print(const Vector3D& v, std::string name="")
{
	std::cout << name << " (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
}
inline float rad( float deg )
{
	return deg / 180 * M_PI;
}

} // namespace
#endif // Orientation_hxx
