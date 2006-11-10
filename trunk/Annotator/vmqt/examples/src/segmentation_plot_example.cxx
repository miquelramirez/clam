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
#include <CLAM/DiscontinuousSegmentation.hxx>
#include "vmSegmentationPlot.hxx"


int main(int argc, char** argv)
{
	double xmax = 22050.0;
	CLAM::Segmentation* seg = new CLAM::DiscontinuousSegmentation(xmax);
	unsigned nSegments = 15;
	double step = xmax/double(nSegments);
	// make some divisions
	for(unsigned i=0; i < nSegments; i++)
	{
		seg->insert(double(i)*step);
	}
	seg->current(2);

	QApplication app( argc, argv );

	CLAM::VM::SegmentationPlot plot;
	plot.SetTitle("Discontinuous Segmentation");
	plot.SetGeometry(100,50,600,300);
	plot.SetXRange(0.0,xmax);
	plot.SetYRange(-150.0,0.0);
	plot.SetZoomSteps(10,6);
	plot.SetSegmentation(seg);
	plot.show();

// uncomment the following line to see the example on background black mode
//	plot.backgroundBlack();

	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

// END



