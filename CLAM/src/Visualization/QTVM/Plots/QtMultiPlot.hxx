#ifndef __QTMULTIPLOT__
#define __QTMULTIPLOT__

#include "Array.hxx"
#include "BPF.hxx"
#include "QtPresentation.hxx"

namespace CLAM
{
	namespace VM
	{
		class SingleLabel;

		class QtMultiPlot : public QtPresentation
		{
			Q_OBJECT

			public:
				QtMultiPlot(QWidget* parent=0);
				virtual ~QtMultiPlot();

				void AddData(std::string key, const DataArray& array);
				void AddData(std::string key, const BPF& data, int samples = 100);
				void RemoveData( std::string key );
				void RemoveAllData();

				void SetColor(std::string key, Color c);

				void SetUnits(const std::string& xunits, const std::string& yunits);
				void SetToolTips(const std::string& xtooltip, const std::string& ytooltip);

				void SetXRange(const TData& xmin, const TData& xmax);
				void SetYRange(const TData& ymin, const TData& ymax);

				void SetMarks(std::list<unsigned>& marks);
				void SetMarksColor(Color c);

			private slots:
				void updateXLabel(TData);
				void updateYLabel(TData);

				void clearLabels();

			protected:
				virtual void SetPlotController();
				virtual void Connect();

				virtual void DisplayBackgroundBlack();
				virtual void DisplayBackgroundWhite();

			private:
				SingleLabel *_xLabel,*_yLabel;

				void InitMultiPlot();

		};
	}
}

#endif

