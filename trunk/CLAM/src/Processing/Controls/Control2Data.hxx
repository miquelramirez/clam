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

#ifndef _Control2Data_
#define _Control2Data_

#include "Processing.hxx"
#include "ProcessingData.hxx"
#include "InControlTmplArray.hxx"

#include <vector>
#include <queue>

#include "Mutex.hxx"

namespace CLAM {

//////////////////////////////////////////////////////////////////////////////////////////
/**
* Configuration for the Control2Data PO. It holds a name (as usual), the number of
* controls, and two optional vectors: min and max values for the controls. That is the rang
* of floats that can be generated for each control.
*/
class Control2DataConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (Control2DataConfig, 3, ProcessingConfig);
	DYN_ATTRIBUTE          (0, public, unsigned, NumControls);
	DYN_CONTAINER_ATTRIBUTE (1, public, std::vector<TControlData>, MinValues, foo);
	DYN_CONTAINER_ATTRIBUTE (2, public, std::vector<TControlData>, MaxValues, foo);
protected:
	/** Dynamic type initialization */
	void DefaultInit(void);
	
};
//////////////////////////////////////////////////////////////////////////////////////////
/**
* This processing object is mainly an aggregate of input control data
* It works by asynchronously receiving messages, keeping them enqueued and synchronously 
* converting them to a Processing Data, at Do() time. It is an abstract class because the 
* responsability for the concrete processing of controls and conversion to a particular
* processing data is deferred to subclasses.
* <p>
* Note that if mulithreading is enabled, the EnqueueControl calls can be done from different
* threads than the execution method. A lock for the mDataQueue structure ensures it.
*/
class Control2Data : public Processing
{
public:
	Control2Data ();	
	Control2Data(const Control2DataConfig& c);
	virtual ~Control2Data (){};
	/** Already implemented Do operation. This operation is in charge of reading the enqueued
	 *	events and producing the processing data output. But for doing so it relies on the 
	 *	particular implementation of the GenerateOutputData operation.
	 */
	bool Do();
	/** Returns internal configuration method */
	const ProcessingConfig&  GetConfig() const;	
protected:

	//typedefs

	/** typedef used for convenience */
	typedef std::queue<TControlData> TQueue;
	/** typedef used for convenience */
	typedef std::list<int> IdxList;

	//protected methods
	
	/** This pure virtual operation should be implemented in the derived classes. It is 
	 *	responsible for implementing the concrete behaviour of how to covert the controls
	 *	into a particular processing data
	 */
	virtual bool GenerateOutputData(int id, TControlData val)=0;

	/** Initialization for queues of control events
	 *	@param ncontrols is the number of controls that the converter has.
	 */
	void BufferQueueInit( int ncontrols );
	/** Add new control event to internal queue.
	 *	@param id is the identifier of the control to which the event is sent
	 *	@param data is teh value of the event
	 */
	void EnqueueControl(unsigned id, TControlData data);
	/** Read and remove last control event for a given control id.*/
	TControlData PopControl(unsigned id);
	/** Whether the queue for a particular event is empty or not.*/
	bool Empty(unsigned id);
	/** Returns list of controls that are being enqueued*/
	IdxList GetQueues();

	/** Callback that will be called when a particular control receives an event and that
	 *	will be in charge of enqueing the event.
	 *	@param id is the identifier of the control
	 *	@val is the value of the control event
	 */
	int ControlCallbackId(int id, TControlData val );

	/** Concrete configuration for particular Processing */
	virtual bool ConcreteConfigure(const ProcessingConfig&);
	/** Compulsory method for Processing derived classes */
	const char *GetClassName() const {return "Control2Data";}

protected:
//protected member attributes

	/** Array of input control arrays that is initialized in the ConcreteConfigure method. 
	 *	Number of controls is indicated in the configuration
	 */
	InControlTmplArray<Control2Data>* mpInArray;
	
	/** Special input control to be able to stop the process in response to an external
	 *	control event.
	 */
	FloatInControl mStop;
	
	/** Internal configuration */
	Control2DataConfig mConfig;
	
	/** Vector of queues containing events for each control*/
	std::vector<TQueue> mDataQueues;
	
	/** Mutex object for ensuring multithread capabilities */
	Mutex mDataMutex;
	/** Mutex object for ensuring multithread capabilities */
	Mutex mControl2DataDoMutex;
};
//////////////////////////////////////////////////////////////////////////////////////////
}; // namespace CLAM

#endif // _Control2Data_

