#include <QApplication>
#include "Melody.hxx"
#include "XMLStorage.hxx"
#include "vm_playable_multibpf_plot.hxx"

void extract_data(const CLAM::Melody& in, CLAM::BPF& out, double& min, double& max);

int main(int argc, char** argv)
{
	CLAM::Melody melody;
    CLAM::XMLStorage::Restore(melody,"../../data/melody.xml");
    double duration = double(melody.GetNoteArray()[melody.GetNumberOfNotes()-1].GetTime().GetEnd());
	double min = 1E9;
    double max = -1E9;

    CLAM::BPF bpf0,bpf1,bpf2;
	extract_data(melody,bpf0,min,max);
	CLAM::XMLStorage::Restore(melody,"../../data/birthday.xml");
	extract_data(melody,bpf1,min,max);
	CLAM::XMLStorage::Restore(melody,"../../data/star_wars.xml");
    extract_data(melody,bpf2,min,max);

    double span = max-min;

	QApplication app( argc, argv );

	CLAM::VM::PlayableMultiBPFPlot plot;
	plot.set_title("MultiBPF");
	plot.set_geometry(100,50,600,300);
	plot.set_xrange(0.0,duration);
	plot.set_yrange(min-span*0.1,max+span*0.1);
	plot.set_zoom_steps(10,6);
	plot.set_grid_steps(0.5,span/20.0);
	plot.add_bpf("third_phase",&bpf0);
	plot.add_bpf("happy_birthday",&bpf1);
	plot.add_bpf("star_wars",&bpf2);
	plot.set_colors("third_phase",CLAM::VM::Color(255,0,0),CLAM::VM::Color(0,255,255));
	plot.set_colors("happy_birthday",CLAM::VM::Color(0,255,255),CLAM::VM::Color(255,0,0));
	plot.set_colors("star_wars",CLAM::VM::Color(0,255,0),CLAM::VM::Color(0,0,255));
	plot.show();

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



