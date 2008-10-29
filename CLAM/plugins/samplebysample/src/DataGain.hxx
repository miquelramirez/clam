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

#ifndef DataGain_hxx
#define DataGain_hxx

#include "SampleDefines.hxx"
#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include "SampleBySampleConfig.hxx"
#include "BinaryDataOp.hxx"

namespace CLAM
{
	class DataGain:public CLAM::Processing
	{
	public:
		DataGain():mGainCtl("Gain",this),mInput("Input",this), mOutput("Output",this){}	
		bool Do();
		const char* GetClassName() const {return "DataGain";}
		bool ConcreteConfigure(const CLAM::ProcessingConfig &){return true;}
		inline const CLAM::ProcessingConfig &GetConfig() const { return mConfig;}
	protected:
		/** todo: maybe we want to have a specific config defining the range of the gain*/
		SampleBySampleConfig mConfig;
	
		InDataPort mInput;
		OutDataPort mOutput;
		
		FloatInControl mGainCtl;
	};
}

#endif 

