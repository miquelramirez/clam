
#include "SMSIo.hxx"
#include "AudioFileOut.hxx"
#include "AudioFileConfig.hxx"

namespace CLAM
{
	TSize
	WAVELoad(const std::string& filename, Segment& segment)
	{
		AudioFileIn myAudioFileIn;
		AudioFileConfig infilecfg;
		infilecfg.SetFilename(filename);
		infilecfg.SetFiletype(EAudioFileType::eWave);
		if(!myAudioFileIn.Configure(infilecfg))
		{
			return 0;
		}

		// Initialization of the processing data objects :
		TSize fileSize=myAudioFileIn.Size();

		TSize SamplingRate=int(myAudioFileIn.SampleRate());

		// Spectral Segment that will actually hold data
		float duration=fileSize/SamplingRate;
		segment.SetEndTime(duration);
		segment.SetSamplingRate(SamplingRate);
		segment.mCurrentFrameIndex=0;
		segment.GetAudio().SetSize(fileSize);
		segment.GetAudio().SetSampleRate(SamplingRate);


		//Read Audio File
		myAudioFileIn.Start();
		myAudioFileIn.Do(segment.GetAudio());
		myAudioFileIn.Stop();
		return SamplingRate;
	}

	bool
	WAVEStore(const std::string& filename, Audio& AudioOutput)
	{
		AudioFileOut myAudioFileOut;
		AudioFileConfig outfilecfg;
		outfilecfg.SetChannels(1);
		outfilecfg.SetFiletype(EAudioFileType::eWave);
		outfilecfg.SetSampleRate(AudioOutput.GetSampleRate());

		outfilecfg.SetFilename(filename);

		myAudioFileOut.Configure(outfilecfg);

		myAudioFileOut.Start();

		myAudioFileOut.Do( AudioOutput );

		myAudioFileOut.Stop( );

		/*
		int nFrames = segment.GetnFrames();
		for( int i=0; i<nFrames; i++ )
		{
			Audio outputAudio = segment.GetFrame(i).GetSynthAudioFrame();

			// is this audio already windowed?
			myAudioFileOut.Do( outputAudio );
		}
		myAudioFileOut.Stop( );
		*/

		return true;
	}

	bool
	SDIFStore(const std::string&  fileName, Segment& segment)
	{
		SDIFOut mSDIFWriter;
		int i;
		SDIFOutConfig cfg;
		int nFrames = segment.GetnFrames();

		cfg.SetSamplingRate( segment.GetSamplingRate() );
		cfg.SetFrameSize((segment.GetFrame(0).GetResidualSpec().GetSize()-1)*2+1);
		cfg.SetSpectrumSize(segment.GetFrame(0).GetResidualSpec().GetSize());

		cfg.SetFileName( fileName );
		cfg.SetEnableResidual( true );

		bool configOk = mSDIFWriter.Configure( cfg );
		CLAM_DEBUG_ASSERT(configOk, "Error storing temporal SDIF for Morph" );

		mSDIFWriter.Start();
		for( i=0; i<nFrames; i++ )
		{
			mSDIFWriter.Do( segment.GetFrame( i ) );
		}
		mSDIFWriter.Stop();

		return true;
	}

	bool
	SDIFLoad(const std::string& filename, Segment& segment)
	{
/*
		try
		{
*/
			SDIFInConfig cfg;
			SDIFIn SDIFLoader;

			cfg.SetFileName( filename );

			if ( !SDIFLoader.Configure( cfg ) )
			{
				// If SDIF file loading file fails we abort the execution
				std::cerr << "ERROR: SDIF file load aborted" << std::endl;
				return false;
			}


			// we start the SDIF loader
			SDIFLoader.Start();
			// we place the data stored in the file onto the newly created segment
			while(SDIFLoader.Do(segment));
			// and we stop the SDIF loader
			SDIFLoader.Stop();
/*		}
		catch( Err& e)
		{
			e.Print();
			return false;
		}
		catch( std::exception& e )
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
*/
		return true;
	}

}

