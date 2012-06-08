/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _SMSAnalysisSynthesisConfig_
#define _SMSAnalysisSynthesisConfig_
#include <typeinfo>

#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/EPhaseGeneration.hxx>
#include <CLAM/Filename.hxx>
#include <CLAM/GlobalEnums.hxx>

namespace CLAM{


class SMSAnalysisSynthesisConfig:public ProcessingConfig
{

	DYNAMIC_TYPE_USING_INTERFACE (SMSAnalysisSynthesisConfig,24,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);

/** General	**/
	DYN_ATTRIBUTE(1,public,Filename,InputSoundFile);
	DYN_ATTRIBUTE(2,public,Filename,OutputSoundFile);
	DYN_ATTRIBUTE(3,public,Filename,OutputAnalysisFile);
	DYN_ATTRIBUTE(4,public,Filename,InputAnalysisFile);
/** Analysis **/
	DYN_ATTRIBUTE(5,public,TSize,AnalysisWindowSize);
	DYN_ATTRIBUTE(6,public,TSize,AnalysisHopSize);
	DYN_ATTRIBUTE(7,public,EWindowType, AnalysisWindowType);
	DYN_ATTRIBUTE(8,public,TSize,ResAnalysisWindowSize);
	DYN_ATTRIBUTE(9,public,EWindowType, ResAnalysisWindowType);

	DYN_ATTRIBUTE(10,public,TSize,AnalysisZeroPaddingFactor);
	DYN_ATTRIBUTE(11,public,TData,AnalysisPeakDetectMagThreshold);
	DYN_ATTRIBUTE(12,public,TSize,AnalysisPeakDetectMaxFreq);
	DYN_ATTRIBUTE(13,public,TData,AnalysisSinTrackingFreqDeviation);
	DYN_ATTRIBUTE(14,public,TData,AnalysisReferenceFundFreq); 
	DYN_ATTRIBUTE(15,public,TData,AnalysisLowestFundFreq); 
	DYN_ATTRIBUTE(16,public,TData,AnalysisHighestFundFreq); 
		
	DYN_ATTRIBUTE(17,public,bool, AnalysisHarmonic);
	DYN_ATTRIBUTE(18,public,bool, DoCleanTracks);
	DYN_ATTRIBUTE(19,public,TSize, CleanTracksMaxDropOut );
	DYN_ATTRIBUTE(20,public,TSize, CleanTracksMinLength );
/** Synthesis **/
	DYN_ATTRIBUTE(21,public,TSize,SynthesisFrameSize);
	DYN_ATTRIBUTE(22,public,EWindowType,SynthesisWindowType);
	
/** File To Morph **/
	DYN_ATTRIBUTE(23,public, Filename, MorphSoundFile);

private:

	void DefaultInit();
	void DefaultValues();

	/** Ad-hoc routine for finding FFT Size*/
	TInt32 PowerOfTwo(TInt32 size);
public:
	~SMSAnalysisSynthesisConfig(){};
};

};//namespace

#endif //_SMSAnalysisSynthesisConfig_

