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

#ifndef _SegmentAnalysisSystem_
#define _SegmentAnalysisSystem_

#include "AudioFileIn.hxx"
#include "FFT_rfftw.hxx"
#include "PeakDetect.hxx"
#include "FundFreqDetect.hxx"
#include "ProcessingComposite.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "DataTypes.hxx"

namespace CLAM{


/**
* Flags that are used to decide which Snapshots will be shown
*/
class SegmentAnalysisSnapshotsFlags : public Flags<2> {
public:
	static tFlagValue sFlagValues[];
	virtual CLAM::Component * Species() const {
		return new SegmentAnalysisSnapshotsFlags();
	}
	typedef enum {
		eShowAudioFrame=0,
		eShowFrameSpectrum=1
	} tFlag;
	SegmentAnalysisSnapshotsFlags () :
		CLAM::Flags<2>(sFlagValues),
		bShowAudioFrame(operator[](eShowAudioFrame)),
		bShowFrameSpectrum(operator[](eShowFrameSpectrum))
	{};
	template <class T> SegmentAnalysisSnapshotsFlags(const T &t) : 
		CLAM::Flags<2>(sFlagValues,t),
		bShowAudioFrame(operator[](eShowAudioFrame)),
		bShowFrameSpectrum(operator[](eShowFrameSpectrum))
	{};
	template <class T1, class T2> SegmentAnalysisSnapshotsFlags(const T1 &t1, const T2 &t2) : 
		CLAM::Flags<2>(sFlagValues,t1,t2),
		bShowAudioFrame(operator[](eShowAudioFrame)),
		bShowFrameSpectrum(operator[](eShowFrameSpectrum))
	{};

	reference bShowAudioFrame;
	reference bShowFrameSpectrum;

};


class SegmentAnalysisSystemConfig:public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE(SegmentAnalysisSystemConfig,4,ProcessingConfig);
	DYN_ATTRIBUTE (0,public, std::string, Name);
	DYN_ATTRIBUTE (1,public, int, AudioFrameSize);
	DYN_ATTRIBUTE (2,public, TData, SamplingRate);
	DYN_ATTRIBUTE (3,public, SegmentAnalysisSnapshotsFlags, DisplayFlags);
	
	void Init();
};


/** Class that encapsulates all the building blocks of the SegmentAnalysis example */
class SegmentAnalysisSystem:public ProcessingComposite
{

public:
	
	SegmentAnalysisSystem(SegmentAnalysisSystemConfig& config);
	SegmentAnalysisSystem();
			
	const ProcessingConfig &GetConfig() const {return mConfig;}

	/** Supervised mode execution */
	bool Do(void){return false;}

	/** Unsupervised mode execution */
	bool Do(Segment& in,int frameIndex);

private:

	// Configuration data
	SegmentAnalysisSystemConfig mConfig;

	// The internal Processing Objects
	FFT_rfftw			mFFT;
	PeakDetect			mPeakDetect;
	FundFreqDetect		mFundDetect;
	

	//Internal DataObjects
	Audio mAudioFrame; // smaller chunk of audio that will be processed thru the FFT
	Spectrum mSpec;
	Fundamental mFund;
	Frame mFrame;

	//Internal GUI object
	//GUIObject*			mGUI;


	// Internal convenience methods.

	void AttachChildren();
	bool ConfigureChildren();
	void ConfigureData();

	// Processing Object compliance methods.

	const char *GetClassName() {return "SegmentAnalysisSystem";}

	/** Configuration method */
	bool ConcreteConfigure(const ProcessingConfig&);

	//Ad-hoc conversion routine
	void Lin2dB(Spectrum& spec);
	

};

};//namespace

#endif
