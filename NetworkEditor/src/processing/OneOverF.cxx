/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "OneOverF.hxx"
#include <CLAM/OSDefines.hxx>
#include <CLAM/CLAM_Math.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include <ctime>
#include <cstdlib>

#define DEFAULTDELAYFACTOR 10


namespace CLAM
{

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, OneOverF> regOneOverF("OneOverF");
}
	

void OneOverFConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
	SetName("OneOverF");
	SetDelayFactor(10);
	SetMinNote(36);
	SetMaxNote(98);
	SetDices(4); // 1..MAXDICES
	srand( (unsigned)time( NULL ) );
}

void OneOverF::ResetSeq()
{
	PrevSeqIndex = SeqLength -1;
	SeqIndex = 0;
	for (int i=0;i<mDices;i++)
	  Values[i] = RandomMax;
}

OneOverF::OneOverF()
	: mInput("In delay factor", this ),
	  mOutput("Out frequency", this ),
	  mMidiToFreq(),
	  mCounter(DEFAULTDELAYFACTOR)
{
	mInput.DoControl(10);
	mMidiToFreq.Set(69.0,440.0);
	OneOverFConfig cfg;
	Configure(cfg);

	RandomMax = (int)((mMaxNote-mMinNote) / mDices);
	SeqLength = (int)pow(2, mDices);
	ResetSeq();
}

OneOverF::OneOverF( const OneOverFConfig & cfg)
	: mInput("In delay factor", this ),
	  mOutput("Out frequency", this ),
	  mMidiToFreq(),
	  mCounter(DEFAULTDELAYFACTOR)
{
	Configure(cfg);
}


bool OneOverF::Do()
{
	if( !AbleToExecute() ) return true;

	TData OutValue = 0.0;

	//static int Counter = (int)mInput.GetLastValue(); // Sends control from the first call
	mCounter++;
	if (mCounter > (int)mInput.GetLastValue())
	{
	  mCounter = 0;
	  RandomMax = (int)((mMaxNote-mMinNote) / mDices);
	  if (RandomMax == 0)
	    RandomMax = 1;
	  for (int i=0;i<mDices;i++) // Bit check loop
	    {
	      if ( ((SeqIndex >> i) && 0x0001) != ((PrevSeqIndex >> i) && 0x0001) )
	        Values[i] = rand() % RandomMax;
	      OutValue += Values[i];
	    }
	  PrevSeqIndex = SeqIndex;
	  SeqIndex++;
	  if (SeqIndex >= SeqLength)
	    ResetSeq();
	  mOutput.SendControl(mMidiToFreq.Map((TData)mMinNote + OutValue));
	}

	return true;
}

bool OneOverF::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	mDelayFactor = mConfig.GetDelayFactor();
	mMinNote = mConfig.GetMinNote();
	mMaxNote = mConfig.GetMaxNote();
	mDices = mConfig.GetDices();
	mInput.DoControl(mDelayFactor);

	return true;
}

} // namespace CLAM




