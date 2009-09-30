#ifndef OcclusionFilterControl_hxx 
#define OcclusionFilterControl_hxx


#include <CLAM/AudioInPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/Filename.hxx>
#include <cmath>
#include "Orientation.hxx"
#include "scene.hxx"

namespace CLAM
{

class OcclusionFilterControl : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE(Config,7,ProcessingConfig);
		DYN_ATTRIBUTE(0,public,TData,DistanceExponent);
		DYN_ATTRIBUTE(1,public,TData,MinimumDistance);
		DYN_ATTRIBUTE(2,public,TData,DistanceThreshold);
		DYN_ATTRIBUTE(3,public,TData,FadeInMs);
		DYN_ATTRIBUTE(4,public,TData,MinCutoffFrequency);
		DYN_ATTRIBUTE(5,public,TData,OcclusionGainFactor);
		DYN_ATTRIBUTE(6,public,InFilename,GeometryFile);
protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetDistanceExponent(1.0);
			SetMinimumDistance(1.0);
			SetDistanceThreshold(0.0);	//0 means no limit
			SetFadeInMs(500.);
			SetMinCutoffFrequency(350.);
			SetOcclusionGainFactor(0.3);
			SetGeometryFile("");
		};
	};
	Config _config;

	FloatInControl _sourceXInControl;
	FloatInControl _sourceYInControl;
	FloatInControl _sourceZInControl;
	FloatInControl _listenerXInControl;
	FloatInControl _listenerYInControl;
	FloatInControl _listenerZInControl;

	CLAM::FloatInControl _exponent;
	CLAM::FloatInControl _minimumDistance;
	CLAM::FloatInControl _distanceThreshold;

	CLAM::FloatInControl _fadeInMsControl;
	CLAM::FloatInControl _minCutoffFrequencyControl;
	CLAM::FloatInControl _occlusionGainFactorControl;

	AudioInPort _inSync;

	CLAM::FloatOutControl _gainOutControl;
	CLAM::FloatOutControl _cutoffFrequencyOutControl;

	float _minCutoffFrequency,_maxCutoffFrequency;
	float _minOcclusionFactor;

	int _framesForFade; // number of frames to make the fades in/out
	int _counterFrames; // counter of frames of fades

	enum StatesEnum {DirectSound,FadeOut,Occlusion,FadeIn};
	StatesEnum _actualState;

	Scene * _scene;

	Scene * createScene()
	{
		std::ifstream modelcheck( _config.GetGeometryFile().c_str() );
		if ( not modelcheck.is_open() )
		{
			std::string err("Could not open the environments geometry definitions: "); 
			err += _config.GetGeometryFile();
			AddConfigErrorMessage(err);
			return 0;
		}
		Settings settings;
                settings.repeatable = true;
                settings.extra_bound = 0.5;
                settings.human_height = 1.70;
                settings.model_filename = _config.GetGeometryFile();
                settings.num_rays = 0;
                settings.num_rebounds = 0;
                settings.physical_output = 0;
                settings.ir_length = 0;
                settings.ignore_sources_models = true;
                settings.use_osc = false;
                settings.use_dat = false;
                settings.hoa_order = 0;
		return new Scene(settings);
	}
