#ifndef _SineSynthesis_
#define _SineSynthesis_


#include "ProcessingComposite.hxx"
#include "Spectrum.hxx"
#include "CSaltoSynthFrame.hxx"
#include "SynthSineSpectrum.hxx"
#include "PhaseManagement.hxx"
#include "Parameters.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"

#include "Parameters.hxx"


namespace SALTO
{

using CLAM::TSize;
using CLAM::ProcessingComposite;
using CLAM::ProcessingConfig;
using CLAM::DynamicType;
using CLAM::SynthSineSpectrum;
using CLAM::SynthSineSpectrumConfig;
using CLAM::PhaseManagement;
using CLAM::PhaseManagementConfig;
using CLAM::Spectrum;
using CLAM::InControl;
using CLAM::OutControl;
using CLAM::TControlData;

// SALTO things in CLAM namespace
using CLAM::Parameters;
using CLAM::CSaltoSynthFrame;


class SineSynthesisConfig
	: public ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE( SineSynthesisConfig, 5, ProcessingConfig );
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
		inAttackTimbreLevel = 0,
		inUsePhaseAlignment = 1,
		inLastAlignedFrame = 2,
		inBreathOnlySound = 3		
	} InControls;
	
	typedef enum 
	{ 
		outLastAlignedFrame = 0,
		outBreathOnlySound = 1
	} OutControls;
public:

	SineSynthesis();

	SineSynthesis( const SineSynthesisConfig& cfg );

	virtual ~SineSynthesis();

	const char* GetClassName() const
	{
		return "SALTO::SineSynthesis";
	}

	virtual const ProcessingConfig &GetConfig() const { return mConfig; };

	virtual bool Do();

	bool Do( CSaltoSynthFrame& synthesisFrame );

	bool Reset();

//public methods
	void 	DoSineSynthesis( CSaltoSynthFrame &synthFrame, Parameters* mpParameter );
	void    ResetSineSynthesis();

protected:

	bool ConcreteStart();

	bool ConcreteStop();

	bool ConcreteConfigure( const ProcessingConfig& c );

	int AttackTimbreLevelCB( TControlData value );

	int UsePhaseAlignmentCB( TControlData value );

	int LastAlignedFrameCB( TControlData value );

	int BreathOnlySoundCB( TControlData value );

private:
	// settings
	SineSynthesisConfig			mConfig;

	double                      mCurrentTime;
	double                      mFrameTime;
 	bool                        mSwitchToRandomPhases;

	//Sub-Processing
	SynthSineSpectrum			mSpecSynthPO;
	PhaseManagement          mPhaseManagerPO;
	
	// In and out controls

	// mAtkTimLvlCtl => Attack Timbre Level in Control
	CLAM::InControlTmpl< SineSynthesis >   mAtkTimLvlCtl;
	// mEnPhAlignCtl => Enable/Disable Phase Alignment
	CLAM::InControlTmpl< SineSynthesis >   mEnPhAlignCtl;
	// mInLAFrameCtl => Last Frame Alignment detection outside PO
	CLAM::InControlTmpl< SineSynthesis >   mInLAFrameCtl;
	// mOutLAFrameCtl => Last Frame Alignment detection on PO's Do
	OutControl                             mOutLAFrameCtl;
	// mInBrOnlyCtl => Breath Only sound signal
	CLAM::InControlTmpl< SineSynthesis >   mInBrOnlyCtl;
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
