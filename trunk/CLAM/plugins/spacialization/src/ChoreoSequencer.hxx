#ifndef ChoreoSequencer_hxx
#define ChoreoSequencer_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/Filename.hxx>
#include "AbsoluteCoordinates2RelativeAngles.hxx"
#include <string>
#include <cmath>
#include <fstream>

namespace CLAM
{

/**
 @todo Document ChoreoSequencer
 @ingroup SpatialAudio
*/
class ChoreoSequencer : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{ 
		DYNAMIC_TYPE_USING_INTERFACE( Config, 9, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, InFilename, Filename);
		DYN_ATTRIBUTE( 1, public, unsigned, SourceIndex); // first is 0
		DYN_ATTRIBUTE( 2, public, unsigned, FrameSize);
		DYN_ATTRIBUTE( 3, public, unsigned, SampleRate);
		DYN_ATTRIBUTE( 4, public, unsigned, ControlsPerSecond);
		DYN_ATTRIBUTE( 5, public, TData, SizeX);
		DYN_ATTRIBUTE( 6, public, TData, SizeY);
		DYN_ATTRIBUTE( 7, public, TData, SizeZ);
		DYN_ATTRIBUTE( 8, public, bool, UseSpiralIfNoFilename);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetFilename("");
			SetSourceIndex(0);
			SetFrameSize(512);
			SetSampleRate(48000);
			SetControlsPerSecond(24);
			SetSizeX(1);
			SetSizeY(1);
			SetSizeZ(1);
			SetUseSpiralIfNoFilename(false);		
		};
	};

	Config _config;
	FloatInControl _frameSeek;
	FloatOutControl _frame;
	FloatOutControl _targetX;
	FloatOutControl _targetY;
	FloatOutControl _targetZ;
	FloatOutControl _targetAzimuth;
	FloatOutControl _targetElevation;
	FloatOutControl _sourceX;
	FloatOutControl _sourceY;
	FloatOutControl _sourceZ;
	FloatOutControl _sourceAzimuth;
	FloatOutControl _sourceElevation;
	AudioInPort _syncIn;
	unsigned _lastFrameSeek;

	unsigned _samplesPerControl;
	unsigned _sampleCount;
	unsigned _frameSize;
	unsigned _sequenceIndex;

	typedef std::vector<float> Row;
	std::vector<Row> _controlSequence;
	double _sizeX;
	double _sizeY;
	double _sizeZ;

