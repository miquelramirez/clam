#ifndef _PhaseManagement_
#define _PhaseManagement_

#include "OSDefines.hxx"
#include "DataTypes.hxx"
#include "SpectralPeakArray.hxx"
#include "Frame.hxx"
#include "Processing.hxx"


namespace CLAM{

/** PhaseGeneration
*   Enumeration to use for specifying the type of phase generation used
*/
class EPhaseGeneration: public Enum
{
public:
	
	static tEnumValue sEnumValues[];
	static tValue sDefault;
	EPhaseGeneration() : Enum(sEnumValues, sDefault) {}
	EPhaseGeneration(tValue v) : Enum(sEnumValues, v) {};
	EPhaseGeneration(std::string s) : Enum(sEnumValues, s) {};

	typedef enum {
		eAlign,
		eRandom,
		eContinuation
	} tEnum;

	virtual Component* Species() const
	{
		return (Component*) new EPhaseGeneration;
	};
};

class PhaseManagementConfig:public ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE (PhaseManagementConfig,4,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
	DYN_ATTRIBUTE(1,public,TSize,MaxSines);
	DYN_ATTRIBUTE(2,public,TData,SamplingRate);
	DYN_ATTRIBUTE(3,public,EPhaseGeneration,Type);
public:
	~PhaseManagementConfig(){};
	void DefaultInit();
	void DefaultValues();
};


/** 
* The  PhaseManagement Class takes care about either
* <ol>
* <li> generating a random set of phases and their continuation</li>
* <li> phase alignment</li>
*
*/
class	PhaseManagement:public Processing
{
public:

	PhaseManagement();
	PhaseManagement(PhaseManagementConfig& c);
	~PhaseManagement	();

	const char * GetClassName() const {return "PhaseManagement";}
	
	bool Do(void){ return false;}
	bool Do(Frame& currentFrame);
	
	/** Basic non-supervised Do.@todo: the way the algorithms are now, they just allow inplace
	 *	processing. This should be changed.*/
	bool Do(SpectralPeakArray& in);

	/** Configuration method */
	bool ConcreteConfigure(const ProcessingConfig&);

	const ProcessingConfig &GetConfig() const {return mConfig;}

	void Init();

	//Controls
	InControlTmpl<PhaseManagement> mCurrentTime;
	InControlTmpl<PhaseManagement> mCurrentPitch;

public:
	void ResetPhaseAlignment();	

	void DoPhaseAlignment (SpectralPeakArray& peakArray);
	void DoRandomPhases	(SpectralPeakArray& peakArray);
	void DoPhaseContinuation(SpectralPeakArray& p);
	void SetLastPhasesAndFreqs (SpectralPeakArray& peakArray);
	void GenerateRandomPhases(DataArray& a);




private:
	double	mFrameTime,
			mLastPeriodTime,
			mNextPeriodTime,
			mLastFundFreq;
						
	DataArray	mRandomPhase;
	SpectralPeakArray	mLastPeakArray;
				
	
	/* private configuration*/
	PhaseManagementConfig mConfig;


};

};//CLAM namespace
#endif
