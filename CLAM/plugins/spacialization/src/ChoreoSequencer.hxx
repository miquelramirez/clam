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
			SetControlsPerSecond(24);
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
	OutControl _targetZenith;
	OutControl _sourceX;
	OutControl _sourceY;
	OutControl _sourceZ;
	OutControl _sourceAzimuth;
	OutControl _sourceZenith;
	AudioInPort _syncIn;

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
		: _targetX("target X", this)
		, _targetY("target Y", this)
		, _targetZ("target Z", this)
		, _targetAzimuth("target azimuth", this)
		, _targetZenith("target elevation", this)
		, _sourceX("source X", this)
		, _sourceY("source Y", this)
		, _sourceZ("source Z", this)
		, _sourceAzimuth("source azimuth", this)
		, _sourceZenith("source elevation", this)
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
			//std::cout << "("<<_sequenceIndex << "/" <<_controlSequence.size() << ")" << std::flush;
			const Row & row = _controlSequence[_sequenceIndex];
			//TODO check that _indexTargetX,Y,Z < row.size()
			_targetX.SendControl( row[TargetXColumn] );
			_targetY.SendControl( row[TargetYColumn] );
			_targetZ.SendControl( row[TargetZColumn] );
			_targetAzimuth.SendControl( row[TargetAzimutColumn] );
			_targetZenith.SendControl( row[TargetZenitColumn] );
			_sourceX.SendControl( row[SourceXColumn+3*sourceIndex] );
			_sourceY.SendControl( row[SourceYColumn+3*sourceIndex] );
			_sourceZ.SendControl( row[SourceZColumn+3*sourceIndex] );
			double dx = _sizeX * (row[SourceXColumn+3*sourceIndex] - row[TargetXColumn]);
			double dy = _sizeY * (row[SourceYColumn+3*sourceIndex] - row[TargetYColumn]);
			double dz = _sizeZ * (row[SourceZColumn+3*sourceIndex] - row[TargetZColumn]);
			double cosAzimuth = std::cos(M_PI/180*row[TargetAzimutColumn]);
			double sinAzimuth = std::sin(M_PI/180*row[TargetAzimutColumn]);
			double cosZenith = std::cos(M_PI/180*row[TargetZenitColumn]);
			double sinZenith = std::sin(M_PI/180*row[TargetZenitColumn]);
			double rotatedX = + cosAzimuth*sinZenith * dx + sinAzimuth * dy - cosAzimuth*cosZenith * dz;
			double rotatedY = - sinAzimuth*sinZenith * dx + cosAzimuth * dy + sinAzimuth*cosZenith * dz;
			double rotatedZ = + cosZenith * dx + /* 0 * vy[i] */  + sinZenith  * dz;

			// TODO: Test that with target elevation and azimuth
			double dazimuth = 180./M_PI*std::atan2(rotatedY,rotatedX);
			double delevation = 180./M_PI*std::asin(rotatedZ/std::sqrt(rotatedX*rotatedX+rotatedY*rotatedY+rotatedZ*rotatedZ));
			_sourceAzimuth.SendControl( dazimuth );
			_sourceZenith.SendControl( delevation );
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
		// TODO: What does the column 1 means?
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
			row.push_back(90); // Target Zenith 
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

		//Load the sequence
		if (_config.GetFilename()=="") // Walk in circles version
		{
			fillDummyChoreo();
			return true;
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
