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

#ifndef _OutControl_
#define _OutControl_

#include "InControl.hxx" // TControlData defined there.
#include <list>
#include <string>

namespace CLAM {

class OutControl : public ControlLinker
{
//Attributes
private:
	std::list<InControl*> mLinks;
	std::string mName;
//Constructor/Destructor
public:
	~OutControl();
	/**
	* Constructor of the OutControl.
	* @param publish This flag (true by default) concerns the publication of the 
	* control pointer at the base processing object.
	* <b>Important:</b> notice that if the out control object is going to suffer a copy
	* (i.e.creating controls that will be copied inside an STL container)
	* the original pointer published will be no longer meaningfull . For avoiding this
	* case we recommend using the flag publish=true. And invoque the PublishInControl
	* method of the processing object, once the copy is made.
	* @parent The processing object that owns the control object. The one where
	* to publish the control if it is the case (publish flag set)
	*/

	OutControl(std::string name, Processing* parent, const bool publish=true);

	
//Methods
public:
	void AddLink(InControl* in);
	void RemoveLink(InControl* in);

	int SendControl(TControlData val);
	/**
	 *  See comments on InControl.hxx about InControl::GetLastValueAsBoolean
	 */
	inline int SendControlAsBoolean( bool booleanValue )
	{
		return SendControl( booleanValue ? 1 : -1 );
	}

//Redefined Methods
	OutControlIterator GetOutControls() const;
	InControlIterator GetInControls() const;

	const std::string& GetName(void) const { return mName; }
};


/////////////////////////////////////////////////////////////////////////////////////////
//  Control Arrays
//


class OutControlArray
{
	Array<OutControl*> mArray;
public:
    inline OutControlArray() {};
	inline OutControlArray(int size, const std::string &name, Processing* whereToPublish=0);
	inline void Configure(int size, const std::string &name, Processing* whereToPublish=0);
	inline ~OutControlArray();

	inline OutControl       &operator[](int i)        { return *mArray[i]; }
	inline const OutControl &operator[](int i) const  { return *mArray[i]; }

	inline const int Size() {return mArray.Size();}
};


OutControlArray::OutControlArray(int size,
								 const std::string &name,
								 Processing *wtp)
{
	Configure(size,name,wtp);
}

void OutControlArray::Configure(int size,
								const std::string &name,
								Processing *wtp)
{
	CLAM_ASSERT(mArray.Size() == 0,
				"OutControlArray::Configure(): Called twice.");
	mArray.Resize(size);
	mArray.SetSize(size);
	for (int i=0; i<size; i++) {
		std::stringstream str(name);
		str << "_" << i;
		mArray[i] = new OutControl(str.str(),wtp);
	}
}

OutControlArray::~OutControlArray()
{
	int size = mArray.Size();
	for (int i=0; i<size; i++)
		delete mArray[i];
}



}; // namespace CLAM

#endif //_OutControl_
