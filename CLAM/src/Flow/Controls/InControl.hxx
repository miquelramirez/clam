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

namespace CLAM {

// Forward Declaration:
class Processing;
class OutControl;

typedef float TControlData;

/**
* \brief Processing in control class.
* 
*/
class InControl
{
// Attributes:
protected:
	std::list<OutControl*> mLinks;
	TControlData mLastValue;
	std::string mName;
	Processing * mParent;
	TControlData mDefaultValue;
	TControlData mUpperBound;
	TControlData mLowerBound;

	bool mBounded;
	bool mHasDefaultValue;

// Methods:
public:
	/**
	 * Stores the incoming control value. It can be retrieved
	 * using \c GetLastValue
	 */
	virtual int DoControl(TControlData val) { mLastValue = val; return 0; };
	virtual const TControlData& GetLastValue() const { return mLastValue; };
	const std::string& GetName() const { return mName; }
	bool IsConnectedTo( OutControl & );
	bool IsConnected() const;
	bool IsBounded() const;
	TControlData UpperBound() const;
	TControlData LowerBound() const;
	/** Returns the bounds mean or the value set with SetDefaultValue() if its the case */
	TControlData DefaultValue() const;
	void SetDefaultValue(TControlData val);
	void SetBounds(TControlData lower, TControlData upper);

	Processing * GetProcessing() const { return mParent; }
	
	/// Implementation detail just to be used from OutControl
	void OutControlInterface_AddLink(OutControl & outControl);
	/// Implementation detail just to be used from OutControl
	void OutControlInterface_RemoveLink(OutControl & outControl);

//Constructor/Destructor
	/**
	 * \todo constructor rework. 
	 * \argument \c parent Optional. If present, is the processing where to be published.
	 */
	InControl(const std::string &name, Processing* parent=0, const bool publish=true);
	virtual ~InControl();
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
	ProcObj* mProcObj;
	int mId;

public:
	// redeclaration
	int DoControl(TControlData val);

	bool ExistMemberFunc() { return (mFunc==0); };
	void SetMemberFunc(TPtrMemberFunc f) { mFunc = f; };

	int GetId(void) const { return mId; }

//Constructor/Destructor

	/**
	* Constructor of the InControlTmpl with a member-service-function associated. 
	* @param publish This flag (true by default) concerns the publication of the 
	* control pointer at the base processing object.
	* <b>Important:</b> notice that if the out control object is going to suffer a copy
	* (i.e.creating controls that will be copied inside an STL container)
	* the original pointer published will be no longer . For avoiding this
	* case we recommend using the flag publish=true. And invoke the PublishInControl
	* method of the processing object, once the copy is made.
	* @param f The member function that will act as a service funtion each time
	* the DoControl method is invoqued.
	* @parent The processing object that owns the control object. The one where
	* to publish the control if it is the case (publish flag set)
	*/
	InControlTmpl(const std::string &name, ProcObj* parent, TPtrMemberFunc f = 0,const bool publish=true )	:
		InControl(name,parent,publish),
		mFunc(f),
		mFuncId(0),
		mProcObj(parent)

		{
//			if (publish) mProcObj->PublishInControl(this);
		};

	InControlTmpl(int id,const std::string &name, ProcObj* parent, TPtrMemberFuncId f,const bool publish=true )	:
		InControl(name,parent,publish),
		mFunc(0),
		mFuncId(f),
		mProcObj(parent),
		mId(id)
		{
//			if (publish && mProcObj) mProcObj->PublishInControl(this);
		};

	~InControlTmpl(){};
	
};


/////////////////////////////////////////////////////////////////////////////////////////
//  Implementation of class InControlTmpl
//

template<class ProcObj>
int InControlTmpl<ProcObj>::DoControl(TControlData val)
{
	InControl::DoControl(val);
	if(mFunc)
		return (mProcObj->*mFunc)(val);
	else if (mFuncId)
		return (mProcObj->*mFuncId)(mId,val);
	else
		return 0;
}



} // namespace CLAM

#endif //_InControl_

