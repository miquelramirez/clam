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


#ifndef _SMSSpectralShapeShift_
#define _SMSSpectralShapeShift_

#include "SMSTransformation.hxx"
#include "SpectralEnvelopeExtract.hxx"
#include "SpectralEnvelopeApply.hxx"
#include "FDCombFilter.hxx"


namespace CLAM{


	/** 
	 *	Pitch shift with timbre preservation using the SMS model. In order to preserve timbre, 
	 *	the original spectral shape is extracted and then applied back.
	 */
	class SMSSpectralShapeShift: public SMSTransformationTmpl<SpectralPeakArray>
	{
		
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SMSSpectralShapeShift";}


	public:
		/** Base constructor of class. Calls Configure method with a SMSTransformationConfig initialised by default*/
		SMSSpectralShapeShift()
		{
			mSpectralRange=22050;//default
		}
		/** Constructor with an object of SMSTransformationConfig class by parameter
		 *  @param c SMSTransformationConfig object created by the user
		*/
		SMSSpectralShapeShift(const SMSTransformationConfig &c):SMSTransformationTmpl<SpectralPeakArray>(c)
		{
		}

		/** Destructor of the class*/
 		~SMSSpectralShapeShift()
		{}

		
		bool Do(const SpectralPeakArray& inpeaks,SpectralPeakArray& out);
	private:
		SpectralEnvelopeExtract mPO_SpectralEnvelopeExtract;
		SpectralEnvelopeApply mPO_SpectralEnvelopeApply;
		Spectrum mSpectralEnvelope;
		TSize mSpectralRange;
	};		
};//namespace CLAM

#endif // _SMSSpectralShapeShift_

