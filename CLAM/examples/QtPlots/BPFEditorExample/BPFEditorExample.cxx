#include "XMLStorage.hxx"
#include "QtAppWrapper.hxx"
#include "BPFEditor.hxx"

void ExtractData(const CLAM::Melody& in, CLAM::BPF& out, CLAM::TData& min, CLAM::TData& max);

int main()
{
    CLAM::Melody melody;
    CLAM::XMLStorage::Restore(melody,"../data/melody.xml");

    CLAM::TData duration = melody.GetNoteArray()[melody.GetNumberOfNotes()-1].GetTime().GetEnd();

    CLAM::TData max = -1E9;
    CLAM::TData min = 1E9;
	
    CLAM::BPF bpf0,bpf1,bpf2;
	
	ExtractData(melody,bpf0,min,max);
	CLAM::XMLStorage::Restore(melody,"../data/melody1.xml");
	ExtractData(melody,bpf1,min,max);
	CLAM::XMLStorage::Restore(melody,"../data/melody2.xml");
    ExtractData(melody,bpf2,min,max);

    CLAM::TData span = max-min;

    CLAM::VM::QtAppWrapper::Init();

    CLAM::VM::BPFEditor editor0;
    editor0.Label("Single BPF example");
    editor0.Geometry(100,10,600,300);
    editor0.SetXRange(0.0,duration);
    editor0.SetYRange(double(min)-double(span)*0.1, double(max)+double(span)*0.1);
    editor0.SetData(bpf0);

	CLAM::VM::BPFEditor editor1(CLAM::VM::AllowInsert |
								CLAM::VM::AllowRemove |
								CLAM::VM::AllowHorizontalEdition | 
								CLAM::VM::AllowVerticalEdition |
								CLAM::VM::HasHorizontalScroll | 
								CLAM::VM::HasVerticalScroll | 
								CLAM::VM::HasPlayer);
    editor1.Label("Multiple BPF example");
    editor1.Geometry(100,335,600,300);
    editor1.SetXRange(0.0,duration);
    editor1.SetYRange(double(min)-double(span)*0.1, double(max)+double(span)*0.1);
    editor1.SetData(bpf0);
	editor1.AddData("star-wars",bpf1);
	editor1.AddData("2001",bpf2);
	editor1.SetDataColor("star-wars",CLAM::VM::VMColor::Red(),CLAM::VM::VMColor::Yellow());
	editor1.SetDataColor("2001",CLAM::VM::VMColor::Green(),CLAM::VM::VMColor::Magenta());

    editor0.Show();
    editor1.Show();


    CLAM::VM::QtAppWrapper::Run();

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