public:
	const char* GetClassName() const { return "OcclusionFilterControl"; }
	OcclusionFilterControl(const Config& config= Config())
		: _sourceXInControl ("Source X",this, &OcclusionFilterControl::InControlCallback)
		, _sourceYInControl ("Source Y",this, &OcclusionFilterControl::InControlCallback)
		, _sourceZInControl ("Source Z",this, &OcclusionFilterControl::InControlCallback)
		, _listenerXInControl ("Listener X",this, &OcclusionFilterControl::InControlCallback)
		, _listenerYInControl ("Listener Y",this, &OcclusionFilterControl::InControlCallback)
		, _listenerZInControl ("Listener Z",this, &OcclusionFilterControl::InControlCallback)
		, _exponent ("inverse exponent to calculate gain",this, &OcclusionFilterControl::InControlCallback)
		, _minimumDistance ("minimum distance",this, &OcclusionFilterControl::InControlCallback)
		, _distanceThreshold ("distance threshold (maximum distance of hearing sound)",this, &OcclusionFilterControl::InControlCallback)
		, _fadeInMsControl (1, "Fade time for the occlusions (in miliseconds)", this, &OcclusionFilterControl::InConfigControlCallback )
		, _minCutoffFrequencyControl (2, "Minimum cutoff frequency for the LP filter", this, &OcclusionFilterControl::InConfigControlCallback )
		, _occlusionGainFactorControl (3, "Occlusion gain factor", this, &OcclusionFilterControl::InConfigControlCallback )
		, _inSync("Sync in",this)
		, _gainOutControl ("calculated output gain",this)
		, _cutoffFrequencyOutControl("LP cutoff frequency",this)
		, _framesForFade(0)
		, _counterFrames(0)
		, _actualState(DirectSound)
		, _scene(0)
	{
		Configure( config );
	}

	~OcclusionFilterControl()
	{
		if (_scene) delete _scene;
	}

	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}


	void InConfigControlCallback (unsigned controlId, const TControlData & value)
	{
		if (controlId==1)
			_framesForFade=float(_fadeInMsControl.GetLastValue()*0.001) / (float(BackendBufferSize())/float(BackendSampleRate()));
		if (controlId==2)
			_minCutoffFrequency = _minCutoffFrequencyControl.GetLastValue();
		if (controlId==3)
			_minOcclusionFactor = _occlusionGainFactorControl.GetLastValue();
	}
	void InControlCallback (const TControlData & value)
	{
		if (not _scene) 
			return; // don't do anything in the configuration DoControls.... 
		const double sourceX=_sourceXInControl.GetLastValue();
		const double sourceY=_sourceYInControl.GetLastValue();
		const double sourceZ=_sourceZInControl.GetLastValue();
		const double listenerX=_listenerXInControl.GetLastValue();
		const double listenerY=_listenerYInControl.GetLastValue();
		const double listenerZ=_listenerZInControl.GetLastValue();

		const Vector direction = Vector( sourceX-listenerX, sourceY-listenerY, sourceZ-listenerZ );
		const Vertex origin = Vertex( listenerX, listenerY, listenerZ );
		double distance=std::sqrt( std::pow(direction.x(),2) + std::pow(direction.y(),2) + std::pow(direction.z(),2) );
		const bool ignoreNormalsDirection=true;

		const std::string occlusionMeshName=_scene->getNearestIntersectionFaceMeshName (origin, ignoreNormalsDirection, direction, distance);
		const bool isCurrentlyOccluded = (occlusionMeshName!="");
		double exponent = _exponent.GetLastValue();
		double minimumDistance = _exponent.GetLastValue();
		double distanceThreshold = _distanceThreshold.GetLastValue();
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
				if (isCurrentlyOccluded) //if it keeps in FadeOut/Occlusion mode
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
				if (not isCurrentlyOccluded)
					occlusionFactor = float(_counterFrames) / _framesForFade;
				else
				{
					_actualState=FadeOut;
					_counterFrames=1;
					occlusionFactor = 1. - float(_counterFrames) / _framesForFade;
				}
				break;
		}

//		if (isCurrentlyOccluded)
//		{
			float cutoffFrequency=occlusionFactor*float(_maxCutoffFrequency-_minCutoffFrequency);
			cutoffFrequency+=_minCutoffFrequency;
			occlusionFactor=occlusionFactor*(1.-_minOcclusionFactor);
			occlusionFactor+=_minOcclusionFactor;
			gain*=occlusionFactor;
//		}
		_gainOutControl.SendControl(gain);
		_cutoffFrequencyOutControl.SendControl(cutoffFrequency);
	}


	bool Do()
	{
		_inSync.Consume();
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
		_framesForFade=float(_config.GetFadeInMs()*0.001) / (float(BackendBufferSize())/float(BackendSampleRate()));
 		_maxCutoffFrequency = nyquistFreq;
		_minOcclusionFactor = _config.HasOcclusionGainFactor() ? _config.GetOcclusionGainFactor() : 0.3;

		if (_scene) delete _scene;
		_scene = createScene();
		if (not _scene)	return false;

		return true;
	}
};

} //namespace

#endif
