#ifndef __VMQT_SPECGRAM_PANEL_H__
#define __VMQT_SPECGRAM_PANEL_H__

#include <vector>
#include <QWidget>
#include "vm_data_types.hxx"

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

			void set_color_scale(const std::vector<Color>& scale);
			
		signals:
			void colorScaleWidthChanged(int);

		public slots:
			void setFixedLabels(QString, QString);
			void updateLabels(QString, QString, QString, QString);
			
		private:
			QLabel* sp_frequency;
			QLabel* sp_decibels;
			QLabel* sp_slice;
			QLabel* sp_spectral_range;
			QLabel* sp_total_slices;
			QLabel* sp_time;

			ColorScale* sp_color_scale;

			void init_specgram_panel();

			QWidget* get_left_side();
			QWidget* get_right_side();
		};
	}
}

#endif

