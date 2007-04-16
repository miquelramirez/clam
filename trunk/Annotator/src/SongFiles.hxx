/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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


#ifndef _SongFiles_
#define _SongFiles_

#include <CLAM/DynamicType.hxx>
#include <CLAM/Filename.hxx>

namespace CLAM_Annotator{

	class Song:public CLAM::DynamicType
	{
		DYNAMIC_TYPE(Song,2);
		DYN_ATTRIBUTE(0,public, CLAM::Filename, SoundFile);
		/**
		 * Descriptor File Associated to the sound file. In most applications if this attribute
		 * is missing the application is expected to add the .pool extension to the SoundFile name
		 */
		DYN_ATTRIBUTE(1,public, std::string, PoolFile);
		
		void DefaultInit()
		{
			AddSoundFile();
			UpdateData();
		}
	};
};

#endif

