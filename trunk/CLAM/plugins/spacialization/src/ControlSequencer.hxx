#ifndef ControlSequencer_hxx
#define ControlSequencer_hxx

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

/**
 Sends the control values stored in a file in tab separated columns.
 @todo: move ControlSequencer into CLAM as it is too general to be in the spacialization plugin.
*/

class ControlSequencer : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{ 
		DYNAMIC_TYPE_USING_INTERFACE( Config, 4, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, DataInFilename, Filename);
		DYN_ATTRIBUTE( 1, public, unsigned, FrameSize);
		DYN_ATTRIBUTE( 2, public, unsigned, SampleRate);
		DYN_ATTRIBUTE( 3, public, unsigned, ControlsPerSecond);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetFilename("");
			SetFrameSize(512);
			SetSampleRate(48000);
			SetControlsPerSecond(15);
		};
	};

	Config _config;
	FloatOutControl _out1;
	FloatOutControl _out2;
	FloatOutControl _out3;
	FloatOutControl _out4;
	FloatOutControl _out5;
	FloatOutControl _out6;
	FloatOutControl _out7;
	AudioInPort _syncIn;

	unsigned _samplesPerControl;
	unsigned _sampleCount;
	unsigned _frameSize;
	unsigned _sequenceIndex;

	typedef std::vector<float> Row;
	std::vector<Row> _controlSequence;
	unsigned _indexOut1;
	unsigned _indexOut2;
	unsigned _indexOut3;
	unsigned _indexOut4;
	unsigned _indexOut5;
	unsigned _indexOut6;
	unsigned _indexOut7;

public:
	ControlSequencer(const Config& config = Config()) 
		: _out1("out 1", this)
		, _out2("out 2", this)
		, _out3("out 3", this)
		, _out4("out 4", this)
		, _out5("out 5", this)
		, _out6("out 6", this)
		, _out7("out 7", this)
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
			//TODO check that _indexOut1,2,3 < _controlSequence[_sequenceIndex].size()
			_out1.SendControl( _controlSequence[_sequenceIndex][_indexOut1] );
			_out2.SendControl( _controlSequence[_sequenceIndex][_indexOut2] );
			_out3.SendControl( _controlSequence[_sequenceIndex][_indexOut3] );
			_out4.SendControl( _controlSequence[_sequenceIndex][_indexOut4] );
			_out5.SendControl( _controlSequence[_sequenceIndex][_indexOut5] );
			_out6.SendControl( _controlSequence[_sequenceIndex][_indexOut6] );
			_out7.SendControl( _controlSequence[_sequenceIndex][_indexOut7] );
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

		_controlSequence.clear();

		//Load the sequence
		if (_config.GetFilename()=="") // Walk in circles version
		{
			std::cout << "spiral version"<<std::endl;
			_indexOut1=0;
			_indexOut2=1;
			_indexOut3=2;

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
		std::cout << "ControlSequencer: read from file version. File: "<< _config.GetFilename() << std::endl;
		_indexOut1=4; // x target
		_indexOut2=5; // y target	
		_indexOut3=2; // beta target
		_indexOut4=7; // x orig 1
		_indexOut5=8; // y orig 1
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