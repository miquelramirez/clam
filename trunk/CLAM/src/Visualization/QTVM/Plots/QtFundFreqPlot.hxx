#ifndef __QTFUNDFREQPLOT__
#define __QTFUNDFREQPLOT__

#include "Segment.hxx"
#include "MediaTime.hxx"
#include "QtPresentation.hxx"
#include "PlayablePlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class TimeSegmentLabelsGroup;
		class SingleLabel;
	
		class QtFundFreqPlot : public QtPresentation, public PlayablePlot
		{
			Q_OBJECT

			public:

				QtFundFreqPlot(QWidget* parent=0);
				virtual ~QtFundFreqPlot();

				void SetData(const Segment& segment);

				void SetMarks(std::list<unsigned>& marks);
				void SetMarksColor(Color c);

				void SetForegroundColor(Color c);
				void SetDialColor(Color c);
				void SetRegionColor(Color c);

			protected slots:
				void updateRegion(MediaTime);

			protected:
				virtual void keyPressEvent(QKeyEvent* e);
				virtual void keyReleaseEvent( QKeyEvent* e);

				virtual void closeEvent(QCloseEvent* e);

				virtual void SetPlotController();
				virtual void Connect();

				virtual void DisplayBackgroundBlack();
				virtual void DisplayBackgroundWhite();

				void SetPData(const Segment& seg);

			private:
				TimeSegmentLabelsGroup* _labelsGroup;
				SingleLabel *_leftFreqLab, *_rightFreqLab;
				
				void InitFundFreqPlot();
				void UpdateFreqLabels(MediaTime time);
		};
	}
}

#endif

