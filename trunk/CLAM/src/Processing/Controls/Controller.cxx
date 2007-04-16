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

#include "Controller.hxx"
#include "Factory.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM {

	namespace detail
	{
		static ProcessingFactory::Registrator<Controller> regtController( "Controller" );
	}
	
Controller::Controller()
{

}


bool Controller::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);
	int n = mConfig.GetNumControls();

	CLAM_ASSERT( !OutControls.Size() ||
				 n == OutControls.Size(),
				 "Controller::ConcreteConfigure(): Size can not change.");

	if (!OutControls.Size())
		OutControls.Resize(n,"Output",this);

	OutValues.resize(n);

	for (int k = 0; k < n ; k++ )
	{
		OutValues[k]=0.0;
	}

	// Buffer Queues initialization

	BufferQueueInit( n );

	return true;

}

void Controller::BufferQueueInit( int ncontrols )
{
	Mutex::ScopedLock lock( mDataMutex );

	mDataQueues.resize(0);
	mDataQueues.reserve(ncontrols);
	for (int j = 0; j < ncontrols  ;j ++ )
	{
		mDataQueues.push_back( TQueue() );
	}

}

const ProcessingConfig&  Controller::GetConfig() const 
{ 
	return mConfig; 
}


void Controller::EnqueueControl(unsigned id, TControlData data)
{
	Mutex::ScopedLock lock( mDataMutex );
	
#ifdef HAVE_STANDARD_VECTOR_AT
	mDataQueues.at(id).push(data);
#else
	mDataQueues[id].push(data);
#endif

}

TControlData Controller::LastDequeuedValue(unsigned id)
{
	TControlData val;

	Mutex::ScopedLock lock( mDataMutex );

#ifdef HAVE_STANDARD_VECTOR_AT
	val = OutValues.at(id);
#else
	val = OutValues[id];
#endif

	return val;
}

bool Controller::Empty(unsigned id)
{
	Mutex::ScopedLock lock( mDataMutex );
	
#ifdef HAVE_STANDARD_VECTOR_AT
	return mDataQueues.at(id).empty();
#else
	return mDataQueues[id].empty();
#endif

}

TControlData Controller::PopControl(unsigned id)
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



}; //namespace CLAM

