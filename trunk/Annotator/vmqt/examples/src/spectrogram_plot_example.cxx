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
#include "wave_utils.hxx"
#include "analysis_utils.hxx"
#include "vmSpectrogramPlot.hxx"

int main(int argc, char** argv)
{
	QApplication app( argc, argv );

	CLAM::Audio wave;
    CLAM::Segment segment;
	
	double freq0 = 15000.0;
    double freq1 = 300.0;
    double duration = 12.0;
    double sampleRate = 8000.0;

    printf("Building a chirp signal freq=%.0lf..%.0lf Hz duration=%.0lf sec. sample rate=%.0lf Hz\n",
		   freq0,freq1,duration,sampleRate);
    qtvm_examples_utils::chirp(freq0,freq1,duration,sampleRate,wave);

    printf("Analysing");
    qtvm_examples_utils::analyze(wave,segment);    
    printf("\ndone!\n");
	
	CLAM::VM::SpectrogramPlot plot;
	plot.SetTitle("Spectrogram");
	plot.SetGeometry(100,50,600,300);
	plot.SetData(segment);
	plot.show();

// uncomment the following line to see the example with sonogram on black&white 
//	plot.blackWhiteSpecgram();

// uncomment the following line to see the example on background black mode
//	plot.backgroundBlack();

	return app.exec();

}

// END



