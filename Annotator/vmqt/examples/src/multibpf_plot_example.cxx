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
#include "Melody.hxx"
#include "XMLStorage.hxx"
#include "vmMultiBPFPlot.hxx"

void extract_data(const CLAM::Melody& in, CLAM::BPF& out, double& min, double& max);

int main(int argc, char** argv)
{
	CLAM::Melody melody;
    CLAM::XMLStorage::Restore(melody,"data/melody.xml");
    double duration = double(melody.GetNoteArray()[melody.GetNumberOfNotes()-1].GetTime().GetEnd());
	double min = 1E9;
    double max = -1E9;

    CLAM::BPF bpf0,bpf1,bpf2;
	extract_data(melody,bpf0,min,max);
	CLAM::XMLStorage::Restore(melody,"data/birthday.xml");
	extract_data(melody,bpf1,min,max);
	CLAM::XMLStorage::Restore(melody,"data/star_wars.xml");
    extract_data(melody,bpf2,min,max);

    double span = max-min;

	QApplication app( argc, argv );

	CLAM::VM::MultiBPFPlot plot;
	plot.SetTitle("MultiBPF");
	plot.SetGeometry(100,50,600,300);
	plot.SetXRange(0.0,duration);
	plot.SetYRange(min-span*0.1,max+span*0.1);
	plot.SetZoomSteps(10,6);
	plot.AddBPF("third_phase",&bpf0);
	plot.AddBPF("happy_birthday",&bpf1);
	plot.AddBPF("star_wars",&bpf2);
	plot.SetColors("third_phase",QColor(255,0,0),QColor(0,255,255));
	plot.SetColors("happy_birthday",QColor(0,255,255),QColor(255,0,0));
	plot.SetColors("happy_birthday",QColor(0,255,0),QColor(0,0,255));
	plot.show();

// comment the following lines for hide the grid, deactivate snap to grid feature or change grid steps
	plot.showGrid(true);
	plot.snapToGrid(true);
	plot.SetGridSteps(0.5,span/20.0);

// uncomment one of the following lines to set the current bpf, current: wtar_wars (last entry)
//	plot.setCurrentBPF("third_phase");
	plot.setCurrentBPF("happy_birthday");

// uncomment the following line to see the example on background black mode
//	plot.backgroundBlack();

	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

void extract_data(const CLAM::Melody& in, CLAM::BPF& out, double& min, double& max)
{
	for(int i=0; i < in.GetNumberOfNotes(); i++)
    {
		double value = double(in.GetNoteArray()[i].GetFundFreq());
		if(value < min) min = value;
		if(value > max) max = value;
		out.Insert(in.GetNoteArray()[i].GetTime().GetBegin(), value);
    }
}

// END



