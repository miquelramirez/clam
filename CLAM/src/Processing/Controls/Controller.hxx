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

// TokenDelay_example.cxx
// example for the TokenDelay processing object

#ifndef _Controller_
#define _Controller_

#include "OutControl.hxx"
#include "OutControlArray.hxx"
#include "Processing.hxx"
#include "ProcessingData.hxx"

#include <typeinfo> // for bad_cast definition
#include <string>
#include <vector>
#include <queue>

#include "Mutex.hxx"

namespace CLAM {

//////////////////////////////////////////////////////////////////////////////////////////
/**
* Configuration for the Controller PO. It holds a name (as usual), the number of
* controls, and two vectors: min and max values for the controls. That is the rang
* of floats that can be generated (i.e. by a View-slider) for each control.
*/
class ControllerConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (ControllerConfig, 3, ProcessingConfig);
	DYN_ATTRIBUTE          (0, public, unsigned, NumControls);
	DYN_CONTAINER_ATTRIBUTE (1, public, std::vector<TControlData>, MinValues, foo);
	DYN_CONTAINER_ATTRIBUTE (2, public, std::vector<TControlData>, MaxValues, foo);
protected:
	/** 
	* Dynamic type initialization: all attributes are instantiated.
	*/
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetNumControls(0);
	}
};
//////////////////////////////////////////////////////////////////////////////////////////
/**
* This processing object is mainly an aggregate of out controls, that can be connected with
* other POs in controls, as usual. This objecte can be wrapped by a Midi Object or View 
* and generate ControlSend messages.
* It works by asynchronously receiving messages, keeping them enqueued and synchronously 
* dispatching them, at Do() time.
* <p>
* Note that if mulithreading is enabled, the EnqueueControl calls can be done from different
* threads than the execution method. A lock for the mDataQueue structure ensures it.
*/
class Controller : public Processing
{
public:
	Controller ();
	Controller(const ControllerConfig& c)
	{
		Configure(c);
	}

	const char *GetClassName() const {return "Controller";}

	bool Do()
	{	
		IdxList::iterator ListIt;		

		Mutex::ScopedLock lock( mControllerDoMutex );
		
		IdxList qs = getQueues();
		
		if (!qs.empty())
			{
				for (ListIt=qs.begin();ListIt!=qs.end() ;ListIt++ )
					{
						TControlData val = PopControl( (*ListIt) );
						OutControls[(*ListIt)].SendControl(val);
						OutValues[(*ListIt)] = val;
					}
			}
		
		
		return true;
	}
	
	const ProcessingConfig&  GetConfig() const;
	virtual bool ConcreteConfigure(const ProcessingConfig&);
	
	

	/** Interface for the users of the class. As: a sliders view class, a midi class... */
	void EnqueueControl(unsigned id, TControlData data);
	TControlData LastDequeuedValue(unsigned id);

	OutControlArray OutControls;
	std::vector<TControlData> OutValues;
private:
	ControllerConfig mConfig;
	// private methods
	TControlData PopControl(unsigned id);
	bool Empty(unsigned id);
	// implementation details
	typedef std::queue<TControlData> TQueue;
	typedef std::list<int> IdxList;
	
	std::vector<TQueue> mDataQueues;

	void BufferQueueInit( int ncontrols );

	IdxList getQueues()
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
	Mutex mDataMutex;
	Mutex mControllerDoMutex;
};
//////////////////////////////////////////////////////////////////////////////////////////
}; // namespace CLAM

#endif // _Controller_

