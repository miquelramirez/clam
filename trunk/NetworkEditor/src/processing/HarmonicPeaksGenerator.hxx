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


#ifndef _HarmonicPeaksGenerator_
#define _HarmonicPeaksGenerator_

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/SpectralPeakArray.hxx>
#include <CLAM/NullProcessingConfig.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/Frame.hxx>

namespace CLAM{

	class HarmonicPeaksGenerator: public Processing
	{
		OutPort<SpectralPeakArray> mOut;
		InControl mFundamental;
		TData mTime;

	public:
		typedef NullProcessingConfig Config;
		const char* GetClassName() const
		{
			return "HarmonicPeaksGenerator";
		}

		HarmonicPeaksGenerator() 
			: mOut("Out SpectralPeaks", this) 
			, mFundamental("Fundamental", this)
		{
			Configure( Config() );
		}

 		~HarmonicPeaksGenerator() {}
		virtual bool ConcreteConfigure(const CLAM::ProcessingConfig&)
		{
			return true;
		}
		const ProcessingConfig& GetConfig() const
		{
			static NullProcessingConfig config;
			return config;
		}
		
		bool Do(SpectralPeakArray& out);

		bool Do()
		{
			bool result = Do(mOut.GetData()); 
			mOut.Produce();
			return result;
		}
	
	};		
};//namespace CLAM

#endif // _HarmonicPeaksGenerator_

