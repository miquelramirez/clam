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

#ifndef _ControlScaler_
#define _ControlScaler_

#include "DataTypes.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"

namespace CLAM
{

class ControlScalerConfig : public ProcessingConfig
{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (ControlScalerConfig, 1, ProcessingConfig);
		DYN_ATTRIBUTE ( 0, public, TData, Amount );

	private:
		void DefaultInit();
};

class ControlScaler : public Processing
{
	ControlScalerConfig mConfig;
	FloatInControl mInControl;
	FloatInControl mGainControl;
	FloatOutControl mOutControl;
	void InControlCallback(const TControlData & value);

	public:

		const char *GetClassName() const { return "ControlScaler"; }


		ControlScaler();
		ControlScaler( const ControlScalerConfig& cfg );

		bool ConcreteConfigure( const ProcessingConfig& cfg ); 

		const ProcessingConfig& GetConfig() const { return mConfig; }

		bool Do();
};

}

#endif

