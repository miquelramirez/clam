#include "QtAppWrapper.hxx"
#include "BPFEditor.hxx"

using CLAM::TData;

int main()
{
    CLAM::BPF bpf;
    bpf.SetSize(0);
    bpf.Insert(TData(0.0),TData(0.0));
    bpf.Insert(TData(20.0),TData(20000.0));
    bpf.Insert(TData(60.0),TData(11000.0));
    bpf.Insert(TData(150.0),TData(11000.0));
    bpf.Insert(TData(200.0),TData(0.0));

    CLAM::VM::QtAppWrapper::Init();

    CLAM::VM::BPFEditor editor;
    editor.Label("BPFEditor");
    editor.Geometry(100,100,600,400);
    editor.SetXRange(0.0,200.0);
    editor.SetYRange(0.0,22050.0);
    editor.SetData(bpf);
    editor.Show();

    CLAM::VM::QtAppWrapper::Run();

    return 0;
}

// END
