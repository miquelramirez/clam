#ifndef _PhaseManagement_
#define _PhaseManagement_

#include "OSDefines.hxx"
#include "DataTypes.hxx"
#include "SpectralPeakArray.hxx"
#include "Frame.hxx"
#include "Processing.hxx"


#include "PhaseManagementConfig.hxx"

namespace CLAM{


	/* PhaseManagementConfig moved to PhaseManagementConfig.hxx */



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
