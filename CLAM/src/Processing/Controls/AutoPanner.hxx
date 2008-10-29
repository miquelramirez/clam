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

#ifndef _AutoPanner_hxx_
#define _AutoPanner_hxx_

#include "DataTypes.hxx"
#include "OutControl.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include <string>

namespace CLAM
{

class AutoPannerConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (AutoPannerConfig, 4, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, TData, Frequency);
	DYN_ATTRIBUTE (1, public, TData , SamplingRate);
	DYN_ATTRIBUTE (2, public, TData , Phase);
	DYN_ATTRIBUTE (3, public, int , FrameSize);
protected:
	void DefaultInit(void);
};

class AutoPanner : public Processing
{
private:
	
	TData mFreq;
	TData mSamplingRate;
	TData mPhase;
	TData mDeltaPhase;
	int mFrameSize;

	AutoPannerConfig mConfig;
	FloatOutControl mLeft;
	FloatOutControl mRight;

public:
	AutoPanner();
	AutoPanner( const AutoPannerConfig & );

	bool Do();
	virtual ~AutoPanner(){}
	const char * GetClassName() const {return "AutoPanner";}
	
	inline const ProcessingConfig &GetConfig() const { return mConfig;}
	bool ConcreteConfigure(const ProcessingConfig& c);

};

} //namespace CLAM

#endif

