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

#ifndef _InControl_
#define _InControl_

#include "ControlLinker.hxx"
#include "mtgsstream.h"
#include <string>
#include "Array.hxx"

namespace CLAM {

// Forward Declaration:
class Processing;

typedef float TControlData;

/**
* @todo Document!
*/
class InControl : public ControlLinker
{
// Attributes:
private:
	TControlData mLastValue;
	std::string mName;

// Methods:
public:
	/**
	 *   TODO: document why is implemented here.
	 */
	virtual int DoControl(TControlData val) { mLastValue = val; return 0;};
	TControlData GetLastValue() const { return mLastValue; };
	const std::string& GetName() const { return mName; }

//Redefined Methods
	OutControlIterator GetOutControls() const;  //Iterators types defined at ControlLinker.hxx
	InControlIterator GetInControls() const;
//Constructor/Destructor
	/**
	 * TODO: document when to use one or the other constructor.
	 */
	InControl(const std::string &name, Processing* whereToPublish);
	InControl(const std::string &name);
	virtual ~InControl();
	
};

/**
* @todo Document!
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
	* case we recommend using the flag publish=true. And invoque the PublishInControl
	* method of the processing object, once the copy is made.
	* @param f The member function that will act as a service funtion each time
	* the DoControl method is invoqued.
	* @parent The processing object that owns the control object. The one where
	* to publish the control if it is the case (publish flag set)
	*/
	InControlTmpl(const std::string &name, ProcObj* parent, TPtrMemberFunc f = 0,const bool publish=true )	:
		InControl(name),
		mFunc(f),
		mFuncId(0),
		mProcObj(parent)

		{
			if (publish) mProcObj->PublishInControl(this);
		};

	InControlTmpl(int id,const std::string &name, ProcObj* parent, TPtrMemberFuncId f,const bool publish=true )	:
		InControl(name),
		mFunc(0),
		mFuncId(f),
		mProcObj(parent),
		mId(id)
		{
			if (publish) mProcObj->PublishInControl(this);
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

/////////////////////////////////////////////////////////////////////////////////////////
//  Control Arrays
//


class InControlArray
{
	Array<InControl*> mArray;
public:

	inline InControlArray(int size, const std::string &name, Processing* whereToPublish=0);
	inline ~InControlArray();

	inline InControl       &operator[](int i)        { return *mArray[i]; }
	inline const InControl &operator[](int i) const  { return *mArray[i]; }
};


InControlArray::InControlArray(int size,
                               const std::string &name,
                               Processing *wtp)
{
	mArray.Resize(size);
	mArray.SetSize(size);
	for (int i=0; i<size; i++) {
		std::stringstream str(name);
		str << "_" << i;
		if (wtp)
			mArray[i] = new InControl(str.str(),wtp);
		else
			mArray[i] = new InControl(str.str());
	}
}

InControlArray::~InControlArray()
{
	int size = mArray.Size();
	for (int i=0; i<size; i++)
		delete mArray[i];
}

//////////////////////////////////////////////////
// 
template <class Processing>
class InControlTmplArray
{
	typedef typename InControlTmpl<Processing> TInControl;
	typedef typename TInControl::TPtrMemberFuncId TPtrMemberFuncId;

	Array<TInControl*> mArray;

public:
	InControlTmplArray(int size, const std::string &name, Processing* parent,
		TPtrMemberFuncId f, const bool publish=true);
	~InControlTmplArray();

	inline TInControl& operator[](int i) { return *mArray[i]; }
	inline const TInControl& operator[](int i) const { return *mArray[i]; }

};
/////////////////////////////////////////////
// Implementation
template <class Processing>
InControlTmplArray<Processing>::InControlTmplArray(int size, 
								   const std::string &name,
								   Processing *parent, 
								   TPtrMemberFuncId f,
								   const bool publish)
{
	mArray.Resize(size);
	mArray.SetSize(size);
	for (int i=0; i<size; i++) {
		std::stringstream str;
		str << name << "_" << i;
		CLAM_ASSERT(parent, "ArrayControls not being published. TODO: check ctr parameters");
		mArray[i] = new TInControl(i, str.str(), parent, f, publish);
		
	}
}
template <class Processing>
InControlTmplArray<Processing>::~InControlTmplArray()
{
	int size = mArray.Size();
	for (int i=0; i<size; i++)
		delete mArray[i];
}



}; // namespace CLAM

#endif //_InControl_
