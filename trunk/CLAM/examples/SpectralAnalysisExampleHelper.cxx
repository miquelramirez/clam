#include "SpectralAnalysisExampleHelper.hxx"
#include "Segment.hxx"
#include "CLAM_Math.hxx"

namespace CLAMExamples
{

	void SpectralAnalysis::DefaultInit()
	{
		AddAll();
		UpdateData();
	}

	void SpectralAnalysisSettings::DefaultInit()
	{
		AddAnalysisWindowWidth();
		AddOverlapFactor();
		AddWindowFunction();
		AddFFTAlgorithm();
		AddOutputSpectrumFormat();
		UpdateData();

		SetAnalysisWindowWidth( 20.f );
		SetOverlapFactor( 50.f );
		SetWindowFunction( "BlackmanHarris92" );
		SetFFTAlgorithm( "FFTW" );
		SetSampleRate( 44100. );

		CLAM::SpecTypeFlags defaultFlags;

		defaultFlags.bComplex = 0;
		defaultFlags.bMagPhase = 1;
		defaultFlags.bPolar = 0;
		defaultFlags.bMagPhaseBPF = 0;

		SetOutputSpectrumFormat( defaultFlags );
	}

	SpectralFourierAnalyzer::SpectralFourierAnalyzer( )
		: mInput( "Input audio samples", this, 1 ),
		  mOutput( "Output frames", this, 1 ),
		  mpFFTAlgorithm( NULL ),
		  mStreamingBuffer( NULL ),
		  mDataProvider( NULL ),
		  mDataConsumer( NULL )
		  
	{
		SetExecState( CLAM::Processing::Unconfigured );		
	}

	SpectralFourierAnalyzer::~SpectralFourierAnalyzer()
	{
		if ( mpFFTAlgorithm )
			delete mpFFTAlgorithm;
		if ( mStreamingBuffer )
			delete mStreamingBuffer;
		if ( mDataProvider )
			delete mDataProvider;
		if ( mDataConsumer )
			delete mDataConsumer;
	}

	bool SpectralFourierAnalyzer::Do()
	{
		CLAM_ASSERT( AbleToExecute(), "This processing is not ready to do anything" );

		CLAM::Audio tmpAudio;

		mStreamingBuffer->GetAndActivate( mDataProvider, tmpAudio );
		tmpAudio.GetBuffer() = mInput.GetData().GetBuffer();
		mStreamingBuffer->LeaveAndAdvance( mDataProvider );

		if ( !mStreamingBuffer->GetAndActivate( mDataConsumer, mAcquiredSamples ) )
		{
			mStreamingBuffer->Leave( mDataConsumer );
			return false;
		}

		do
		{
			mWindowApplier.Do();

			mCircularShift.Do();

			if ( mConfig.HasZeroPaddingFactor() )
				mZeroPadder.Do();

			mpFFTAlgorithm->Do();

			mOutput.GetData().GetResultingFrames().push_back( mResultingSpectrum );

			mStreamingBuffer->LeaveAndAdvance( mDataConsumer );

		} while( mStreamingBuffer->GetAndActivate( mDataConsumer, mAcquiredSamples ) );

		mStreamingBuffer->Leave( mDataConsumer );

		return true;
	}

	
	bool SpectralFourierAnalyzer::ConcreteConfigure( const CLAM::ProcessingConfig& cfg )
	{
		CopyAsConcreteConfig( mConfig, cfg );
		// Construction of FFT algorithm used for analysis
		if ( mConfig.GetFFTAlgorithm() == "FFTW" )
			mpFFTAlgorithm = new CLAM::FFT_rfftw();
		else if ( mConfig.GetFFTAlgorithm() == "NumericalRecipes" )
			mpFFTAlgorithm = new CLAM::FFT_numrec();
		else if ( mConfig.GetFFTAlgorithm() == "Ooura" )
			mpFFTAlgorithm = new CLAM::FFT_ooura();
		else
		{
			mStatus = "Error configuring processing:\n";
			mStatus += "requested algorithm '";
			mStatus += mConfig.GetFFTAlgorithm();
			mStatus += "' is not supported. Supported algorithms are:\n";
			mStatus += "'FFTW', 'NumericalRecipes' and 'Ooura'";

			return false;
		}

		ConfigureData();
		AttachChildren();
		return ConfigureChildren();
	}

	void SpectralFourierAnalyzer::AttachChildren()
	{
		CLAM_ASSERT( mpFFTAlgorithm!=NULL, 
			     "No FFT algorithm has been selected!" );

		mpFFTAlgorithm->SetParent( this );
		mCircularShift.SetParent( this );
		mWindowGen.SetParent( this );
		mWindowApplier.SetParent( this );
		mZeroPadder.SetParent( this );
	}

