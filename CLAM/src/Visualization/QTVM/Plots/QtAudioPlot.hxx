#ifndef __QTAUDIOPLOT__
#define __QTAUDIOPLOT__

#include "Audio.hxx"
#include "MediaTime.hxx"
#include "QtPresentation.hxx"
#include "PlayablePlot.hxx"

namespace CLAM
{
	namespace VM
	{
		class TimeSegmentLabelsGroup;
		class SingleLabel;
	
		class QtAudioPlot : public QtPresentation, public PlayablePlot
		{
			Q_OBJECT

			public:

				QtAudioPlot(QWidget* parent=0);
				virtual ~QtAudioPlot();

				void SetData(const Audio& audio);

				void SetMarks(std::list<unsigned>& marks);
				void SetMarksColor(Color c);

				void SetForegroundColor(Color c);
				void SetDialColor(Color c);
				void SetRegionColor(Color c);

				void RemovePlayPanel();

				void SetKeyPressed(QKeyEvent* e);
				void SetKeyReleased(QKeyEvent* e);

				void UpdateRegion(MediaTime);

			signals:
				void regionTime(MediaTime);

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

				void SetPData(const Audio& audio);

			private:
				QBoxLayout* _panel;
				TimeSegmentLabelsGroup* _labelsGroup;
				SingleLabel *_leftAmpLab, *_rightAmpLab;
				MediaTime _playBounds;
				bool showRightAmp;

				// holes
				QFrame *lefthole,*righthole;
				
				void UpdateAmpLabels(MediaTime time);
				void InitAudioPlot();
		};
	}
}

#endif

