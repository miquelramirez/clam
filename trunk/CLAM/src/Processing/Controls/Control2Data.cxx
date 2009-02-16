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

#include "Control2Data.hxx"

namespace CLAM {

void Control2DataConfig::DefaultInit(void)
{
	AddNumControls();
	UpdateData();
	SetNumControls(0);
}

Control2Data::Control2Data(const Config& c)
	: mStop("stop",this)
{
	Configure(c);
}

bool Control2Data::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	int nControls=mConfig.GetNumControls();
	
	// Initializing InControlArray
	mInArray.Resize(nControls, "array_control", this, &Control2Data::EnqueueControl);

	// Buffer Queues initialization
	BufferQueueInit( nControls );
	//Initialize mStop to false
	mStop.DoControl(0);

	return true;

}

bool Control2Data::Do()
{			
	IdxList::iterator ListIt;				
	Mutex::ScopedLock lock( mControl2DataDoMutex );
		
	IdxList qs = GetQueues();
	if (!qs.empty())  
	{ 
		for (ListIt=qs.begin();ListIt!=qs.end() ;ListIt++ )
		{					
			TControlData val = PopControl( (*ListIt) );
			GenerateOutputData((*ListIt),val); 
		}	
	}
	return !mStop.GetLastValue();
}

void Control2Data::BufferQueueInit( int ncontrols )
{
	Mutex::ScopedLock lock( mDataMutex );

	mDataQueues.resize(0);
	mDataQueues.reserve(ncontrols);
	for (int j = 0; j < ncontrols  ;j ++ )
	{
		mDataQueues.push_back( TQueue() );
	}

}

const ProcessingConfig&  Control2Data::GetConfig() const 
{ 
	return mConfig; 
}


void Control2Data::EnqueueControl(unsigned id, TControlData data)
{
	Mutex::ScopedLock lock( mDataMutex );
	
#ifdef HAVE_STANDARD_VECTOR_AT
	mDataQueues.at(id).push(data);
#else
	mDataQueues[id].push(data);
#endif

}

Control2Data::IdxList Control2Data::GetQueues()
{
	IdxList modifiedQs;
	std::vector<TQueue>::iterator it;

	int k = 0;
	for (it=mDataQueues.begin(); it != mDataQueues.end() ; it++ )
	{
		if (!(*it).empty())
		{
			modifiedQs.push_back(k);
		}
		k++;
	}
	return modifiedQs;
}

bool Control2Data::Empty(unsigned id)
{
	Mutex::ScopedLock lock( mDataMutex );
	
#ifdef HAVE_STANDARD_VECTOR_AT
	return mDataQueues.at(id).empty();
#else
	return mDataQueues[id].empty();
#endif

}

TControlData Control2Data::PopControl(unsigned id)
{
	Mutex::ScopedLock lock( mDataMutex );
#ifdef HAVE_STANDARD_VECTOR_AT
	TControlData ret=mDataQueues.at(id).front();
#else
	TControlData ret=mDataQueues[id].front();
#endif
	mDataQueues[id].pop();
	return ret;
}

void Control2Data::ControlCallbackId(int id, TControlData val)
{
	EnqueueControl(id,val);
}

}; //namespace CLAM

