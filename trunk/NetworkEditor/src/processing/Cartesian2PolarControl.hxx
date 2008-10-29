#ifndef Cartesian2PolarControl_hxx
#define Cartesian2PolarControl_hxx
#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>

class Cartesian2PolarControl : public CLAM::Processing
{ 
	CLAM::FloatInControl _x;
	CLAM::FloatInControl _y;
	CLAM::FloatOutControl _angle;
	CLAM::FloatOutControl _module;
public:
	const char* GetClassName() const { return "Cartesian2PolarControl"; }
	Cartesian2PolarControl(const Config& config = Config()) 
		: _x("x", this)
		, _y("y", this)
		, _angle("angle", this)
		, _module("module", this)
	{
		Configure( config );
		_x.SetBounds(-1, 1); 
		_y.SetBounds(-1, 1); 
	}
 
	bool Do()
	{
		const CLAM::TControlData& x = _x.GetLastValue();
		const CLAM::TControlData& y = _y.GetLastValue();
		_angle.SendControl(std::atan2(y,x)*180/M_PI-90);
		_module.SendControl(std::sqrt(x*x+y*y));
		return true;
	}

};
#endif