	bool SpectralFourierAnalyzer::ConfigureChildren()
	{
		CLAM::FFTConfig cfgFFT;
		cfgFFT.SetAudioSize( mEffectiveFFTInputSize );
		
		CLAM_ASSERT( mpFFTAlgorithm!=NULL,
			     "No FFT algorithm was selected!" );

		if (!mpFFTAlgorithm->Configure( cfgFFT ) )
			return false;

		CLAM::CircularShiftConfig cfgCShift;
		cfgCShift.SetAmount( -(mSamplesToAcquire/2) );
		
		if ( !mCircularShift.Configure( cfgCShift ) )
			return false;

		CLAM::WindowGeneratorConfig cfgWinGen;
		cfgWinGen.SetType( mConfig.GetWindowFunction() );
		cfgWinGen.SetSize( mSamplesToAcquire );
		cfgWinGen.SetNormalize( CLAM::EWindowNormalize::eAnalysis );

		if (!mWindowGen.Configure( cfgWinGen ))
			return false;

		CLAM::AudioMultiplierConfig cfgApplier;
		if (!mWindowApplier.Configure( cfgApplier ) )
			return false;

		CLAM::ZeroPadderConfig cfgPadder;
		if (!mZeroPadder.Configure( cfgPadder ) )
			return false;

		return true;
	}

	void SpectralFourierAnalyzer::ConfigureData( )
	{
		CLAM::TData sampleRate = mConfig.GetSampleRate();
		mSamplesToAcquire = CLAM::TSize( (mConfig.GetAnalysisWindowWidth()/1000.)*sampleRate );
		mHopSize = CLAM::TSize( mSamplesToAcquire * (mConfig.GetOverlapFactor()/100.) );

		if ( mSamplesToAcquire %2 == 0 )
			mSamplesToAcquire++;
		
//		mAcquiredSamples.SetSize( mSamplesToAcquire );
		mAcquiredSamples.SetSampleRate( sampleRate );

		mAnalysisWindowSamples.SetSize( mSamplesToAcquire );
		mAnalysisWindowSamples.SetSampleRate( sampleRate );

		(*mWindowGen.GetOutPorts().Begin())->Attach( mAnalysisWindowSamples );


		CLAM::PublishedInPorts::Iterator i = mWindowApplier.GetInPorts().Begin();
		(*i)->Attach( mAcquiredSamples );
		i++;
		(*i)->Attach( mAnalysisWindowSamples );

		mWindowedAudioData.SetSize( mSamplesToAcquire );
		mWindowedAudioData.SetSampleRate( sampleRate );

		(*mWindowApplier.GetOutPorts().Begin())->Attach( mWindowedAudioData );
		
		(*mCircularShift.GetInPorts().Begin())->Attach( mWindowedAudioData );

		mShiftedSamples.SetSize( mSamplesToAcquire );
		mShiftedSamples.SetSampleRate( sampleRate );

		(*mCircularShift.GetOutPorts().Begin())->Attach( mShiftedSamples );

		if ( mConfig.HasZeroPaddingFactor() )
		{
			(*mZeroPadder.GetInPorts().Begin())->Attach( mShiftedSamples );

			mEffectiveFFTInputSize = 
				nextPowerOfTwo( CLAM::TSize(mSamplesToAcquire* pow(2, mConfig.GetZeroPaddingFactor() ) ));

			mFFTInput.SetSize( mEffectiveFFTInputSize );
			mFFTInput.SetSampleRate( sampleRate );

			(*mZeroPadder.GetOutPorts().Begin())->Attach( mFFTInput );
			
			(*mpFFTAlgorithm->GetInPorts().Begin())->Attach( mFFTInput );
		}
		else
		{
			mEffectiveFFTInputSize = mSamplesToAcquire;
			(*mpFFTAlgorithm->GetInPorts().Begin())->Attach( mShiftedSamples );
		}
		
		mResultingSpectrum.SetSize( mEffectiveFFTInputSize/2 + 1 );
		mResultingSpectrum.SetType( mConfig.GetOutputSpectrumFormat() );

		(*mpFFTAlgorithm->GetOutPorts().Begin())->Attach( mResultingSpectrum );

	}

	bool SpectralFourierAnalyzer::ConcreteStart()
	{

		if ( mStreamingBuffer != NULL )
			delete mStreamingBuffer;

		mStreamingBuffer = new CLAM::AudioStreamBuffer< CLAM::CircularStreamImpl< CLAM::TData> >();
		mStreamingBuffer->SetSampleRate( mConfig.GetSampleRate() );
		
		mDataProvider=mStreamingBuffer->NewWriter( mHopSize, mHopSize );
		mDataConsumer=mStreamingBuffer->NewReader( mHopSize, mSamplesToAcquire-1 );
		mStreamingBuffer->Configure( mSamplesToAcquire * 2 );
		

		SpectralAnalysis& outputData = mOutput.GetData();

		outputData.AddAnalysisSettings();
		outputData.UpdateData();
		outputData.SetAnalysisSettings( mConfig );

		CLAM_ASSERT( mpFFTAlgorithm!=NULL,
			     "No FFT algorithm was selected!" );
		mpFFTAlgorithm->Start();
		mCircularShift.Start();
		mWindowGen.Start();
		mWindowApplier.Start();
		
		if ( mConfig.HasZeroPaddingFactor() )
			mZeroPadder.Start();
		
		CLAM_ASSERT( mWindowGen.Do(), "Window generation failed!" );

		return true;
	}

	bool SpectralFourierAnalyzer::ConcreteStop()
	{
		CLAM_ASSERT( mpFFTAlgorithm!=NULL,
			     "No FFT algorithm was selected!" );

		mpFFTAlgorithm->Stop();
		mCircularShift.Stop();
		mWindowGen.Stop();
		mWindowApplier.Stop();
		
		if ( mConfig.HasZeroPaddingFactor() )
			mZeroPadder.Stop();
		
		return true;
	}

}
