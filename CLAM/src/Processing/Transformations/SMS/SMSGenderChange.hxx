/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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


#ifndef _SMSGenderChange_
#define _SMSGenderChange_

#include "SMSTransformation.hxx"
#include "SMSPitchShift.hxx"
#include "SMSSpectralShapeShift.hxx"


namespace CLAM{


	class SMSGenderChange: public SMSTransformationTmpl<Frame>
	{
		
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SMSGenderChange";}


	public:
		/** Base constructor of class. Calls Configure method with a SMSTransformationConfig initialised by default*/
		SMSGenderChange()
		{
		}
		/** Constructor with an object of SMSTransformationConfig class by parameter
		 *  @param c SMSTransformationConfig object created by the user
		*/
		SMSGenderChange(const SMSTransformationConfig &c):SMSTransformationTmpl<Frame>(c)
		{
		}

		virtual bool ConcreteConfigure(const ProcessingConfig& c)
		{
			SMSTransformationTmpl<Frame>::ConcreteConfigure(c);
			//configure children by default
			mPO_PitchShift.Configure(SMSTransformationConfig());
			mPO_SpectralShapeShift.Configure(SMSTransformationConfig());
			return true;
		}

		/** Destructor of the class*/
 		~SMSGenderChange()
		{}

		bool Do(const Frame& in, Frame& out);
	private:
		SMSPitchShift mPO_PitchShift;
		SMSSpectralShapeShift mPO_SpectralShapeShift;
	};		
};//namespace CLAM

#endif // _SMSGenderChange_

