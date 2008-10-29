/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef _CardinalVowel_
#define _CardinalVowel_

#include "DataTypes.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"

namespace CLAM
{
	
	class CardinalVowelConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (CardinalVowelConfig, 1, ProcessingConfig);
		DYN_ATTRIBUTE ( 0, public, TData, Amount );
		
	private:
		void DefaultInit();
	};
	
	
	/*
	  \class CardinalVowel
	  \brief  This class returns formants 1 and 2 for the cardinal vowels
	  
	  The cardinal vowels are reference vowels that can be used to
	  place vowels from unstudied languages.  There are 4 front
	  and 4 back vowels, starting with the high front IY and
	  ending with the high back UW: IY EY EH AE AA AO OW UW
	 
	  This class assumes as input a continuum from 0 (IY) to 1
	  (UW) and maps the values between 0 and 1 to first and second
	  formant of vowels along the cardinal vowel progression
	  
	  \TODO the second input, the number of steps will represent
	  discrete steps along the path of cardinal vowels.  For the
	  cardinal vowels series, the number of steps = 8.  Setting
	  this to different values can be seen as roughly a parameter
	  to say how many vowels for a given language.

	  \TODO, the mapping of the [0,1] interval to f1 and f2 values
	  was done by hand by fitting linear (piecewise in the case of
	  f1) functions.  In the future, it would be cool to have the
	  interpolation of values between vowels to be done
	  automatically.  The interpolation of formant values between
	  vowels could be done with some of the clam tools.  Also,
	  this can be seen as a dimension reduction problem, where the
	  f1 and f2 space is reduced to a single dimension.

	*/
	class CardinalVowel : public Processing
	{
		CardinalVowelConfig mConfig;
		FloatInControl mVowelControl;
		FloatInControl mStepControl;
		FloatOutControl mF1Control;
		FloatOutControl mF2Control;
		FloatOutControl mF3Control;
		float CalcF1();
		float CalcF2();
		float CalcF3();
		
	public:
		
		const char *GetClassName() const { return "CardinalVowel"; }
		
		
		CardinalVowel();
		CardinalVowel( const CardinalVowelConfig& cfg );
		
		bool ConcreteConfigure( const ProcessingConfig& cfg ); 
		
		const ProcessingConfig& GetConfig() const { return mConfig; }
		
		bool Do();
		
	};
	
}

#endif

