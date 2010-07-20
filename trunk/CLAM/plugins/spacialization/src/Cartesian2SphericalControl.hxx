#ifndef Cartesian2SphericalControl_hxx
#define Cartesian2SphericalControl_hxx
#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include "Geometry.hxx"

class Cartesian2SphericalControl : public CLAM::Processing
{ 
	CLAM::FloatInControl _x;
	CLAM::FloatInControl _y;
	CLAM::FloatInControl _z;
	CLAM::FloatOutControl _azimuth;
	CLAM::FloatOutControl _elevation;
	CLAM::FloatOutControl _module;
public:
	const char* GetClassName() const { return "Cartesian2SphericalControl"; }
	Cartesian2SphericalControl(const Config& config = Config()) 
		: _x("x", this)
		, _y("y", this)
		, _z("z", this)
		, _azimuth("azimuth", this)
		, _elevation("elevation", this)
		, _module("module", this)
	{
		Configure( config );
		_x.SetBounds(-1, 1); 
		_y.SetBounds(-1, 1); 
		_z.SetBounds(-1, 1); 
	}
 
	bool Do()
	{
		
		SphericalCoords sphericalCoords(CartesianCoords(_x.GetLastValue(), _y.GetLastValue(), _z.GetLastValue()));
		_azimuth.SendControl(sphericalCoords.a);
		_elevation.SendControl(sphericalCoords.e);
		_module.SendControl(sphericalCoords.d);
		return true;
	}

};
#endif

