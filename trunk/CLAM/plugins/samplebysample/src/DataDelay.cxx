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

#include "DataDelay.hxx"
#include <CLAM/FastRounding.hxx>
/////////////////////////////////////////////////////////////////////////////////////////777
// Class implementation


#include <CLAM/Err.hxx>
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{
	
namespace detail
{
	static const char * metadata[] = {
		"key", "DataDelay",
		"category", "Sample by Sample (experimental)",
		"description", "DataDelay",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, DataDelay> regDataDelay("DataDelay");
	static FactoryRegistrator<ProcessingFactory, DataDelay> reg = metadata;
}

/*bool DataDelay::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	mCapacity = mConfig.GetMaxDelay();
	mDelayControl.DoControl(TControlData(mConfig.GetDelay()));
	mGivenDelay = CastDelayControlValue(mDelayControl.GetLastValue());
	mBuffer.resize(mCapacity);
	mWritePointer = 0;
	mReadPointer = -mGivenDelay;
	return true;
}

bool DataDelay::Do(void)
{
	Do(mInput.GetData(), mOutput.GetData());
	mInput.Consume();
	mOutput.Produce();
	return true;
}

unsigned DataDelay::CastDelayControlValue(TControlData readControlValue) {
	if (readControlValue > mCapacity) return mCapacity;
	if (readControlValue < 0) return 0;
	return Chop(readControlValue);
}

bool DataDelay::Do(const TData& in, TData& out)
// implementation using the supervised-mode Do
{
	//test
	//out = in;
	//return true;
	
	mLastDelay = mGivenDelay;
	mGivenDelay = mDelayControl.GetLastValue();
	// If the value is different make the difference efective
	if (mLastDelay != mGivenDelay)
		UpdateBuffersToDelay();
	
	WriteNewData(in);
	out=ReadCurrentData();
	return true;
}

void DataDelay::WriteNewData(TData in)
{
	//float test = 0;
	//std::cout<<"Write Pointer: "<<mWritePointer<<std::endl;
	mBuffer[mWritePointer] = in;
	mWritePointer++;
	if(mWritePointer==mCapacity) mWritePointer=0;
}

TData DataDelay::ReadCurrentData()
{
	if(mReadPointer<0) return 0;
	//std::cout<<"Read Pointer: "<<mReadPointer<<std::endl;
	TData out = mBuffer[mReadPointer];
	mReadPointer++;
	if(mReadPointer==mCapacity) mReadPointer=0;
	return out;
}

// Delay change
void DataDelay::UpdateBuffersToDelay()
{
	mWritePointer =  mReadPointer + CastDelayControlValue(mGivenDelay);
	if(mWritePointer>=mCapacity) mWritePointer-=mCapacity;
	return;
}


*/

};

