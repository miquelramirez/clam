#ifndef __QTSMSBPFEDITOR__
#define __QTSMSBPFEDITOR__

#include <qwidget.h>
#include <CLAM/BPF.hxx>

class QCheckBox;
class QComboBox;

namespace CLAM
{
	namespace VM
	{
		class BPFEditor;
	}
}

namespace QtSMS
{
	class Qt_SMS_BPF_Editor : public QWidget
	{
		Q_OBJECT
	public:
		Qt_SMS_BPF_Editor(QWidget* parent=0);
		~Qt_SMS_BPF_Editor();

		void InitPoints(double yvalue);
		void InitPoints(const CLAM::BPF& originalBPF);
		void Clear();

		void SetGridWidth(double xwidth, double ywidth);
		void SetHorizontalRange(double xmin, double xmax);
		void SetVerticalRange(double ymin, double ymax);

		CLAM::BPF& GetBPF() const;

		void WhatsThis(const char* text);

	signals:
		void pointsChanged();
		void xValueChanged(int, float);
		void yValueChanged(int, float);
		void elementAdded(int, float, float);
		void elementRemoved(int);

	private slots:
		void showGrid();
		void snapToGrid();
		void updateColorScheme(int);
		
	private:
		CLAM::VM::BPFEditor* mBPFEditor;
		QCheckBox*           mShowGrid;
		QCheckBox*           mSnapToGrid;
		QComboBox*           mColorScheme;

		void Init();
	};
}

#endif


