/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _PhaseManagement_
#define _PhaseManagement_

#include "OSDefines.hxx"
#include "DataTypes.hxx"
#include "SpectralPeakArray.hxx"
#include "Frame.hxx"
#include "Processing.hxx"
#include "InControl.hxx"


#include "PhaseManagementConfig.hxx"

namespace CLAM
{
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
	typedef PhaseManagementConfig Config;
	PhaseManagement(const PhaseManagementConfig& c = Config());
	~PhaseManagement	();

	const char * GetClassName() const {return "PhaseManagement";}
	
	bool Do(){ return false;}
	bool Do(Frame& currentFrame);
	
	/** Basic non-supervised Do.@todo: the way the algorithms are now, they just allow inplace
	 *	processing. This should be changed.*/
	bool Do(SpectralPeakArray& in);

	/** Configuration method */
	bool ConcreteConfigure(const ProcessingConfig&);

	const ProcessingConfig &GetConfig() const {return mConfig;}

	void Init();

	//Controls
	FloatInControl mCurrentTime;
	FloatInControl mCurrentPitch;

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

} //CLAM namespace
#endif

