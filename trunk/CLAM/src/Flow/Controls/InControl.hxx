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

#ifndef _InControl_
#define _InControl_

#include <sstream>
#include <string>
#include <list>
#include "InControlBase.hxx"

namespace CLAM {

// Forward Declaration:
class Processing;
class OutControl;

/**
* Processing in control class for floats.
* Controls are limited to emmit and receive TControlData (float) numbers. 
*/
class InControl : public InControlBase
{
// Attributes:
protected:
	TControlData mLastValue;

public:
//Constructor/Destructor
	InControl(const std::string &name, Processing* parent=0);
	virtual ~InControl();
	const std::type_info & ControlType() const
	{
		return typeid(TControlData);
	}
// Methods:
public:
	/**
	 * Stores the incoming control value. It can be retrieved
	 * using \c GetLastValue
	 */
	virtual void DoControl(TControlData val) { mLastValue = val; };
	/// Returns the last TControlData (float) received event
	const TControlData& GetLastValue() const { return mLastValue; };
	/// Returns the last TControlData (float) received interpreted as a bool
	bool GetLastValueAsBoolean() const 
	{ 
		return (mLastValue > 0) ? mLastValue>0.01 : mLastValue<-0.01;
	};
	/// Returns the last TControlData (float) received interpireted as an integer
	int GetLastValueAsInteger() const { return (int)(mLastValue+0.5f); };
	/** Returns the bounds mean or the value set with SetDefaultValue() if its the case */
	TControlData DefaultValue() const;
};

/**
* Subclass of InControl that provides the incontrol with a callback method
* The method must be defined inside the parent \c Processing class.
* See the \c InControlTmpl constructors for learn how to provide
* the callback to the \c InControlTmpl
*/
template<class ProcObj>
class InControlTmpl : public InControl
{
public:
	typedef int (ProcObj::*TPtrMemberFunc)(TControlData);
	typedef int (ProcObj::*TPtrMemberFuncId)(int,TControlData);

private:
	TPtrMemberFunc   mFunc;
	TPtrMemberFuncId mFuncId;
	int mId;

public:
	// redeclaration
	void DoControl(TControlData val);

	bool ExistMemberFunc() { return (mFunc==0); };
	void SetMemberFunc(TPtrMemberFunc f) { mFunc = f; };

	int GetId(void) const { return mId; }

//Constructor/Destructor

	/**
	* Constructor of the InControlTmpl with a member-service-function associated. 
	* @param name The name for the control.
	* @param parent The processing object that owns the control object.
	* @param f The member function that will act as a service funtion each time
	* the DoControl method is invoqued.
	*/
	InControlTmpl(const std::string &name, ProcObj* parent, TPtrMemberFunc f = 0)
		: InControl(name,parent)
		, mFunc(f)
		, mFuncId(0)
		{
		};

	/**
	* Constructor of the InControlTmpl with a member-service-function associated
	* and an extra identifier. The identifier can be used for the function to
	* discriminate the control if several controls are served by the same callback.
	* @param id The id number that will be sent to the callback.
	* @param name The name for the control.
	* @param parent The processing object that owns the control object.
	* @param f The member function that will act as a service funtion each time
	* the DoControl method is invoqued.
	*/
	InControlTmpl(int id,const std::string &name, ProcObj* parent, TPtrMemberFuncId f)
		: InControl(name,parent)
		, mFunc(0)
		, mFuncId(f)
		, mId(id)
		{
		};

	~InControlTmpl(){};
	
};


// REFACTORING typed connections
typedef InControl FloatInControl;

/////////////////////////////////////////////////////////////////////////////////////////
//  Implementation of class InControlTmpl
//

template<class ProcObj>
void InControlTmpl<ProcObj>::DoControl(TControlData val)
{
	InControl::DoControl(val);
	ProcObj * processing = (ProcObj *) mProcessing;
	if (mFunc)
		((processing)->*mFunc)(val);
	else if (mFuncId)
		((processing)->*mFuncId)(mId,val);
}



} // namespace CLAM

#endif //_InControl_

