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


#ifndef _FFT_numrec_
#define _FFT_numrec_

#include "FFT_base.hxx"
#include "DataTypes.hxx"
#include "SpecTypeFlags.hxx"

namespace CLAM {

	struct FFTConfig;
	class Spectrum;
	class Audio;
	class ProcessingConfig;

	/** Implementation of the FFT using the algorithm in Numerical Recipies
	 * in C.
	 * @see <a HREF="http://www.nr.com/"> Numerical Recipies Home Page</a>
	 */
	class FFT_numrec: public FFT_base
	{
	
   	
	  	bool FFTConfigure();


		/** Configuration change method
		 */
		bool ConcreteConfigure(const ProcessingConfig&);

		// Output conversions

		void ToComplex(Spectrum &out);

	public:

		FFT_numrec();

		FFT_numrec(const FFTConfig &c);

		~FFT_numrec();

		const char * GetClassName() const {return "FFT_numrec";}

		// Execution methods

		bool Do();

		bool Do(const Audio& in, Spectrum &out);

		// Port interfaces.

		bool MayDisableExecution() const {return true;}

	};

};//namespace CLAM

#endif // _FFT_numrec_