public:
	ChoreoSequencer(const Config& config = Config()) 
		: _frameSeek("frame seek", this)
		, _frame("frame number", this)
		, _targetX("target X", this)
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
		, _lastFrameSeek(0)
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
		unsigned frameSeek = _frameSeek.GetLastValue();
		if (frameSeek != _lastFrameSeek)
		{
			unsigned firstFrame = _controlSequence[0][FrameColumn];
			_sequenceIndex = frameSeek - firstFrame;
			_lastFrameSeek = frameSeek;
		}
		_sampleCount += _frameSize;
		int sourceIndex = _config.GetSourceIndex();
		if (_sampleCount>=_samplesPerControl)
		{
			//std::cout << "("<<_sequenceIndex << "/" <<_controlSequence.size() << ")" << std::flush;
			const Row & row = _controlSequence[_sequenceIndex];
			//TODO check that _indexTargetX,Y,Z < row.size()
			_frame.SendControl( row[FrameColumn]);
			_targetX.SendControl( row[TargetXColumn] );
			_targetY.SendControl( row[TargetYColumn] );
			_targetZ.SendControl( row[TargetZColumn] );
			_targetAzimuth.SendControl( row[TargetAzimutColumn] );

			const bool readElevation = true;
			double targetElevation = readElevation ?  row[TargetZenitColumn] : 90 - row[TargetZenitColumn];
			_targetElevation.SendControl( targetElevation );

			_sourceX.SendControl( row[SourceXColumn+3*sourceIndex] );
			_sourceY.SendControl( row[SourceYColumn+3*sourceIndex] );
			_sourceZ.SendControl( row[SourceZColumn+3*sourceIndex] );

			double targetX = _sizeX * row[TargetXColumn];
			double targetY = _sizeY * row[TargetYColumn];
			double targetZ = _sizeZ * row[TargetZColumn];
			double targetAzimuth = row[TargetAzimutColumn];
			double targetRoll = 0;
			double sourceX = _sizeX * row[SourceXColumn+3*sourceIndex];
			double sourceY = _sizeY * row[SourceYColumn+3*sourceIndex];
			double sourceZ = _sizeZ * row[SourceZColumn+3*sourceIndex];

			Orientation orientation = AbsoluteCoordinates2RelativeAngles::computeRelativeOrientation(
				targetX, targetY, targetZ,
				targetAzimuth, targetElevation, targetRoll,
				sourceX, sourceY, sourceZ);
			_sourceAzimuth.SendControl( orientation.azimuth );
			_sourceElevation.SendControl( orientation.elevation );
			/* 
			std::cout 
				<< "\t" << _sizeX*row[TargetXColumn+3*sourceIndex]
				<< "\t" << _sizeY*row[TargetYColumn+3*sourceIndex] 
				<< "\t" << _sizeZ*row[TargetZColumn+3*sourceIndex] 
				<< "\t" << _sizeX*row[SourceXColumn+3*sourceIndex]
				<< "\t" << _sizeY*row[SourceYColumn+3*sourceIndex] 
				<< "\t" << _sizeZ*row[SourceZColumn+3*sourceIndex] 
				<< "\t\t" << rotatedX << "\t" << rotatedY << "\t" << rotatedZ 
				<< "\t\t" << dazimuth << "\t" << delevation
				<< std::endl;
			*/
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
	bool ConcreteStart()
	{
		_sampleCount=0;
		_sequenceIndex=0;
		return true;
	}
	enum FileColumns {
		FrameColumn=0,
		FieldOfView=1,
		TargetAzimutColumn=2,
		TargetZenitColumn=3,
		TargetXColumn=4,
		TargetYColumn=5,
		TargetZColumn=6,
		SourceXColumn=7,
		SourceYColumn=8,
		SourceZColumn=9,
	};
	void fillDummyChoreo()
	{
//		std::cout << "spiral version"<<std::endl;
		unsigned NPoints = 1000;
		float alpha=0.; //listener angle;
		float horizontalLoops=10;
		float delta=horizontalLoops*2*M_PI/NPoints;
		for (unsigned i=0; i<NPoints; i++)
		{
			double t = float(i)/NPoints;
			float x=0.5+.2*std::cos(delta*i);
			float y=0.5+.2*std::sin(delta*i);
			float z=t;
			Row row;
			row.push_back(i);
			row.push_back(0); //Dummy
			row.push_back(alpha); // Target Azimuth
			row.push_back(0.); // Target Elevation
			row.push_back(.5); // receiver X
			row.push_back(.5); // receiver Y
			row.push_back(.5); // receiver Z
			row.push_back(x);
			row.push_back(y);
			row.push_back(z);
			_controlSequence.push_back(row);
		}
	}
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

		if (_config.HasUseSpiralIfNoFilename())
		{
			// if UseSpiralIfNoFilename==true, Load the sequence
			if (_config.GetUseSpiralIfNoFilename()==true and _config.GetFilename()=="") // Walk in circles version
			{
				fillDummyChoreo();
				return true;
			}
		}
		// Load the file version
//		std::cout << "ChoreoSequencer: read from file version. File: "<< _config.GetFilename() << std::endl;
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
		if (!_controlSequence.size())
		{
			AddConfigErrorMessage("Empty file "+_config.GetFilename());
			return false;
		}
		if (_samplesPerControl<_frameSize)
		{
			AddConfigErrorMessage("Samples per control should be greater than frame size. (This limitation is provisional: To be implemented)");
			return false;
		}	
		return true;
	}
};

} //namespace

#endif
