#ifndef __QTSTEREOAUDIOPLOT__
#define __QTSTEREOAUDIOPLOT__

#include <vector>
#include <qwt/qwt_scldiv.h>
#include "Audio.hxx"
#include "MediaTime.hxx"
#include "QtPlot.hxx"
#include "PlayablePlot.hxx"

class QPushButton;

namespace CLAM
{
	namespace VM
	{
		class XRuler;
		class HScrollGroup;
		class QtAudioPlot;
		class TimeSegmentLabelsGroup;

		class QtStereoAudioPlot : public QtPlot, public PlayablePlot
		{
			Q_OBJECT

			public:

				QtStereoAudioPlot(QWidget* parent=0);
				virtual ~QtStereoAudioPlot();

				void SetData(std::vector<Audio> data);

				void SetMarks(std::list<unsigned>& marks);
				void SetMarksColor(Color c);

				void SetBackgroundColor(Color c);
				void SetForegroundColor(Color c);
				void SetDialColor(Color c);
				void SetRegionColor(Color c);

				void Label(const std::string& label);
				void Geometry(int x,int y,int w,int h);
				void Show();
				void Hide();

				void SwitchDisplayColors(bool b);

			public slots:
				void hZoomIn();
				void hZoomOut();
				void hScrollValue(int);
				void hMaxScroll(int);
				void updateHScroll(int);
				void receivedHZoomRatio(int);
				
				void setXRulerScaleDiv(QwtScaleDiv);

				void switchColors();

			protected slots:
				void updateRegion(MediaTime);

			private slots:
				void leftChannelClicked();
				void rightChannelClicked();

			protected:
				virtual void keyPressEvent(QKeyEvent* e);
				virtual void keyReleaseEvent( QKeyEvent* e);

				virtual void closeEvent(QCloseEvent* e);

				void SetPData(std::vector<Audio> data);

			private:
				QtAudioPlot *_leftChannel,*_rightChannel;
				XRuler* _xRuler;
				HScrollGroup* _hs;
				QPushButton *_muteLeft,*_muteRight;
				QPushButton* _btoggle_color;
				TimeSegmentLabelsGroup *_labelsGroup;

				void InitStereoAudioPlot();
				void DirtyTrick();
		};
	}
}

#endif

