#include "CSaltoDSP.hxx"
#include "CSaltoEditor.hxx"
#include "Assert.hxx"
#include <pthread.h>
#include <iostream>
#include "HeapDbg.hxx"

#ifdef WIN32
	#include <windows.h>
	#undef CreateWindow
	#undef GetClassName
#endif

//#define AUDIO_FILE_OUT

namespace CLAM
{

pthread_mutex_t DrawingMutex = PTHREAD_MUTEX_INITIALIZER;


void setscheduler(void)
{
	#ifdef LINUX
		struct sched_param sched_param;
		int policy;
			
		if (pthread_getschedparam(pthread_self(), &policy, &sched_param) < 0) {
			throw Err("Failed to get scheduler settings");
			return;
		}
		sched_param.sched_priority = sched_get_priority_max(SCHED_FIFO);
		if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sched_param)) {
			throw Err("Failed to set scheduler");
		}
	#endif //LINUX
	#ifdef WIN32
		BOOL res;
		DWORD err;
		
		// Maximum priority for the process: this has the upper hand over
		// everything else, including most essential OS services
		//res = SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		// Toned down priority: this enables cache flush (at least)
		//res = SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS );

		// With the new AudioIO approach (POLL instead of NOTIFY), it seems that
		// we should NORMAL priority. If not, the system gets very slow.
		// 
		// the other solution (now used) is to have a Sleep in the polling loop
		res = SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS );

		err = GetLastError();
		
		// Maximum priority for the thread under Win32
		//res = SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);
		//Toned down thread priority
		//res = SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST );
		// With the new AudioIO approach (POLL instead of NOTIFY), it seems that
		// we should should NORMAL priority. If not, the system gets very slow
		res = SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_NORMAL );
		err = GetLastError();
	#endif //WIN32
}

void CSaltoDSPConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();

	SetAnalysisFrameSize( 256 );
	SetSampleRate( 44100 );
	SetAnalysisWindowType( EWindowType::eBlackmanHarris92 );
	SetPitchModRange( 0.05946309436 ); // 1 semitone
}

CSaltoDSP::CSaltoDSP():
//  GLOBAL_DRAWING      (0),
//	mpMIDI              (pMIDI),
	mpSpectralBuffer		(NULL),
	mpSynthFrame				(NULL),
	mpDataManagment			(NULL),
	mpCurrPeakArrayTarget(NULL),
	mpTransitionFrameBase(NULL),
	mpTransitionFrameTarget(NULL),
	mpCurrSpectralFrameBase(NULL),
	mpCurrSpectralFrameResidual(NULL),
	mFrameCounterSDIFEdit(0),
	mSynthesisFlow      (FLOW_FOLLOW_TRACK),
	mEventSample				(0),
	mLoopStart          (0),
	mLoopEnd            (0),
	mLimitLoopStart     (0),
	mLimitLoopEnd       (0),
	mInterpolEnd        (0),
	mInterpolStart      (0),
	mResFadeStart       (0),
	mResFadeEnd         (0),
	mAttackResVolume    (1.0),
	mStatResVolume      (1.0),
	mIPFactor           (0.0),
	mStatResFadeInFrom  (20),
	mStatResFadeInTo    (40),
	mLoopDirectionFW    (true),
	mStatResLoopDirectionFW(true),
	mTestCounter        (0),
	mTestFreq1          (440),
	mTestFreq2          (440),
	mTestPhase1         (0),
	mTestPhase2         (0),
	mEditFrameCounter   (0),
	mFrameCounterStatRes(0),
	mFrameCounterAttackResidual(0),
	mNumFramesStatRes   (0),
	mSinAttackOffset    (0),
	mSinAttackOffsetCounter(0),
	mIndividualGain(50.0),
	mLastPitchCorrectionFactor(0),
	mpSineSynthPO(NULL),
	mpResSynthPO(NULL),
	mpInterpolPO(NULL),
	mStateIn( "StateIn", this, &CSaltoDSP::UpdateState ),
	mState( Idle )
{
		CSaltoDSPConfig cfg;

		Configure( cfg );
}

CSaltoDSP::CSaltoDSP( const CSaltoDSPConfig& cfg ):
//  GLOBAL_DRAWING      (0),
//	mpMIDI              (pMIDI),
	mpSpectralBuffer		(NULL),
	mpSynthFrame				(NULL),
	mpDataManagment			(NULL),
	mpCurrPeakArrayTarget(NULL),
	mpTransitionFrameBase(NULL),
	mpTransitionFrameTarget(NULL),
	mpCurrSpectralFrameBase(NULL),
	mpCurrSpectralFrameResidual(NULL),
	mFrameCounterSDIFEdit(0),
	mSynthesisFlow      (FLOW_FOLLOW_TRACK),
	mEventSample				(0),
	mLoopStart          (0),
	mLoopEnd            (0),
	mLimitLoopStart     (0),
	mLimitLoopEnd       (0),
	mInterpolEnd        (0),
	mInterpolStart      (0),
	mResFadeStart       (0),
	mResFadeEnd         (0),
	mAttackResVolume    (1.0),
	mStatResVolume      (1.0),
	mIPFactor           (0.0),
	mStatResFadeInFrom  (20),
	mStatResFadeInTo    (40),
	mLoopDirectionFW    (true),
	mStatResLoopDirectionFW(true),
	mTestCounter        (0),
	mTestFreq1          (440),
	mTestFreq2          (440),
	mTestPhase1         (0),
	mTestPhase2         (0),
	mEditFrameCounter   (0),
	mFrameCounterStatRes(0),
	mFrameCounterAttackResidual(0),
	mNumFramesStatRes   (0),
	mSinAttackOffset    (0),
	mSinAttackOffsetCounter(0),
	mIndividualGain(50.0),
	mLastPitchCorrectionFactor(0),
	mpSineSynthPO(NULL),
	mpResSynthPO(NULL),
	mpInterpolPO(NULL),
	mStateIn( "StateIn", this, &CSaltoDSP::UpdateState ),
	mState( Idle )
{
		Configure( cfg );
}

bool CSaltoDSP::ConcreteConfigure( const ProcessingConfig& cfg)throw( std::bad_cast )
{
	mConfig = dynamic_cast< const CSaltoDSPConfig& >( cfg );
	
	mpParams = mConfig.GetParams();

	mMelody = mConfig.GetMelody();

	mMelody->LinkOutWithInControl( 0, &mMIDIHandler, 0);
	mMelody->LinkOutWithInControl( 1, &mMIDIHandler, 1);

	mpSynthFrame= new CSaltoSynthFrame(	MAX_SINES, 
										mConfig.GetSpectralFrameSize(), 
										mConfig.GetSampleRate() );
	
	
	AudioOutBuffer.SetSize(mConfig.GetHopSize());
	AudioInBuffer.SetSize(mConfig.GetHopSize());
	EmptyAudioBuffer.SetSize(mConfig.GetHopSize());


	MIDIHandlerConfig MIDIHandlerCfg;
	MIDIHandlerCfg.SetTranspose( mpParams->GetTranspose() );
	MIDIHandlerCfg.SetPitchModRange( mConfig.GetPitchModRange() );
	mMIDIHandler.Configure( MIDIHandlerCfg );

	mMIDIHandler.LinkOutWithInControl( 0, this, 0 ); 

	mDrawingBuffer.Resize(AudioOutBuffer.GetSize());
	mDrawingBuffer.SetSize(AudioOutBuffer.GetSize());
	

	SpectrumConfig Scfg; //BUGSPECTRUM? no creo
	Scfg.SetScale(EScale::eLinear);
	SpecTypeFlags sflags;
	sflags.bComplex = 1;
	sflags.bPolar = 0;
	sflags.bMagPhase = 0;
	sflags.bMagPhaseBPF = 0;
	Scfg.SetType(sflags);
	Scfg.SetSize(mConfig.GetSpectralFrameSize());
	Scfg.SetSpectralRange(mConfig.GetSpectralRange());
	mpSpectralBuffer = new Spectrum(Scfg);

	mpDataManagment = new CSaltoDataManagment(mpParams);
	

	// processing objects
	mpSineSynthPO = new CSaltoSineSynthesis(	mConfig.GetSpectralFrameSize(),
												MAX_SINES,
												mConfig.GetFrameTime(),
												mConfig.GetSampleRate(),
												mpParams);
																							
	mpResSynthPO = new CSaltoResidualSynthesis(	mConfig.GetSpectralFrameSize(),
												mConfig.GetFrameTime());
	SpectralSynthesisConfig sscfg;
	sscfg.SetFrameSize(mConfig.GetHopSize());
	sscfg.SetHopSize(mConfig.GetHopSize());
	sscfg.SetAnalWindowType(mConfig.GetAnalysisWindowType());
	sscfg.SetSamplingRate(mConfig.GetSampleRate());
	sscfg.SetSynthWindowSize(mConfig.GetAudioFrameSize()+1);
	mSpectralSynthesisPO.Configure(sscfg);
	mSpectralSynthesisPO.Start();
			
	CSaltoInterpolationConfig interpCfg;
	interpCfg.SetParams( *mpParams );
	interpCfg.SetSpectralRange( mConfig.GetSpectralRange() );

	mpInterpolPO				= new CSaltoInterpolation( interpCfg );
	
	if(mpSineSynthPO==NULL||mpResSynthPO==NULL
										||mpInterpolPO==NULL)
			throw Err("OOM in CSaltoDSP::CSaltoDSP");

	ConfigureSaltoIO();

	return true;
}

bool CSaltoDSP::ConcreteStart()
{
	mMIDIHandler.Start();
	mSpectralSynthesisPO.Start();

	return true;
}

