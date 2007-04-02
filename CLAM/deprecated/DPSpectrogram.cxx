#include "QtAppWrapper.hxx"
#include "QtSpectrogram.hxx"
#include "DPSpectrogram.hxx"

namespace CLAM
{
    namespace VM
    {

		void PlotSpectrogram( const Array<Spectrum>& specMtx,
							  const TData& duration,
							  const std::string& label,
							  int x, int y, int w, int h)
		{
			QtAppWrapper::Init();

			QtSpectrogram plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(specMtx,duration);
			plot.Show();

			QtAppWrapper::Run();
		}

		void PlotSpectrogram( const Array<Spectrum>& specMtx,
							  const TData& duration,
							  std::vector<unsigned>& marks,
							  const std::string& label,
							  int x, int y, int w, int h)
		{
			QtAppWrapper::Init();

			QtSpectrogram plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetMarks(marks);
			plot.SetData(specMtx,duration);
			plot.Show();

			QtAppWrapper::Run();
		}
    }
}

// END

