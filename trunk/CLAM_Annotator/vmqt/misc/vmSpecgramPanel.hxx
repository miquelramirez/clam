#ifndef __VMQT_SPECGRAM_PANEL_H__
#define __VMQT_SPECGRAM_PANEL_H__

#include <vector>
#include <QtGui/QWidget>
#include "vmDataTypes.hxx"

class QLabel;

namespace CLAM
{
	namespace VM
	{
		class ColorScale;

		class SpecgramPanel : public QWidget
		{
			Q_OBJECT
		public:
			SpecgramPanel(QWidget* parent=0);
			~SpecgramPanel();

			void SetColorScale(const std::vector<Color>& scale);
			
		signals:
			void colorScaleWidthChanged(int);

		public slots:
			void setFixedLabels(QString, QString);
			void updateLabels(QString, QString, QString, QString);
			
		private:
			QLabel* mFrequency;
			QLabel* mDecibels;
			QLabel* mSlice;
			QLabel* mSpectralRange;
			QLabel* mTotalSlices;
			QLabel* mTime;

			ColorScale* mColorScale;

			void InitSpecgramPanel();

			QWidget* GetLeftSide();
			QWidget* GetRightSide();
		};
	}
}

#endif