bool CSaltoDSP::ConcreteStop()
{
	mMIDIHandler.Stop();
	mSpectralSynthesisPO.Stop();

	return true;
}
	/*
CSaltoDSP::CSaltoDSP(Parameters *pParams/*,CSaltoMIDIInOut *pMIDI*///):
//  GLOBAL_DRAWING      (0),
/*	mpParams						(pParams),
//	mpMIDI              (pMIDI),
/*	mpSpectralBuffer		(NULL),
	mpSynthFrame				(NULL),
	mpDataManagment			(NULL),
	mpCurrPeakArrayTarget(NULL),
	mpTransitionFrameBase(NULL),
	mpTransitionFrameTarget(NULL),
	mpCurrSpectralFrameBase(NULL),
	mpCurrSpectralFrameResidual(NULL),
	mFrameCounterSDIFEdit(0),
	mSynthesisFlow      (FLOW_FOLLOW_TRACK),
	mEventSample				(0),
	mLoopStart          (0),
	mLoopEnd            (0),
	mLimitLoopStart     (0),
	mLimitLoopEnd       (0),
	mInterpolEnd        (0),
	mInterpolStart      (0),
	mResFadeStart       (0),
	mResFadeEnd         (0),
	mAttackResVolume    (1.0),
	mStatResVolume      (1.0),
	mIPFactor           (0.0),
	mStatResFadeInFrom  (20),
	mStatResFadeInTo    (40),
	mLoopDirectionFW    (true),
	mStatResLoopDirectionFW(true),
	mTestCounter        (0),
	mTestFreq1          (440),
	mTestFreq2          (440),
	mTestPhase1         (0),
	mTestPhase2         (0),
	mEditFrameCounter   (0),
	mFrameCounterStatRes(0),
	mFrameCounterAttackResidual(0),
	mNumFramesStatRes   (0),
	mSinAttackOffset    (0),
	mSinAttackOffsetCounter(0),
	mIndividualGain(50.0),
	mLastPitchCorrectionFactor(0),
	mpSineSynthPO(NULL),
	mpResSynthPO(NULL),
	mpInterpolPO(NULL)
{

	// storage
	mpSynthFrame				= new CSaltoSynthFrame(	MAX_SINES, mpParams->GetSpectralFrameSize(), mpParams->GetSampleRate() );
	
	
	AudioOutBuffer.SetSize(mpParams->GetHopSize());
	AudioInBuffer.SetSize(mpParams->GetHopSize());
	EmptyAudioBuffer.SetSize(mpParams->GetHopSize());


	CSaltoHandleParamChangesConfig handleParamCfg;
	handleParamCfg.SetTranspose( pParams->GetTranspose() );
	mHandleParams.Configure( handleParamCfg );


	mDrawingBuffer.Resize(AudioOutBuffer.GetSize());
	mDrawingBuffer.SetSize(AudioOutBuffer.GetSize());
	

	SpectrumConfig Scfg; //BUGSPECTRUM? no creo
	Scfg.SetScale(EScale::eLinear);
	SpecTypeFlags sflags;
	sflags.bComplex = 1;
	sflags.bPolar = 0;
	sflags.bMagPhase = 0;
	sflags.bMagPhaseBPF = 0;
	Scfg.SetType(sflags);
	Scfg.SetSize(mpParams->GetSpectralFrameSize());
	Scfg.SetSpectralRange(mpParams->GetSpectralRange());
	mpSpectralBuffer = new Spectrum(Scfg);

	mpDataManagment = new CSaltoDataManagment(mpParams);
	

	// processing objects
	mpSineSynthPO = new CSaltoSineSynthesis(  mpParams->GetSpectralFrameSize(),
	                                          MAX_SINES,
																						mpParams->GetFrameTime(),
																						mpParams->GetSampleRate(),
																						mpParams);
																							
	mpResSynthPO	= new CSaltoResidualSynthesis(	mpParams->GetSpectralFrameSize(),
																								mpParams->GetFrameTime());
	SpectralSynthesisConfig sscfg;
	sscfg.SetFrameSize(mpParams->GetHopSize());
	sscfg.SetHopSize(mpParams->GetHopSize());
	sscfg.SetAnalWindowType(mpParams->GetAnaWindowType());
	sscfg.SetSamplingRate(mpParams->GetSampleRate());
	sscfg.SetSynthWindowSize(mpParams->GetAudioFrameSize()+1);
	mSpectralSynthesisPO.Configure(sscfg);
	mSpectralSynthesisPO.Start();
																									
	mpInterpolPO				= new CSaltoInterpolation(mpParams);
	
	if(mpSineSynthPO==NULL||mpResSynthPO==NULL
										||mpInterpolPO==NULL)
			throw Err("OOM in CSaltoDSP::CSaltoDSP");

}
*/
//----------------------------------------------------------------------------//

CSaltoDSP::~CSaltoDSP()
{
	//storage
	if (mpSpectralBuffer!=NULL)
		delete mpSpectralBuffer;
	
	if (mpSynthFrame!= NULL)
		delete mpSynthFrame;
		
	if (mpDataManagment!= NULL)
		delete mpDataManagment;
	
	// processing objects
	if (mpSineSynthPO!=NULL)
		delete mpSineSynthPO;
	
	if (mpResSynthPO!=NULL)
		delete mpResSynthPO;
		
	if (mpInterpolPO!=NULL)
		delete mpInterpolPO;
}
//#pragma mark -
//----------------------------------------------------------------------------//
/** MAIN EVENT LOOP
  the CSaltoDSP::Do()-Method is called from SaltoApp. We enter the main loop
  and do all the flow specific stuff. Dued to the CLAM Structure we deal with two
  loops. One when the port audio is activated and one when it is inactive. 
  The problem is that we call the GUI-Update in the method mpAudioIO->Idle(),
  this is either done in 
  case a.) in the idle time after audio processing (sync())  
  case b.) directly from the loop in CSaltoDSP::Do()
*/
//----------------------------------------------------------------------------//

#ifdef ALSA
extern void audioloop(void);
#endif //ALSA

void CSaltoDSP::ConfigureSaltoIO()
{
	AudioFileConfig outcfg;
	outcfg.SetFilename("salto.wav");
	outcfg.SetFiletype(EAudioFileType::eWave);
	outcfg.SetChannels(1);

	mFileAudioOut.Configure( outcfg );

	
	mAudioManager = new AudioManager( mConfig.GetSampleRate(), 4096);
	

	AudioIOConfig iocfg;
	iocfg.SetName("left out");
	iocfg.SetChannelID(0);
	mAudioOut = new AudioOut(iocfg);
	iocfg.SetName("left in");
	mAudioIn = new AudioIn(iocfg);


	mDummyIn.SetSize( mConfig.GetHopSize() );
}

void CSaltoDSP::HandleMIDI(void)
{
	if (mpParams->GetUseMelody())
		{
			TTime currentTime = mEventSample/mConfig.GetSampleRate();
			if (!mMelody->Do( mpParams->GetPlay(), currentTime ))
				{
					mMIDIHandler.Do( *mpParams );
					mpParams->SetUseMelody( false );
					mEventSample = 0.0;
				}
			else
				mMIDIHandler.Do( *mpParams );
		}
		
	if ( mpParams->GetUseMidiKeyboard() || mpParams->GetUseBreathController() )
		{
			mMIDIManager.Check();
			mMIDIHandler.Do( *mpParams );
		}		

	// End of SALTO MIDI Messages Processing
}

void CSaltoDSP::Do( bool& sentinel)
{
//#ifdef AUDIO_FILE_OUT
	AudioFileOut		   mFileAudioOut;
	AudioFileConfig outcfg;
	outcfg.SetFilename("salto.wav");
	outcfg.SetFiletype(EAudioFileType::eWave);
	outcfg.SetChannels(1);

	mFileAudioOut.Configure( outcfg );

	mFileAudioOut.Start();

//#else
	AudioManager manager(mConfig.GetSampleRate(),4096);


	Audio                  dummyAudioBuffer;
	dummyAudioBuffer.SetSize( mConfig.GetHopSize() );

	AudioOut               mAudioOut;
	AudioIn                mAudioIn;
	
	AudioIOConfig iocfg;
	iocfg.SetName("left out");
	iocfg.SetChannelID(0);
	mAudioOut.Configure(iocfg);
	iocfg.SetName("left in");
	mAudioIn.Configure(iocfg);

	
	MIDIManager midiManager;

	MIDIInConfig inNoteCfg;

	inNoteCfg.SetName("in");
	inNoteCfg.SetDevice("default:default");
	inNoteCfg.SetChannelMask(
		MIDI::ChannelMask(1)|
		MIDI::ChannelMask(2)
	);

	inNoteCfg.SetMessageMask(
		MIDI::MessageMask(MIDI::eNoteOn)|
		MIDI::MessageMask(MIDI::eNoteOff)
	);

	MIDIInControl inNote(inNoteCfg);

	MIDIInConfig inPitchBendCfg;
	
	inPitchBendCfg.SetName("inPithcBend");
	inPitchBendCfg.SetDevice("default:default");
	inPitchBendCfg.SetChannelMask(MIDI::ChannelMask(1));
	inPitchBendCfg.SetMessageMask(MIDI::MessageMask(MIDI::ePitchbend));
	
	MIDIInControl inPitchBend(inPitchBendCfg);

	inPitchBend.LinkOutWithInControl( 0, &mMIDIHandler, 2 );

	inNote.LinkOutWithInControl( 0, &mMIDIHandler, 1 );
	inNote.LinkOutWithInControl( 1, &mMIDIHandler, 0 );
	inNote.LinkOutWithInControl( 2, &mMIDIHandler, 1 );
	inNote.LinkOutWithInControl( 3, &mMIDIHandler, 0 );
	
	midiManager.Start();
	manager.Start();
	
	mFileAudioOut.Start();
	mMIDIManager.Start();
	mAudioManager->Start();

//#endif
	bool startMelody=true;
	while (!mpParams->GetExit()&& !sentinel )
	{
		if (mpParams->GetUseMelody())
		{
			if(startMelody){
				mEventSample=0;
				//mMelody.GetNoteArray().DoFirst();
				mMelody->Do();
				startMelody=false;}
			if(!UpdateParams())
			{
				startMelody=true;
				//mpParams->SetExit(true);
				mpParams->SetUseMelody(false);
				//break;
			}
		}
		
		if(mpParams->GetUseMidiKeyboard())
		{
			midiManager.Check();

			mMIDIHandler.Do( *mpParams );
		}
		
		if(mpParams->GetPlay())
		{
  			mpSynthFrame->SetSynthTime(0.0);
			mpSineSynthPO->ResetSineSynthesis();
      // --------- process specific inits on first Note On
			switch (mpParams->GetSynthesisFlow())
			{
      			case FLOW_INTERPOLATE:
        			InitInterpolatingSynthesis();
       				break;
      			case FLOW_GENERATE_SINE:
        			InitGenerateTestSine();
       	 			break;
      			case FLOW_FOLLOW_TRACK:
        			InitFollowOneTrackSynthesis();
        			break;
      			case FLOW_SDIF_EDIT:
        			InitSDIFEditSynthesis();
        			break;
 			}
      //  --calc synthframe for output --
			while(mpParams->GetPlay())
			{
				if(mpParams->GetUseMelody())
				{
					if(!UpdateParams())
					{
						startMelody=true;
						//mpParams->SetExit(true);
						mpParams->SetUseMelody(false);
						break;
					}
				}

				if(mpParams->GetUseMidiKeyboard())
				{
					midiManager.Check();

					mMIDIHandler.Do( *mpParams );
				}

//#ifndef AUDIO_FILE_OUT				
				if(!mpParams->GetWriteToFile())
					mAudioIn.Do( dummyAudioBuffer );
//#endif
				switch (mpParams->GetSynthesisFlow())
	      		{
        			case FLOW_INTERPOLATE:
        				if(mpParams->GetTransitionMode())
        					DoTransitionSynthesis(mpSynthFrame); // transition between two notes
						else
    	      				DoInterpolatingSynthesis(mpSynthFrame);
        	  			break;

					case FLOW_GENERATE_SINE:
			    		DoGenerateTestSineSynthesis(mpSynthFrame);
          				break;

					case FLOW_FOLLOW_TRACK:
        	 			DoFollowOneTrackSynthesis(mpSynthFrame);
		       			break;

					case FLOW_SDIF_EDIT:
        				DoSDIFEditSynthesis(mpSynthFrame);
		       			break;
				}

				//  -- generate output from synthframe --
				DoSynthesisProcess(mpSynthFrame);
				//	std::cout << "P" << std::endl;
//#ifdef AUDIO_FILE_OUT
				if(mpParams->GetWriteToFile())
				mFileAudioOut.Do( AudioOutBuffer );
//#else			
				else
					mAudioOut.Do(AudioOutBuffer);
//#endif						
     		}
   			// -- release part --
     		switch (mpParams->GetSynthesisFlow())
     		{
     			case FLOW_INTERPOLATE:
        			mpParams->SetTransitionMode(false);
        			DoInterpolatingSynthesisRelease(mpSynthFrame,0.9);
        			break;

				case FLOW_GENERATE_SINE:
        			break;

				case FLOW_FOLLOW_TRACK:
        			break;

				case FLOW_SDIF_EDIT:
        			break;
     		}
		}
		else
		{
			
//#ifdef AUDIO_FILE_OUT
			if(mpParams->GetWriteToFile()&&mpParams->GetUseMelody())
//			if(mpParams->GetUseMelody())
				mFileAudioOut.Do(EmptyAudioBuffer); 
//#else
			else
			{
				mAudioIn.Do( dummyAudioBuffer );
				mAudioOut.Do(EmptyAudioBuffer); //otherwise the file will be filled with a lot of zeros
			}
//#endif		
			if(mpParams->GetUseMelody())
			{
				mEventSample += mConfig.GetAnalysisFrameSize();	// Count processed samples
			}

		}

		std::cout << "." << std::endl;
	}

//#ifdef AUDIO_FILE_OUT
	if(mpParams->GetWriteToFile())
	{	
		std::cout << "closing........... " << std::endl;
		mpParams->SetExit(true);
		mFileAudioOut.Stop();
	}
//#endif

}

