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

#ifndef _ControlComparison_
#define _ControlComparison_

#include "DataTypes.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"

namespace CLAM
{

class ControlComparisonConfig : public ProcessingConfig
{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (ControlComparisonConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE ( 0, public, TData, RightTerm);
		DYN_ATTRIBUTE ( 1, public, bool, ConvertOps2IntegersFirst );
	private:
		void DefaultInit();
};

class ControlComparison : public Processing
{
	ControlComparisonConfig mConfig;
	FloatInControl _inOperator1;
	FloatInControl _inOperator2;
	OutControl<bool> _outControlBool;
	FloatOutControl _outControlFloat;
	bool _firstValueReceived;

	void InControlCallback(unsigned controlId, const TControlData & value);

	public:

		const char *GetClassName() const { return "ControlComparison"; }

		ControlComparison();
		ControlComparison( const ControlComparisonConfig& cfg );

		bool ConcreteConfigure( const ProcessingConfig& cfg ); 

		const ProcessingConfig& GetConfig() const { return mConfig; }

		bool Do();
};

}

#endif

