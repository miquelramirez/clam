#include "QtAppWrapper.hxx"
#include "QtMultiPlot.hxx"
#include "DPMultiPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		DPMultiPlot::DPMultiPlot()
		{
			QtAppWrapper::Init();
			_plot = new QtMultiPlot();
			_plot->Geometry(100,100,500,225);
			_plot->SwitchDisplayColors(true);
			_plot->Show();
		}

		DPMultiPlot::~DPMultiPlot()
		{
			delete _plot;
		}

		void DPMultiPlot::Label(const std::string& label)
		{
			_plot->Label(label);
		}
				
		void DPMultiPlot::Geometry(int x, int y, int w, int h)
		{
			_plot->Geometry(x,y,w,h);
		}

		void DPMultiPlot::AddData(const std::string& key, const DataArray& array)
		{
			_plot->AddData(key,array);
		}

		void DPMultiPlot::AddData(std::string key, const BPF& data, int samples)
		{
			_plot->AddData(key,data,samples);
		}

		void DPMultiPlot::RemoveData(std::string key)
		{
			_plot->RemoveData(key);
		}

		void DPMultiPlot::RemoveAllData()
		{
			_plot->RemoveAllData();
		}

		void DPMultiPlot::SetColor(std::string key, Color c)
		{
			_plot->SetColor(key,c);
		}
				
		void DPMultiPlot::SetXRange(const TData& xmin, const TData& xmax)
		{
			_plot->SetXRange(xmin,xmax);
		}

		void DPMultiPlot::SetYRange(const TData& ymin, const TData& ymax)
		{
			_plot->SetYRange(ymin,ymax);
		}

		void DPMultiPlot::SetUnits(const std::string& xunits, const std::string& yunits)
		{
			_plot->SetUnits(xunits,yunits);
		}

		void DPMultiPlot::SetToolTips(const std::string& xtooltip, const std::string& ytooltip)
		{
			_plot->SetToolTips(xtooltip,ytooltip);
		}

		void DPMultiPlot::Run()
		{
			QtAppWrapper::Run();
		}

		void DPMultiPlot::SetMarks(std::list<unsigned>& marks)
		{
			_plot->SetMarks(marks);
			_plot->SetMarksColor(VMColor::Red());
		}
	}
}

// END


