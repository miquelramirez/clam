#ifndef Spherical2CartesianControl_hxx
#define Spherical2CartesianControl_hxx
#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>

class Spherical2CartesianControl : public CLAM::Processing
{ 
	CLAM::InControl _azimut;
	CLAM::InControl _elevation;
	CLAM::InControl _module;
	CLAM::OutControl _x;
	CLAM::OutControl _y;
	CLAM::OutControl _z;
public:
	const char* GetClassName() const { return "Spherical2CartesianControl"; }
	Spherical2CartesianControl(const Config& config = Config()) 
		: _azimut("azimut", this)
		, _elevation("elevation", this)
		, _module("module", this)
		, _x("x", this)
		, _y("y", this)
		, _z("z", this)
	{
		Configure( config );
		_azimut.SetBounds(-180,180);
		_elevation.SetBounds(-90,90);
		_module.DoControl(0);
	}
 
	bool Do()
	{
		const CLAM::TControlData& azimut = _azimut.GetLastValue()*M_PI/180;
		const CLAM::TControlData& elevation = _elevation.GetLastValue()*M_PI/180;
		const CLAM::TControlData& module = _module.GetLastValue();
		_x.SendControl(module * std::cos(elevation) * std::cos(azimut));
		_y.SendControl(module * std::cos(elevation) * std::sin(azimut));
		_z.SendControl(module * std::sin(elevation));
		return true;
	}

};
#endif

