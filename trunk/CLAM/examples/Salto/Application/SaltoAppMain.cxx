//#include "CSaltoParameter.hxx"
#include "Parameters.hxx"
#include "SaltoSynth.hxx"
#include "CSaltoEditor.hxx"
#include "SaltoConfig.hxx"
//#include "CSaltoMIDIInOut.hxx"
#include "Err.hxx"
#include "GUIAudioApplication.hxx"
#include "WidgetTKWrapper.hxx"
#include "AudioManager.hxx"
#include "AudioIn.hxx"
#include "AudioOut.hxx"
#include "MIDIManager.hxx"
#include "MIDIInControl.hxx"
#include "AudioFileOut.hxx"
#include "MIDIHandler.hxx"
#include "MIDIBreathController.hxx"
#include "CSaltoDataManagment.hxx"
#include <iostream>
#include "MelodyTranslator.hxx"
#include <FL/Fl_Double_Window.H>
#ifdef CreateWindow
#undef CreateWindow
#endif
using SALTO::CSaltoEditor;
using CLAMVM::WidgetTKWrapper;
using namespace CLAM;

class SaltoApp:public GUIAudioApplication
{
public:
	SaltoApp()
	{
		pDSP = NULL;
		pMelody = NULL;
		pGUI=NULL;
	}

	virtual ~SaltoApp()
	{
		
		if (pDSP) 
		{
			delete pDSP;
			pDSP=NULL;
		}
		if (pMelody)
		{ 
			delete pMelody;
			pMelody=NULL;
		}
		if (pGUI)
		{ 
			delete pGUI;
			pGUI=NULL;
		}
	}

	Fl_Window* CreateWindow(int argc,char** argv)
	{
		CSaltoDataManagment::InitSaltoDB( &Parameters::GetInstance() );

		pMelody = new MelodyTranslator;
		CLAM_ASSERT( pMelody,
			"OOM in MAIN cant construct melody");


		DSPCfg.SetParams( Parameters::GetInstance() );
		mMIDIHandler.SetParams( &Parameters::GetInstance() );
		pDSP = new SaltoSynth(DSPCfg);
		CLAM_ASSERT(pDSP,
			"OOM in MAIN cant construct CSaltoDSP");

		// gui , user interface interacts also with parameter class
		pGUI = new CSaltoEditor(&Parameters::GetInstance(), pDSP, pMelody, this);//,pMIDI);
		CLAM_ASSERT(pGUI,
			"OOM in MAIN cant construct editor");

		MIDIHandlerConfig MIDIHandlerCfg;
		MIDIHandlerCfg.SetPitchModRange( DSPCfg.GetPitchModRange() );
		mMIDIHandler.Configure( MIDIHandlerCfg );
		mMIDIHandler.SetParams(&Parameters::GetInstance());
		mMIDIHandler.LinkOutControl(0, pDSP, 0);

		pMelody->GetOutControls().GetByNumber(0).AddLink( &(mMIDIHandler.GetInControls().GetByNumber(0)) );
		pMelody->GetOutControls().GetByNumber(1).AddLink( &(mMIDIHandler.GetInControls().GetByNumber(1)) );		
		pGUI->mpEditorWindow->set_non_modal();
		pGUI->mpEditorWindow->show(argc,argv);


		return pGUI->mpEditorWindow;
	}

protected:
	SaltoSynth*       pDSP;
	MelodyTranslator* pMelody;
	CSaltoEditor*     pGUI;
	SaltoSynthConfig  DSPCfg;

	void UserMain(void)
	{
		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");

		Start();

		tk.Run();

		Stop();
	}


