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


#ifndef _VowelResonator_
#define _VowelResonator_

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/SpectralPeakArray.hxx>
#include <CLAM/NullProcessingConfig.hxx>

namespace CLAM{
	
	/** 
	 * Reshapes a set of harmonic peaks using the two formants of
	 * F1 and F2 a vowel.
	 */
	class VowelResonator: public Processing
	{
		InPort<SpectralPeakArray> mIn;
		OutPort<SpectralPeakArray> mOut;
		InControl mF1;
		InControl mF2;
		InControl mF3;
	public:
		typedef NullProcessingConfig Config;
		const char* GetClassName() const { return "VowelResonator"; }
		VowelResonator() 
			: mIn("In SpectralPeaks", this)
			, mOut("Out SpectralPeaks", this) 
			, mF1("F1", this)
			, mF2("F2", this)
			, mF3("F3", this)
		{
			Configure( Config() );
			mF1.DoControl(700);
			mF2.DoControl(1200);
			mF3.DoControl(2500);
		}

 		~VowelResonator() {}

		bool ConcreteConfigure(const CLAM::ProcessingConfig&)
		{
			return true;
		}
		const ProcessingConfig& GetConfig() const
		{
			static Config config;
			return config;
		}
		
		bool Do(const SpectralPeakArray& in, SpectralPeakArray& out);

		bool Do()
		{
			bool result = Do(mIn.GetData(), mOut.GetData()); 
			mIn.Consume();
			mOut.Produce();
			return result;
		}
	
	};		
};//namespace CLAM

#endif // _VowelResonator_

