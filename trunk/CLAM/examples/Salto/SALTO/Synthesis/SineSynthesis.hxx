#ifndef _CSaltoSineSynthesis_
#define _CSaltoSineSynthesis_


#include "Processing.hxx"
#include "Spectrum.hxx"
#include "CSaltoSynthFrame.hxx"
#include "SynthSineSpectrum.hxx"
#include "SynPhaseManagement.hxx"
#include "Parameters.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"


namespace SALTO
{

using CLAM::TSize;
using CLAM::Processing;
using CLAM::ProcessingConfig;
using CLAM::SynthSineSpectrum;
using CLAM::SynthSineSpectrumConfig;
using CLAM::SynPhaseManagement;
using CLAM::SynPhaseManagementConfig;
using CLAM::Spectrum;
using CLAM::InControl;
using CLAM::OutControl;

// SALTO things in CLAM namespace
using CLAM::Parameters;
using CLAM::CSaltoSynthFrame;


class SineSynthesisConfig
	: public ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE( SineSynthesisConfig, 4, ProcessingConfig );
	DYN_ATTRIBUTE( 0, public, std::string, Name );
	DYN_ATTRIBUTE( 1, public, TSize, MaxSines );
	DYN_ATTRIBUTE( 2, public, double, FrameTime );
	DYN_ATTRIBUTE( 3, public, double, SampleRate );
	DYN_ATTRIBUTE( 4, public, TSize, SpectralFrameSize );
protected:
	
	void DefaultInit();

};

class  SineSynthesis
	: public ProcessingComposite
{
public:

	typedef enum 
	{
		eAttackTimbreLevel = 0,
		eUsePhaseAlignment = 1,
		eLastAlignedFrame = 2,
		eBreathOnlySound = 3		
	} InControls;
	
	typedef enum 
	{ 
		eLastAlignedFrame = 0,
		eBreathOnlySound = 1
	} OutControls;
public:

	SineSynthesis();

	SineSynthesis( CSaltoSineSynthesisConfig& cfg );

	virtual ~SineSynthesis();

	virtual const char* GetClassName()
	{
		return "SALTO::SineSynthesis";
	}

	virtual bool Do();

	bool Do( CSaltoSynthFrame& synthesisFrame );

	bool Reset();

//public methods
	void 	DoSineSynthesis( CSaltoSynthFrame &synthFrame );
	void    ResetSineSynthesis();

protected:

	bool ConcreteStart();

	bool ConcreteStop();

	bool ConcreteConfigure( const ProcessingConfig& cfg );

	int AttackTimbreLevelCB( TControlData value );

	int UsePhaseAlignmentCB( TControlData value );

	int LastAlignedFrameCB( TControlData value );

	int BreathOnlySoundCB( TControlData value );

private:
	// settings
	SineSynthesisConfig   mConfig;

	double                      mCurrentTime;
	double                      mFrameTime;
 	bool                        mSwitchToRandomPhases;

	//Sub-Processing
	SynthSineSpectrum			mSpecSynthPO;
	SynPhaseManagement          mPhaseManagerPO;
	
	// In and out controls

	// mAtkTimLvlCtl => Attack Timbre Level in Control
	CLAM::InControlTmpl< CSaltoSineSynthesis >   mAtkTimLvlCtl;
	// mEnPhAlignCtl => Enable/Disable Phase Alignment
	CLAM::InControlTmpl< CSaltoSineSynthesis >   mEnPhAlignCtl;
	// mInLAFrameCtl => Last Frame Alignment detection outside PO
	CLAM::InControlTmpl< CSaltoSineSynthesis >   mInLAFrameCtl;
	// mOutLAFrameCtl => Last Frame Alignment detection on PO's Do
	OutControl                             mOutLAFrameCtl;
	// mInBrOnlyCtl => Breath Only sound signal
	CLAM::InControlTmpl< CSaltoSineSynthesis >   mInBrOnlyCtl;
	// mOutBrOnlyCtl => Breath Only sound notification
	OutControl                             mOutBrOnlyCtl;

	// Derived from mAtkTimLvlCtl
	double                 mGain;
	// Derived from mEnPhAligCtl
	bool                   mPhaseAlignmentEnabled;
	// Related with m*LAFrameCtl
	bool                   mLastAlignedFrame;
	// Related with m*BrOnlyCtl
	bool                   mBreathOnlySound;
};

} // end of namespace CLAM
#endif
