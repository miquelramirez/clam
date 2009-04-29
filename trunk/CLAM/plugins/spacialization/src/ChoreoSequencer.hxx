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
 Reads a CLAM 3D object choreography and sequences its values as controls.
 It uses  the "sync" input as time synchrony and allows frame seeking by
 using the "frame seek" control.

 Choreo files are text files containing a row per frame and each
 frame contains the following space separated columns:
 -# frame number
 -# field of view (not used)
 -# listener azimuth
 -# listener elevation
 -# listener roll
 -# listener x coord
 -# listener y coord
 -# listener z coord
 -# source0 x coord
 -# source0 y coord
 -# source0 z coord
 -# source1 x coord
 -# ...

 @todo Document where to find the blender choreography exporters

 @param[in] InFilename [Config] The file containing the choreo
 @param[in] SourceIndex [Config] The selected source (choreos may have more than one)
 @param[in] FrameSize [Config] How many samples the controls must be sent
 @param[in] SampleRate [Config] The sample rate of the input audio
 @param[in] ControlsPerSecond [Config] How many lines of the coreos are feed in a second
 @param[in] SizeX [Config] X scaling factor for the coordinates (just used to compute relative angles)
 @param[in] SizeY [Config] Y scaling factor for the coordinates (just used to compute relative angles)
 @param[in] SizeZ [Config] Z scaling factor for the coordinates (just used to compute relative angles)
 @param[in] UseSpiralIfNoFilename [Config] Enables a dummy choreo instead of the file.
 @param[in] "sync" [Port] Audio used as time synchrony
 @param[in] "frame seek" [Control] Change current time position to the one of the received frame.
 @param[out] "frame number" [Control] outputs the current frame number being sent
 @param[out] "listener X" [Control] X coordinate of the listener
 @param[out] "listener Y" [Control] Y coordinate of the listener
 @param[out] "listener Z" [Control] Z coordinate of the listener
 @param[out] "listener roll" [Control] roll of the listener (degrees)
 @param[out] "listener azimuth" [Control] azimuth of the listener (degrees)
 @param[out] "listener elevation" [Control] elevation of the listener (degrees)
 @param[out] "source X" [Control] X coordinate of the audio source
 @param[out] "source Y" [Control] Y coordinate of the audio source
 @param[out] "source Z" [Control] Z coordinate of the audio source
 @param[out] "relative azimuth" [Control] azimuth of the source orientation in listener reference fram (computed, not in file)
 @param[out] "relative elevation" [Control] elevation of the source orientation in listener reference frame (computed, not in file)
 @todo Document ChoreoSequencer
 @ingroup SpatialAudio
*/
class ChoreoSequencer : public CLAM::Processing
{
	class Config : public CLAM::ProcessingConfig
	{ 
		DYNAMIC_TYPE_USING_INTERFACE( Config, 6, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, InFilename, Filename);
		DYN_ATTRIBUTE( 1, public, unsigned, SourceIndex); // first is 0
		DYN_ATTRIBUTE( 2, public, unsigned, FrameSize);
		DYN_ATTRIBUTE( 3, public, unsigned, SampleRate);
		DYN_ATTRIBUTE( 4, public, unsigned, ControlsPerSecond);
		DYN_ATTRIBUTE( 5, public, bool, UseSpiralIfNoFilename);
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
			SetUseSpiralIfNoFilename(false);		
		};
	};

	Config _config;

	FloatInControl _frameSeek;
	FloatOutControl _frame;
	FloatOutControl _listenerX;
	FloatOutControl _listenerY;
	FloatOutControl _listenerZ;
	FloatOutControl _listenerRoll;
	FloatOutControl _listenerAzimuth;
	FloatOutControl _listenerElevation;

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

