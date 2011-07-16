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

#ifndef ControlSource_hxx
#define ControlSource_hxx

#include "Processing.hxx"
#include "OutControl.hxx"

#include <vector>

namespace CLAM{
	
	class ControlSourceConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE ( ControlSourceConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE(0,public,TData, MinValue);		
		DYN_ATTRIBUTE(1,public,TData, MaxValue);
		DYN_ATTRIBUTE(2,public,TData, DefaultValue);
		DYN_ATTRIBUTE(3,public,TData, Step);
		DYN_ATTRIBUTE(4,public, std::string, UnitName);
		DYN_ATTRIBUTE(5,public, int, NrOfControls);
	protected:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetMinValue(0.0);
			SetMaxValue(100.0);
			SetDefaultValue(50.0);
			SetStep(0.1);
			SetUnitName("-");
			SetNrOfControls(1);
		}

		void LoadFrom(Storage & storage)
		{
			ProcessingConfig::LoadFrom(storage);
			if (!HasNrOfControls())
			{
				AddNrOfControls();
				UpdateData();
				SetNrOfControls(1);
			}
		}
	};

	class ControlSource : public Processing
	{
	private:
		ControlSourceConfig mConf;

		typedef std::vector<FloatOutControl*> OutControls;
		OutControls _outControls;

		void ResizeControls(unsigned controls);

	public:
		typedef ControlSourceConfig Config;

		ControlSource(const Config & c= Config())
		{
			ConcreteConfigure(c);
		}
		~ControlSource();
		
		bool Do()
		{
			return true;
		}
		
		bool Do( const float value, unsigned index=0);
		
		const char* GetClassName() const { return "ControlSource";}
		
		bool ConcreteConfigure(const ProcessingConfig &c);
		
		const ProcessingConfig& GetConfig() const
		{
			return mConf;
		}
	};
} //namespace CLAM

#endif//ControlSource_hxx

