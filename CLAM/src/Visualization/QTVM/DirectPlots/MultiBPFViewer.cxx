#include "QtAppWrapper.hxx"
#include "BPFEditor.hxx"
#include "MultiBPFViewer.hxx"

namespace CLAM
{
	namespace VM
	{
		MultiBPFViewer::MultiBPFViewer()
			: mPlot(0)
		{
			QtAppWrapper::Init();
			int eFlags = CLAM::VM::HasVerticalScroll | CLAM::VM::HasHorizontalScroll | CLAM::VM::HasPlayer;
			mPlot = new BPFEditor(eFlags);
			mPlot->Geometry(100,100,500,225);
			mPlot->Show();
		}

		MultiBPFViewer::~MultiBPFViewer()
		{
			delete mPlot;
		}
				
		void MultiBPFViewer::Label(const std::string& label)
		{
			mPlot->Label(label);
		}

		void MultiBPFViewer::Geometry(int x, int y, int w, int h)
		{
			mPlot->Geometry(x,y,w,h);
		}

		void MultiBPFViewer::SetXRange(const double& min, const double& max, const EScale& scale)
		{
			mPlot->SetXRange(min,max,scale);
		}

		void MultiBPFViewer::SetYRange(const double& min, const double& max, const EScale& scale)
		{
			mPlot->SetYRange(min,max,scale);
		}

		void MultiBPFViewer::SetXScale(const EScale& scale)
		{
			mPlot->SetXScale(scale);
		}

		void MultiBPFViewer::SetYScale(const EScale& scale)
		{
			mPlot->SetYScale(scale);
		}

		void MultiBPFViewer::SetData(const BPF& bpf)
		{
			mPlot->SetData(bpf);
		}

		void MultiBPFViewer::AddData(const std::string& key, const BPF& bpf)
		{
			mPlot->AddData(key,bpf);
		}
		   
		void MultiBPFViewer::SetDataColor(const std::string& key, const Color& lines_color, const Color& handlers_color)
		{
			mPlot->SetDataColor(key,lines_color,handlers_color);
		}

		void MultiBPFViewer::Run()
		{
			QtAppWrapper::Run();
		}
	}
}

// END

