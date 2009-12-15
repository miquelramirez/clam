#ifndef GainBecauseOfDistance_hxx
#define GainBecauseOfDistance_hxx


#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>
#include "Orientation.hxx"

namespace CLAM
{

class GainBecauseOfDistance : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE(Config,3,ProcessingConfig);
		DYN_ATTRIBUTE(0,public,TData,DistanceExponent);
		DYN_ATTRIBUTE(1,public,TData,MinimumDistance);
		DYN_ATTRIBUTE(2,public,TData,DistanceThreshold);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetDistanceExponent(1.0);
			SetMinimumDistance(1.0);
			SetDistanceThreshold(0.0);	//0 means no limit
		};
	};
	Config _config;

	CLAM::FloatInControl _distance;
	CLAM::FloatInControl _exponent;
	CLAM::FloatInControl _minimumDistance;
	CLAM::FloatInControl _distanceThreshold;

	CLAM::FloatOutControl _gain;
public:
	const char* GetClassName() const { return "GainBecauseOfDistance"; }
	GainBecauseOfDistance(const Config& config= Config())
		: _distance ("input distance",this)
		, _exponent ("inverse exponent to calculate gain",this)
		, _minimumDistance ("minimum distance",this)
		, _distanceThreshold ("distance threshold (maximum distance of hearing sound)",this)
		, _gain ("calculated output gain",this)
	{
		Configure( config );
	}

	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}

	bool Do()
	{
		double distance=_distance.GetLastValue();
		double exponent=_exponent.GetLastValue();
		double minimumDistance=_minimumDistance.GetLastValue();
		double distanceThreshold=_distanceThreshold.GetLastValue();
		double gain=0;
		if (minimumDistance==0)
			minimumDistance=0.0001; // avoid by zero division
		if (distance < minimumDistance)
			distance=minimumDistance;
		if (distanceThreshold==0 or distance <= distanceThreshold)
			gain=1.0/pow(distance,exponent);
		_gain.SendControl(gain);
		return true;
	}
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config,config);
		double exponent = _config.HasDistanceExponent() ? _config.GetDistanceExponent() : 1.5;
		_exponent.DoControl(exponent);
		double minimumDistance = _config.HasMinimumDistance() ? _config.GetMinimumDistance() : 1.0;
		_minimumDistance.DoControl(minimumDistance);
		double distanceThreshold= _config.HasDistanceThreshold() ? _config.GetDistanceThreshold() : 0; //default: no limit
		_distanceThreshold.DoControl(distanceThreshold);
		return true;
	}
};

} //namespace
#endif // GainBecauseOfDistance_hxx
