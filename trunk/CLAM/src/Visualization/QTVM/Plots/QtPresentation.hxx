#ifndef __QTPRESENTATION__
#define __QTPRESENTATION__

#include <qwt/qwt_scldiv.h>
#include "QtPlot.hxx"

class QLayout;
class QBoxLayout;
class QFrame;
class QPushButton;

namespace CLAM
{
	namespace VM
	{
		class PlotController;
		class DisplaySurface;
		class XRuler;
		class YRuler;
		class HScrollGroup;
		class VScrollGroup;
		
		/*
		* This class implements a common presentation. 
		* A view with: 
		*	- surface 
		*   - scroll bars
		*   - rulers: horizontal/vertical 
		*   - zoom controls: in/out 
		*/ 
		class QtPresentation : public QtPlot
		{
			Q_OBJECT
			
			public:
				QtPresentation(QWidget* parent = 0);
				virtual ~QtPresentation();
			
				virtual void SetMarks(std::list<unsigned>& marks)=0;
				virtual void SetMarksColor(Color c)=0;

				void SetBackgroundColor(Color c);
				void Label(const std::string& label);
				void Geometry(int x,int y,int w,int h);
				void Show();
				void Hide();

				void SetToggleColorOn(bool b);
				void SwitchDisplayColors(bool b);

				void RemoveXRuler();
				void RemoveYRuler();
				void RemoveHScrollGroup();
				void RemoveVScrollGroup();

				void SetFlag(bool f);

			signals:
				void HZoomIn();
				void HZoomOut();
				void HScrollValue(int);
				void HZoomRatio(int);
				void HMaxScroll(int);
				void UpdatedHScroll(int);
				void HScrollClicked();
				void HScrollReleased();

				void VZoomIn();
				void VZoomOut();
				void VScrollValue(int);
				void VZoomRatio(int);
				void VMaxScroll(int);
				void UpdatedVScroll(int);
				void VScrollClicked();
				void VScrollReleased();

				void xRulerScaleDiv(QwtScaleDiv);
				void yRulerScaleDiv(QwtScaleDiv);

				void switchColorsRequested();

				void selPos(TData);

			public slots:
				void hZoomIn();
				void hZoomOut();
				void hScrollValue(int);
				void receivedHZoomRatio(int);
				void setMaxHScroll(int);
				void updateHScroll(int);
				
				void vZoomIn();
				void vZoomOut();				
				void vScrollValue(int);
				void receivedVZoomRatio(int);
				void setMaxVScroll(int);
				void updateVScroll(int);
				
				void setXRulerScaleDiv(QwtScaleDiv);
				void setYRulerScaleDiv(QwtScaleDiv);

				void switchColors();

				void setSelPos(TData);

			protected:
				PlotController* _controller;
				
				void SetController(PlotController* controller);

				void AddToMainLayout(QLayout* layout);
				void RemoveFromMainLayout(QLayout* layout);

				void SetXRulerLabelFormat(char f,int prec,int fieldWidth);
				void SetYRulerLabelFormat(char f,int prec,int fieldWidth);

				virtual void SetPlotController()=0;
				virtual void Connect()=0;

				virtual void DisplayBackgroundBlack()=0;
				virtual void DisplayBackgroundWhite()=0;

			private:
				DisplaySurface* _surf;
				QBoxLayout *_mainLayout,*_top,*_middle,*_bottom;

				XRuler* _xRuler;
				YRuler* _yRuler;

				HScrollGroup* _hs;
				VScrollGroup* _vs;

				QPushButton* _btoggle_color;

				bool flag;

				// holes
				QFrame *topLeftHole,*topRightHole;
				QFrame *bottomLeftHole,*bottomRightHole;

				void Init();
		};
	}
}

#endif
