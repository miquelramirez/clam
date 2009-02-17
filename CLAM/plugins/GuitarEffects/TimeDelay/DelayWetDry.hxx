/*
 * Copyright (c) 2009 Hernán Ordiales
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

#ifndef _DelayWetDry_
#define _DelayWetDry_

#include "TimeDelay.hxx"

namespace CLAM {

	/**	\brief Simple audio delay processing.
	*
	*/
	class DelayWetDry: public TimeDelay
	{	
		/** This method returns the name of the object
		*	@return Char pointer with the name of object
		*/
		const char *GetClassName() const { return "DelayWetDry"; }

		FloatInControl mWetDry;

	public:
		DelayWetDry()
			:
			TimeDelay(),
			mWetDry("Wet/Dry", this)
		{
		}

		~DelayWetDry() {}

		bool Do()
		{
			bool result = Do( mInput.GetAudio(), mOutput.GetAudio() );

			mInput.Consume(); 
			mOutput.Produce();

			return result;
		}	

		bool Do(const Audio& in, Audio& out);

	};



};//namespace CLAM

#endif // _DelayWetDry_