void CSaltoDSP::RenderSynthesis()
{
	if(mpParams->GetWriteToFile())
		mFileAudioOut.Do( AudioOutBuffer );
	else
	{
		mAudioIn->Do( mDummyIn );
		mAudioOut->Do(AudioOutBuffer);
	}
}

void CSaltoDSP::RenderSilence()
{
	if(mpParams->GetWriteToFile()&&mpParams->GetUseMelody())
		mFileAudioOut.Do(EmptyAudioBuffer); 
	else
		{
		//	mAudioIn.Do( dummyAudioBuffer );
			mAudioIn->Do( mDummyIn );
			mAudioOut->Do(EmptyAudioBuffer); //otherwise the file will be filled with a lot of zeros
		}
}

void CSaltoDSP::DoFlowInterpolate( bool& sentinel )
{
	MIDIInConfig inNoteCfg;

	inNoteCfg.SetName("in");
	inNoteCfg.SetDevice("default:default");
	inNoteCfg.SetChannelMask( 
								MIDI::ChannelMask(1) |
								MIDI::ChannelMask(2)
							);

	inNoteCfg.SetMessageMask(
		MIDI::MessageMask(MIDI::eNoteOn)|
		MIDI::MessageMask(MIDI::eNoteOff)
	);

	MIDIInControl keyboardNote( inNoteCfg );

	MIDIInConfig inPitchBendCfg;
	
	inPitchBendCfg.SetName("inPithcBend");
	inPitchBendCfg.SetDevice("default:default");
	inPitchBendCfg.SetChannelMask(MIDI::ChannelMask(1));
	inPitchBendCfg.SetMessageMask(MIDI::MessageMask(MIDI::ePitchbend));

	MIDIInControl pitchBend( inPitchBendCfg );

	MIDIInConfig inBreathNoteCfg;

	inBreathNoteCfg.SetName("in2");
	inBreathNoteCfg.SetDevice("default:default");
	inBreathNoteCfg.SetChannelMask( 				
									MIDI::ChannelMask(3) |
									MIDI::ChannelMask(4)
								  );

	inBreathNoteCfg.SetMessageMask(
		MIDI::MessageMask(MIDI::eNoteOn)|
		MIDI::MessageMask(MIDI::eNoteOff)
	);

	MIDIInConfig inCtrlCfg;
	
	inCtrlCfg.SetName("inctrl");
	inCtrlCfg.SetDevice("default:default");
	inCtrlCfg.SetChannelMask(MIDI::ChannelMask(2));
	inCtrlCfg.SetMessageMask(MIDI::MessageMask(MIDI::eControlChange));
	inCtrlCfg.SetFilter(0x02);

	BreathController breathController( inBreathNoteCfg, inCtrlCfg );

	pitchBend.LinkOutWithInControl( 0, &mMIDIHandler, 2);

	keyboardNote.LinkOutWithInControl( 0, &mMIDIHandler, 1);
	keyboardNote.LinkOutWithInControl( 1, &mMIDIHandler, 0);
	keyboardNote.LinkOutWithInControl( 2, &mMIDIHandler, 1);
	keyboardNote.LinkOutWithInControl( 3, &mMIDIHandler, 0);

	mFileAudioOut.Start();
	mMIDIManager.Start();
	mAudioManager->Start();
	// End of Salto IO Setup
	bool startMelody = true;
	

	while ( !mpParams->GetExit() && !sentinel )
		{
			//  SALTO MIDI Messages Processing

			HandleMIDI(); // MIDI things startup

			if ( mpParams->GetPlay() )
				{
					mpSynthFrame->SetSynthTime( 0.0 );
					mpSineSynthPO->ResetSineSynthesis();

					InitInterpolatingSynthesis();

					while ( mpParams->GetPlay() )
						{
							if ( mpParams->GetTransitionInit() )
							{
									InitTransitionSynthesis( mpSynthFrame );

									while ( mFrameCounterTransition < mNumTransitionFrames )
										{
											DoTransitionSynthesis2( mpSynthFrame );
											
											DoStationarySynthesisProcess( mpSynthFrame );
											
											RenderSynthesis();
											
											UpdateGUI();
											
											HandleMIDI();
											
											mFrameCounterTransition++;
										}
							
									EndTransitionSynthesis( mpSynthFrame );
								}
							else
								{
									DoInterpolatingSynthesis( mpSynthFrame );

									mState = Sustain; // Attack ends

									std::cout << "SALTO State: Sustain " <<  std::endl;
									
									DoStationarySynthesisProcess( mpSynthFrame );
							
									RenderSynthesis();

									UpdateGUI();

									HandleMIDI();
								}
						}
					CLAM_CHECK_HEAP("ADS Synthesis corrupted the heap");

					// Note release synthesis
					mpParams->SetTransitionMode( false );
					
					int nReleaseFrames = 6; // very simple release to avoid clicks
					int currRelFrame = 1;

					while ( !mpParams->GetPlay() && (currRelFrame < nReleaseFrames ) )
					{

						DoInterpolatingReleaseSynthesis( mpSynthFrame, 0.9, currRelFrame  );

						DoReleaseSynthesisProcess( mpSynthFrame );

						RenderSynthesis();

						UpdateGUI();

						HandleMIDI();

						currRelFrame++;
					}

					mState = Idle;

					std::cout << "SALTO State: Idle " << std::endl;

					CLAM_CHECK_HEAP("Release Synthesis corrupted the heap");

				}
			else
				{
					RenderSilence();
					if ( mpParams->GetUseMelody())
						mEventSample +=mConfig.GetAnalysisFrameSize();
				}
			
		//	std::cout << "samples:" << mEventSample << std::endl;
		}

	// Closing file
	if ( mpParams->GetWriteToFile())
		{
			std::cout << "Closing audio File ..... " << std::endl;
			mpParams->SetExit(true); // MRJ: Ya lo es no?
			mFileAudioOut.Stop();
		}
}

int CSaltoDSP::UpdateState( TControlData state )
{
	if( mState == Termination ) // Ignore any update data
		return 0;
	if( state == 1 )
	{
		mState = Attack;
		return 0;
	}
	if( state == 2 )
	{
		mState = Transition;
		return 0;
	}
	if( ( state == 4 ) && ( ( mState == Sustain ) || ( mState == Transition ) ) )
	{
		mState = Release;
		return 0;
	}

	return 0;
}

void CSaltoDSP::HandleIdle(void)
{
	RenderSilence();
	if ( mpParams->GetUseMelody())
		mEventSample +=mConfig.GetAnalysisFrameSize();
}

void CSaltoDSP::HandleAttack(void)
{
	mpSynthFrame->SetSynthTime( 0.0 );
	mpSineSynthPO->ResetSineSynthesis();

	InitInterpolatingSynthesis();

	DoInterpolatingSynthesis(mpSynthFrame);
	DoSynthesisProcess(mpSynthFrame);

	mState = Sustain;

	RenderSynthesis();
	UpdateGUI();
}

void CSaltoDSP::HandleRelease(void)
{
	mpParams->SetTransitionMode( false );
	
	int nReleaseFrames = 6; // very simple release to avoid clicks
	int currRelFrame = 1;

	while ( !mpParams->GetPlay() && (currRelFrame < nReleaseFrames ) )
	{

		DoInterpolatingReleaseSynthesis( mpSynthFrame, 0.9, currRelFrame  );

		DoReleaseSynthesisProcess( mpSynthFrame );

		RenderSynthesis();

		UpdateGUI();

		currRelFrame++;
	}

	CLAM_CHECK_HEAP("Release Synthesis corrupted the heap");

	mState = Idle;
}

