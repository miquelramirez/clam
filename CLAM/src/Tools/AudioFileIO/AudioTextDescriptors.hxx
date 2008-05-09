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

#ifndef __AUDIOTEXTDESCRIPTORS__
#define __AUDIOTEXTDESCRIPTORS__
#include <typeinfo>
#include "DynamicType.hxx"
#include "Text.hxx"

namespace CLAM
{
	/** @ingroup AudioFileIO */
	class AudioTextDescriptors : public DynamicType
	{
	public:
		DYNAMIC_TYPE( AudioTextDescriptors, 6 );
		DYN_ATTRIBUTE( 0, public, Text, Artist );
		DYN_ATTRIBUTE( 1, public, Text, Title );
		DYN_ATTRIBUTE( 2, public, Text, Album );
		DYN_ATTRIBUTE( 3, public, Text, TrackNumber );
		DYN_ATTRIBUTE( 4, public, Text, Composer );
		DYN_ATTRIBUTE( 5, public, Text, Performer );

	protected:
		void DefaultInit();

	};
}

#endif // AudioTextDescriptors.hxx


