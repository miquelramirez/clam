
#include "SMSIo.hxx"
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/SDIFOut.hxx>
#include <CLAM/SDIFIn.hxx>

namespace CLAM
{
	TSize
	WAVELoad(const std::string& filename, Segment& segment)
	{
		MonoAudioFileReaderConfig infilecfg;
		infilecfg.SetSourceFile(filename);
		MonoAudioFileReader myAudioFileIn(infilecfg);
		if(!myAudioFileIn.IsConfigured())
		{
			return 0;
		}

		// Initialization of the processing data objects :
		TSize fileSize=myAudioFileIn.GetHeader().GetSamples();

		TSize samplingRate=int(myAudioFileIn.GetHeader().GetSampleRate());

		// Spectral Segment that will actually hold data
		float duration=fileSize/samplingRate;
		segment.SetEndTime(duration);
		segment.SetSamplingRate(samplingRate);
		segment.mCurrentFrameIndex=0;
		segment.GetAudio().SetSize(fileSize);
		segment.GetAudio().SetSampleRate(samplingRate);


		//Read Audio File
		myAudioFileIn.Start();
		myAudioFileIn.Do(segment.GetAudio());
		myAudioFileIn.Stop();
		return samplingRate;
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
		try
		{
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
		}
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
		return true;
	}

}

