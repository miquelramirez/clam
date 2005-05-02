#ifndef __BPFEDITOR__
#define __BPFEDITOR__

#include <string>
#include <qwidget.h>
#include "Melody.hxx"
#include "MIDIMelody.hxx"
#include "MediaTime.hxx"
#include "PlayablePlot.hxx"
#include "BPFEditorController.hxx"

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

	class BPFEditor : public QWidget, public PlayablePlot
	{
	    Q_OBJECT

	public:
	    BPFEditor(QWidget* parent=0, const char* name=0, int eFlags=CLAM::VM::AllowAll);
	    ~BPFEditor();
	    
	    void Label(const std::string& label);
	    void Geometry(int x, int y, int w, int h);

	    void SetData(const BPF& bpf);
	    BPF& GetData();

	    void SetXRange(const double& min, const double& max, const EScale& scale=EScale::eLinear);
	    void SetYRange(const double& min, const double& max, const EScale& scale=EScale::eLinear);

	    void SetXScale(const EScale& scale);
	    void SetYScale(const EScale& scale);

	    Melody& GetMelody();
	    MIDIMelody& GetMIDIMelody();
	    
	    void Show();
	    void Hide();

	    void SetActivePlayer(bool active);

	    void SetAudioPtr(const Audio* audio);
	    
        signals:
	    void xValueChanged(int, float);
	    void yValueChanged(int, float);

	    void selectedXPos(double);

	public slots:
	    void setHBounds(double, double);
	    void selectPointFromXCoord(double);

	    void switchColors();

	    void setRegionTime(MediaTime);
	    void setRegionTime(float, float);

	    void stopPendingTasks();

	    void playSimultaneously(bool);

	protected:
	    void keyPressEvent(QKeyEvent* e);
	    void keyReleaseEvent(QKeyEvent* e);

	private slots:
	    void updateLabels(QString, QString);
	    void setMaxVScroll(int);

	    void showPopupMenu();
	    void activePlayer();

	private:
	    int mEFlags;
	    bool mActivePlayer;
	    Ruler *mXRuler, *mYRuler;
	    BPFEditorController* mController;
	    BPFEditorDisplaySurface* mDisplaySurface;
	    QLabel *mXLabelInfo, *mYLabelInfo;
	    QFrame* labelsContainer;
	    QLabel *fixed_x_label, *fixed_y_label;
	    QFont labelFont;
	    
	    enum { EWhiteOverBlack = 0, EBlackOverWhite };
	    
	    int mColorScheme;

	    VScrollGroup *mVScroll;
	    QBoxLayout *mainLayout, *topLayout, *bottomLayout;
	    QFrame *bottomRightHole;

	    Melody mMelody;
	    MIDIMelody mMIDIMelody;

	    bool mHasPlayData;
	    bool mWhiteOverBlackScheme;

	    QPopupMenu* mPopupMenu;

	    void InitBPFEditor();

	    void SetScheme(int scheme);
	    void WhiteOverBlack();
	    void BlackOverWhite();

	    void AdjustLeft(const double& min, const double& max, bool y_axis=true);
	    void CreateVScroll();

	    void ShowPlayer();
	    void HidePlayer();

	    void InitPopupMenu();
	    
	};
    }
}

#endif

