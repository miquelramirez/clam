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

#include <stdlib.h>
#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include "vmAudioStereoPlot.hxx"
#include "audio_file_utils.hxx"

int main(int argc, char** argv)
{
	QApplication app( argc, argv );
	QString fn = QFileDialog::getOpenFileName(0,"Choose a stereo audio file",".","Audio (*.wav *.ogg *.mp3)");
	if(fn.isEmpty())
	{
		printf("No file was choosen.\n");
		exit(1);
	}
	std::vector<CLAM::Audio> channels;
	printf("loading audio...\n");
	qtvm_examples_utils::load_audio_st(fn.toAscii().data(),channels);
	printf("done\n");
	
	CLAM::VM::AudioStereoPlot plot;
	plot.SetTitle("Audio Stereo");
	plot.SetGeometry(100,50,600,350);
	plot.SetData(channels[0],channels[1]);
	plot.show();

// uncomment the following line to see the example on background black mode
//	plot.backgroundBlack();

	return app.exec();
}

// END



