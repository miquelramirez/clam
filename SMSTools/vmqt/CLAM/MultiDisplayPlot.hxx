#ifndef __MULTIDISPLAYPLOT__
#define __MULTIDISPLAYPLOT__

#include <CLAM/PlotController.hxx>
#include <CLAM/DisplaySurface.hxx>
#include <CLAM/QtPlot.hxx>

class QFrame;
class QPushButton;

namespace CLAM
{
	namespace VM
	{
		class Ruler;
		class HScrollGroup;
		class EditTagDialog;

		class MultiDisplayPlot : public QtPlot
		{

			Q_OBJECT

		public:
			MultiDisplayPlot(QWidget* parent=0,  const char * name = 0, WFlags f = 0);
			virtual ~MultiDisplayPlot();

			void Label(const std::string& label);
			void Geometry(int x,int y,int w,int h);
			void Show();
			void Hide();

			void SetBackgroundColor(Color c);
			void SetDialColor(Color c);

			virtual void SetMarks(std::vector<unsigned>& marks);
			virtual std::vector<unsigned>& GetMarks();

			void SetMarksColor(Color c);
			std::vector<QString> GetSegmentationTags();

			void SetToggleColorOn(bool on);
			void SwitchDisplayColors(bool sw);

		signals:
			void switchColorsRequested();
			void focusIn();
			
		public slots:
			void switchColors();

		protected:
			std::vector<DisplaySurface*> mSurfaces;
			std::vector<PlotController*> mControllers;

			virtual void CreateControllers()=0;
			virtual void CreateSurfaces()=0;

			virtual void SynchronizeDialPos()=0;
			virtual void SynchronizeInsertMark()=0;
			virtual void SynchronizeRemoveMark()=0;
			virtual void SynchronizeUpdateMark()=0;
			virtual void SynchronizeUpdateTag()=0;

			virtual void DisplayBackgroundBlack()=0;
			virtual void DisplayBackgroundWhite()=0;

			void SyncDialPos(const int& senderId);
			void SyncInsertMarkBySampleIndex(const int& senderId);
			void SyncRemoveMarkBySampleIndex(const int& senderId);
			void SyncUpdateMarkBySampleIndex(const int& senderId);
			void SyncUpdateTag(const int& senderId);

			void SetMasterId(const int& mid);
			void BindToSurfaces();
			void SynchronizeHZoom();

			void SynchronizeFocusIn();
			
			void keyPressEvent(QKeyEvent* e);
			void keyReleaseEvent( QKeyEvent* e);

			void CreateToggleColorFrame();
			void SetToggleColorFrameWidth(const int& w, bool center=false);
			
			void CreateXRuler();
			void CreateHScrollGroup();
			void ConnectXRuler();
			void ConnectHScrollGroup();
			void ConnectEditTagSlot();

			QWidget* GetToggleColorFrame();
			QWidget* GetXRuler();
			QWidget* GetHScrollGroup();

		private slots:
			void setMaxHScroll(int);
			void showEditTagDialog();

		private:
			int           mMasterId;
			Ruler*        mXRuler;
			HScrollGroup* mHScrollBar;
			QFrame*       mToggleContainer;
			QPushButton*  mToggleButton;

			// edit tag dialog
			EditTagDialog* editTagDlg;

			enum { XRULER_HEIGHT=40 };
		  
		};
	}
}

#endif

