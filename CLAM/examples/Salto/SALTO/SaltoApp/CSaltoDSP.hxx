#ifndef _CSaltoDSP_
#define _CSaltoDSP_

#include "Err.hxx"
#include "OSDefines.hxx"
//#include "CSaltoMIDIInOut.hxx"
//#include "CSaltoEditor.hxx"
#include "Array.hxx"
#include "CSaltoSineSynthesis.hxx"
#include "AudioIO.hxx"
#include "MIDIIO.hxx"
#include "MIDIInControl.hxx"
//#include "CSaltoFFT.hxx"
#include "SpectralSynthesis.hxx"
#include "CSaltoSynthFrame.hxx"
#include "CSaltoDataManagment.hxx"
#include "CSaltoResidualSynthesis.hxx"
#include "CSaltoTimbreVektor.hxx"
#include "CSaltoInterpolation.hxx"
#include <stdio.h>
#include <stdlib.h>

//XA testing
#include "AudioFileOut.hxx"
#include "Melody.hxx"
//#include "CSaltoHandleParamChanges.hxx"
#include "MIDIHandler.hxx"

namespace CLAM
{

class CSaltoEditor;

class CSaltoDSPConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (CSaltoDSPConfig, 5, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, TSize, AnalysisFrameSize );
	DYN_ATTRIBUTE (2, public, TData, SampleRate );
	DYN_ATTRIBUTE (3, public, EWindowType, AnalysisWindowType );
	DYN_ATTRIBUTE (4, public, TData, PitchModRange );
private:
	Parameters*			mpParams;
	MelodyTranslator*	mMelody;
protected:
	void DefaultInit(void);
public:
	inline Parameters* GetParams() { return mpParams; }
	inline void SetParams( Parameters& params ) { mpParams = &params; }

	inline MelodyTranslator* GetMelody() { return mMelody; }
	inline void SetMelody( MelodyTranslator& melody ) { mMelody = &melody; }

	inline TSize GetSpectralFrameSize() const
	{
		return GetAnalysisFrameSize()+1;
	}

	inline TSize GetHopSize() const
	{
		return GetAnalysisFrameSize();
	}

	inline TTime GetFrameTime() const
	{
		return GetAnalysisFrameSize()/GetSampleRate();
	}

	inline TData GetSpectralRange() const
	{
		return GetSampleRate()/TData(2);
	}

	inline TSize GetAudioFrameSize() const
	{
		return GetAnalysisFrameSize()*2;
	}
};

class CSaltoDSP:public ProcessingComposite
{
public:
//	CSaltoDSP(CSaltoParameter *pParams,CSaltoMIDIInOut *pMIDI);
//	CSaltoDSP(Parameters *pParams);//,CSaltoMIDIInOut *pMIDI);
	CSaltoDSP();

	CSaltoDSP( const CSaltoDSPConfig& cfg );

	~CSaltoDSP();

	const ProcessingConfig &GetConfig() const { return mConfig; }

	bool Do(void) { return true; }
	

	void Do( bool& sentinel );

	void DoFlowInterpolate( bool& sentinel );

	void DoFlowInterpolate2( bool& sentinel );

	void InitInterpolatingSynthesis();
	void InitFollowOneTrackSynthesis();
	void InitGenerateTestSine();
	void InitSDIFEditSynthesis();

	void DoInterpolatingSynthesis  (CSaltoSynthFrame *pSynthFrame);
	void InitTransitionSynthesis(CSaltoSynthFrame *pSynthFrame);
	void DoTransitionSynthesis(CSaltoSynthFrame *pSynthFrame);
	void EndTransitionSynthesis(CSaltoSynthFrame *pSynthFrame);
	void DoGenerateTestSineSynthesis(CSaltoSynthFrame *pSynthFrame);
	void DoFollowOneTrackSynthesis (CSaltoSynthFrame *pSynthFrame);
	void DoSDIFEditSynthesis(CSaltoSynthFrame *pSynthFrame);
	void DoInterpolatingSynthesisRelease(CSaltoSynthFrame *mpSynthFrame,double gain);
	void DoSynthesisProcess(CSaltoSynthFrame *pSynthFrame, bool inReleaseMode = false);
	void DoInterruptAudio();
	const SpectralPeakArray& GetCurrentPeakArray();
	const SpectralPeakArray* GetCurrentPeakArrayPtr();
	const Spectrum* GetCurrentResidualPtr();
	Frame* GetStatTmplFrame(TIndex pos);
	TIndex GetnFramesInStatTmplSeg();
	void SetSDIFToEdit(const char *fileName);
	void PlaySDIFEdit(CSaltoSynthFrame *pSynthFrame);
	void AddStatTmplToDatabase(CSaltoStatTmplData& d,TIndex pos);
	void StoreAllParams();
	bool UpdateParams();
	const DataArray& GetAudioBufferReference();
	void BindWithGUI( CSaltoEditor* pGUI );

	CSaltoSynthFrame* GetSynthFramePtr(){ return mpSynthFrame; };

	inline void SetEventSample( double sample ) { mEventSample = sample; }

protected:
	void DoStationarySynthesisProcess( CSaltoSynthFrame* pSynthFrame );
	void DoReleaseSynthesisProcess( CSaltoSynthFrame* pSynthFrame );
	void SinusoidalSynthesis(CSaltoSynthFrame* pSynthFrame );
	void AttackResidualSynthesis( CSaltoSynthFrame* pSynthFrame );
	void StationaryResidualSynthesis( CSaltoSynthFrame* pSynthFrame );
	void UpdateGUI();
	void DoInterpolatingReleaseSynthesis( CSaltoSynthFrame* pSynthFrame, double gain, int currRelFrame );
	void DoTransitionSynthesis2( CSaltoSynthFrame* pSynthFrame );

