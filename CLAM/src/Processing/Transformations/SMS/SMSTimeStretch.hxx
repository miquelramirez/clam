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


#ifndef _SMSTimeStretch_
#define _SMSTimeStretch_

#include "SegmentTransformation.hxx"
#include "FrameInterpolator.hxx"
#include "SMSTimeStretchConfig.hxx"

namespace CLAM{


	
	/** @todo: introduce interpolation of spectral shapes, before that, interpolations may be
	 *	extracted to external Processing's*/
	class SMSTimeStretch: public SegmentTransformation
	{
					
	public:
		/** Base constructor of class. Calls Configure method with a SegmentTransformationConfig initialised by default*/
		SMSTimeStretch();
		/** Constructor with an object of SegmentTransformationConfig class by parameter
		 *  @param c SegmentTransformationConfig object created by the user
		*/
		SMSTimeStretch(const SegmentTransformationConfig &c);
		
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SMSTimeStretch";}

		bool ConcreteConfigure(const ProcessingConfig& c);
		bool ConcreteStart();
		bool ConcreteStop();
		
		/** Destructor of the class*/
 		~SMSTimeStretch(){};

		bool Do()
		{
			CLAM_ASSERT(false, "Do with ports not implemented");
			return false;
		}

		bool Do(const Frame& in, Frame& out);
		bool Do(const Segment& in, Segment& out);
		bool CanProcessInplace() {return false;}
		bool IsLastFrame();
		
		Frame& GetCurrentFrame(Segment& out);
		const Frame& GetCurrentFrame(const Segment& in);
		
			
	protected:
		bool HaveFinished();
		void UpdateTimeAndIndex(const Segment& in);
		
		TTime mSynthesisTime;
		TTime mAnalysisTime;
		TIndex mnSynthesisFrames;
		
		Frame mLeftFrame;

		SMSTimeStretchConfig mConcreteConfig;

		/** Child processings **/
		FrameInterpolator mPO_FrameInterpolator;
	
	};		
};//namespace CLAM

#endif // _SMSTimeStretch_

