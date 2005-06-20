#include <qlayout.h>
#include <qframe.h>
#include <qlabel.h>
#include "ColorScale.hxx"
#include "SpectrogramPlotController.hxx"
#include "QtSpectrogram.hxx"

namespace CLAM
{
    namespace VM
    {
		QtSpectrogram::QtSpectrogram(QWidget* parent, const char * name, WFlags f) 
			: QtPresentation(parent,name,f)
			, mColorSonogram(true)
		{
			SetPlotController();
			InitSpectrogram();
			Connect();
			SwitchDisplayColors(true);
		}

		QtSpectrogram::~QtSpectrogram()
		{
		}

		void QtSpectrogram::SetData(const Array<Spectrum>& specMtx, const TData& dur)
		{
			((SpectrogramPlotController*)mController)->SetData(specMtx,dur);
			FillRightGroupLabels();
		}

		void QtSpectrogram::keyPressEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Insert:
					((SpectrogramPlotController*)mController)->SetKeyInsertPressed(true); 
					break;
						
				case Qt::Key_Delete:
					((SpectrogramPlotController*)mController)->SetKeyDeletePressed(true); 
					break;

				default:
					break;
			}
		}

		void QtSpectrogram::keyReleaseEvent( QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Insert:
					((SpectrogramPlotController*)mController)->SetKeyInsertPressed(false); 
					break;
						
				case Qt::Key_Delete:
					((SpectrogramPlotController*)mController)->SetKeyDeletePressed(false); 
					break;

				default:
					break;
			}
		}

		void QtSpectrogram::SetPlotController()
		{			
			SetController(new SpectrogramPlotController());		
		}		

		void QtSpectrogram::Connect()
		{
			// Connections
			connect(((SpectrogramPlotController*)mController),SIGNAL(initialYRulerRange(double,double)),this,SLOT(initialYRulerRange(double,double)));
			connect(((SpectrogramPlotController*)mController),SIGNAL(sendLabels(QString,QString,QString)),this,SLOT(updateLabels(QString,QString,QString)));
			connect(mColorScale,SIGNAL(widthChanged(int)),this,SLOT(updateColorScale(int)));
		}

		void QtSpectrogram::InitSpectrogram()
		{
			QBoxLayout* panel = new QVBoxLayout;
			QFrame* hole = new QFrame(this); 
			hole->setFixedHeight(10);
			panel->addWidget(hole);
			QBoxLayout* innerLayout = new QHBoxLayout(panel);
			innerLayout->setSpacing(2);
			innerLayout->addWidget(LeftGroup());
			innerLayout->addWidget(RightGroup());
			AddToMainLayout(panel);
		}

		void QtSpectrogram::DisplayBackgroundBlack()
		{
			((SpectrogramPlotController*)mController)->SetDialColor(VMColor::Red());
			((SpectrogramPlotController*)mController)->SetMarksColor(VMColor::Orange());
			((SpectrogramPlotController*)mController)->SetRenderingMode(CLAM::VM::BlackWhiteSonogram);
			mColorScale->SetScale(((SpectrogramPlotController*)mController)->GetGrayScale(mColorScale->width()));
			mColorSonogram=false;
		}

		void QtSpectrogram::DisplayBackgroundWhite()
		{
			((SpectrogramPlotController*)mController)->SetDialColor(VMColor::Black());
			((SpectrogramPlotController*)mController)->SetMarksColor(VMColor::Red());
			((SpectrogramPlotController*)mController)->SetRenderingMode(CLAM::VM::ColorSonogram);
			mColorScale->SetScale(((SpectrogramPlotController*)mController)->GetColorScale(mColorScale->width()));
			mColorSonogram=true;
		}

		void QtSpectrogram::SetMarks(std::vector<unsigned>& marks)
		{
			((SpectrogramPlotController*)mController)->SetMarks(marks);
		}

		std::vector<unsigned>& QtSpectrogram::GetMarks()
		{
			return ((SpectrogramPlotController*)mController)->GetMarks();
		}

		void QtSpectrogram::SetMarksColor(Color c)
		{
			// empty body
		}

		std::vector<QString> QtSpectrogram::GetSegmentationTags()
		{
			return ((SpectrogramPlotController*)mController)->GetTags();
		}

		QWidget* QtSpectrogram::LeftGroup()
		{
			QFrame* leftGroup = new QFrame(this);
			leftGroup->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			QFontMetrics fm(font());
			int width = fm.width("Frequency (Hz): 00000")+6;
			leftGroup->setFixedSize(width,66);

			QBoxLayout* lfMainLayout = new QVBoxLayout(leftGroup);
			lfMainLayout->setMargin(3);
			QBoxLayout* lfInnerTopLayout = new QHBoxLayout(lfMainLayout);
			QBoxLayout* lfInnerMiddleLayout = new QHBoxLayout(lfMainLayout);
			QBoxLayout* lfInnerBottomLayout = new QHBoxLayout(lfMainLayout);

			QLabel* freqTxtLabel = new QLabel(leftGroup);
			freqTxtLabel->setText("Frequency (Hz): ");
			mFrequency = new QLabel(leftGroup);
			mFrequency->setFixedWidth(leftGroup->width()-freqTxtLabel->width()-6);
			mFrequency->setAlignment(Qt::AlignRight);
			mFrequency->setText("--");
			lfInnerTopLayout->addWidget(freqTxtLabel);
			lfInnerTopLayout->addWidget(mFrequency);

			QLabel* decibelTxtLabel = new QLabel(leftGroup);
			decibelTxtLabel->setText("Decibels (dB): ");
			mDecibels = new QLabel(leftGroup);
			mDecibels->setFixedWidth(leftGroup->width()-decibelTxtLabel->width()-6);
			mDecibels->setAlignment(Qt::AlignRight);
			mDecibels->setText("--");
			lfInnerMiddleLayout->addWidget(decibelTxtLabel);
			lfInnerMiddleLayout->addWidget(mDecibels);

			QLabel* sliceTxtLabel = new QLabel(leftGroup);
			sliceTxtLabel->setText("Slice: ");
			mSlice = new QLabel(leftGroup);
			mSlice->setFixedWidth(leftGroup->width()-sliceTxtLabel->width()-6);
			mSlice->setAlignment(Qt::AlignRight);
			mSlice->setText("--");
			lfInnerBottomLayout->addWidget(sliceTxtLabel);
			lfInnerBottomLayout->addWidget(mSlice);
	    
			return leftGroup;
		}

		QWidget* QtSpectrogram::RightGroup()
		{
			QFrame* rightGroup = new QFrame(this);
			rightGroup->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			rightGroup->setFixedHeight(66);

			QBoxLayout* rfMainLayout = new QVBoxLayout(rightGroup);
			rfMainLayout->setMargin(3);
			QBoxLayout* rfInnerTopLayout = new QHBoxLayout(rfMainLayout);
			rfInnerTopLayout->setSpacing(5);
			QBoxLayout* rfInnerMiddleLayout = new QHBoxLayout(rfMainLayout);
			QBoxLayout* rfInnerBottomLayout = new QHBoxLayout(rfMainLayout);
			rfInnerBottomLayout->setSpacing(3);

			mFFTSize = new QLabel(rightGroup);
			mTotalSlices = new QLabel(rightGroup);
			rfInnerTopLayout->addWidget(mFFTSize);
			rfInnerTopLayout->addWidget(mTotalSlices);
			rfInnerTopLayout->addStretch();

			mSpectralRange = new QLabel(rightGroup);
			rfInnerMiddleLayout->addWidget(mSpectralRange);
			rfInnerMiddleLayout->addStretch();

			QLabel* leftTag = new QLabel(rightGroup);
			leftTag->setText("0 dB");
			QLabel* rightTag = new QLabel(rightGroup);
			rightTag->setText("-150 dB");
			mColorScale = new ColorScale(rightGroup);
			rfInnerBottomLayout->addWidget(leftTag);
			rfInnerBottomLayout->addWidget(mColorScale,1);
			rfInnerBottomLayout->addWidget(rightTag);
			rfInnerBottomLayout->addStretch();
	    
			return rightGroup;
		}

		void QtSpectrogram::updateLabels(QString freq, QString dB, QString slice)
		{
			mFrequency->setText(freq);
			mDecibels->setText(dB);
			mSlice->setText(slice);
		}

		void QtSpectrogram::updateColorScale(int width)
		{
			if(mColorSonogram)
				mColorScale->SetScale(((SpectrogramPlotController*)mController)->GetColorScale(width));
			else
				mColorScale->SetScale(((SpectrogramPlotController*)mController)->GetGrayScale(width));
		
		}

		void QtSpectrogram::FillRightGroupLabels()
		{
			QString fftSize = "FFT Size: "+QString::number(((SpectrogramPlotController*)mController)->FFTSize());
			QString totalSlices = "Slices: "+QString::number(((SpectrogramPlotController*)mController)->TotalSlices());
			QString spectralRange = "Spectral Range (Hz): "+QString::number(((SpectrogramPlotController*)mController)->SpectralRange());
			mFFTSize->setText(fftSize);
			mTotalSlices->setText(totalSlices);
			mSpectralRange->setText(spectralRange);
		}

    }
}

// END

