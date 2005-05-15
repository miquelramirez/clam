#include "XMLStorage.hxx"
#include "QtAppWrapper.hxx"
#include "BPFEditor.hxx"

int main()
{
    CLAM::Melody melody;
    CLAM::XMLStorage::Restore(melody,"../data/melody.xml");

    CLAM::TData duration = melody.GetNoteArray()[melody.GetNumberOfNotes()-1].GetTime().GetEnd();

    CLAM::TData max = -1E9;
    CLAM::TData min = 1E9;

    CLAM::BPF bpf;

    for(int i=0; i < melody.GetNumberOfNotes(); i++)
    {
	CLAM::TData value = melody.GetNoteArray()[i].GetFundFreq();
	if(value < min) min = value;
	if(value > max) max = value;
	bpf.Insert(melody.GetNoteArray()[i].GetTime().GetBegin(), value);
    }
    
    CLAM::TData span = max-min;

    CLAM::VM::QtAppWrapper::Init();

    CLAM::VM::BPFEditor editor0;
    editor0.Label("BPFEditor0");
    editor0.Geometry(100,10,600,300);
    editor0.SetXRange(0.0,duration);
    editor0.SetYRange(double(min)-double(span)*0.1, double(max)+double(span)*0.1);
    editor0.SetData(bpf);

     CLAM::VM::BPFEditor editor1(0,0,
				 CLAM::VM::AllowHorizontalEdition | 
				 CLAM::VM::AllowVerticalEdition |
				 CLAM::VM::HasHorizontalScroll | 
				 CLAM::VM::HasVerticalScroll | 
				 CLAM::VM::HasPlayer);
    editor1.Label("BPFEditor1");
    editor1.Geometry(100,335,600,300);
    editor1.SetXRange(0.0,duration);
    editor1.SetYRange(double(min)-double(span)*0.1, double(max)+double(span)*0.1);
    editor1.SetData(bpf);

    editor0.Show();
    editor1.Show();

    CLAM::VM::QtAppWrapper::Run();

    return 0;
}

// END