	void AudioMain(void)
	{

		try
		{
			bool not_finished = true;
			Audio* synthbuffer = NULL;

			ConfigureSampleBasedIO();

			pDSP->BindWithGUI( pGUI );
			pDSP->Start();

			MIDIInConfig inNoteCfg;

			inNoteCfg.SetDevice("default:default");
			inNoteCfg.SetChannelMask(MIDI::ChannelMask(1));
			inNoteCfg.SetMessageMask(
				MIDI::MessageMask(MIDI::eNoteOn)|
				MIDI::MessageMask(MIDI::eNoteOff)
			);
			MIDIInControl keyboardNote( inNoteCfg );

			MIDIInConfig inPitchBendCfg;
			
			inPitchBendCfg.SetDevice("default:default");
			inPitchBendCfg.SetChannelMask(MIDI::ChannelMask(1));
			inPitchBendCfg.SetMessageMask(MIDI::MessageMask(MIDI::ePitchbend));

			MIDIInControl pitchBend( inPitchBendCfg );

			MIDIInConfig inBreathNoteCfg;

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
			
			inCtrlCfg.SetDevice("default:default");
			inCtrlCfg.SetChannelMask(MIDI::ChannelMask(1));
			inCtrlCfg.SetMessageMask(MIDI::MessageMask(MIDI::eControlChange));
			inCtrlCfg.SetFilter(11);

			SALTO::BreathController breathController( inBreathNoteCfg, inCtrlCfg );

			pitchBend.GetOutControls().GetByNumber(0).AddLink( &(mMIDIHandler.GetInControls().GetByNumber(2)));

			keyboardNote.GetOutControls().GetByNumber(0).AddLink(&(mMIDIHandler.GetInControls().GetByNumber(1)));
			keyboardNote.GetOutControls().GetByNumber(1).AddLink(&(mMIDIHandler.GetInControls().GetByNumber(0)));
			keyboardNote.GetOutControls().GetByNumber(2).AddLink(&(mMIDIHandler.GetInControls().GetByNumber(1)));
			keyboardNote.GetOutControls().GetByNumber(3).AddLink(&(mMIDIHandler.GetInControls().GetByNumber(0)));		

			breathController.mInNote.GetOutControls().GetByNumber(0).AddLink(&(mMIDIHandler.GetInControls().GetByNumber(1)));
			breathController.mInNote.GetOutControls().GetByNumber(1).AddLink(&(mMIDIHandler.GetInControls().GetByNumber(0)));
			breathController.mInNote.GetOutControls().GetByNumber(2).AddLink(&(mMIDIHandler.GetInControls().GetByNumber(1)));
			breathController.mInNote.GetOutControls().GetByNumber(3).AddLink(&(mMIDIHandler.GetInControls().GetByNumber(0)));


			breathController.mAirSpeed.GetOutControls().GetByNumber(0).AddLink(&(mMIDIHandler.GetInControls().GetByNumber(3)));

			mFileAudioOut.Start();
			mMIDIManager.Start();
			mAudioManager->Start();
			mMIDIHandler.Start();
			mAudioOutL->Start(); 

			while ( !Canceled() )
			{

				ProcessMIDIMessages();
				
				not_finished = pDSP->Do( synthbuffer );

				CLAM_DEBUG_ASSERT( synthbuffer != NULL, "Whooops! Synthesis buffer was void!" );
			
				RenderSynthesis( *synthbuffer );

			}

			mMIDIHandler.Stop();
			pDSP->Stop();

			if ( Parameters::GetInstance().GetWriteToFile())
				{
					std::cout << "Closing audio File ..... " << std::endl;
					Parameters::GetInstance().SetExit(true); // MRJ: Ya lo es no?
					mFileAudioOut.Stop();
				}


			std::cout << "exiting..." << std::endl;
		

		}
		catch (Err err)
		{
			err.Print();
			exit(-1);
		}
	}

	void ConfigureSampleBasedIO()
	{
		AudioFileConfig outcfg;
		outcfg.SetFilename("salto.wav");
		outcfg.SetFiletype(EAudioFileType::eWave);
		outcfg.SetChannels(1);

		mFileAudioOut.Configure( outcfg );

		mAudioManager = new AudioManager( DSPCfg.GetSampleRate(), DSPCfg.GetHopSize());	

		AudioIOConfig iocfgL;
		iocfgL.SetChannelID(0);
		AudioIOConfig iocfgR;
		iocfgR.SetChannelID(1);
		mAudioOutL = new AudioOut(iocfgL);
	}

	void RenderSynthesis(Audio& synthbuffer)
	{
		if( Parameters::GetInstance().GetWriteToFile())
			mFileAudioOut.Do( synthbuffer );
		else
		{
			
			mAudioOutL->Do( synthbuffer );
//			mAudioOutR->Do( synthbuffer );
		}
	}

	void ProcessMIDIMessages(void)
	{
		if ( Parameters::GetInstance().GetPlayNote() )
		{
			mMIDIHandler.DoNote();
		}
		if (Parameters::GetInstance().GetUseMelody())
		{
			TTime currentTime = pDSP->GetEventSample()/DSPCfg.GetSampleRate();
			if (!pMelody->Do( Parameters::GetInstance().GetPlay(), currentTime ))
			{
				Parameters::GetInstance().SetUseMelody( false );
				pDSP->ResetEventSample();
			}
			mMIDIHandler.Do();
		}
		if ( Parameters::GetInstance().GetUseMidiKeyboard() || Parameters::GetInstance().GetUseBreathController() )
		{
			mMIDIManager.Check();
			mMIDIHandler.Do();
		}
	}

private:

	// Sample Based
	AudioManager*    mAudioManager;
	AudioOut*        mAudioOutL;
	AudioOut*        mAudioOutR;
	AudioFileOut     mFileAudioOut;
	//MIDI based
	MIDIManager      mMIDIManager;
	MIDIHandler      mMIDIHandler;
	// Audio for the AudioIn
	//Audio            mDummyIn;


};

int main (int argc,char** argv)
{
	//WHY DOES THIS NOT WORK? IT SEEMS TO CAUSE SOME DATA CORRUPTION,
	//OR MORE LIKELY, CAUSE SOME DATA CORRUPTION TO BECOME EVIDENT...
	//OR MAYBE... IT'S TRYING TO RUN Fl::run TWICE IS CAUSING PROBLEMS?
	
	//SaltoConfig config(argc,argv);
	//InstrumentValues::Instantiate(config.mInstrument,config.mDataFolder);

	InstrumentValues::Instantiate(0,"SaltoDataFolder");

	SaltoApp mySalto;

	try
	{
		mySalto.Run(argc,argv);
	}
	catch ( Err& e )
	{
		e.Print();
		return -1;
	}
	return 0;
}
