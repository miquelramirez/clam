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

#ifndef _ControlMapper_
#define _ControlMapper_

#include "Mapping.hxx"
#include "Array.hxx"
#include "Processing.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include <iosfwd>
#include <string>

namespace CLAM {

	class ControlMapperConfig:public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (ControlMapperConfig, 3,ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, std::string, Mapping);
		DYN_ATTRIBUTE (2, public, DataArray, Arguments);
	protected:
		void DefaultInit(void);		
	};

	class ControlMapper:public Processing
	{
	private:
		ControlMapperConfig mConfig;
		Mapping* mMappingPtr;

		InControlTmpl<ControlMapper> mInput;
		OutControl mOutput;
	public:
		ControlMapper();
		ControlMapper(const ControlMapperConfig& cfg);
		~ControlMapper()
		{
		}
		const char * GetClassName() const {return "ControlMapper";}
		bool ConcreteConfigure(const ProcessingConfig& c);
		int DoMap(TControlData val)
		{
			mOutput.SendControl(TControlData(mMappingPtr->Map(val)));
			return 1;
		}
		bool Do(void) { return true; }
		inline const ProcessingConfig &GetConfig() const { return mConfig;}
	};

	
}

#endif //_ControlMapper_



