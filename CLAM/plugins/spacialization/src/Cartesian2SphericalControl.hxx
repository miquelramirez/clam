#ifndef Cartesian2SphericalControl_hxx
#define Cartesian2SphericalControl_hxx
#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include "Geometry.hxx"
//#include <cmath>

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
		const CLAM::TControlData& x = _x.GetLastValue();
		const CLAM::TControlData& y = _y.GetLastValue();
		const CLAM::TControlData& z = _z.GetLastValue();
		SphericalCoords sphericalCoords=SphericalCoords(CartesianCoords(x,y,z));
		_azimuth.SendControl(sphericalCoords.a);
		_elevation.SendControl(sphericalCoords.e);
		_module.SendControl(sphericalCoords.d);
		return true;
	}

};
#endif

