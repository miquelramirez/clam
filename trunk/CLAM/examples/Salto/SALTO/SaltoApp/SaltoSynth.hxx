#ifndef __SALTOSYNTH__
#define __SALTOSYNTH__

#include "Err.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Array.hxx"
#include "CSaltoSineSynthesis.hxx"
#include "SpectralSynthesis.hxx"
#include "CSaltoSynthFrame.hxx"
#include "CSaltoResidualSynthesis.hxx"
#include "CSaltoTimbreVektor.hxx"
#include "CSaltoInterpolation.hxx"
#include <string>

namespace CLAM
{

class CSaltoStatTmplData;
class CSaltoEditor;

class SaltoSynthConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (SaltoSynthConfig, 5, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, TSize, AnalysisFrameSize );
	DYN_ATTRIBUTE (2, public, TData, SampleRate );
	DYN_ATTRIBUTE (3, public, EWindowType, AnalysisWindowType );
	DYN_ATTRIBUTE (4, public, TData, PitchModRange );
private:
	Parameters*			mpParams;
protected:
	void DefaultInit(void);
public:
	inline Parameters* GetParams() { return mpParams; }
	inline void SetParams( Parameters& params ) { mpParams = &params; }

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

class SaltoSynth : public ProcessingComposite
{
	//Types Definition
public:

	//SALTO states
	enum State{
		Idle		   = 0,
		Attack		   = 1,
		Sustain		   = 2,
		Transition	   = 3,
		Release		   = 4,
		Termination    = 5,
		TransitionLoop = 6,
		ReleaseLoop    = 7	} mState;


	// Methods Definition
public:
	SaltoSynth();

	SaltoSynth( const SaltoSynthConfig& cfg );

	~SaltoSynth();

	inline const ProcessingConfig& GetConfig() const
	{
		return mConfig;
	}

	// Non-supervised mode Do
	bool Do()
	{
		return true;
	}

	bool Do( Audio*& synthBuffer );

	void InitInterpolatingSynthesis();

	void DoInterpolatingSynthesis  (CSaltoSynthFrame *pSynthFrame);

	void InitTransitionSynthesis(CSaltoSynthFrame *pSynthFrame);

	void EndTransitionSynthesis(CSaltoSynthFrame *pSynthFrame);
	
	void DoInterpolatingSynthesisRelease( CSaltoSynthFrame *mpSynthFrame, double gain );

	const SpectralPeakArray* GetCurrentPeakArrayPtr();

	const Spectrum* GetCurrentResidualPtr();

	void BindWithGUI( CSaltoEditor* pGUI );

	Frame* GetStatTmplFrame(TIndex pos);

	const DataArray& GetAudioBufferReference();

	inline double GetEventSample() const
	{
		return mEventSample;
	}

	inline void ResetEventSample()
	{
		mEventSample = 0.0;
	}

	inline CSaltoSynthFrame* GetSynthFramePtr()
	{ 
		return mpSynthFrame; 
	}

	inline void SetEventSample( double sample ) 
	{ 
		mEventSample = sample; 
	}

	inline void SetTerminationState( void )
	{
		mState = Termination;
	}

protected:

	void DoStationarySynthesisProcess( CSaltoSynthFrame* pSynthFrame );

	void DoReleaseSynthesisProcess( CSaltoSynthFrame* pSynthFrame );

	void AttackResidualSynthesis( CSaltoSynthFrame* pSynthFrame );

	void StationaryResidualSynthesis( CSaltoSynthFrame* pSynthFrame );

	void UpdateGUI();

	void DoInterpolatingReleaseSynthesis( CSaltoSynthFrame* pSynthFrame, double gain, int currRelFrame );

	void DoTransitionSynthesis2( CSaltoSynthFrame* pSynthFrame );

	bool ConcreteConfigure( const ProcessingConfig& cfg ) throw(std::bad_cast);

	bool ConcreteStart();

	bool ConcreteStop();

	void InitReleaseSynthesis();

	void HandleIdle(Audio*& synthBuffer);

	void HandleAttack(Audio*& synthBuffer);

	void HandleRelease(Audio*& synthBuffer);

	void HandleTransition(Audio*& synthBuffer);

	void HandleSustain(Audio*& synthBuffer);

	int UpdateState( TControlData state );

	// Attributes
private:

	SaltoSynthConfig mConfig;
	/*settings*/
	double mEventSample;

	/* runtime */
	Frame*	           mpCurrSpectralFrameBase;
	Frame*	           mpCurrSpectralFrameResidual;	// for TESTING
	SpectralPeakArray* mpCurrPeakArrayTarget;
	Frame*             mpTransitionFrameBase;
	Frame*             mpTransitionFrameTarget;
	
	CSaltoTimbreVektor mTimbreVektorBase;

	TIndex  mFrameCounterBase;
	TIndex  mFrameCounterAttackResidual;
	TIndex  mFrameCounterStatRes;
	TIndex  mFrameCounterTransition;
	TIndex  mNumTransitionFrames;
	TIndex  mNumFramesBase;
	TIndex  mLoopStart;
	TIndex  mLoopEnd;
	TIndex  mLimitLoopStart;
	TIndex  mLimitLoopEnd;
	TIndex  mInterpolStart;
	TIndex  mInterpolEnd;
	TIndex  mInterpolDuration;
	TIndex  mSegPositionBase;
	TIndex  mSegPositionTransition;
	TIndex  mResFadeStart;
	TIndex  mResFadeEnd;
	TIndex  mStatResFadeInFrom;
	TIndex  mStatResFadeInTo;
	TIndex  mNumFramesStatRes;
	TIndex  mSinAttackOffset;
	TIndex  mSinAttackOffsetCounter;
	
	bool mLoopDirectionFW;
	bool mStatResLoopDirectionFW;

	double mAttackResVolume;
	double mStatResVolume;
	double mIPFactor;
	double mPitchReference;
	double mIndividualGain;
	double mLastIndividualGain;
	double mLastPitchCorrectionFactor;
	  
	TIndex  mNumFramesRelease;
	TIndex  mFrameCounterRelease;
	
	/*storage*/
	Audio AudioOutBuffer;
	Audio EmptyAudioBuffer;

	DataArray           mDrawingBuffer;
	CSaltoSynthFrame*   mpSynthFrame;
	
	/* processing objects* all named PO*/
	CSaltoSineSynthesis*     mpSineSynthPO;
	SpectralSynthesis        mSpectralSynthesisPO;
	CSaltoResidualSynthesis* mpResSynthPO;
	CSaltoInterpolation*     mpInterpolPO;
		
	/* pointer to other classes*/
	Parameters	        *mpParams;
	CSaltoEditor* mpGUI;

	InControlTmpl< SaltoSynth >		mStateIn;

	/* Sample stepping for counting time: */
	TSize    mSampleStepping;
};

}

#endif // SaltoSynth.hxx
