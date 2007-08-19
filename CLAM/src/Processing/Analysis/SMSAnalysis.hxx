/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __SMSAnalysis_hxx__
#define __SMSAnalysis_hxx__

#include "DataTypes.hxx"
#include "SMSAnalysisConfig.hxx"
#include "AudioOutPort.hxx"
#include "ProcessingComposite.hxx"
#include "SMSAnalysisCore.hxx"
#include "Frame.hxx"


namespace CLAM 
{

class ProcessingConfig;

/** Class that encapsulates all the building blocks of the SegmentAnalysis example */
class SMSAnalysis : public ProcessingComposite
{

public:

// Processing Object compliance methods.

	const char *GetClassName() const {return "SMSAnalysis";}

	SMSAnalysis(SMSAnalysisConfig& config);
	SMSAnalysis();
	~SMSAnalysis();

	const ProcessingConfig &GetConfig() const {return mConfig;}

	/** Supervised mode execution */
	bool Do(void);

	bool Do( Frame & frame );
	bool Do( Segment & segment );

	bool ConcreteStart();
	
private:

/** Configuration data */
	SMSAnalysisConfig mConfig;

	SMSAnalysisCore mCore;

	/** Index that indicates how many audio frames have been processed until now*/
	TIndex mAudioFrameIndex;

/**	This method notifies child processing objets that "this" is their parent*/
	void AttachChildren();
/** Configures child processing using configuration data in mConfig. */
	bool ConfigureChildren();
/** Configures internal data objects and stream buffer. */
	void ConfigureData();

	
	/** Configuration method */
	bool ConcreteConfigure(const ProcessingConfig&);

	/** Internal convenience method used for initializing frames */
	void InitFrame(Frame& in);

	AudioOutPort mAudioProvider;

	Frame tmpFrame;
};

} //namespace CLAM

#endif // __SMSAnalysis_hxx__

