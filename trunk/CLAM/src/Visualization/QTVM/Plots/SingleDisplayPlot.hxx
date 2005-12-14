#ifndef __SINGLEDISPLAYPLOT__
#define __SINGLEDISPLAYPLOT__

#include "QtPlot.hxx"

class QLayout;
class QBoxLayout;
class QFrame;
class QPushButton;

namespace CLAM
{
	class Segmentation;

    namespace VM
    {
		class PlotController;
		class DisplaySurface;
		class Ruler;
		class HScrollGroup;
		class VScrollGroup;
		class EditTagDialog;
		
		/**
		 * This class implements a single display common presentation. 
		 * A view with: 
		 *   - surface 
		 *   - scroll bars
		 *   - rulers: horizontal/vertical 
		 *   - zoom controls: in/out 
		 */ 
		class SingleDisplayPlot : public QtPlot
		{
			Q_OBJECT		
	
		public:
			SingleDisplayPlot(QWidget* parent = 0, const char * name = 0, WFlags f = 0);
			virtual ~SingleDisplayPlot();
			
			void SetBackgroundColor(Color c);
			void SetDialColor(Color c);
			void Label(const std::string& label);
			void Geometry(int x,int y,int w,int h);
			void Show();
			void Hide();

			void SetMarks(std::vector<unsigned>& marks);
			std::vector<unsigned>& GetMarks();
			void SetMarksColor(Color c);
			std::vector<QString> GetSegmentationTags();

			void SetToggleColorOn(bool b);
			void SwitchDisplayColors(bool b);

			void SetEditTagDialogEnabled(bool e);

			void SetSegmentation(Segmentation* s);

			void UseFocusColors();
		   
		signals:
			void xRulerRange(double, double);
			void yRulerRange(double, double);
			void switchColorsRequested();
			void insertedMark(unsigned);
			void removedMark(int, unsigned);
			void updatedMark(int, unsigned);
			void updatedTag(int, QString);
			void selectedXPos(double);
			void requestSegmentationTag(unsigned);

			// signals from SegmentEditor
			void segmentOnsetChanged(unsigned,double);
			void segmentOffsetChanged(unsigned,double);
			void segmentInserted(unsigned);
			void segmentDeleted(unsigned);
			void currentSegmentChanged(unsigned);
	    
		public slots:	
			void switchColors();
			void setSelectedXPos(double);

		protected slots:
			virtual void initialYRulerRange(double, double);

		private slots:
			void setMaxHScroll(int);
			void setMaxVScroll(int);
			void showEditTagDialog();

			void focusInColor();
			void focusOutColor();

		protected:
			PlotController* mController;
			
			virtual void showEvent(QShowEvent* e);
			virtual void hideEvent(QHideEvent* e);

			virtual void SetPlotController()=0;
			virtual void Connect()=0;
			virtual void DisplayBackgroundBlack()=0;
			virtual void DisplayBackgroundWhite()=0;

			void SetController(PlotController* controller);
			void AddToMainLayout(QLayout* layout);

			int YRulerWidth();

			virtual void keyPressEvent(QKeyEvent* e);
			virtual void keyReleaseEvent( QKeyEvent* e);

		private:
			DisplaySurface* mDisplaySurface;
			QBoxLayout*     mMainLayout;
			Ruler*          mXRuler;
			Ruler*          mYRuler; 
			HScrollGroup*   mHScrollBar;
			VScrollGroup*   mVScrollBar;
			QPushButton*    mToggleColor;
			
			// holes
			QFrame* topLeftHole;
			QFrame* topRightHole;
			QFrame* bottomLeftHole;
			QFrame* bottomRightHole;

			QColor sysColor;
			
			// edit tag dialog
			EditTagDialog* editTagDlg;

			bool            mIsEditTagDialogEnabled;
			bool            mUseFocusColors;

			void Init();
		};
    }
}

#endif

