#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "vm_color_scale.hxx"
#include "vm_specgram_panel.hxx"

namespace CLAM
{
	namespace VM
	{
		SpecgramPanel::SpecgramPanel(QWidget* parent)
			: QWidget(parent)
		{
			init_specgram_panel();
		}

		SpecgramPanel::~SpecgramPanel()
		{
		}

		void SpecgramPanel::set_color_scale(const std::vector<Color>& scale)
		{
			sp_color_scale->set_scale(scale);
		}

		void SpecgramPanel::setFixedLabels(QString specRange, QString totalSlices)
		{
			sp_spectral_range->setText("Spectral Range (Hz): "+specRange);
			sp_total_slices->setText("Slices: "+totalSlices);
		}

		void SpecgramPanel::updateLabels(QString freq, QString db, QString slice, QString time)
		{
			sp_frequency->setText(freq);
			sp_decibels->setText(db);
			sp_slice->setText(slice);
			(time=="--") ? sp_time->setText("  t: "+time) : sp_time->setText("  t: "+time+" s.");
		}

		void SpecgramPanel::init_specgram_panel()
		{
			QBoxLayout* layout = new QHBoxLayout(this);
			layout->setMargin(0);
			layout->setSpacing(2);
			layout->addWidget(get_left_side());
			layout->addWidget(get_right_side());

			connect(sp_color_scale,SIGNAL(widthChanged(int)),this,SIGNAL(colorScaleWidthChanged(int)));
		}

		QWidget* SpecgramPanel::get_left_side()
		{
			QFrame* leftGroup = new QFrame(this);
			leftGroup->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			QFontMetrics fm(font());
			int width = fm.width("Frequency (Hz): 00000")+6;
			leftGroup->setFixedSize(width,66);

			QVBoxLayout* lfMainLayout = new QVBoxLayout(leftGroup);
			lfMainLayout->setMargin(3);
			lfMainLayout->setSpacing(0);
			QHBoxLayout* lfInnerTopLayout = new QHBoxLayout;
			QHBoxLayout* lfInnerMiddleLayout = new QHBoxLayout;
			QHBoxLayout* lfInnerBottomLayout = new QHBoxLayout;
			lfInnerTopLayout->setMargin(0);
			lfInnerMiddleLayout->setMargin(0);
			lfInnerBottomLayout->setMargin(0);
			lfInnerTopLayout->setSpacing(0);
			lfInnerMiddleLayout->setSpacing(0);
			lfInnerBottomLayout->setSpacing(0);

			QLabel* freqTxtLabel = new QLabel(leftGroup);
			freqTxtLabel->setText("Frequency (Hz): ");
			sp_frequency = new QLabel(leftGroup);
			sp_frequency->setFixedWidth(leftGroup->width()-freqTxtLabel->width()-6);
			sp_frequency->setAlignment(Qt::AlignRight);
			sp_frequency->setText("--");
			lfInnerTopLayout->addWidget(freqTxtLabel);
			lfInnerTopLayout->addWidget(sp_frequency);

			QLabel* decibelTxtLabel = new QLabel(leftGroup);
			decibelTxtLabel->setText("Decibels (dB): ");
			sp_decibels = new QLabel(leftGroup);
			sp_decibels->setFixedWidth(leftGroup->width()-decibelTxtLabel->width()-6);
			sp_decibels->setAlignment(Qt::AlignRight);
			sp_decibels->setText("--");
			lfInnerMiddleLayout->addWidget(decibelTxtLabel);
			lfInnerMiddleLayout->addWidget(sp_decibels);

			QLabel* sliceTxtLabel = new QLabel(leftGroup);
			sliceTxtLabel->setText("Slice: ");
			sp_slice = new QLabel(leftGroup);
			sp_slice->setFixedWidth(leftGroup->width()-sliceTxtLabel->width()-6);
			sp_slice->setAlignment(Qt::AlignRight);
			sp_slice->setText("--");
			lfInnerBottomLayout->addWidget(sliceTxtLabel);
			lfInnerBottomLayout->addWidget(sp_slice);

			lfMainLayout->addLayout(lfInnerTopLayout);
			lfMainLayout->addLayout(lfInnerMiddleLayout);
			lfMainLayout->addLayout(lfInnerBottomLayout);

			return leftGroup;
		}

		QWidget* SpecgramPanel::get_right_side()
		{
			QFrame* rightGroup = new QFrame(this);
			rightGroup->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			rightGroup->setFixedHeight(66);

			QVBoxLayout* rfMainLayout = new QVBoxLayout(rightGroup);
			rfMainLayout->setMargin(3);
			rfMainLayout->setSpacing(0);
			QHBoxLayout* rfInnerTopLayout = new QHBoxLayout;
			rfInnerTopLayout->setMargin(0);
			rfInnerTopLayout->setSpacing(5);
			QHBoxLayout* rfInnerMiddleLayout = new QHBoxLayout;
			rfInnerMiddleLayout->setMargin(0);
			rfInnerMiddleLayout->setSpacing(0);
			QHBoxLayout* rfInnerBottomLayout = new QHBoxLayout;
			rfInnerBottomLayout->setMargin(0);
			rfInnerBottomLayout->setSpacing(3);

			sp_total_slices = new QLabel(rightGroup);
			sp_time = new QLabel(rightGroup);
			rfInnerTopLayout->addWidget(sp_total_slices);
			rfInnerTopLayout->addWidget(sp_time);
			rfInnerTopLayout->addStretch();

			sp_spectral_range = new QLabel(rightGroup);
			rfInnerMiddleLayout->addWidget(sp_spectral_range);
			rfInnerMiddleLayout->addStretch();

			QLabel* leftTag = new QLabel(rightGroup);
			leftTag->setText("0 dB");
			QLabel* rightTag = new QLabel(rightGroup);
			rightTag->setText("-150 dB");
			sp_color_scale = new ColorScale(rightGroup);
			rfInnerBottomLayout->addWidget(leftTag);
			rfInnerBottomLayout->addWidget(sp_color_scale,1);
			rfInnerBottomLayout->addWidget(rightTag);
			rfInnerBottomLayout->addStretch();

			sp_time->setText("  t: --");
			sp_total_slices->setText("Slices: --");
			sp_spectral_range->setText("Spectral Range (Hz): --");

			rfMainLayout->addLayout(rfInnerTopLayout);
			rfMainLayout->addLayout(rfInnerMiddleLayout);
			rfMainLayout->addLayout(rfInnerBottomLayout);
	    
			return rightGroup;
		}
	}
}

// END
