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

#ifndef _SMSAppState_
#define _SMSAppState_


#include "DynamicType.hxx"

namespace CLAM{

	/** Class to store internal internal state of the SMS application*/
	class SMSAppState:public DynamicType
	{
		DYNAMIC_TYPE(SMSAppState, 9);
		/** Indicates whether there is a valid analysis-synthesis configuration */
		DYN_ATTRIBUTE(0, public, bool, HasConfig);
		/** Indicates whether an analysis has been performed */
		DYN_ATTRIBUTE(1, public, bool, HasAnalysis);
		/** Indicates whether there is a valid input audio */
		DYN_ATTRIBUTE(2, public, bool, HasAudioIn);
		/** Indicates whether there is a valid audio to morph*/
		DYN_ATTRIBUTE(3, public, bool, HasAudioMorph);
		/** Indicates whether there is a valid output audio */
		DYN_ATTRIBUTE(4, public, bool, HasAudioOut);
		/** Indicates whether there is a valid analyzed melody */
		DYN_ATTRIBUTE(5, public, bool, HasMelody);
		/** Indicates whether there is a valid transformation score */
		DYN_ATTRIBUTE(6, public, bool, HasTransformationScore);
		/** Indicates whether there is a valid spectrum, needed for melody analysis */
		DYN_ATTRIBUTE(7, public, bool, HasSpectrum);
		/** Indicates whether there a transformation has been performed */
		DYN_ATTRIBUTE(8, public, bool, HasTransformation);

		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetHasConfig(false);
			SetHasAnalysis(false);
			SetHasAudioIn(false);
			SetHasAudioMorph(false);
			SetHasAudioOut(false);
			SetHasMelody(false);
			SetHasTransformationScore(false);
			SetHasSpectrum(false);
			SetHasTransformation(false);
		}

	};

};

#endif

