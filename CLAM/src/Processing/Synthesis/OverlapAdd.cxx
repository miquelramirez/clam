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

#include "OverlapAdd.hxx"


using namespace CLAM;


//////////////////////////////////////////////////////////////////////
//
// OverlapAddConfig
//
//////////////////////////////////////////////////////////////////////

/* the configuration object has at least to have a name */

void OverlapAddConfig::DefaultInit()
{
	/* the dynamic type takes care if we add an existing attr .. */
	AddHopSize();
	AddFrameSize();
	AddBufferSize();

	/* all attributes are added */
	UpdateData();
	DefaultValues();
}

void OverlapAddConfig::DefaultValues()
{
	/* set default values */
	SetHopSize(256);
	SetFrameSize(256);
	SetBufferSize(768);
}

//////////////////////////////////////////////////////////////////////
//
// OverlapAdd
//
//////////////////////////////////////////////////////////////////////

/* processing object method implementations */

OverlapAdd::OverlapAdd(void)
{
	mStreamBuffer=NULL;
	mReader=NULL;
	mWriter=NULL;
	mAdder=NULL;
	Configure(OverlapAddConfig());
}

OverlapAdd::OverlapAdd(const OverlapAddConfig &c)
{
	mStreamBuffer=NULL;
	mReader=NULL;
	mWriter=NULL;
	mAdder=NULL;
	Configure(c);
}

OverlapAdd::~OverlapAdd()
{
	if (mStreamBuffer) delete mStreamBuffer;
	if(mWriter) delete mWriter;
	if(mAdder) delete mAdder;
	if(mReader) delete mReader;
	
}


/* configure the processing object according to the config object */

bool OverlapAdd::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	int hopSize=mConfig.GetHopSize();
	int frameSize=mConfig.GetFrameSize();

	if (mStreamBuffer) delete mStreamBuffer;
	mStreamBuffer= new StreamBuffer<TData, CircularStreamImpl<TData> >;
	if(mWriter) delete mWriter;
	mWriter=mStreamBuffer->NewWriter(hopSize,frameSize);
	if(mAdder) delete mAdder;
	mAdder=mStreamBuffer->NewAdder(hopSize,frameSize);
	if(mReader) delete mReader;
	mReader=mStreamBuffer->NewReader(hopSize,frameSize, mAdder);
	mStreamBuffer->Configure(hopSize*3);
	
	mStreamBuffer->LeaveAndAdvance(mWriter);
	return true;
}

/* other functions */



/* setting prototypes for faster processing */

bool OverlapAdd::SetPrototypes(){
	return false;
}

bool OverlapAdd::UnsetPrototypes(){
	return false;
}

/* the supervised Do() function */

bool OverlapAdd::Do(void)
{
	CLAM_ASSERT(false,"OverlapAdd::Do(): Supervised mode not implemented");
	return false;
}

bool OverlapAdd::Do(DataArray &in, DataArray &out)
{
	int hopSize=mConfig.GetHopSize();
	DataArray inToAdd,inToWrite;
	
	mStreamBuffer->GetAndActivate(mWriter,inToWrite);
	mStreamBuffer->GetAndActivate(mAdder,inToAdd);

	in.CopyChunk(0,hopSize,inToAdd);
	in.CopyChunk(hopSize,hopSize,inToWrite);

	mStreamBuffer->LeaveAndAdvance(mWriter);
	mStreamBuffer->LeaveAndAdvance(mAdder);

	if(mStreamBuffer->GetAndActivate(mReader,mBuffer))
	{
		mStreamBuffer->LeaveAndAdvance(mReader);
		out=mBuffer;
		return true;
	}
	else
	{
		mStreamBuffer->Leave(mReader);
		return false;
	}

} 

bool OverlapAdd::Do(Audio &in, Audio &out)
{
	return Do(in.GetBuffer(), out.GetBuffer());
}
