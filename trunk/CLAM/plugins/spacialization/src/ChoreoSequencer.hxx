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

class ChoreoSequencer : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{ 
		DYNAMIC_TYPE_USING_INTERFACE( Config, 8, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, InFilename, Filename);
		DYN_ATTRIBUTE( 1, public, unsigned, SourceIndex); // first is 0
		DYN_ATTRIBUTE( 2, public, unsigned, FrameSize);
		DYN_ATTRIBUTE( 3, public, unsigned, SampleRate);
		DYN_ATTRIBUTE( 4, public, unsigned, ControlsPerSecond);
		DYN_ATTRIBUTE( 5, public, TData, SizeX);
		DYN_ATTRIBUTE( 6, public, TData, SizeY);
		DYN_ATTRIBUTE( 7, public, TData, SizeZ);
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
			SetSizeX(1);
			SetSizeY(1);
			SetSizeZ(1);
		
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
	OutControl _sourceAzimuth;
	OutControl _sourceElevation;
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
	unsigned _indexTargetElevation;
	unsigned _indexSourceX;
	unsigned _indexSourceY;
	unsigned _indexSourceZ;
	double _sizeX;
	double _sizeY;
	double _sizeZ;

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
		, _sourceAzimuth("source azimuth", this)
		, _sourceElevation("source elevation", this)
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
		int sourceIndex = _config.GetSourceIndex();
		if (_sampleCount>=_samplesPerControl)
		{
			std::cout << "("<<_sequenceIndex << "/" <<_controlSequence.size() << ")" << std::flush;
			const Row & row = _controlSequence[_sequenceIndex];
			//TODO check that _indexTargetX,Y,Z < row.size()
			_targetX.SendControl( row[_indexTargetX] );
			_targetY.SendControl( row[_indexTargetY] );
			_targetZ.SendControl( row[_indexTargetZ] );
			_targetAzimuth.SendControl( row[_indexTargetAzimuth] );
			_targetElevation.SendControl( row[_indexTargetElevation] );
			_sourceX.SendControl( row[_indexSourceX+3*sourceIndex] );
			_sourceY.SendControl( row[_indexSourceY+3*sourceIndex] );
			_sourceZ.SendControl( row[_indexSourceZ+3*sourceIndex] );
			double dx = _sizeX * (row[_indexSourceX+3*sourceIndex] - row[_indexTargetX]);
			double dy = _sizeY * (row[_indexSourceY+3*sourceIndex] - row[_indexTargetY]);
			double dz = _sizeZ * (row[_indexSourceZ+3*sourceIndex] - row[_indexTargetZ]);
			_sourceAzimuth.SendControl( std::atan2(dx,dy) - row[_indexTargetAzimuth] );
			_sourceElevation.SendControl( std::asin(dz) - row[_indexTargetElevation] );
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
		_sizeX = _config.HasSizeX()? _config.GetSizeX(): 1.;
		_sizeY = _config.HasSizeY()? _config.GetSizeY(): 1.;
		_sizeZ = _config.HasSizeZ()? _config.GetSizeZ(): 1.;

		_controlSequence.clear();

		//Load the sequence
		if (_config.GetFilename()=="") // Walk in circles version
		{
			std::cout << "spiral version"<<std::endl;
			_indexTargetX=0;
			_indexTargetY=1;
			_indexTargetAzimuth=2;
			_indexTargetElevation=3;

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
		// Load table from file
		std::ifstream file( _config.GetFilename().c_str() );
		if (!file)
		{
			AddConfigErrorMessage("Unable to open the file "+_config.GetFilename());
			return false;
		}
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
