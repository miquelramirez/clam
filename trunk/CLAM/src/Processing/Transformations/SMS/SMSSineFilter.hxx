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


#ifndef _SMSSineFilter_
#define _SMSSineFilter_

#include "SMSTransformation.hxx"


namespace CLAM{


	class SMSSineFilter: public SMSTransformationTmpl<SpectralPeakArray>
	{
		
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SMSSineFilter";}


	public:
		/** Base constructor of class. Calls Configure method with a SMSTransformationConfig initialised by default*/
		SMSSineFilter()
		{
		}
		/** Constructor with an object of SMSTransformationConfig class by parameter
		 *  @param c SMSTransformationConfig object created by the user
		*/
		SMSSineFilter(const SMSTransformationConfig &c):SMSTransformationTmpl<SpectralPeakArray>(c)
		{
		}

		virtual bool ConcreteConfigure(const ProcessingConfig& c)
		{
			SMSTransformationTmpl<SpectralPeakArray>::ConcreteConfigure(c);
			//BPF will be used in a non temporal sense
			mUseTemporalBPF=false;
			return true;
		}

		/** Destructor of the class*/
 		~SMSSineFilter()
		{}

		bool Do(const SpectralPeakArray& in, SpectralPeakArray& out);
	
	};		
};//namespace CLAM

#endif // _SMSSineFilter_

