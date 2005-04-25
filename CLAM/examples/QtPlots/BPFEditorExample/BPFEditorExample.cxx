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

    CLAM::VM::BPFEditor editor;
    editor.Label("BPFEditor");
    editor.Geometry(100,100,600,400);
    editor.SetXRange(0.0,duration);
    editor.SetYRange(double(min)-double(span)*0.1, double(max)+double(span)*0.1);
    editor.SetData(bpf);
    editor.Show();

    CLAM::VM::QtAppWrapper::Run();

    return 0;
}

// END
