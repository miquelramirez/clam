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

#ifndef _ControlSelector_
#define _ControlSelector_

#include "DataTypes.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InControl.hxx"
#include <vector>
#include <iostream>

namespace CLAM
{

class ControlSelectorConfig : public ProcessingConfig
{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (ControlSelectorConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, unsigned, NumberOfInputs);
		DYN_ATTRIBUTE (1, public, unsigned, DefaultIndex);
	private:
		void DefaultInit();
};

class ControlSelector : public Processing
{
	ControlSelectorConfig _config;
	FloatInControl _indexControl;
	std::vector<FloatInControl *> _inControls;
	FloatOutControl _outputControl;

	public:
		const char *GetClassName() const { return "ControlSelector"; }
		ControlSelector();
		ControlSelector( const ControlSelectorConfig& cfg ); 
		~ControlSelector();
		bool ConcreteConfigure( const ProcessingConfig& cfg ); 
		const ProcessingConfig& GetConfig() const { return _config; }
		bool Do();
		void InControlCallback(unsigned controlId, const TControlData & value);
	protected:
		void RemoveOldControls();
		void CreateInControls(unsigned numberOfControls);
};

}

#endif

