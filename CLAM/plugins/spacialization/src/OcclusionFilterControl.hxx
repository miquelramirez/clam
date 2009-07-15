#ifndef OcclusionFilterControl_hxx 
#define OcclusionFilterControl_hxx


#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <cmath>
#include "Orientation.hxx"

namespace CLAM
{

class OcclusionFilterControl : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE(Config,6,ProcessingConfig);
		DYN_ATTRIBUTE(0,public,TData,DistanceExponent);
		DYN_ATTRIBUTE(1,public,TData,MinimumDistance);
		DYN_ATTRIBUTE(2,public,TData,DistanceThreshold);
		DYN_ATTRIBUTE(3,public,TData,MaxCutoffFrequency);
		DYN_ATTRIBUTE(4,public,TData,MinCutoffFrequency);
		DYN_ATTRIBUTE(5,public,TData,OcclusionGainFactor);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetDistanceExponent(1.0);
			SetMinimumDistance(1.0);
			SetDistanceThreshold(0.0);	//0 means no limit
			SetMaxCutoffFrequency(24000.);
			SetMinCutoffFrequency(350.);
			SetOcclusionGainFactor(0.3);
		};
	};
	Config _config;

	CLAM::FloatInControl _distance;
	CLAM::FloatInControl _exponent;
	CLAM::FloatInControl _minimumDistance;
	CLAM::FloatInControl _distanceThreshold;
	CLAM::FloatInControl _directSoundPressureWithOcclusions;

	CLAM::FloatOutControl _gainOutControl;
	CLAM::FloatOutControl _cutoffFrequencyOutControl;

	float _minCutoffFrequency,_maxCutoffFrequency;
	float _minOcclusionFactor;

	int _framesForFade; // number of frames to make the fades in/out
	int _counterFrames; // counter of frames of fades

	enum StatesEnum {DirectSound,FadeOut,Occlusion,FadeIn};
	StatesEnum _actualState;

public:
	const char* GetClassName() const { return "OcclusionFilterControl"; }
	OcclusionFilterControl(const Config& config= Config())
		: _distance ("input distance",this)
		, _exponent ("inverse exponent to calculate gain",this)
		, _minimumDistance ("minimum distance",this)
		, _distanceThreshold ("distance threshold (maximum distance of hearing sound)",this)
		, _directSoundPressureWithOcclusions ("direct sound pressure with occlusions",this)
		, _gainOutControl ("calculated output gain",this)
		, _cutoffFrequencyOutControl("LP cutoff frequency",this)

		, _framesForFade(18)
		, _counterFrames(0)
		, _actualState(DirectSound)
	{
		Configure( config );
	}

	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}

	bool Do()
	{
		double directSoundPressure=_directSoundPressureWithOcclusions.GetLastValue();
		const bool isActuallyOccluded = (directSoundPressure==0);
		double distance=_distance.GetLastValue();
		double exponent=_exponent.GetLastValue();
		double minimumDistance=_exponent.GetLastValue();
		double distanceThreshold=_distanceThreshold.GetLastValue();
		double gain=0;

		if (minimumDistance==0)
			minimumDistance=0.0001; // avoid by zero division
		if (distance < minimumDistance)
			distance=minimumDistance;
		if (distanceThreshold==0 or distance <= distanceThreshold)
			gain=1.0/pow(distance,exponent);

		if (_counterFrames==_framesForFade)
		{
			if (_actualState==FadeOut)
				_actualState=Occlusion;
			if (_actualState==FadeIn)
				_actualState=DirectSound;
		}

		double occlusionFactor=1;

		switch (_actualState)
		{
			case FadeOut:
				_counterFrames+=1;
			case Occlusion:
				if (isActuallyOccluded) //if it keeps in FadeOut/Occlusion mode
					occlusionFactor = 1. - float(_counterFrames) / _framesForFade;
				else
				{
					_actualState=FadeIn;
					_counterFrames=1;
					occlusionFactor =  float(_counterFrames) / _framesForFade;
				}
				break;
			case FadeIn:
				_counterFrames+=1;
			case DirectSound:
			default:
				if (not isActuallyOccluded)
					occlusionFactor = float(_counterFrames) / _framesForFade;
				else
				{
					_actualState=FadeOut;
					_counterFrames=1;
					occlusionFactor = 1. - float(_counterFrames) / _framesForFade;
				}
				break;
		}

		if (isActuallyOccluded)
		{
			occlusionFactor=occlusionFactor*(1.-_minOcclusionFactor);
			occlusionFactor+=_minOcclusionFactor;
			gain*=occlusionFactor;
		}
		_gainOutControl.SendControl(gain);
		_cutoffFrequencyOutControl.SendControl( directSoundPressure==0 ? _minCutoffFrequency : _maxCutoffFrequency);
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

		_minCutoffFrequency = _config.HasMinCutoffFrequency() ? _config.GetMinCutoffFrequency() : 350;
		const unsigned nyquistFreq=unsigned (BackendSampleRate()/2);
		_maxCutoffFrequency = _config.HasMaxCutoffFrequency() ? _config.GetMaxCutoffFrequency() : nyquistFreq;
		_minOcclusionFactor = _config.HasOcclusionGainFactor() ? _config.GetOcclusionGainFactor() : 0.3;

		return true;
	}
};

} //namespace
#endif // OcclusionFilterControl_hxx
