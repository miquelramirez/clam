#include <QtGui/QApplication>
#include "Melody.hxx"
#include "XMLStorage.hxx"
#include "vmBPFPlot.hxx"

void extract_data(const CLAM::Melody& in, CLAM::BPF& out, double& min, double& max);

int main(int argc, char** argv)
{
	CLAM::Melody melody;
	CLAM::XMLStorage::Restore(melody,"data/melody.xml");
    double duration = double(melody.GetNoteArray()[melody.GetNumberOfNotes()-1].GetTime().GetEnd());
	double min = 1E9;
    double max = -1E9;
    CLAM::BPF bpf;
	extract_data(melody,bpf,min,max);
    double span = max-min;

	QApplication app( argc, argv );

	CLAM::VM::BPFPlot plot;
	plot.SetTitle("BPF");
	plot.SetGeometry(100,50,600,300);
	plot.SetXRange(0.0,duration);
	plot.SetYRange(min-span*0.1,max+span*0.1);
	plot.SetZoomSteps(10,6);
	plot.SetData(&bpf);
	plot.show();

// comment the following lines for hide the grid, deactivate snap to grid feature or change grid steps
	plot.showGrid(true);
	plot.snapToGrid(true);
	plot.SetGridSteps(0.5,span/20.0);

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



