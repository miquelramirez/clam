#include "XMLStorage.hxx"
#include "Melody.hxx"
#include "BPF.hxx"
#include "MultiBPFViewer.hxx"

void ExtractData(const CLAM::Melody& in, CLAM::BPF& out, CLAM::TData& min, CLAM::TData& max);

int main()
{
	// prepared data
    CLAM::Melody melody;
    CLAM::XMLStorage::Restore(melody,"../../data/melody.xml");

    CLAM::TData duration = melody.GetNoteArray()[melody.GetNumberOfNotes()-1].GetTime().GetEnd();

    CLAM::TData max = -1E9;
    CLAM::TData min = 1E9;
	
    CLAM::BPF bpf0,bpf1,bpf2;
	
	ExtractData(melody,bpf0,min,max);
	CLAM::XMLStorage::Restore(melody,"../../data/melody1.xml");
	ExtractData(melody,bpf1,min,max);
	CLAM::XMLStorage::Restore(melody,"../../data/melody2.xml");
    ExtractData(melody,bpf2,min,max);

    CLAM::TData span = max-min;
	// end prepared data

    CLAM::VM::MultiBPFViewer plot;
    plot.Label("MultiBPFViewer");
    plot.Geometry(100,100,600,400);
    plot.SetXRange(0.0,duration);
    plot.SetYRange(double(min)-double(span)*0.1, double(max)+double(span)*0.1);

	/*
	 * set default BPF
	 */
    plot.SetData(bpf0); 

	/*
	 * add additional BPFs
	 */
	plot.AddData("star-wars",bpf1);
	plot.AddData("2001",bpf2);
	plot.SetDataColor("star-wars",CLAM::VM::VMColor::Red(),CLAM::VM::VMColor::Yellow());
	plot.SetDataColor("2001",CLAM::VM::VMColor::Green(),CLAM::VM::VMColor::Magenta());

    plot.Run(); // show

    return 0;

} // end main

void ExtractData(const CLAM::Melody& in, CLAM::BPF& out, CLAM::TData& min, CLAM::TData& max)
{
	for(int i=0; i < in.GetNumberOfNotes(); i++)
    {
		CLAM::TData value = in.GetNoteArray()[i].GetFundFreq();
		if(value < min) min = value;
		if(value > max) max = value;
		out.Insert(in.GetNoteArray()[i].GetTime().GetBegin(), value);
    }
}

// END

