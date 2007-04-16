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

#ifndef _OutControl_
#define _OutControl_

#include "InControl.hxx" // TControlData defined there.
#include <list>
#include <string>
#include "Assert.hxx"

namespace CLAM {

class Processing;

//free method to link two Processing
//	void LinkOutWithInControl(Processing* outProc, std::string outControl, 
//				  Processing* inProc, std::string inControl);

class OutControl
{
//Attributes
private:
	std::list<InControl*> mLinks;
	std::string mName;
	Processing * mParent;
//Constructor/Destructor
public:
	virtual ~OutControl();
	/**
	* Constructor of the OutControl.
	* 
	* \warning notice that if the out control object is going to suffer a copy
	* (i.e.creating controls that will be copied inside an STL container)
	* the original pointer published will be no longer meaningfull . For avoiding this
	* case we recommend using the flag publish=false. And invoque the PublishInControl
	* method of the processing object, once the copy is made.
	* \param publish This flag (true by default) concerns the publication of the 
	* control pointer at the base processing object.
	* \param parent Optional. The processing object that owns the control object. The one where
	* to publish the control if it is the case (publish flag set)
	* \todo improve construction mechanism (params set)
	*/
	OutControl(const std::string& name, Processing* parent=0, const bool publish=true );	
	
	
//Methods
public:
	virtual void AddLink(InControl& in); //TODO think if should be renamed to Connect
	void RemoveLink(InControl& in);

	std::list<InControl*>::iterator BeginInControlsConnected();
	std::list<InControl*>::iterator EndInControlsConnected();

	int SendControl(TControlData val);
	/**
	 *  See comments on InControl.hxx about InControl::GetLastValueAsBoolean
	 */
	inline int SendControlAsBoolean( bool booleanValue )
	{
		return SendControl( booleanValue ? TControlData(1) : TControlData(-1) );
	}

	const std::string& GetName(void) const { return mName; }
	bool IsConnected();
	bool IsConnectedTo( InControl & );
	Processing * GetProcessing() const { return mParent;}
};



}; // namespace CLAM

#endif //_OutControl_

