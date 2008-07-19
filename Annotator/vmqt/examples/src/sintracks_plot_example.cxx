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

#include <QtGui/QApplication>
#include "audio_file_utils.hxx"
#include "analysis_utils.hxx"
#include "vmSinTracksPlot.hxx"

int main(int argc, char** argv)
{
	CLAM::Audio audio;
	CLAM::Segment segment;

	printf("loading audio...\n");
	qtvm_examples_utils::load_audio("data/birthday.wav",audio);
	printf("analysing");
	qtvm_examples_utils::analyze(audio,segment);
	printf("\ndone!\n");

	QApplication app( argc, argv );

	CLAM::VM::SinTracksPlot plot;
	plot.SetTitle("Sinusoidal tracks");
	plot.SetGeometry(100,100,600,300);
	plot.SetData(segment);
	plot.show();

// uncomment the following line to see the example on background black mode
//	plot.backgroundBlack();
	
	return app.exec();

}

// END



