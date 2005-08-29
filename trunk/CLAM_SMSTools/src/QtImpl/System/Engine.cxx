#include "XMLStorage.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include "Message.hxx"
#include "ViewManager.hxx"
#include "Engine.hxx"

namespace QtSMS
{
	Engine* Engine::mInstance = 0;

	Engine* Engine::Instance()
	{
		if(!mInstance)
		{
			mInstance = new Engine;
		}
		return mInstance;
	}

	Engine::Engine(){}	 

	ViewManager* Engine::DisplayManager()
	{
		return ViewManager::Instance();
	}

	bool Engine::LoadConfiguration(const std::string& filename)
	{
		LoadConfig(filename);
		return LoadInputSound();
	}

	void Engine::StoreConfiguration(const std::string& filename)
	{
		StoreConfig(filename);
	}

	bool Engine::LoadAnalysis(const std::string& filename)
	{
		return SMSBase::LoadAnalysis(filename);
	}

	void Engine::StoreAnalysis(const std::string& filename)
	{
		mCurrentFileName = filename;
		LaunchMethodOnThread(makeMemberFunctor0(*this,Engine,DoStoreAnalysis));
	}

	void Engine::StoreMelody(const std::string& filename)
	{
		mCurrentFileName = filename;
		LaunchMethodOnThread(makeMemberFunctor0(*this,Engine,DoStoreMelody));
	}

	void Engine::LoadTransformationScore(const std::string& filename)
	{
		// TODO: thread?
		SMSBase::LoadTransformationScore(filename);
	}

	void Engine::StoreTransformationScore(const std::string& filename)
	{
		// TODO: thread?
		SMSBase::StoreTransformationScore(filename);
	}

	void Engine::StoreOutputSound(const std::string& filename)
	{
		StoreSound(GetSynthesizedSound(),filename);
	}

	void Engine::StoreOutputSoundResidual(const std::string& filename)
	{
		StoreSound(GetSynthesizedResidual(),filename);
	}

	void Engine::StoreOutputSoundSinusoidal(const std::string& filename)
	{
		StoreSound(GetSynthesizedSinusoidal(),filename);
	}

	void Engine::Analyze()
	{
		SMSBase::Analyze();
	}

	void Engine::ExtractMelody()
	{
		LaunchMethodOnThread(makeMemberFunctor0(*this,Engine,DoMelodyExtraction));
	}

	void Engine::DoTransformations()
	{
		// TODO: thread
		Transform();
	}

	void Engine::UndoTransformations()
	{
		// TODO
	}

	void Engine::Synthesize()
	{
		SMSBase::Synthesize();
	}

	bool  Engine::LoadInputSound()
	{
		GetState().SetHasAudioIn(LoadSound(mGlobalConfig.GetInputSoundFile(),GetOriginalSegment()));
		if(!GetState().GetHasAudioIn())
		{
			CLAM::VM::Message( QMessageBox::Critical,
							   "SMS Tools 2",
							   "Unavailable to get input sound." );
		}
		GetState().SetHasAudioMorph (LoadSound(mGlobalConfig.GetMorphSoundFile(),mMorphSegment));
		return GetState().GetHasAudioIn();
	}

	bool Engine::LoadSound(const std::string& filename,CLAM::Segment& segment)
	{
		if ( filename.length() <= 0 ) return false;
		
		CLAM::AudioFile selectedFile;
		selectedFile.OpenExisting( filename );

		if ( !selectedFile.IsReadable() )
		{
			std::string messageShown;
			messageShown = "Sorry, but the file ";
			messageShown += filename;
			messageShown += " \ncannot be used: \n";
			messageShown += "Unable to open or unrecognized format";

			CLAM::VM::Message(QMessageBox::Critical,"SMS Tools 2",messageShown.c_str());

			return false;
		}

		if ( selectedFile.GetHeader().GetChannels() > 1 )
		{
			std::string messageShown;
			messageShown = "The file ";
			messageShown += filename;
			messageShown += " \nhas several channels, but SMSTools only \n";
			messageShown += "can process mono channel signals. By default\n";
			messageShown += "the channel to be analyzed will be the first\n";
			messageShown +="channel in the file.\n";

			CLAM::VM::Message(QMessageBox::Critical,"SMS Tools 2",messageShown.c_str());
			
		}
		
		int selectedChannel = 0;
		
		CLAM::MonoAudioFileReaderConfig cfg;
		cfg.SetSourceFile( selectedFile );
		cfg.SetSelectedChannel( selectedChannel );

		CLAM::MonoAudioFileReader fileReader;
		fileReader.Configure( cfg );

		fileReader.Start();

		/////////////////////////////////////////////////////////////////////////////
		// Initialization of the processing data objects :
		CLAM::TSize samplesInFile=int( (selectedFile.GetHeader().GetLength()/1000.)*
			selectedFile.GetHeader().GetSampleRate() );

		SetSamplingRate(int(selectedFile.GetHeader().GetSampleRate()));
		
		// Spectral Segment that will actually hold data
		segment.SetEndTime(selectedFile.GetHeader().GetLength());
		segment.SetSamplingRate(selectedFile.GetHeader().GetSampleRate());
		segment.mCurrentFrameIndex=0;
		segment.GetAudio().SetSize(samplesInFile);
		segment.GetAudio().SetSampleRate(selectedFile.GetHeader().GetSampleRate());

		fileReader.Do(segment.GetAudio());
		fileReader.Stop();

		return true;
	}

	void Engine::StoreSound(const CLAM::Audio& audio, const std::string& filename)
	{
		CLAM::EAudioFileFormat desiredOutputFmt = 
			CLAM::EAudioFileFormat::FormatFromFilename( filename );

		CLAM::AudioFile outputFile;
		CLAM::AudioFileHeader fileHeader;
		fileHeader.SetValues( audio.GetSampleRate(), 1, desiredOutputFmt );

		outputFile.CreateNew( filename, fileHeader );

		CLAM::MonoAudioFileWriterConfig cfgWriter;
		cfgWriter.SetTargetFile(outputFile);

		CLAM::MonoAudioFileWriter proc;
		proc.Configure( cfgWriter );

		proc.Start();
		proc.Do(audio);
		proc.Stop();		
	}

	void Engine::LaunchMethodOnThread(CBL::Functor0 method)
	{
		mThread.SetThreadCode(method);
		mThread.Start();
	}

	void Engine::DoStoreAnalysis()
	{
		SMSBase::StoreAnalysis(mCurrentFileName);
	}

	void Engine::DoStoreMelody()
	{
		CLAM::XMLStorage::Dump(GetMelody(),"Analyzed_Melody",mCurrentFileName);
	}

	void Engine::DoMelodyExtraction()
	{
		mCurrentWaitMessage = CreateWaitMessage("Analyzing melody, please wait");
		SMSBase::AnalyzeMelody();
		DestroyWaitMessage();
	}
}

// END

