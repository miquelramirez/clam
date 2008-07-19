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
#include "vmAudioPlot.hxx"
#include <CLAM/ContiguousSegmentation.hxx>
#include "audio_file_utils.hxx"

int main(int argc, char** argv)
{
	QApplication app( argc, argv );
	QString fn = QFileDialog::getOpenFileName(0,"Choose a mono audio file",".","Audio (*.wav *.ogg *.mp3)");
	if(fn.isEmpty())
	{
		printf("No file was choosen.\n");
		exit(1);
	}
	CLAM::Audio audio;
	printf("loading audio...\n");
	qtvm_examples_utils::load_audio(fn.toAscii().data(),audio);
	printf("done\n");
	double xmax = audio.GetDuration()/1000.0;
	CLAM::Segmentation* seg = new CLAM::ContiguousSegmentation(xmax);
	unsigned nSegments = 15;
	double step = xmax/double(nSegments);
	// make some divisions
	for(unsigned i=1; i < nSegments; i++)
	{
		seg->insert(double(i)*step);
	}
	seg->current(2);
	
	CLAM::VM::AudioPlot plot;
	plot.SetTitle("Contiguous Segmentation");
	plot.SetGeometry(100,50,600,300);
	plot.SetData(audio);
	plot.SetSegmentation(seg);
	plot.show();

// uncomment the following line to see the example on background black mode
//	plot.backgroundBlack();

	return app.exec();
}

// END



