/*
 * Copyright (c) 2009 Fundacio Barcelona Media Universitat Pompeu Fabra.
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

#ifndef _ControlIntervalMapper_
#define _ControlIntervalMapper_

#include "DataTypes.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"

namespace CLAM
{

class ControlIntervalMapperConfig : public ProcessingConfig
{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (ControlIntervalMapperConfig, 4, ProcessingConfig);
		DYN_ATTRIBUTE ( 0, public, float, InputMin);
		DYN_ATTRIBUTE ( 1, public, float, InputMax);
		DYN_ATTRIBUTE ( 2, public, float, OutputMin);
		DYN_ATTRIBUTE ( 3, public, float, OutputMax);

	private:
		void DefaultInit();
};

class ControlIntervalMapper : public Processing
{
	ControlIntervalMapperConfig _config;
	FloatInControl _inControl;
	FloatOutControl _outControl;
	float _min;
	float _max;
	float _newmin;
	float _newmax;

	void InControlCallback(const TControlData & value);

	public:

		const char *GetClassName() const { return "ControlIntervalMapper"; }

		ControlIntervalMapper();
		ControlIntervalMapper( const ControlIntervalMapperConfig& cfg );

		bool ConcreteConfigure( const ProcessingConfig& cfg ); 

		const ProcessingConfig& GetConfig() const { return _config; }

		bool Do();
};

}

#endif

