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

#include "AutoPanner.hxx"
#include "OSDefines.hxx"
#include "CLAM_Math.hxx"
#include "ProcessingFactory.hxx"



namespace CLAM
{

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, AutoPanner> regAutoPanner("AutoPanner");
}

void AutoPannerConfig::DefaultInit(void)
{
	AddAll();       
	UpdateData();	
	SetFrequency(440.0);
	SetPhase(0.0);
	SetSamplingRate( 44100 );
	SetFrameSize( 512 );
}

AutoPanner::AutoPanner()
	: mLeft("Left Control", this ),
	  mRight("Right Control", this )
{
	AutoPannerConfig cfg;

	Configure(cfg);
}

AutoPanner::AutoPanner( const AutoPannerConfig & cfg)
	: mLeft("Left Control", this ),
	  mRight("Right Control", this )
{

	Configure(cfg);
}


bool AutoPanner::Do()
{	
	if( !AbleToExecute() ) return true;

	CLAM::TData newValue = sin(mPhase);
	mPhase += mDeltaPhase;
	if (mPhase > (2*M_PI))
	{
		mPhase = fmod(mPhase,TData(2*M_PI));
	}

	CLAM::TData firstValue = fabs(newValue);
	CLAM::TData secondValue = 1 - fabs(newValue);

	mLeft.SendControl(firstValue);
	mRight.SendControl(secondValue);
       	return true;
}

bool AutoPanner::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	mFreq = mConfig.GetFrequency();
	mSamplingRate = mConfig.GetSamplingRate();
	mPhase = mConfig.GetPhase();
	mFrameSize = mConfig.GetFrameSize();
	mDeltaPhase = ((2* TData(M_PI) *mFreq)/mSamplingRate)* mFrameSize;

	return true;
}

} // namespace CLAM