public:
	ChoreoSequencer(const Config& config = Config()) 
		: _frameSeek("frame seek", this)
		, _frame("frame number", this)
		, _listenerX("listener X", this)
		, _listenerY("listener Y", this)
		, _listenerZ("listener Z", this)
		, _listenerRoll("listener roll", this)
		, _listenerAzimuth("listener azimuth", this)
		, _listenerElevation("listener elevation", this)
		, _sourceX("source X", this)
		, _sourceY("source Y", this)
		, _sourceZ("source Z", this)
		, _sourceAzimuth("relative azimuth", this)
		, _sourceElevation("relative elevation", this)
		, _syncIn("sync", this)
		, _lastFrameSeek(0)
	{
		Configure( config );
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}

	
	void sendRowControls (const Row & row)
	{
		int sourceIndex = _config.GetSourceIndex();
		_frame.SendControl( row[FrameColumn]);

		double listenerX = row[ListenerXColumn];
		double listenerY = row[ListenerYColumn];
		double listenerZ = row[ListenerZColumn];
		double listenerAzimuth = row[ListenerAzimutColumn];
		double listenerElevation =  row[ListenerElevationColumn]; 
		double listenerRoll = row[ListenerRollColumn];
		double sourceX = row[SourceXColumn+3*sourceIndex];
		double sourceY = row[SourceYColumn+3*sourceIndex];
		double sourceZ = row[SourceZColumn+3*sourceIndex];

		_listenerX.SendControl( listenerX );
		_listenerY.SendControl( listenerY );
		_listenerZ.SendControl( listenerZ );
		_listenerAzimuth.SendControl( listenerAzimuth );
		_listenerElevation.SendControl( listenerElevation );
		_listenerRoll.SendControl( listenerRoll ); 

		_sourceX.SendControl( sourceX );
		_sourceY.SendControl( sourceY );
		_sourceZ.SendControl( sourceZ );


		Orientation orientation = AbsoluteCoordinates2RelativeAngles::computeRelativeOrientation(
			listenerX, listenerY, listenerZ,
			listenerAzimuth, listenerElevation, listenerRoll,
			sourceX, sourceY, sourceZ);
		_sourceAzimuth.SendControl( orientation.azimuth );
		_sourceElevation.SendControl( orientation.elevation );
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
		if (_sampleCount>=_samplesPerControl)
		{
			//std::cout << "("<<_sequenceIndex << "/" <<_controlSequence.size() << ")" << std::flush;


			const Row & row = _controlSequence[_sequenceIndex];
			sendRowControls (row);
		/* 
			std::cout 
				<< "\t" << _sizeX*row[ListenerXColumn+3*sourceIndex]
				<< "\t" << _sizeY*row[ListenerYColumn+3*sourceIndex] 
				<< "\t" << _sizeZ*row[ListenerZColumn+3*sourceIndex] 
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
		sendRowControls(_controlSequence[_sequenceIndex]);
		_frameSeek.DoControl(0);
		return true;
	}
	enum FileColumns {
		FrameColumn=0,
		FieldOfView=1,
		ListenerAzimutColumn=2,
		ListenerElevationColumn=3,
		ListenerRollColumn=4,
		ListenerXColumn=5,
		ListenerYColumn=6,
		ListenerZColumn=7,
		SourceXColumn=8,
		SourceYColumn=9,
		SourceZColumn=10,
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
			row.push_back(alpha); // Listener Azimuth
			row.push_back(0.); // Listener Elevation
			row.push_back(0.); // Listener Roll
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
		_controlSequence.clear();

		if (_config.HasUseSpiralIfNoFilename())
		{
			// if UseSpiralIfNoFilename==true, Load the sequence
			if (_config.GetUseSpiralIfNoFilename() and _config.GetFilename()=="") // Walk in circles version
			{
				fillDummyChoreo();
				return true;
			}
		}
		// Load the file version
//		std::cout << "ChoreoSequencer: read from file version. File: "<< _config.GetFilename() << std::endl;
		// Load table from file
		std::ifstream file( _config.GetFilename().c_str() );
		if (not file)
			return AddConfigErrorMessage("Unable to open the file "+_config.GetFilename());
		unsigned lineCount = 0;
		std::string filetype = "ClamChoreoVersion 1.3";
		std::string firstLine;
		std::getline(file,firstLine);
		if (firstLine != ("#"+filetype))
		{
			AddConfigErrorMessage("File format is not a "+filetype);
			return false;			
		}
		while (file)
		{
			lineCount++;
			std::string line;
			std::getline(file, line);
			line.erase(0,line.find_first_not_of("\t\r\n "));
			if (line=="" or line[0]=='#') continue;
			std::istringstream is(line);
			Row row;
			while (is and not is.eof())
			{
				float data;
				is >> data;
				if (not is) break;
				row.push_back(data);
			}
			const unsigned int minSourceColumns=(_config.GetSourceIndex()+1)*3;
			if ( row.size() < 8 + minSourceColumns) // if number of columns is not 8 + 3*sources:
				return AddConfigErrorMessage("Wrong number of columns in choreo file! It needs to have 8, plus 3 more (X,Y,Z positions) for each source.");
			if ( (row.size()-8)%3 != 0 ) // if number of columns is not 8 + 3*sources:
				return AddConfigErrorMessage("Wrong number of columns in choreo file! It needs to have 8, plus 3 more (X,Y,Z positions) for each source.");
			_controlSequence.push_back(row);
		}

		if (!_controlSequence.size())
			return AddConfigErrorMessage("Empty file "+_config.GetFilename());

		if (_samplesPerControl<_frameSize)
			return AddConfigErrorMessage("Samples per control should be greater than frame size. (This limitation is provisional: To be implemented)");
		return true;
	}
};

} //namespace

#endif
