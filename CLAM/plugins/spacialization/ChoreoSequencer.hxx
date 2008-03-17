#ifndef ChoreoSequencer_hxx
#define ChoreoSequencer_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/Filename.hxx>
#include <string>
#include <cmath>
#include <fstream>


namespace CLAM
{

/*
// Don't know how to make this class work. So use string instad --Pau

class DataInFilename : public InFilename
{
public:
	DataInFilename(const std::string & s="") : InFilename(s) {}
	DataInFilename(const char * s) : InFilename(s) {}
	const char* TypeFamily() const;
	const Filter * Filters() const;
};
CLAM_TYPEINFOGROUP(BasicCTypeInfo, CLAM::DataInFilename);
*/


typedef std::string DataInFilename;

class ChoreoSequencer : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{ 
	    DYNAMIC_TYPE_USING_INTERFACE( Config, 5, ProcessingConfig );
	    DYN_ATTRIBUTE( 0, public, DataInFilename, Filename);
	    DYN_ATTRIBUTE( 1, public, unsigned, SourceIndex); // first is 0
	    DYN_ATTRIBUTE( 2, public, unsigned, FrameSize);
	    DYN_ATTRIBUTE( 3, public, unsigned, SampleRate);
	    DYN_ATTRIBUTE( 4, public, unsigned, ControlsPerSecond);
	protected:
	    void DefaultInit()
	    {
		  AddAll();
		  UpdateData();
		  SetFilename("");
		  SetSourceIndex(0);
		  SetFrameSize(512);
		  SetSampleRate(48000);
		  SetControlsPerSecond(15);
	    };
	};

	Config _config;
	OutControl _targetX;
	OutControl _targetY;
	OutControl _targetZ;
	OutControl _targetAzimuth;
	OutControl _targetElevation;
	OutControl _sourceX;
	OutControl _sourceY;
	OutControl _sourceZ;
	AudioInPort _syncIn;

	unsigned _samplesPerControl;
	unsigned _sampleCount;
	unsigned _frameSize;
	unsigned _sequenceIndex;

	typedef std::vector<float> Row;
	std::vector<Row> _controlSequence;
	unsigned _indexTargetX;
	unsigned _indexTargetY;
	unsigned _indexTargetZ;
	unsigned _indexTargetAzimuth;
	unsigned _indexSourceX;
	unsigned _indexSourceY;
	unsigned _indexSourceZ;
	unsigned _indexOut6;
	unsigned _indexOut7;

public:
	ChoreoSequencer(const Config& config = Config()) 
		: _targetX("target X", this)
		, _targetY("target Y", this)
		, _targetZ("target Z", this)
		, _targetAzimuth("target azimuth", this)
		, _targetElevation("target elevation", this)
		, _sourceX("source X", this)
		, _sourceY("source Y", this)
		, _sourceZ("source Z", this)
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
			//std::cout << ", ctl "<<_sequenceIndex << "/" <<_controlSequence.size() << std::flush;
			//TODO check that _indexTargetX,Y,Z < _controlSequence[_sequenceIndex].size()
			_targetX.SendControl( _controlSequence[_sequenceIndex][_indexTargetX] );
			_targetY.SendControl( _controlSequence[_sequenceIndex][_indexTargetY] );
			_targetZ.SendControl( _controlSequence[_sequenceIndex][_indexTargetZ] );
			_targetAzimuth.SendControl( _controlSequence[_sequenceIndex][_indexTargetAzimuth] );
			_sourceX.SendControl( _controlSequence[_sequenceIndex][_indexSourceX+3*sourceIndex] );
			_sourceY.SendControl( _controlSequence[_sequenceIndex][_indexSourceY+3*sourceIndex] );
			_sourceZ.SendControl( _controlSequence[_sequenceIndex][_indexSourceZ+3*sourceIndex] );
			_sampleCount -= _samplesPerControl;
			_sequenceIndex++;
			if (_sequenceIndex >= _controlSequence.size())
			{
				_sequenceIndex=0;
				std::cout << std::endl << "\n End of control sequence. Starting new iteration\n"<<std::endl;;
			}
		}
		_syncIn.Consume();
		return true;
	}
	const char* GetClassName() const
	{
		return "ChoreoSequencer";
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

		_controlSequence.clear();

		//Load the sequence
		if (_config.GetFilename()=="") // Walk in circles version
		{
			std::cout << "spiral version"<<std::endl;
			_indexTargetX=0;
			_indexTargetY=1;
			_indexTargetAzimuth=2;

			// by now, let's do a circle in the room
			unsigned NPoints = 100;
			float delta=3*M_PI/2;
			float alpha=0.; //listener angle;
			float deltaInc=2*M_PI/NPoints;
			float ratio=1;
//			float alphaInc=360./NPoints;
			for (unsigned i=0; i<NPoints; i++)
			{
				ratio = float(NPoints-i)/NPoints;
				float x=std::cos(delta)*0.49*ratio+0.5;
				float y=std::sin(delta)*0.49*ratio+0.5;
				Row row;
				row.push_back(x);
				row.push_back(y);
				row.push_back(alpha);
				_controlSequence.push_back(row);
				delta+=deltaInc;
		//		alpha+=alphaInc;
			}
			return true;
		}
		// Load the file version
		std::cout << "ChoreoSequencer: read from file version. File: "<< _config.GetFilename() << std::endl;
		_indexTargetX=4; 
		_indexTargetY=5; 	
		_indexTargetZ=6; 	
		_indexTargetAzimuth=2; // beta target
		_indexSourceX=7; // x orig 1
		_indexSourceY=8; // y orig 1
		_indexSourceZ=9; // y orig 1
		_indexOut6=10; // x orig 2
		_indexOut7=11; //y orig 2
		// Load table from file
		std::ifstream file( _config.GetFilename().c_str() );
		while (file)
		{
			std::string line;
			std::getline(file, line);
			if (line=="" or line[0]=='#') continue; 
			std::istringstream is(line);
			Row row;
			while (is)
			{
				float data;
				is >> data;
				row.push_back(data);
			}
			_controlSequence.push_back(row);
		}
		return true;
	}
};

} //namespace

#endif