void CSaltoDSP::HandleTransition(void)
{
	DoTransitionSynthesis(mpSynthFrame);
	DoSynthesisProcess(mpSynthFrame);

	RenderSynthesis();
	UpdateGUI();

	mState = Sustain;
}

void CSaltoDSP::HandleSustain(void)
{
	DoInterpolatingSynthesis(mpSynthFrame);
	DoSynthesisProcess(mpSynthFrame);

	RenderSynthesis();
	UpdateGUI();
}

void CSaltoDSP::DoFlowInterpolate2( bool& sentinel )
{
	MIDIInConfig inNoteCfg;

	inNoteCfg.SetName("in");
	inNoteCfg.SetDevice("default:default");
	inNoteCfg.SetChannelMask( 
								MIDI::ChannelMask(1) |
								MIDI::ChannelMask(2)
							);

	inNoteCfg.SetMessageMask(
		MIDI::MessageMask(MIDI::eNoteOn)|
		MIDI::MessageMask(MIDI::eNoteOff)
	);

	MIDIInControl keyboardNote( inNoteCfg );

	MIDIInConfig inPitchBendCfg;
	
	inPitchBendCfg.SetName("inPithcBend");
	inPitchBendCfg.SetDevice("default:default");
	inPitchBendCfg.SetChannelMask(MIDI::ChannelMask(1));
	inPitchBendCfg.SetMessageMask(MIDI::MessageMask(MIDI::ePitchbend));

	MIDIInControl pitchBend( inPitchBendCfg );

	MIDIInConfig inBreathNoteCfg;

	inBreathNoteCfg.SetName("in2");
	inBreathNoteCfg.SetDevice("default:default");
	inBreathNoteCfg.SetChannelMask( 				
									MIDI::ChannelMask(3) |
									MIDI::ChannelMask(4)
								  );

	inBreathNoteCfg.SetMessageMask(
		MIDI::MessageMask(MIDI::eNoteOn)|
		MIDI::MessageMask(MIDI::eNoteOff)
	);

	MIDIInConfig inCtrlCfg;
	
	inCtrlCfg.SetName("inctrl");
	inCtrlCfg.SetDevice("default:default");
	inCtrlCfg.SetChannelMask(MIDI::ChannelMask(2));
	inCtrlCfg.SetMessageMask(MIDI::MessageMask(MIDI::eControlChange));
	inCtrlCfg.SetFilter(0x02);

	BreathController breathController( inBreathNoteCfg, inCtrlCfg );

	pitchBend.LinkOutWithInControl( 0, &mMIDIHandler, 2);

	keyboardNote.LinkOutWithInControl( 0, &mMIDIHandler, 1);
	keyboardNote.LinkOutWithInControl( 1, &mMIDIHandler, 0);
	keyboardNote.LinkOutWithInControl( 2, &mMIDIHandler, 1);
	keyboardNote.LinkOutWithInControl( 3, &mMIDIHandler, 0);

	mFileAudioOut.Start();
	mMIDIManager.Start();
	mAudioManager->Start();
	// End of Salto IO Setup	

	while ( mState != Termination )
		{

			HandleMIDI( ); // MIDI things startup

			switch( mState )
			{
			case Idle:
				{
					HandleIdle();
					break;
				}
			case Attack:
				{
					HandleAttack();
					break;
				}
			case Sustain:
				{
					HandleSustain();
					break;
				}
			case Transition:
				{
					HandleTransition();
					break;
				}
			case Release:
				{
					HandleRelease();
					break;
				}
			}
		}

	// Closing file
	if ( mpParams->GetWriteToFile())
		{
			std::cout << "Closing audio File ..... " << std::endl;
			mpParams->SetExit(true); // MRJ: Ya lo es no?
			mState = Termination;
			mFileAudioOut.Stop();
		}

}


