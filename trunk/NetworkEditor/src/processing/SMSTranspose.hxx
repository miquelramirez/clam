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


#ifndef _SMSTranspose_
#define _SMSTranspose_

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/SpectralPeakArray.hxx>
#include <CLAM/Frame.hxx>
#include <CLAM/SegmentTransformationConfig.hxx>
#include <CLAM/FrameTransformation.hxx>
#include <CLAM/FrameTransformationConfig.hxx>

namespace CLAM{

	class SMSTranspose: public FrameTransformation
	{
		InPort<SpectralPeakArray> mIn;
		OutPort<SpectralPeakArray> mOut;

		InControl mControl;
	public:
		const char* GetClassName() const
		{
			return "SMSTranspose";
		}

		SMSTranspose() 
			: 
			mIn("In SpectralPeaks", this),
			mOut("Out SpectralPeaks", this),
			mControl("Transpose amount", this)
		{
			Configure( FrameTransformationConfig() );
		}

 		~SMSTranspose() {}

		bool ConcreteConfigure( const ProcessingConfig& config )
		{
			mControl.SetBounds(-36,36);
			mControl.DoControl(0);
			return true;
		}
		
		bool Do(const Frame& in, Frame& out)
		{
			return Do(in.GetSpectralPeakArray(), 
				  out.GetSpectralPeakArray());
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

#endif // _SMSTranspose_

