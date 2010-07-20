#ifndef Spherical2CartesianControl_hxx
#define Spherical2CartesianControl_hxx
#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include "Geometry.hxx"

/**
 Processing that converts spherical coordinates in \ref AmbisonicsConventions
 into cartesian X, Y, Z coordinates also in \ref AmbisonicsConventions
 @todo Domument Spherical2CartesianControl
 @todo Check that Spherical2CartesianControl follows ambisonics convetions.
 @ingroup SpatialAudio
 */
class Spherical2CartesianControl : public CLAM::Processing
{ 
	CLAM::FloatInControl _azimuth;
	CLAM::FloatInControl _elevation;
	CLAM::FloatInControl _module;
	CLAM::FloatOutControl _x;
	CLAM::FloatOutControl _y;
	CLAM::FloatOutControl _z;
public:
	const char* GetClassName() const { return "Spherical2CartesianControl"; }
	Spherical2CartesianControl(const Config& config = Config()) 
		: _azimuth("azimuth", this)
		, _elevation("elevation", this)
		, _module("module", this)
		, _x("x", this)
		, _y("y", this)
		, _z("z", this)
	{
		Configure( config );
		_azimuth.SetBounds(-180,180);
		_elevation.SetBounds(-90,90);
		_module.DoControl(1.0);
	}
 
	bool Do()
	{
		CartesianCoords cartesianCoords(SphericalCoords(_azimuth.GetLastValue(), _elevation.GetLastValue(),_module.GetLastValue()));
		_x.SendControl(cartesianCoords.x);
		_y.SendControl(cartesianCoords.y);
		_z.SendControl(cartesianCoords.z);
		return true;
	}

};
#endif