void
CSaltoDSP::InitInterpolatingSynthesis()
{
	  // remember : after a transition we dont step into here again
	// this means we have do the necessary inits in CSaltoDSP::EndTransitionSynthesis too !!!
	mpParams->SetInLoopSynthesis(false);// we're not yet in the loop
	mpParams->SetTransitionMode(false); // we're not yet in transition mode
	mLoopDirectionFW = true;// start the loop always in forwards direction
	mStatResLoopDirectionFW = true; // start stat res loop in forwards direction
	mFrameCounterTransition = 0;
	mSinAttackOffsetCounter = 0;

	mTimbreVektorBase = mpParams->GetAttackTimbre();

	// inital values for interpolation
	mSegPositionBase = mTimbreVektorBase.GetPosition();

	// for security reasons only, can go out later
	if (mSegPositionBase>=SPECTRAL_SEGMENTS_IN_USE)
	mSegPositionBase=0;

	// num frames in the current sdif segment
	mNumFramesBase = mpDataManagment->GetFramesInSeg(mSegPositionBase);

	mFrameCounterBase = (mpParams->GetSegDataByPos(mSegPositionBase)).GetPlayBackStart();

	double dB =(mpParams->GetSegDataByPos(mSegPositionBase)).GetIndividualVolume(); // CHANGE THIS NAME !!!!

	mIndividualGain = pow(10,dB/20.);

	mLimitLoopStart =(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopStart();
	mLimitLoopEnd =(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopEnd();
	if (mLimitLoopEnd<mLimitLoopStart)
	mLimitLoopEnd=mLimitLoopStart; // security

	mLoopStart = mLimitLoopStart; // variable borders for Random Mirror Looping
	mLoopEnd = mLimitLoopEnd; // dto.

	mInterpolStart =(mpParams->GetSegDataByPos(mSegPositionBase)).GetInterpolStart();
	mInterpolEnd =(mpParams->GetSegDataByPos(mSegPositionBase)).GetInterpolEnd();
	mInterpolDuration = mInterpolEnd-mInterpolStart;

	mResFadeStart =(mpParams->GetSegDataByPos(mSegPositionBase)).GetResFadeStart();
	mResFadeEnd =(mpParams->GetSegDataByPos(mSegPositionBase)).GetResFadeEnd();
	mAttackResVolume = (double)(mpParams->GetSegDataByPos(mSegPositionBase)).GetAttackResVolume();
	CLAM_ASSERT (mAttackResVolume >= 0, "mAttackResVolume < 0!!");

	mSinAttackOffset =0;

	mFrameCounterStatRes= 100;// manually set for now... should be adjustable
	mNumFramesStatRes = mpDataManagment->GetFramesInStatResidualSegment();
	mStatResFadeInFrom= mResFadeStart;
	mStatResFadeInTo= mResFadeEnd;

	mStatResVolume = (double)(mpParams->GetSegDataByPos(mSegPositionBase)).GetStatResVolume();
	CLAM_ASSERT(mStatResVolume >= 0, "mStatResVolume <0!");

	mFrameCounterAttackResidual= 0;//(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopEnd();

	// pitch correction (fund.freq for pitch 0) use for tuning the instrument
	// and correcting individual deviation of samples

	mPitchReference = LOWEST_PITCH *pow(2.0,mTimbreVektorBase.GetPitch()/12.0);

}
//----------------------------------------------------------------------------//
void CSaltoDSP::DoInterpolatingSynthesis(CSaltoSynthFrame *pSynthFrame)
{
  mpSynthFrame->ClearSpectrum();  // reset synth frame OPTIMIZE

  // -- data managment BaseFrame --
  if (mpParams->GetUseRandomLoop())
  {
    if (mFrameCounterBase==mLoopStart)
    {
      mLoopDirectionFW = true;
      mpParams->SetInLoopSynthesis(true);       // indicates that we are in loop mode
      int randRange = mLimitLoopEnd-mLoopStart;
      //mLoopEnd = (Random()+32767)/65535.0*randRange+mLoopStart;	
      mLoopEnd = ((TFloat32)rand()/RAND_MAX)*randRange+mLoopStart;	
  	}
    else if (mFrameCounterBase> mLoopEnd)
    {
      mLoopDirectionFW = false;
      mpParams->SetInLoopSynthesis(true);       // indicates that we are in loop mode
      int randRange = mLoopEnd-mLimitLoopStart;
      mLoopStart = ((TFloat32)rand()/RAND_MAX)*randRange+mLimitLoopStart;
      CLAM_ASSERT(mLoopStart >= 0, "mLoopStart < 0!!!");
    }
  }
  else  // repeat always one frame...
  {
    if (mFrameCounterBase>=mLoopStart)    
    {
      mFrameCounterBase=mLoopStart;
      mpParams->SetInLoopSynthesis(true);
    }
  }	

	mpCurrSpectralFrameBase = mpDataManagment->GetSpectralFrame(mSegPositionBase,mFrameCounterBase);
	
	// increase or decrease position of readout frame depending on loop direction
	if (mLoopDirectionFW)
	  mFrameCounterBase++;
	else
	  mFrameCounterBase--;
	
	if(mFrameCounterBase>=mNumFramesBase)        // security-stop at end of the segment
	{
	  mFrameCounterBase=0;
	  mpParams->SetPlay(false);
	  return;
	}
	
	// -- data managment TargetFrame --
	mpCurrPeakArrayTarget = mpParams->GetCurrentStatTemplatePtr();

	// -- data managment attack residual --
	TIndex nResFrames= mpDataManagment->GetFramesInSeg(mpParams->GetAttackTimbre().GetPosition());

	if (mFrameCounterAttackResidual>=nResFrames)
		mFrameCounterAttackResidual=nResFrames-1;
	
	mpCurrSpectralFrameResidual = mpDataManagment->GetSpectralFrame(mpParams->GetAttackTimbre().GetPosition(),mFrameCounterAttackResidual);

	pSynthFrame->SetResidualPtr(&mpCurrSpectralFrameResidual->GetResidualSpec()); //BUGFRAME? //SEGMENTBUG


	mFrameCounterAttackResidual++;
	
	// spectral interpolation process starts after 'mInterpolationstart' Frames
	if(mFrameCounterBase>=mInterpolStart)
	{
		mIPFactor =  mpParams->GetInterPolFactor() * (mFrameCounterBase-mInterpolStart)/mInterpolDuration;
		if (mFrameCounterBase>=mInterpolEnd)
		  mIPFactor = mpParams->GetInterPolFactor();
	}
	else
	{
	  mIPFactor=0;
	}

	mpInterpolPO->DoInterpolation2(mpCurrSpectralFrameBase,
                            	  mpCurrPeakArrayTarget,
                                mpParams->GetCurrentStatTemplateFundFreq(),//0 // default no freq interpol
                          	    mIPFactor,      // mag IP Factor
                          	    0,              // freq IP Factor
                          	    mIndividualGain,// magnitude gain
                          	    pSynthFrame);

	double pitchCorrectionFactor=1.0;
	if  (mpParams->GetUsePitchCorrection()&&mpSynthFrame->GetFundFreq()!=0)
	  pitchCorrectionFactor=mPitchReference/mpSynthFrame->GetFundFreq();
	mLastPitchCorrectionFactor=pitchCorrectionFactor; // used in transition
	
	mpInterpolPO->DoPitchMod2(pSynthFrame,mpParams->GetPitchModFactor()*pitchCorrectionFactor);
}
//----------------------------------------------------------------------------//
void CSaltoDSP::InitTransitionSynthesis(CSaltoSynthFrame *pSynthFrame)
{
	mpTransitionFrameBase = mpCurrSpectralFrameBase;
	mSegPositionTransition = mpParams->GetAttackTimbre().GetPosition(); // its already updated in CSaltoMIDI !
	TIndex loopFrameTarget = (mpParams->GetSegDataByPos(mSegPositionTransition)).GetLoopStart();
	mpTransitionFrameTarget = mpDataManagment->GetSpectralFrame(mSegPositionTransition,loopFrameTarget);
		
	// check for level differences in transition mode
	// mLevelAtBeginOfTransition = mTimbreVektorBase.GetLevel();
	// mLevelAtEndOfTransition = mpParams->GetAttackTimbre().GetLevel();
	mNumTransitionFrames = mpParams->GetAttackTimbre().GetTransitionFrames();
	mFrameCounterTransition = 1;// init to one to discard the first transition frame which would be equal to the last baseframe

	// pitch correction
	mPitchReference = LOWEST_PITCH*pow(2.0,mpParams->GetAttackTimbre().GetPitch()/12.0); // c3 == pitch0

	TFloat64 dB = (mpParams->GetSegDataByPos(mSegPositionTransition)).GetIndividualVolume();
	mLastIndividualGain = mIndividualGain;
	mIndividualGain = pow(10,dB/20.);

	mpParams->SetTransitionInit(false);
}

//----------------------------------------------------------------------------//
void CSaltoDSP::DoTransitionSynthesis(CSaltoSynthFrame *pSynthFrame)
{
  if (mpParams->GetTransitionInit())
    InitTransitionSynthesis(pSynthFrame);

  pSynthFrame->ClearSpectrum();

  double iPFactor = (double) mFrameCounterTransition/mNumTransitionFrames;
    
  double baseFreq,targetFreq,resultFreq,a,b;
  double freqIPFactor=0;
  double gain;
  double gainReductionFactor; // -20=-10dB -40=-20dB -60=-30dB

//  gainReductionFactor= -(mpParams->GetTransitionInterval() + 20); // gain reduction in transition depends on interval..
/* the gain reduction in a transition seems to be dependent on
    a.) the absolut pitch ( the higher the more we have to reduce the gain inbetween)
    b.) the interval ( for small intervall we dont need that much reduction)
*/
  gainReductionFactor= 
    - ( 20 + mpParams->GetTransitionInterval() + mpParams->GetAttackTimbre().GetPitch());
  gainReductionFactor=CLIP(gainReductionFactor,-80,-20);
  
  mpParams->SetTransitionFrequency(gainReductionFactor);
  mpParams->SetDisplayedValuesChanged(true);

  if (mpParams->GetTransitionUpwards())
  { 
    int freqIPUpEnd = mNumTransitionFrames/2+1;
    int freqIPUpStart = mNumTransitionFrames/2-1;
    freqIPUpStart = CLIP(freqIPUpStart,0,mNumTransitionFrames);
    freqIPUpEnd = CLIP(freqIPUpEnd,0,mNumTransitionFrames);
    
    /* frequency transition function */
    if (mFrameCounterTransition<freqIPUpStart)
    {
      freqIPFactor = 0;
    }
    else if (mFrameCounterTransition>=freqIPUpStart&&mFrameCounterTransition<freqIPUpEnd)  
    {
      baseFreq =mpTransitionFrameBase->GetFundamental().GetFreq();
      targetFreq = mpTransitionFrameTarget->GetFundamental().GetFreq();
      b = (freqIPUpEnd - freqIPUpStart) / (log10 (targetFreq/baseFreq)); // optimize and calc this factors in trans init !
      a = baseFreq/pow(10.0,freqIPUpStart/b);
      resultFreq = a * pow (10,mFrameCounterTransition/b);
      freqIPFactor = resultFreq/targetFreq;
    }
    else if (mFrameCounterTransition>freqIPUpEnd)
    {
      freqIPFactor = 1;
    }

    /* gain transition function */
    if (iPFactor<0.5)
    {
       gain=pow(10,gainReductionFactor*iPFactor/20);  // fade down in the first half
    }
    else
    {
      gain=pow(10,(-gainReductionFactor*iPFactor+gainReductionFactor)/20); // fade in again
    }
  }
  else // transition downwards
  {
    int freqIPDownStart = mNumTransitionFrames/2-1;
    int freqIPDownEnd = mNumTransitionFrames/2+1;
    freqIPDownStart = CLIP(freqIPDownStart,0,mNumTransitionFrames);
    freqIPDownEnd = CLIP(freqIPDownEnd,0,mNumTransitionFrames);

    if (mFrameCounterTransition<freqIPDownStart)
    {
      freqIPFactor = 0;
    }
    else if (mFrameCounterTransition==freqIPDownStart)   //we are in the fourth quarter of the transition...
    {
      baseFreq =mpTransitionFrameBase->GetFundamental().GetFreq();
      targetFreq = mpTransitionFrameTarget->GetFundamental().GetFreq();
      b = (freqIPDownEnd - freqIPDownStart) / (log10 (targetFreq/baseFreq)); // optimize and calc this factors in trans init !
      a = baseFreq/pow(10.0,freqIPDownStart/b);
      resultFreq = a * pow (10,mFrameCounterTransition/b);
      freqIPFactor = resultFreq/targetFreq;
    }
    else if (mFrameCounterTransition>freqIPDownEnd) 
    {
      freqIPFactor = 1;
    }

    if (iPFactor<0.5)
    {
       gain=pow(10,gainReductionFactor*iPFactor/20);  // fade down about 10 dB in the first half
    }
    else
    {
      gain=pow(10,(-gainReductionFactor*iPFactor+gainReductionFactor)/20); // fade in again
    }
  }

  mpInterpolPO->DoInterpolation2
	  (mpTransitionFrameBase,
	   &mpTransitionFrameTarget->GetSpectralPeakArray(),
	   mpTransitionFrameTarget->GetFundamental().GetFreq(),
	   iPFactor,
	   freqIPFactor,
	   gain * (mLastIndividualGain+iPFactor*(mIndividualGain-mLastIndividualGain)), // interpolate individual gain factors	
	   pSynthFrame,
	   true,
	   true,
	   mpCurrPeakArrayTarget,
	   mIPFactor);
	
	// use last pith correction factor here too
	mpInterpolPO->DoPitchMod2(pSynthFrame,mpParams->GetPitchModFactor()*mLastPitchCorrectionFactor);

  mFrameCounterTransition++;
  if (mFrameCounterTransition>=mNumTransitionFrames)
    EndTransitionSynthesis(pSynthFrame);
}
//----------------------------------------------------------------------------//
void CSaltoDSP::EndTransitionSynthesis(CSaltoSynthFrame *pSynthFrame)
{
	mFrameCounterTransition = 0;
	mpParams->SetTransitionMode(false);
	mpParams->SetDisplayedValuesChanged(true); // display needs update
	mLoopDirectionFW = true;// start the loop always in forwards direction
	mStatResLoopDirectionFW = true; // start stat res loop in forwards direction

	// set new params for interpolating synthesis
	// just like init interpolating synthesis, but respecting the new stationary
	// base frame
	mSegPositionBase = mSegPositionTransition;
	mpCurrSpectralFrameBase = mpTransitionFrameTarget;

	mNumFramesBase = mpDataManagment->GetFramesInSeg(mSegPositionBase);

	// framecounter already set to enter loop
	mFrameCounterBase =(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopStart();

	double dB = (mpParams->GetSegDataByPos(mSegPositionBase)).GetIndividualVolume();

	mIndividualGain = pow(10,dB/20.);

	mLimitLoopStart =(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopStart();
	mLimitLoopEnd =(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopEnd();
	if (mLimitLoopEnd<mLimitLoopStart)
		mLimitLoopEnd=mLimitLoopStart; // security

	mLoopStart = mLimitLoopStart; // variable borders for Random Mirror Looping
	mLoopEnd = mLimitLoopEnd; // dto.

	// set these anyway, though not needed
	mInterpolStart =(mpParams->GetSegDataByPos(mSegPositionBase)).GetInterpolStart();
	mInterpolEnd =(mpParams->GetSegDataByPos(mSegPositionBase)).GetInterpolEnd();
	mInterpolDuration = mInterpolEnd-mInterpolStart;

	mResFadeStart=(mpParams->GetSegDataByPos(mSegPositionBase)).GetResFadeStart();
	mResFadeEnd=(mpParams->GetSegDataByPos(mSegPositionBase)).GetResFadeEnd();

	mStatResFadeInFrom= mResFadeStart;
	mStatResFadeInTo= mResFadeEnd;

	mAttackResVolume = 0; // there is no need for another attack residual after transition

}

//----------------------------------------------------------------------------//
void CSaltoDSP::DoInterpolatingSynthesisRelease(CSaltoSynthFrame *pSynthFrame,double gain)
{
	int nReleaseFrames=6;
	for (int i=1;i<nReleaseFrames;i++) // a very simple release phase to avoid clicks
	{
		if(mpParams->GetPlay()) break;

		mpSynthFrame->ClearSpectrum();

		mpInterpolPO->DoInterpolation2( mpCurrSpectralFrameBase,
                              	   mpCurrPeakArrayTarget,
                                   mpParams->GetCurrentStatTemplateFundFreq(),
                            	     mIPFactor,
                            	     0,
                            	     mIndividualGain/(i*i),
                            	     mpSynthFrame );

		double pitchCorrectionFactor=1;
	
		if (mpParams->GetUsePitchCorrection()&&mpSynthFrame->GetFundFreq()!=0)
			pitchCorrectionFactor=mPitchReference/mpSynthFrame->GetFundFreq();
	
		mpInterpolPO->DoPitchMod2(pSynthFrame,mpParams->GetPitchModFactor()*pitchCorrectionFactor);
	
		DoSynthesisProcess(mpSynthFrame, true);
		RenderSynthesis();
		std::cout << i << std::endl;

	}
}

//----------------------------------------------------------------------------//
void CSaltoDSP::InitGenerateTestSine()
{
  mTestFreq1 = 440;
  mTestPhase1 = 0;
  mTestPhase2 = 0;
}
//----------------------------------------------------------------------------//

void CSaltoDSP::DoGenerateTestSineSynthesis(CSaltoSynthFrame *pSynthFrame)
{			

	mpSynthFrame->ClearSpectrum();  // reset synth frame OPTIMIZE
	
	SpectralPeakArray* pPeaks = pSynthFrame->GetPeakArrayPtr();
	pPeaks->SetnPeaks(2); //?????
	// GENERATE TWO Sines
	mTestCounter++;
	mTestFreq1 += 0.1;
	if (mTestFreq1>10000) mTestFreq1 = 440;
	pPeaks->GetFreqBuffer()[0] = mTestFreq1;
	pPeaks->GetMagBuffer()[0] = 0.1;
	pPeaks->GetFreqBuffer()[1] = mTestFreq2;
	pPeaks->GetMagBuffer()[1] = 0.0;

	
	double deltaT = mTestFreq1*256.0/44100.0;
	mTestPhase1 = mTestPhase1 + TWO_PI*deltaT;

	double deltaT2 = mTestFreq2*256.0/44100.0;
	mTestPhase2 = mTestPhase2 + TWO_PI*deltaT2;			
	

	while (mTestPhase1 >= TWO_PI) mTestPhase1 = mTestPhase1 -TWO_PI;
	while (mTestPhase2 >= TWO_PI) mTestPhase2 = mTestPhase2 -TWO_PI;
	
	pPeaks->GetPhaseBuffer()[0] = mTestPhase1;
	pPeaks->GetPhaseBuffer()[1] = mTestPhase2;
	
	pSynthFrame->SetFundFreq(440); // needed if  phase alignment is active doesnt work anyway

}


//----------------------------------------------------------------------------//
void CSaltoDSP::InitFollowOneTrackSynthesis()
{
  /*
  //  timbre recognition :
	mTimbreVektorBase.SetPitch(mpParams->GetCurrentAttackPosition());
	//mTimbreVektorTarget.SetPitch(mpParams->GetSelectedSegmentStationary());	
	mTimbreVektorTarget.SetPitch(2);	

  // inital values for interpolation
  mNumFramesBase   = mpDataManagment->GetFramesInSeg(mTimbreVektorBase);
	mNumFramesTarget = mpDataManagment->GetFramesInSeg(mTimbreVektorTarget);
  mSegPositionBase = mpDataManagment->GetSegPositionByTimbre(mTimbreVektorBase);
  mSegPositionTarget = mpDataManagment->GetSegPositionByTimbre(mTimbreVektorTarget);
  mLoopStart       =(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopStart();
  mLoopEnd         =(mpParams->GetSegDataByPos(mSegPositionBase)).GetLoopEnd();
  mInterpolStart   =(mpParams->GetSegDataByPos(mSegPositionBase)).GetInterpolStart();
  mInterpolEnd     =(mpParams->GetSegDataByPos(mSegPositionBase)).GetInterpolEnd();
  mInterpolDuration = mInterpolEnd-mInterpolStart;

  mFrameCounterBase   =
    (mpParams->GetSegDataByPos(mSegPositionBase)).GetPlayBackStart();

  mFrameCounterTarget =
    (mpParams->GetSegDataByPos(mSegPositionTarget)).GetPlayBackStart();

  // inital values for track following
  mpInterpolPO->SetTrackIndex(mpParams->GetMaxNumSines());
  */
}
//----------------------------------------------------------------------------//
void CSaltoDSP::DoFollowOneTrackSynthesis(CSaltoSynthFrame *pSynthFrame)
{		
  /*
  mpSynthFrame->ClearSpectrum();  // reset synth frame OPTIMIZE

  // data managment BaseFrame
  if (mFrameCounterBase>=mLoopEnd && mLoopEnd != 0)
  {
    mFrameCounterBase = mLoopStart;
	  //mpParams->SetLastAlignedFrame(true);    // switch to random phases any way
	                                            // its resetted by resetting the mpSynthPO
	}
	mpCurrSpectralFrameBase = mpDataManagment->GetSpectralFrame
	  (mTimbreVektorBase,mFrameCounterBase);
	mFrameCounterBase++;
	if(mFrameCounterBase>=mNumFramesBase)        // End of this Segment
	{
	  mFrameCounterBase=0;
	  mpParams->SetPlay(false);
	  return;
	}
	
	//  data managment TargetFrame
	// test : we take only one template spectrum as target spectrum...
	mpCurrSpectralFrameTarget = mpDataManagment->GetSpectralFrame
	  (mTimbreVektorTarget,mFrameCounterTarget);
  //mFrameCounterTarget++;
	if(mFrameCounterTarget>=mNumFramesTarget)
	{
	  mFrameCounterTarget=0;
	}
	
	//  interpolation
	double interpol = 0;
	double interpolFactor = mpParams->GetInterPolFactor();
	if(mFrameCounterBase>=mInterpolStart)
	{
    interpol =  interpolFactor* (mFrameCounterBase-mInterpolStart)/mInterpolDuration;

    if (mFrameCounterBase>=mInterpolEnd)
      interpol = interpolFactor;
	}
	
  mpInterpolPO->FollowOneTrack(mpCurrSpectralFrameBase,pSynthFrame);	
  */
}

//----------------------------------------------------------------------------//
void CSaltoDSP::InitSDIFEditSynthesis()
{
  mEditFrameCounter=0;
}

//----------------------------------------------------------------------------//
void CSaltoDSP::DoSDIFEditSynthesis(CSaltoSynthFrame *pSynthFrame)
{		
	mpSynthFrame->ClearSpectrum();  // reset synth frame OPTIMIZE
	
	// for now this is doubled code, but I hink this might change anyway...
	if (mpParams->GetPlayFrameOnly())
  {
    	SpectralPeakArray*	pBasePeakArray = mpParams->GetStatPeakArrayPtr();
      TSize nPeaks = pBasePeakArray->GetnPeaks();
      pSynthFrame->SetPeakArraySize(nPeaks);  // no boundary checking !

      SpectralPeak tempPeak;
      double base;

      for(int i= 0;i<nPeaks;i++)
    	{
    		base = pBasePeakArray->GetFreq(i);
    		tempPeak.SetFreq(base);
    		
    		base = pBasePeakArray->GetPhase(i);
    		tempPeak.SetPhase(base);
    		
    		base = pBasePeakArray->GetMag(i);
    		tempPeak.SetMag(base);
    		
    		tempPeak.SetBinPos(-1);
    		
    		pSynthFrame->SetPeak(i,tempPeak);
    		
    		// sin tracks are "constant" if we play only one frame         pCurrSpecFrame->AddFundamental();
    		pSynthFrame->SetPeakArrayIndex(i,i);

    	}
    	pSynthFrame->SetFundFreq(mpParams->GetStatFundFreq());
  }
  else
  {
    	Frame* pCurrSpecFrame = mpDataManagment->GetEditSpectralFrame(mEditFrameCounter);
    	
    	SpectralPeakArray*	pBasePeakArray = &pCurrSpecFrame->GetSpectralPeakArray(); //BUGFRAME!
    	
    	TSize nPeaks = pBasePeakArray->GetnPeaks();
    	pSynthFrame->SetPeakArraySize(nPeaks);  // no boundary checking !

      SpectralPeak tempPeak;
      double base;

      for(int i= 0;i<nPeaks;i++)
    	{
    		base = pBasePeakArray->GetFreq(i);
    		tempPeak.SetFreq(base);
    		
    		base = pBasePeakArray->GetPhase(i);
    		tempPeak.SetPhase(base);
    		
    		base = pBasePeakArray->GetMag(i);
    		tempPeak.SetMag(base);
    		
    		tempPeak.SetBinPos(-1);
    		
    		pSynthFrame->SetPeak(i,tempPeak);
    		
    		// use base array indizes for sinusoidal track indexing ....
    		pSynthFrame->SetPeakArrayIndex(i,pBasePeakArray->GetIndex(i));
    	}
    	// fund freq of BaseFrame
    	pSynthFrame->SetFundFreq(pCurrSpecFrame->GetFundamental().GetFreq());
    	
    	// residual of BaseFrame
    	pSynthFrame->SetResidualPtr(&pCurrSpecFrame->GetResidualSpec());

      mEditFrameCounter++;
      if (mEditFrameCounter >= mpDataManagment->GetFramesInEditSegment())
      {
        mEditFrameCounter = 0;
        mpParams->SetPlay(false);
      }
   }

}

//----------------------------------------------------------------------------//
void CSaltoDSP::DoSynthesisProcess(CSaltoSynthFrame *pSynthFrame, bool inReleaseMode)
{
  double ipFactor=1;
  // from here on, the content of the synthframe shouldn't be changed anymore
	
	// -- Sinusoidal Synthesis --
	if	(mpParams->GetUseSines())
	{
	    if (mpParams->GetBreathOnlySound()) //BreathOnlySound active
        {
          if(mpParams->GetAttackTimbre().GetLevel() > 0) //at which level we go from breath-only sound to note sound
          {
            mpParams->SetBreathOnlySound(false);
        	mpSineSynthPO->DoSineSynthesis(*pSynthFrame,mpParams->GetAttackTimbre().GetLevel()/127.0);
          }
          else
          {
            mpSineSynthPO->DoSineSynthesis(*pSynthFrame,0.0);
          }
    	}
    	else //BreathOnlySound not active
    	{
          if(mpParams->GetAttackTimbre().GetLevel() < 0) //at which level we go from note sound to breath-only sound
          {
            mpParams->SetBreathOnlySound(true);
            mpSineSynthPO->DoSineSynthesis(*pSynthFrame,0.0);
          }
          else
          {
            mpSineSynthPO->DoSineSynthesis(*pSynthFrame,mpParams->GetAttackTimbre().GetLevel()/127.0);
          }
        }
////		  mpSineSynthPO->DoSineSynthesis(*pSynthFrame,mpParams->GetAttackTimbre().GetLevel()/127.0);
	}
	
	// -- attack residual synthesis --
	if	(mpParams->GetUseAttackResidual()&!inReleaseMode)//&!mpParams->GetPlayFrameOnly())
	{
		if(mFrameCounterBase>=mResFadeStart)
		{
		   // calc fade-out factor for decaying attack residual
		   ipFactor = 1-(mFrameCounterBase-mResFadeStart)/(mResFadeEnd-mResFadeStart);
		   if(ipFactor<0) ipFactor=0;
		}
		mpResSynthPO->DoAttackResidualSynthesis(*pSynthFrame,mAttackResVolume*ipFactor*mpParams->GetAttackTimbre().GetLevel()/127.0);
	}
	
	// -- stationary residual synthesis --
	if	(mpParams->GetUseStatResidual()&!inReleaseMode)
	{
		Frame* pStatResidual=NULL;		
		if (mFrameCounterBase>=mStatResFadeInFrom)
		{
			// calc fade-in factor for increasing stationary residual
			ipFactor = (mFrameCounterBase-mResFadeStart)/(mResFadeEnd-mResFadeStart);
			if(ipFactor>1) ipFactor=1;
	
			// for now the stationary loop is readout forwards,backwards, forward....
			// a random mirror looping would be nice here too ??
			if (mFrameCounterStatRes<mNumFramesStatRes-100 && mStatResLoopDirectionFW)
			{
				pStatResidual = mpDataManagment->GetStatResidualSpectralFrame(mFrameCounterStatRes);
				mFrameCounterStatRes++;
				if (mFrameCounterStatRes >= mNumFramesStatRes-100)
					mStatResLoopDirectionFW=false;

			}
			else if ((mFrameCounterStatRes<=mNumFramesStatRes-100) &&(! mStatResLoopDirectionFW))
			{
				pStatResidual = mpDataManagment->GetStatResidualSpectralFrame(mFrameCounterStatRes);
				mFrameCounterStatRes--;
				if (mFrameCounterStatRes <= 100)
				  mStatResLoopDirectionFW=true;
			}
	  }
	
		// -- TEST increase level of stationary residual for smaller volumes...
		double level = (double) mpParams->GetAttackTimbre().GetLevel()/127.;
		double correctionFactor = 0;
		
		correctionFactor = log(8.5*level+1.5)/150 * mpParams->GetStatResGain();
	
    
		if (mpParams->GetPlay())
		{
			mpResSynthPO->DoStatResidualSynthesis2(*pSynthFrame,pStatResidual,mStatResVolume*ipFactor*correctionFactor,mpParams->GetStatResFreq());
		}

	}
	
	// -- Spectral Synthesis
	mSpectralSynthesisPO.Do(*(pSynthFrame->GetSpectrumPtr()),AudioOutBuffer);
	
	mEventSample += mConfig.GetAnalysisFrameSize();	// Count processed samples


pthread_mutex_lock(&DrawingMutex);
	this->mpGUI->FillDisplayPeakArray(*this->GetCurrentPeakArrayPtr());
	this->mpGUI->FillDisplayAttackResidual(*this->GetCurrentResidualPtr());
//	this->mpGUI->FillDisplayStationaryResidual(this->mpCurrSpectralFrameResidual->GetResidual()); //should be stationary!!
	this->mpGUI->FillDisplaySynthesizedSpectrum(*this->GetSynthFramePtr()->GetSpectrumPtr());
pthread_mutex_unlock(&DrawingMutex);
}

const DataArray& CSaltoDSP::GetAudioBufferReference()
{
  //TODO XA: is this ok?
	int constant;
	if(mpParams->GetWriteToFile())
	{
		constant=32768*10;
	}
	else
		constant=32768;

	DataArray& tmpArray=AudioOutBuffer.GetBuffer();
	for(int i=0;i<mDrawingBuffer.Size();i++)
		mDrawingBuffer[i]=tmpArray[i]*constant;
	return mDrawingBuffer;
}

const SpectralPeakArray& CSaltoDSP::GetCurrentPeakArray()
{
  return mpSynthFrame->GetPeakArray();
}

const SpectralPeakArray* CSaltoDSP::GetCurrentPeakArrayPtr()
{
  return mpSynthFrame->GetPeakArrayPtr();
}

const Spectrum* CSaltoDSP::GetCurrentResidualPtr()
{
  return mpSynthFrame->GetResidualPtr();
}

Frame* CSaltoDSP::GetStatTmplFrame(TIndex pos)
{
  if (pos >= mpDataManagment->GetFramesInEditSegment())
    pos = mpDataManagment->GetFramesInEditSegment()-1;

  Frame* pCurrSpecFrame = mpDataManagment->GetEditSpectralFrame(pos);

  return pCurrSpecFrame;	  	
}
	
//----------------------------------------------------------------------------//	
TIndex CSaltoDSP::GetnFramesInStatTmplSeg()
{
  return mpDataManagment->GetFramesInEditSegment();
}

//----------------------------------------------------------------------------//	
void
CSaltoDSP::SetSDIFToEdit(const char *pFileName)
{
  mpDataManagment->LoadSDIFToEditPosition(pFileName);
}
//----------------------------------------------------------------------------//
void
CSaltoDSP::AddStatTmplToDatabase(CSaltoStatTmplData& d,TIndex pos)
{
  mpDataManagment->AddStatTmpl(d,pos);
}


//----------------------------------------------------------------------------//
void
CSaltoDSP::StoreAllParams()
{
#if defined LINUX || defined WIN32 
  std::cout << "saving parameters database... ";
#endif

  mpDataManagment->StoreSaltoDatabase(); // store all Seg data to HD

#if defined LINUX || defined WIN32  
  std::cout << "done" << std::endl << "saving stat template data... ";
#endif

  mpDataManagment->StoreStatTmplDatabase(); // store all stat Template data to HD
  
#if defined LINUX || defined WIN32 
  std::cout << "done" << std::endl;
#endif
}
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
// eof

bool CSaltoDSP::UpdateParams()
{
	int saltoNoteConstant=3;//Difference between real MIDI note and Salto's
/*	
	//current time in milliseconds
	TTime currentTime=mEventSample/mConfig.GetSampleRate();//for the time being note time is in sec, should be in ms
	Note& currentNote=mMelody.GetNoteArray().Current();
	bool isLast=mMelody.GetNoteArray().IsLast();
	bool isFirst=mMelody.GetNoteArray().IsFirst();
	bool noteOff=false;
	if(isFirst&&!mpParams->GetPlay()&&currentNote.GetTime().GetBegin()<currentTime/*&&currentNote.GetTime().GetEnd()>currentTime*///)
/*	{
		mVelocityOut.SendControl( currentNote.GetVelocity() );
		mNoteOut.SendControl( currentNote.GetNoteNumber()-saltoNoteConstant );
		//mHandleParams.NoteOn(currentNote.GetTime().GetBegin(),currentNote.GetNoteNumber()-saltoNoteConstant,currentNote.GetVelocity(),*mpParams);
	}
	if(mpParams->GetPlay()&&currentNote.GetTime().GetEnd()<currentTime)
	{
		if(isLast)
		{
			mVelocityOut.SendControl( 0.0);//currentNote.GetVelocity() );
			mNoteOut.SendControl( currentNote.GetNoteNumber()-saltoNoteConstant );
		//	mHandleParams.NoteOff(currentNote.GetTime().GetEnd(),currentNote.GetNoteNumber()-saltoNoteConstant,currentNote.GetVelocity(),*mpParams);
			return false;
		}
		else
		{
			noteOff=true;/*now I know there is a note off but have to wait to 
			see if there is a simultaneous noteOn to simulate a transition*/
/*		}

	}
	if(!isLast)
	{
		static int numNote = 1;
		mMelody.GetNoteArray().DoNext();

		Note& newCurrentNote=mMelody.GetNoteArray().Current();
		if(newCurrentNote.GetTime().GetBegin()<currentTime)
		{
			noteOff=false;//no noteOff, transition
			mVelocityOut.SendControl( newCurrentNote.GetVelocity() );
			mNoteOut.SendControl( newCurrentNote.GetNoteNumber()-saltoNoteConstant );
		//	mHandleParams.NoteOn(newCurrentNote.GetTime().GetBegin(),newCurrentNote.GetNoteNumber()-saltoNoteConstant,newCurrentNote.GetVelocity(),*mpParams);
		}
		else
		{
			//we found a note that still has not begun, we must wait
			mMelody.GetNoteArray().DoPrevious();
		}
		if(noteOff)
		{	
			mVelocityOut.SendControl( currentNote.GetVelocity() );
			mNoteOut.SendControl( currentNote.GetNoteNumber()-saltoNoteConstant );
			//mHandleParams.NoteOff(currentNote.GetTime().GetEnd(),currentNote.GetNoteNumber()-saltoNoteConstant,currentNote.GetVelocity(),*mpParams);
		}
	}
*/
	return true;
	
}

void CSaltoDSP::LoadMelody(std::string& fileName)
{

/*	XMLStorage x;
	x.Restore(mMelody,fileName);

	mMelody.GetNoteArray().DoFirst();
*/
}

void CSaltoDSP::BindWithGUI( CSaltoEditor* pGUI )
{
	mpGUI = pGUI;
	mpGUI->SetDisplayParameters( mConfig.GetSampleRate(), mConfig.GetSpectralFrameSize() );
}

void CSaltoDSP::DoStationarySynthesisProcess( CSaltoSynthFrame* pSynthFrame )
{

	if ( mpParams->GetUseSines() )
		{
			SinusoidalSynthesis( pSynthFrame );
		}

	// Attack Residual Synthesis
	if	( mpParams->GetUseAttackResidual() )//&!mpParams->GetPlayFrameOnly())
		{
			AttackResidualSynthesis( pSynthFrame );
		}

	// Stationary Residual Synthesis
	if ( mpParams->GetUseStatResidual() )
		{
			StationaryResidualSynthesis( pSynthFrame );
		}

	// -- Spectral Synthesis
	mSpectralSynthesisPO.Do(*(pSynthFrame->GetSpectrumPtr()),AudioOutBuffer);
	
	mEventSample += mConfig.GetAnalysisFrameSize();	// Count processed samples

}

void CSaltoDSP::DoReleaseSynthesisProcess( CSaltoSynthFrame *pSynthFrame )
{
	 double ipFactor=1;
  // from here on, the content of the synthframe shouldn't be changed anymore
	
	// -- Sinusoidal Synthesis --
	if	(mpParams->GetUseSines())
	{
		SinusoidalSynthesis( pSynthFrame );
	}

	// -- Spectral Synthesis
	mSpectralSynthesisPO.Do(*(pSynthFrame->GetSpectrumPtr()),AudioOutBuffer);
	
	mEventSample += mConfig.GetAnalysisFrameSize();	// Count processed samples


}

void CSaltoDSP::AttackResidualSynthesis( CSaltoSynthFrame* pSynthFrame )
{
	double ipFactor = 1;
	
	if (mFrameCounterBase >= mResFadeStart)
		{
			// calc fade-out factor for decaying attack residual
			ipFactor = 1-(mFrameCounterBase-mResFadeStart)/(mResFadeEnd-mResFadeStart);
		}

	if (ipFactor < 0 )
		{ 
			ipFactor=0;
			mpResSynthPO->DoAttackResidualSynthesis(*pSynthFrame,0);
		}
	else
		{
			mpResSynthPO->DoAttackResidualSynthesis(*pSynthFrame,
												mAttackResVolume*ipFactor*mpParams->GetAttackTimbre().GetLevel()/127.0);
		}
}

void CSaltoDSP::StationaryResidualSynthesis( CSaltoSynthFrame* pSynthFrame )
{
	double ipFactor = 1;

	Frame* pStatResidual=NULL;		
	if (mFrameCounterBase>=mStatResFadeInFrom)
		{
			// calc fade-in factor for increasing stationary residual
			ipFactor = (mFrameCounterBase-mResFadeStart)/(mResFadeEnd-mResFadeStart);
			if(ipFactor>1) ipFactor=1;
			
			// for now the stationary loop is readout forwards,backwards, forward....
			// a random mirror looping would be nice here too ??
			if (mFrameCounterStatRes<mNumFramesStatRes-100 && mStatResLoopDirectionFW)
				{
					pStatResidual = mpDataManagment->GetStatResidualSpectralFrame(mFrameCounterStatRes);
					mFrameCounterStatRes++;
					if (mFrameCounterStatRes >= mNumFramesStatRes-100)
						mStatResLoopDirectionFW=false;
					
				}
			else if ((mFrameCounterStatRes<=mNumFramesStatRes-100) &&(! mStatResLoopDirectionFW))
				{
					pStatResidual = mpDataManagment->GetStatResidualSpectralFrame(mFrameCounterStatRes);
					mFrameCounterStatRes--;
					if (mFrameCounterStatRes <= 100)
						mStatResLoopDirectionFW=true;
				}
		}
	// -- TEST increase level of stationary residual for smaller volumes...
	double level = (double) mpParams->GetAttackTimbre().GetLevel()/127.;
	double correctionFactor = 0;
	
	correctionFactor = log(8.5*level+1.5)/150 * mpParams->GetStatResGain();
		
	mpResSynthPO->DoStatResidualSynthesis2(*pSynthFrame,pStatResidual,
												  mStatResVolume*ipFactor*correctionFactor,mpParams->GetStatResFreq());
	
}

void CSaltoDSP::UpdateGUI( )
{
	pthread_mutex_lock(&DrawingMutex);
	mpGUI->FillDisplayPeakArray(*GetCurrentPeakArrayPtr());
	mpGUI->FillDisplayAttackResidual(*GetCurrentResidualPtr());
	//	this->mpGUI->FillDisplayStationaryResidual(this->mpCurrSpectralFrameResidual->GetResidual()); //should be stationary!!
	mpGUI->FillDisplaySynthesizedSpectrum(*GetSynthFramePtr()->GetSpectrumPtr());
	pthread_mutex_unlock(&DrawingMutex);
}

void CSaltoDSP::SinusoidalSynthesis( CSaltoSynthFrame* pSynthFrame )
{
	if (mpParams->GetBreathOnlySound()) //BreathOnlySound active
        {
			if(mpParams->GetAttackTimbre().GetLevel() > 0) //at which level we go from breath-only sound to note sound
				{
					mpParams->SetBreathOnlySound(false);
					mpSineSynthPO->DoSineSynthesis(*pSynthFrame,mpParams->GetAttackTimbre().GetLevel()/127.0);
				}
			else
				{
					mpSineSynthPO->DoSineSynthesis(*pSynthFrame,0.0);
				}
    	}
	else //BreathOnlySound not active
    	{
			if(mpParams->GetAttackTimbre().GetLevel() < 0) //at which level we go from note sound to breath-only sound
				{
					mpParams->SetBreathOnlySound(true);
					mpSineSynthPO->DoSineSynthesis(*pSynthFrame,0.0);
				}
			else
				{
					mpSineSynthPO->DoSineSynthesis(*pSynthFrame,mpParams->GetAttackTimbre().GetLevel()/127.0);
				}
        }
}

void CSaltoDSP::DoInterpolatingReleaseSynthesis( CSaltoSynthFrame* pSynthFrame, double gain, int currRelFrame )
{
	mpSynthFrame->ClearSpectrum();

	mpInterpolPO->DoInterpolation2( mpCurrSpectralFrameBase,
									mpCurrPeakArrayTarget,
									mpParams->GetCurrentStatTemplateFundFreq(),
									mIPFactor,
									0,
									mIndividualGain/(currRelFrame*currRelFrame),
									mpSynthFrame );
	double pitchCorrectionFactor=1;
	
	if (mpParams->GetUsePitchCorrection()&&mpSynthFrame->GetFundFreq()!=0)
		pitchCorrectionFactor=mPitchReference/mpSynthFrame->GetFundFreq();
	
	mpInterpolPO->DoPitchMod2(pSynthFrame,mpParams->GetPitchModFactor()*pitchCorrectionFactor);
	
}

void CSaltoDSP::DoTransitionSynthesis2( CSaltoSynthFrame* pSynthFrame )
{
	pSynthFrame->ClearSpectrum();
	
	double iPFactor = (double) mFrameCounterTransition/mNumTransitionFrames;
    
	double baseFreq,targetFreq,resultFreq,a,b;
	double freqIPFactor=0;
	double gain;
	double gainReductionFactor; // -20=-10dB -40=-20dB -60=-30dB
	
	//  gainReductionFactor= -(mpParams->GetTransitionInterval() + 20); // gain reduction in transition depends on interval..
	/* the gain reduction in a transition seems to be dependent on
	   a.) the absolut pitch ( the higher the more we have to reduce the gain inbetween)
	   b.) the interval ( for small intervall we dont need that much reduction)
	*/
	gainReductionFactor= 
		- ( 20 + mpParams->GetTransitionInterval() + mpParams->GetAttackTimbre().GetPitch());
	gainReductionFactor=CLIP(gainReductionFactor,-80,-20);
	
	mpParams->SetTransitionFrequency(gainReductionFactor);
	mpParams->SetDisplayedValuesChanged(true);
	
	if (mpParams->GetTransitionUpwards())
		{ 
			int freqIPUpEnd = mNumTransitionFrames/2+1;
			int freqIPUpStart = mNumTransitionFrames/2-1;
			freqIPUpStart = CLIP(freqIPUpStart,0,mNumTransitionFrames);
			freqIPUpEnd = CLIP(freqIPUpEnd,0,mNumTransitionFrames);
			
			/* frequency transition function */
			if (mFrameCounterTransition<freqIPUpStart)
				{
					freqIPFactor = 0;
				}
			else if (mFrameCounterTransition>=freqIPUpStart&&mFrameCounterTransition<freqIPUpEnd)  
				{
					baseFreq =mpTransitionFrameBase->GetFundamental().GetFreq();
					targetFreq = mpTransitionFrameTarget->GetFundamental().GetFreq();
					b = (freqIPUpEnd - freqIPUpStart) / (log10 (targetFreq/baseFreq)); // optimize and calc this factors in trans init !
					a = baseFreq/pow(10.0,freqIPUpStart/b);
					resultFreq = a * pow (10,mFrameCounterTransition/b);
					freqIPFactor = resultFreq/targetFreq;
				}
    else if (mFrameCounterTransition>freqIPUpEnd)
		{
			freqIPFactor = 1;
		}
			
			/* gain transition function */
			if (iPFactor<0.5)
				{
					gain=pow(10,gainReductionFactor*iPFactor/20);  // fade down in the first half
				}
			else
				{
					gain=pow(10,(-gainReductionFactor*iPFactor+gainReductionFactor)/20); // fade in again
				}
		}
	else // transition downwards
		{
			int freqIPDownStart = mNumTransitionFrames/2-1;
			int freqIPDownEnd = mNumTransitionFrames/2+1;
			freqIPDownStart = CLIP(freqIPDownStart,0,mNumTransitionFrames);
			freqIPDownEnd = CLIP(freqIPDownEnd,0,mNumTransitionFrames);
			
			if (mFrameCounterTransition<freqIPDownStart)
				{
					freqIPFactor = 0;
				}
			else if (mFrameCounterTransition==freqIPDownStart)   //we are in the fourth quarter of the transition...
				{
					baseFreq =mpTransitionFrameBase->GetFundamental().GetFreq();
					targetFreq = mpTransitionFrameTarget->GetFundamental().GetFreq();
					// optimize and calc this factors in trans init !
					b = (freqIPDownEnd - freqIPDownStart) / (log10 (targetFreq/baseFreq));
					a = baseFreq/pow(10.0,freqIPDownStart/b);
					resultFreq = a * pow (10,mFrameCounterTransition/b);
					freqIPFactor = resultFreq/targetFreq;
				}
			else if (mFrameCounterTransition>freqIPDownEnd) 
				{
					freqIPFactor = 1;
				}
			
			if (iPFactor<0.5)
				{
					gain=pow(10,gainReductionFactor*iPFactor/20);  // fade down about 10 dB in the first half
				}
			else
				{
					gain=pow(10,(-gainReductionFactor*iPFactor+gainReductionFactor)/20); // fade in again
				}
		}
	
	mpInterpolPO->DoInterpolation2
		(mpTransitionFrameBase,
		 &mpTransitionFrameTarget->GetSpectralPeakArray(),
		 mpTransitionFrameTarget->GetFundamental().GetFreq(),
		 iPFactor,
		 freqIPFactor,
		 gain * (mLastIndividualGain+iPFactor*(mIndividualGain-mLastIndividualGain)), // interpolate individual gain factors	
		 pSynthFrame,
		 true,
		 true,
		 mpCurrPeakArrayTarget,
		 mIPFactor);
	
	// use last pith correction factor here too
	mpInterpolPO->DoPitchMod2(pSynthFrame,mpParams->GetPitchModFactor()*mLastPitchCorrectionFactor);
	
}


} // end of namespace CLAM
