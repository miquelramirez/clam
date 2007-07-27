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
#ifndef __SMSMORPHCONFIG__
#define __SMSMORPHCONFIG__

#include "Processing.hxx"

namespace CLAM
{
	class SMSMorphConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( SMSMorphConfig, 4, ProcessingConfig );

		DYN_ATTRIBUTE( 0, public, bool, IsHarmonic1);
		DYN_ATTRIBUTE( 1, public, bool, IsHarmonic2);
		DYN_ATTRIBUTE( 2, public, bool, UseSpectralShape);
		DYN_ATTRIBUTE( 3, public, bool, IgnoreResidual );

	protected:
		void DefaultInit();

		void DefaultValues();
	};
	
}

#endif // SMSMorphConfig.hxx

