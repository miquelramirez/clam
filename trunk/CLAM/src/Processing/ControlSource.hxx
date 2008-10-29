/*
 * Copyright (c) 2001-2005 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _EXTERN_IN_CONTROL_
#define _EXTERN_IN_CONTROL_

#include "Processing.hxx"
#include "OutControl.hxx"

namespace CLAM{
	
	class ControlSourceConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE ( ControlSourceConfig, 4, ProcessingConfig);
		DYN_ATTRIBUTE(0,public,TData, MinValue);
		DYN_ATTRIBUTE(1,public,TData, MaxValue);
		DYN_ATTRIBUTE(2,public,TData, Step);
		DYN_ATTRIBUTE(3,public, std::string, UnitName);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetMinValue(0.0);
			SetMaxValue(100.0);
			SetStep(1.0);
			SetUnitName("-");
		}
	};

	class ControlSource : public Processing
	{
	private:
		ControlSourceConfig mConf;
		FloatOutControl mOutput;
		
	public:
		ControlSource() 
		: mOutput("output",this)
		{
		}
		
		ControlSource(const ControlSourceConfig & c)
		: mOutput("output",this)
		{
			ConcreteConfigure(c);
		}
		
		~ControlSource() {}
	
		bool Do()
		{
			return true;
		}
		
		bool Do( const float value );
		
		const char* GetClassName() const { return "ControlSource";}
		
		bool ConcreteConfigure(const ProcessingConfig &c);
		
		const ProcessingConfig& GetConfig() const
		{
			return mConf;
		}
	};
} //namespace CLAM

#endif

