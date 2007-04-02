#ifndef __QTSPECTROGRAM__
#define __QTSPECTROGRAM__

#include "Spectrum.hxx"
#include "SingleDisplayPlot.hxx"

class QLabel;

namespace CLAM
{
    namespace VM
    {

		class ColorScale;

		/**
		 * Allows viewing sonogram.
		 *
		 * @ingroup QTVM
		 */

		class QtSpectrogram : public SingleDisplayPlot
		{
			Q_OBJECT

		public:
			QtSpectrogram(QWidget* parent=0,const char * name = 0, WFlags f = 0);
			~QtSpectrogram();

			void SetData(const Array<Spectrum>& specMtx, const double& dur);

		protected:
			void SetPlotController();				
			void Connect();
			void DisplayBackgroundBlack();
			void DisplayBackgroundWhite();

		private slots:
			void updateLabels(QString, QString, QString);
			void updateColorScale(int);

		private:
			QLabel*     mFrequency;
			QLabel*     mDecibels;
			QLabel*     mSlice;
			QLabel*     mFFTSize;
			QLabel*     mTotalSlices;
			QLabel*     mSpectralRange;
			ColorScale* mColorScale;
			bool        mColorSonogram;

			void InitSpectrogram();

			QWidget* LeftGroup();
			QWidget* RightGroup();

			void FillRightGroupLabels();
		};
    }
}

#endif



