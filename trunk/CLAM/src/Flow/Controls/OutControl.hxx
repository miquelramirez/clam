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

#include "InControl.hxx"
#include <list>
#include <string>
#include "Assert.hxx"
#include "OutControlBase.hxx"

namespace CLAM
{

class Processing;

//free method to link two Processing
//	void LinkOutWithInControl(Processing* outProc, std::string outControl, 
//				  Processing* inProc, std::string inControl);

/**
* \brief Processing out control class.
*
* Controls are limited to emit and receive TControlData (float) numbers. 
* Though extensible typed connections are future planned development: @see TypedOutControl
*/
class OutControl : public OutControlBase
{
//Constructor/Destructor
public:
	/**
	* Constructor of the OutControl.
	* \param name The name of the control 
	* \param parent Optional. The processing object that owns the control object.
	*/
	OutControl(const std::string& name, Processing* parent=0);	
	
	
//Methods
public:
	virtual bool IsLinkable(const InControlBase& in);
	virtual const std::type_info & GetTypeId() const 
	{
		return typeid(TControlData);
	};
	void SendControl(TControlData val);
	
	/**
	* \brief Sends a 0.0 or 1.0 float control depending on the parameter.
	*
	* To be used in conjunction with InControl::GetLastValueAsBoolean
	*/
	void SendControlAsBoolean( bool booleanValue )
	{
		SendControl( booleanValue ? TControlData(1) : TControlData(0) );
	}

	/**
	* \brief Sends the given int value as a control (float)
	*
	* To be used in conjunction with InControl::GetLastValueAsBoolean
	*/
	void SendControlAsInteger( int intvalue )
	{
		SendControl( (TControlData)intvalue );
	}

};

//REFACTORING Typed Controls
typedef OutControl FloatOutControl;

}; // namespace CLAM

#endif //_OutControl_

