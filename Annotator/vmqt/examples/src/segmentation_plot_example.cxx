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
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <CLAM/DiscontinuousSegmentation.hxx>
#include <CLAM/ContiguousSegmentation.hxx>
#include <CLAM/UnsizedSegmentation.hxx>
#include "vmSegmentationPlot.hxx"


int main(int argc, char** argv)
{
	QApplication app( argc, argv );

	QString option = QInputDialog::getItem(0, 
		QString("Segment Editor demo"), 
		QString("Choose a kind of segmentation"),
		QStringList() 
			<< "Unsized"
			<< "Contiguous"
			<< "Discontinuous"
			<< "Overlapped"
		,0, false);

	double xmax = 22050.0;
	CLAM::Segmentation* seg;
	if      (option=="Unsized")
		seg = new CLAM::UnsizedSegmentation(xmax);
	else if (option=="Contiguous")
		seg = new CLAM::ContiguousSegmentation(xmax);
	else if (option=="Discontinuous")
		seg = new CLAM::DiscontinuousSegmentation(xmax);
	else if (option=="Overlapped")
	{
		QMessageBox::warning(0, QString("Not yet implemented"), 
			QString("Overlapped Segmentation is not implemented yet. Using Discontinuous segmentation instead."));
		seg = new CLAM::DiscontinuousSegmentation(xmax); // TODO: Overlapped are still not available
	}
	else
	{
		exit(-1);
	}

	unsigned nSegments = 15;
	double step = xmax/double(nSegments);
	// make some divisions
	for(unsigned i=0; i < nSegments; i++)
	{
		seg->insert(double(i)*step);
	}
	seg->current(2);

	CLAM::VM::SegmentationPlot plot;
	plot.SetTitle("Discontinuous Segmentation");
	plot.SetGeometry(100,50,600,300);
	plot.SetXRange(0.0,xmax);
	plot.SetYRange(-150.0,0.0);
	plot.SetZoomSteps(10,6);
	plot.SetSegmentation(seg);
	// uncomment the following line to see the example on background black mode
//	plot.backgroundBlack();
	plot.show();


	return app.exec();
}

// END



