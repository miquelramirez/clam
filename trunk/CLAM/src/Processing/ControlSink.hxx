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

#ifndef _EXTERN_OUT_CONTROL_
#define _EXTERN_OUT_CONTROL_

#include "Processing.hxx"
#include "InControl.hxx"

namespace CLAM{
	
	class ControlSinkConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (ControlSinkConfig,3,ProcessingConfig);
		DYN_ATTRIBUTE(0,public,TData, MinValue);
		DYN_ATTRIBUTE(1,public,TData, MaxValue);
		DYN_ATTRIBUTE(2,public,TData, Step);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetMinValue(0.0);
			SetMaxValue(100.0);
			SetStep(1.0);
		}
	};

	class ControlSink : public Processing
	{
	private:
		ControlSinkConfig mConf;
		FloatInControl mInput;
		
	public:
		ControlSink() 
		: mInput("input",this)
		{
		}
		
		ControlSink(const ControlSinkConfig & c)
		: mInput("input",this)
		{
			ConcreteConfigure(c);
		}
		
		~ControlSink() {}
	
		bool Do();
		float GetControlValue();
		
		const char* GetClassName() const { return "ControlSink";}
		
		bool ConcreteConfigure(const ProcessingConfig &c);
		
		const ProcessingConfig& GetConfig() const
		{
			return mConf;
		}
	};
} //namespace CLAM

#endif

