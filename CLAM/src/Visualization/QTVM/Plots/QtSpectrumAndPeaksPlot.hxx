#ifndef __QTSPECTRUMANDPEAKSPLOT__ 
#define __QTSPECTRUMANDPEAKSPLOT__

#include "SpectralPeakArray.hxx"
#include "QtSpectrumPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class QtSpectrumAndPeaksPlot : public QtSpectrumPlot
		{
			Q_OBJECT

			public:
				QtSpectrumAndPeaksPlot(QWidget* parent=0);
				virtual ~QtSpectrumAndPeaksPlot();

				virtual void SetData(const Spectrum& spec,const SpectralPeakArray& peaks);
				void SetPeaksColor(Color cline,Color cpoint);

			protected:
				void SetData(const Spectrum& spec);

				virtual void SetPlotController();
				virtual void Connect();

				virtual void DisplayBackgroundBlack();
				virtual void DisplayBackgroundWhite();
			
		};
	}
}

#endif