	bool ConcreteConfigure( const ProcessingConfig& cfg ) throw(std::bad_cast);
	bool ConcreteStart();
	bool ConcreteStop();

	void HandleMIDI (void);
	void HandleIdle(void);
	void HandleAttack(void);
	void HandleRelease(void);
	void HandleTransition(void);
	void HandleSustain(void);
	void ConfigureSaltoIO();

	void RenderSynthesis();
	void RenderSilence();

	int UpdateState( TControlData state );
private:
	CSaltoDSPConfig mConfig;
	/*settings*/
	double mSampleRate, mSpectralRange,	mFrameTime, mEventSample;
						
	TSize	mAnalysisFrameSize, mAudioFrameSize, mSpectralFrameSize, mHopSize;

	EWindowType mAnaWindowType;
	EWindowType mOLAWindowType;

	/* runtime */
	Frame*	mpCurrSpectralFrameBase;
	Frame*	mpCurrSpectralFrameTarget;
	Frame*	mpCurrSpectralFrameResidual;	// for TESTING
	SpectralPeakArray* mpCurrPeakArrayTarget;
  Frame*  mpTransitionFrameBase;
	Frame*  mpTransitionFrameTarget;
	
	CSaltoTimbreVektor mTimbreVektorBase, mTimbreVektorTarget;

	TIndex
	  mFrameCounterBase,
	  mFrameCounterTarget,
    mFrameCounterAttackResidual,
	  mFrameCounterStatRes,
	  mFrameCounterSDIFEdit,
	  mFrameCounterTransition,
	  mNumTransitionFrames,
	  mNumFramesBase,
	  mNumFramesTarget,
	  mLoopStart,
	  mLoopEnd,
	  mLimitLoopStart,
	  mLimitLoopEnd,
	  mInterpolStart,
	  mInterpolEnd,
	  mInterpolDuration,
	  mSegPositionBase,
	  mSegPositionTarget,
	  mSegPositionTransition,
	  mResFadeStart,
	  mResFadeEnd,
	  mStatResFadeInFrom,
	  mStatResFadeInTo,
	  mNumFramesStatRes,
	  mSinAttackOffset,
	  mSinAttackOffsetCounter;
	
  bool mLoopDirectionFW, mStatResLoopDirectionFW;

	double
	  mAttackResVolume,
	  mStatResVolume,
	  mIPFactor,
	  mPitchReference,
	  mIndividualGain,
	  mLastIndividualGain,
	  mLastPitchCorrectionFactor;
	  
	TIndex mSynthesisFlow;
	/* SDIF Edit Flow */
	TIndex mEditFrameCounter;
	
	/* Sine Sweep Test Functions */
	TIndex
	  mTestCounter;
	double
	  mTestFreq1,
	  mTestFreq2,
	  mTestPhase1,
	  mTestPhase2;
	
	/*storage*/
	Audio									AudioInBuffer,
											AudioOutBuffer,
											EmptyAudioBuffer,
											mAudioBuffer;												  													
	DataArray						mDrawingBuffer;
	Spectrum						*mpSpectralBuffer;
	CSaltoSynthFrame				*mpSynthFrame;
	CSaltoDataManagment			*mpDataManagment;
	
	/* processing objects* all named PO*/
	CSaltoSineSynthesis			*mpSineSynthPO;
//	CSaltoFFT								*mpFFTPO;
	SpectralSynthesis	mSpectralSynthesisPO;
	CSaltoResidualSynthesis	*mpResSynthPO;
	CSaltoInterpolation			*mpInterpolPO;
	
	//CSaltoHandleParamChanges mHandleParams;
	MIDIHandler mMIDIHandler;


		
	/* pointer to other classes*/
//	CSaltoParameter					*mpParams;
	Parameters	        *mpParams;
//	CSaltoMIDIInOut 				*mpMIDI;
public:
	struct BreathController
	{
	public:
			MIDIInControl	mInNote;
			MIDIInControl	mAirSpeed;

			BreathController( const MIDIInConfig &note,
							  const MIDIInConfig &airspeed ):
							mInNote( note ),
							mAirSpeed( airspeed ) {}

			BreathController(){}
	};

	CSaltoEditor						*mpGUI;
	MelodyTranslator			*mMelody;
	
	void LoadMelody(std::string& fileName);
//  int GLOBAL_DRAWING;

	// Refactoring members
	// Sample Based
	AudioManager*    mAudioManager;
	AudioIn*          mAudioIn;
	AudioOut*         mAudioOut;
	AudioFileOut     mFileAudioOut;
	//MIDI based
	MIDIManager      mMIDIManager;
	// Audio for the AudioIn
	Audio            mDummyIn;

	//SALTO states
	enum State{
		Idle		= 0,
		Attack		= 1,
		Sustain		= 2,
		Transition	= 3,
		Release		= 4,
		Termination = 5
						} mState;

	void SetTerminationState( void )
	{
		mState = Termination;
		std::cout << "TERMINATION"<< std::endl;
	}


	InControlTmpl< CSaltoDSP >		mStateIn;
};

} // end of namespace CLAM
#endif
