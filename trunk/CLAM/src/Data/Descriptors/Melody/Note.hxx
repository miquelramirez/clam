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

/* Note type definition */

#ifndef _Note_
#define _Note_

#include "DynamicType.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingData.hxx"

#include "MediaTime.hxx"
#include "PitchNote.hxx"

namespace CLAM {

	/** 	\brief Used to store note information.
	 *
	 *	 Pitch and intranote time information are not instantiated by default
	 */
	class Note: public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Note, 7, ProcessingData);
		/* Pitch information (not by default) */
		DYN_ATTRIBUTE (0, public, PitchNote, PitchNote);
		/* Note Fundamental frequency (by default) */
		DYN_ATTRIBUTE (1, public, TData, FundFreq); 
		/* Note energy (by default) */
		DYN_ATTRIBUTE (2, public, TData, Energy);
		DYN_ATTRIBUTE (3, public, MediaTime, Time);
		/* Intranote duration (not by default) */
		DYN_ATTRIBUTE (4, public, MediaTime, Attack);
		DYN_ATTRIBUTE (5, public, MediaTime, Sustain);
		DYN_ATTRIBUTE (6, public, MediaTime, Release);

		/** Helper accessor for getting the corresponding MIDI note number
		*/
		int GetNoteNumber()
		{
			return Round(69+log(GetFundFreq()/440)*17.31234);
		}
		int GetVelocity()
		{
		//this mapping is experimental and should be revisited
			TData velocity=65*log10(GetEnergy())+65;
			if(velocity<25) velocity=25;
			return (int)velocity;//65;
		}
	private:
		void DefaultInit();	
	};

}; // namespace CLAM


#endif // _Note_

