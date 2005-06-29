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
#include "QtAudioPlot.hxx"
#include "QtAppWrapper.hxx"
#include "audio_file_utils.hxx"

int main()
{
    
	CLAM::Audio audio;
	
	if(qtvm_examples_utils::load_audio("../../data/birthday.wav",audio))
	{
	    printf("File Error: \'birthday.wav\' audio file not found!\n");
	    exit(1);
	}

	CLAM::VM::QtAppWrapper::Init();

	CLAM::VM::QtAudioPlot aPlot;
	aPlot.Label("Audio");
	aPlot.Geometry(100,100,500,220);
	aPlot.SetData(audio);
	aPlot.SetForegroundColor(CLAM::VM::VMColor::Blue());
	aPlot.Show();
	
	return CLAM::VM::QtAppWrapper::Run();
	
}

// END

