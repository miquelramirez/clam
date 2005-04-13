#ifndef __BPFEDITOR__
#define __BPFEDITOR__

#include <string>
#include <qwidget.h>
#include "BPFEditorController.hxx"

class QLabel;
class QFrame;

namespace CLAM
{
    namespace VM
    {
	class Ruler;
	class BPFEditorDisplaySurface;

	class BPFEditor : public QWidget
	{
	    Q_OBJECT

	public:
	    BPFEditor(QWidget* parent=0, const char* name=0, int eFlags=CLAM::VM::AllowAll);
	    ~BPFEditor();
	    
	    void Label(const std::string& label);
	    void Geometry(int x, int y, int w, int h);

	    void SetData(const BPF& bpf);
	    BPF& GetData();

	    void SetXRange(const double& min, const double& max);
	    void SetYRange(const double& min, const double& max);
	    
	    void Show();
	    void Hide();
	    
        signals:
	    void xValueChanged(int, float);
	    void yValueChanged(int, float);

	protected:
	    void keyPressEvent(QKeyEvent* e);
	    void keyReleaseEvent(QKeyEvent* e);

	private slots:
	    void updateLabels(QString, QString);

	private:
	    int mEFlags;
	    Ruler *mXRuler, *mYRuler;
	    BPFEditorController* mController;
	    BPFEditorDisplaySurface* mDisplaySurface;
	    QLabel *mXLabelInfo, *mYLabelInfo;
	    QFrame* labelsContainer;
	    QLabel *fixed_x_label, *fixed_y_label;
	    QFont labelFont;
	    
	    enum { EWhiteOverBlack = 0, EBlackOverWhite };
	    
	    int mColorScheme;

	    void InitBPFEditor();

	    void SetScheme(int scheme);
	    void WhiteOverBlack();
	    void BlackOverWhite();

	    void AdjustLeft(const double& min, const double& max, bool y_axis=true);
	    //void AdjustLeft(const double& min, const double& max);
	};
    }
}

#endif

