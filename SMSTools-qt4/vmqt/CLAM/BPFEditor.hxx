#ifndef __BPFEDITOR__
#define __BPFEDITOR__

#include <string>
#include <qwidget.h>
#include <CLAM/Melody.hxx>
#include <CLAM/MIDIMelody.hxx>
#include <CLAM/MediaTime.hxx>
#include <CLAM/PlayablePlot.hxx>
#include <CLAM/BPFEditorController.hxx>
#include <CLAM/Slotv1.hxx>

using SigSlot::Slotv1;

class QLabel;
class QFrame;
class QBoxLayout;
class QPopupMenu;

namespace CLAM
{
	class Audio;
	
    namespace VM
    {
		class Ruler;
		class BPFEditorDisplaySurface;
		class VScrollGroup;
		class HScrollGroup;
		class ListItemChooser;
		
		/**
		 * Editable/auralizable plot.
		 *
		 * @ingroup QTVM
		 */

		class BPFEditor : public QWidget, public PlayablePlot
		{
			Q_OBJECT

		public:
			BPFEditor(int eFlags=CLAM::VM::AllowAll, QWidget* parent=0, const char* name=0, WFlags f=0);
			~BPFEditor();
	    
			void Label(const std::string& label);
			void Geometry(int x, int y, int w, int h);

			void SetXRange(const double& min, const double& max, const EScale& scale=EScale::eLinear);
			void SetYRange(const double& min, const double& max, const EScale& scale=EScale::eLinear);

			void SetXScale(const EScale& scale);
			void SetYScale(const EScale& scale);

			void Show();
			void Hide();

			void SetActivePlayer(bool active);
			void SetAudioPtr(const Audio* audio);

			/*
			 * methods to use when you wish a single BPF only
			 * use them also to set the first BPF (default).
			 */
			void SetData(const BPF& bpf);
			BPF& GetData();
			Melody& GetMelody();
			MIDIMelody& GetMIDIMelody();

			/*
			 * methods to add/get data using a key (multiple BPF management)
			 */
			void AddData(const std::string& key, const BPF& bpf);
			void SetDataColor(const std::string& key, const Color& lines_color, const Color& handlers_color);
			BPF& GetData(const std::string& key);
			Melody& GetMelody(const std::string& key);
			MIDIMelody& GetMIDIMelody(const std::string& key);

			void ActiveLocator(bool active);
			void ShowGrid(bool show);
			void SnapToGrid(bool snap);
			void SetGridSteps(double xstep, double ystep);

			void WhiteOverBlack();
			void BlackOverWhite();

			void AllowInsertPoints(bool allow);
			void AllowDeletePoints(bool allow);
			void AllowModifyPoints(bool allow);

			void UseFocusColors();
	    
		signals:
			void xValueChanged(int, float);
			void yValueChanged(int, float);
			void selectedXPos(double);
			void currentPlayingTime(float);
			void stopPlaying(float);
			void pointsChanged();
			void elementAdded(int, float, float);
			void elementRemoved(int);

		public slots:
			void setHBounds(double, double);
			void selectPointFromXCoord(double);
			void switchColors();
			void setRegionTime(MediaTime);
			void setRegionTime(float, float);
			void stopPendingTasks();
			void playSimultaneously(bool);
			void setCurrentPlayingTime(float);
			void receivedStopPlaying(float);

			void updateXValue(int, float);
			void updateYValue(int, float);
			void addElement(int, float, float);
			void removeElement(int);

		protected:
			void keyPressEvent(QKeyEvent* e);
			void keyReleaseEvent(QKeyEvent* e);
			void showEvent(QShowEvent* e);
			void hideEvent(QHideEvent* e);

		private slots:
			void updateLabels(QString, QString);
			void setMaxVScroll(int);
			void setMaxHScroll(int);
			void showPopupMenu();
			void activePlayer();
			void showChooseBPFDlg();

			void focusInColor();
			void focusOutColor();

		private:
			int                      mEFlags;
			bool                     mActivePlayer;
			Ruler*                   mXRuler; 
			Ruler*                   mYRuler;
			BPFEditorController*     mController;
			BPFEditorDisplaySurface* mDisplaySurface;
			QLabel*                  mXLabelInfo; 
			QLabel*                  mYLabelInfo;

			QFrame* labelsContainer;
			QLabel *fixed_x_label, *fixed_y_label;
			QFont labelFont;
	    
			enum { EWhiteOverBlack = 0, EBlackOverWhite };
	    
			int mColorScheme;

			VScrollGroup* mVScroll;
			HScrollGroup* mHScroll;

			QBoxLayout *mainLayout, *topLayout, *middleLayout, *bottomLayout;
			QFrame *topLeftHole, *topRightHole, *bottomRightHole, *playerHole;

			Melody     mMelody;
			MIDIMelody mMIDIMelody;

			bool mWhiteOverBlackScheme;
			bool mUseFocusColors;

			QPopupMenu*      mPopupMenu;
			ListItemChooser* mChooseBPFDialog;

			Slotv1<TData> mSlotPlayingTimeReceived;
			Slotv1<TData> mSlotStopPlayingReceived;

			void InitBPFEditor();

			void SetScheme(int scheme);
			
			void AdjustLeft(const double& min, const double& max, bool y_axis=true);
			void CreateVScroll();
			void CreateHScroll();

			void ShowPlayer();
			void HidePlayer();
			
			void InitPopupMenu();
	    
			void PlayingTime(TData time);
			void StopPlaying(TData time);

		};
    }
}

#endif

