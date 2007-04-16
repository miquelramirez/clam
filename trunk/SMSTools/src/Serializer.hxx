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

#ifndef __SERIALIZER__
#define __SERIALIZER__

namespace CLAM {

	class Segment;

	class Serializer {

	public:
		Serializer(  ) { }
		virtual ~Serializer(  ) { }

		virtual bool DoLoad( const char* fileName, Segment& segment ) = 0;
		virtual bool DoStore( const char* fileName, Segment& segment ) = 0;
	};

}

#endif //__SERIALIZER__

