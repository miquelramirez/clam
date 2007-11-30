#ifndef ControlSequencer_hxx
#define ControlSequencer_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/AudioInPort.hxx>
#include <string>
#include <cmath>
#include "lo/lo.h"


namespace CLAM
{


class ControlSequencer : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{ 
	    DYNAMIC_TYPE_USING_INTERFACE( Config, 4, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, std::string, Filename); //TODO should be CLAM::Filename
	    DYN_ATTRIBUTE( 1, public, unsigned, FrameSize);
	    DYN_ATTRIBUTE( 2, public, unsigned, SampleRate);
	    DYN_ATTRIBUTE( 3, public, unsigned, ControlsPerSecond);
	protected:
	    void DefaultInit()
	    {
		  AddAll();
		  UpdateData();
		  SetFilename("lalala");
		  SetFrameSize(512);
		  SetSampleRate(48000);
		  SetControlsPerSecond(15);
	    };
	};

	Config _config;
	OutControl _out1;
	OutControl _out2;
	OutControl _out3;
	AudioInPort _syncIn;

	unsigned _samplesPerControl;
	unsigned _sampleCount;
	unsigned _frameSize;
	unsigned _sequenceIndex;

	typedef std::vector<TControlData> ControlSequence;
	ControlSequence _controls1;
	ControlSequence _controls2;
	ControlSequence _controls3;

public:
	ControlSequencer(const Config& config = Config()) 
		: _out1("out 1", this)
		, _out2("out 2", this)
		, _out3("out 3", this)
		, _syncIn("sync", this)
	{
		Configure( config );
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}
	bool Do()
	{
		CLAM_DEBUG_ASSERT(_sampleCount<2*_samplesPerControl,"_sampleCount too large" );

		_sampleCount += _frameSize;
		if (_sampleCount>=_samplesPerControl)
		{
			std::cout << _sequenceIndex << " " << std::flush;
			_out1.SendControl( _controls1[_sequenceIndex] );
			_out2.SendControl( _controls2[_sequenceIndex] );
			_out3.SendControl( _controls3[_sequenceIndex] );
			_sampleCount -= _samplesPerControl;
			_sequenceIndex++;
			if (_sequenceIndex==_controls1.size())
			{
				_sequenceIndex=0;
				std::cout << std::endl << "new iteration ";
			}
		}
		
		_syncIn.Consume();
		return true;
	}
	const char* GetClassName() const
	{
		return "ControlSequencer";
	}
protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		_syncIn.SetSize( _config.GetFrameSize() );
		_syncIn.SetHop( _config.GetFrameSize() );
		_samplesPerControl = _config.GetSampleRate()/_config.GetControlsPerSecond();
		_sampleCount=0;
		_frameSize=_config.GetFrameSize();
		_sequenceIndex=0;

		_controls1.clear();
		_controls2.clear();
		_controls3.clear();

		//Load the sequence
		// by now, let's do a circle in the room
		unsigned NPoints = 100;
		float delta=3*M_PI/2;
		float alpha=0.; //listener angle;
		float deltaInc=2*M_PI/NPoints;
		float alphaInc=360./NPoints;
		for (unsigned i=0; i<NPoints; i++)
		{
			float x=std::cos(delta)*0.45+0.5;
			float y=std::sin(delta)*0.45+0.5;
			_controls1.push_back(x);
			_controls2.push_back(y);
			_controls3.push_back(alpha);
			delta+=deltaInc;
			alpha+=alphaInc;
		}
		return true;
	}

};

} //namespace

#endif
