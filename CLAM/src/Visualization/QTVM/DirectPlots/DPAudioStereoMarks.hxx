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

#ifndef __DPAUDIOSTEREOMARKS__
#define __DPAUDIOSTEREOMARKS__

#include <vector>
#include <string>
#include "Audio.hxx"

namespace CLAM
{
	namespace VM
	{
		void PlotAudioStereoMarks(	std::vector<Audio> data,
									std::vector<unsigned>& marks,
									const std::string& label="",
									int x=100, 
									int y=100, 
									int w=660, 
									int h=250 );
	}
}

#endif
