#ifndef __QTSPECTRUMPLOT__
#define __QTSPECTRUMPLOT__
#include "Spectrum.hxx"
#include "QtPresentation.hxx"
namespace CLAM
{
	namespace VM
	{
		class SingleLabel;		
		class QtSpectrumPlot : public QtPresentation
		{
			Q_OBJECT
			public:
				QtSpectrumPlot(QWidget* parent=0);
				virtual ~QtSpectrumPlot();

				virtual void SetData(const Spectrum& spec);

				void SetMarks(std::list<unsigned>& marks);
				void SetMarksColor(Color c);
				void SetForegroundColor(Color c);
				void SetVLineColor(Color c);
			protected slots:
				void updateMagLabel(TData);
				void updateFreqLabel(TData);

			protected:
				virtual void SetPlotController();
				virtual void Connect();

				virtual void DisplayBackgroundBlack();
				virtual void DisplayBackgroundWhite();
			private:
				SingleLabel *_magLabel,*_freqLabel;

				void InitSpectrumPlot();
		};
	}
}

#endif

