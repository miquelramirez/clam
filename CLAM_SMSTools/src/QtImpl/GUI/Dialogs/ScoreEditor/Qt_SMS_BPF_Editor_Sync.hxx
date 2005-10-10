#ifndef __QTSMSBPFEDITORSYNC__
#define __QTSMSBPFEDITORSYNC__

#include <qwidget.h>
#include "BPF.hxx"

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
	class Qt_SMS_BPF_Editor_Sync : public QWidget
	{
		Q_OBJECT
	public:
		Qt_SMS_BPF_Editor_Sync(QWidget* parent=0);
		~Qt_SMS_BPF_Editor_Sync();

		void InitPoints(double yvalue);
		void InitPoints(const CLAM::BPF& originalBPF);
		void Clear();

		void SetGridWidth(double xwidth, double ywidth);
		void SetHorizontalRange(double xmin, double xmax);
		void SetVerticalRange(double ymin, double ymax);

		CLAM::BPF& GetBPF() const;

		void SetSyncToMasterChecked(bool checked);
		void WhatsThis(const char* text);

	public slots:
		void updateXValue(int, float);
		void updateYValue(int, float);
		void addElement(int, float, float);
		void removeElement(int);

	signals:
		void pointsChanged();
		
	private slots:
		void showGrid();
		void snapToGrid();
		void syncToMasterChanged();
		void updateColorScheme(int);
		
	private:
		int                  mId;
		CLAM::VM::BPFEditor* mBPFEditor;
		QCheckBox*           mShowGrid;
		QCheckBox*           mSnapToGrid;
		QCheckBox*           mSyncToMaster;
		QComboBox*           mColorScheme;

		void Init();
	};
}

#endif


