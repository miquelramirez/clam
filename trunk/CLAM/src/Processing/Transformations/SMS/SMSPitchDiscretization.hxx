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


#ifndef _SMSPitchDiscretization_
#define _SMSPitchDiscretization_

#include "SMSTransformation.hxx"
#include "SMSPitchShift.hxx"
#include "SpectrumAdder2.hxx"


namespace CLAM{


	class SMSPitchDiscretization: public SegmentTransformation
	{
		
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SMSPitchDiscretization";}


	public:
		/** Base constructor of class. Calls Configure method with a SegmentTransformationConfig initialised by default*/
		SMSPitchDiscretization()
		{
		}
		/** Constructor with an object of SegmentTransformationConfig class by parameter
		 *  @param c SegmentTransformationConfig object created by the user
		*/
		SMSPitchDiscretization(const SegmentTransformationConfig &c):SegmentTransformation(c)
		{
		}

		virtual bool ConcreteConfigure(const ProcessingConfig& c)
		{
			SegmentTransformation::ConcreteConfigure(c);
			//configure member PitchShift by default
			mPO_PitchShift.Configure(SegmentTransformationConfig());
			return true;
		}

		/** Destructor of the class*/
 		~SMSPitchDiscretization()
		{}

		bool Do()
		{
			CLAM_ASSERT(false, "Do with ports not implemented");
			return false;
		}
		bool Do(const Frame& in, Frame& out);
	private:
		SMSPitchShift mPO_PitchShift;
		
	};		
};//namespace CLAM

#endif // _SMSPitchDiscretization_

