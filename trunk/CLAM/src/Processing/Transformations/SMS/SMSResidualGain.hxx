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


#ifndef _SMSResidualGain_
#define _SMSResidualGain_

#include "SMSTransformation.hxx"


namespace CLAM{


	class SMSResidualGain: public SMSTransformationTmpl<Spectrum>
	{
		
		/** This method returns the name of the object
		 *  @return Char pointer with the name of object
		 */
		const char *GetClassName() const {return "SMSResidualGain";}


	public:
		/** Base constructor of class. Calls Configure method with a SMSTransformationConfig initialised by default*/
		SMSResidualGain()
		{
		}
		/** Constructor with an object of SMSTransformationConfig class by parameter
		 *  @param c SMSTransformationConfig object created by the user
		*/
		SMSResidualGain(const SMSTransformationConfig &c):SMSTransformationTmpl<Spectrum>(c)
		{
		}


		/** Destructor of the class*/
 		~SMSResidualGain()
		{}

		bool Do(const Spectrum& in, Spectrum& out);
	
	};		
};//namespace CLAM

#endif // _SMSResidualGain_

