/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _Fundamental2Control_
#define _Fundamental2Control_

#include "DataTypes.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InPort.hxx"
#include "OutControl.hxx"
#include "Fundamental.hxx"

namespace CLAM
{

class Fundamental2ControlConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (Fundamental2ControlConfig, 0, ProcessingConfig);
	
//	DYN_ATTRIBUTE (0, public, int , Value);
};

class Fundamental2Control : public Processing
{

	InPort< Fundamental > mInFund;
	FloatOutControl mFreqControlOut;


	Fundamental2ControlConfig mConfig;

	public:

		const char *GetClassName() const { return "Fundamental2Control"; }


		Fundamental2Control()
		:
		mInFund("In Fundamental", this),
		mFreqControlOut("Control Out", this)
		{
		Configure( mConfig );	
		}

		Fundamental2Control( const Fundamental2ControlConfig& cfg ) 
		:
		mInFund("In Fundamental", this),
		mFreqControlOut("Control Out", this)
		{ 
			Configure( cfg );
		}

		bool ConcreteConfigure( const ProcessingConfig& cfg ) { return true; }

		const ProcessingConfig& GetConfig() const { return mConfig; }

		bool Do()
		{
			bool result = Do( mInFund.GetData() );
			mInFund.Consume();
			return result;
		}

		bool Do( const Fundamental& inFund );
};

}

#endif

