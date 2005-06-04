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

#include <stdlib.h>
#include "DPAudioStereo.hxx"
#include "audio_file_utils.hxx"

int main()
{
	std::vector<Audio> channels;
	
	// get audio data
	if(qtvm_examples_utils::load_audio_st("../../data/imagine.mp3",channels))
	{
		printf("File Error: \'imagine.mp3\' audio file not found!\n");
		exit(1);
	}

	CLAM::VM::PlotAudioStereo(channels[0], channels[1]); 

	return 0;
}


// END
