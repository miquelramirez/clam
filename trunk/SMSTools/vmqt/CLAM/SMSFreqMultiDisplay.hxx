#ifndef __SMSFREQMULTIDISPLAY__
#define __SMSFREQMULTIDISPLAY__

#include <CLAM/Spectrum.hxx>
#include <CLAM/SpectralPeakArray.hxx>
#include <CLAM/MultiDisplayPlot.hxx>

namespace CLAM
{
	namespace VM
	{
		class VScrollGroup;

		class SMSFreqMultiDisplay : public MultiDisplayPlot
		{
			Q_OBJECT
		public:
			SMSFreqMultiDisplay(QWidget* parent=0, const char * name = 0, WFlags f = 0 );
			~SMSFreqMultiDisplay();

			void SetSpectrumAndPeaks(const Spectrum& spec, const SpectralPeakArray& peaks, bool update=false);
			void SetSinusoidalSpectrum(const Spectrum& spec, bool update=false);
			void SetResidualSpectrum(const Spectrum& spec, bool update=false);

			void SetForegroundColor(Color c);
			void SetPeaksColor(Color cline,Color cpoint);

			void ShowDisplay(int id);
			void HideDisplay(int id);

			void ShowDisplayOnNewData(bool show);

			void Flush();
			
		signals:
			void dataType(QString);

		protected:
			void CreateControllers();
			void CreateSurfaces();

			void SynchronizeDialPos();
			void SynchronizeInsertMark();
			void SynchronizeRemoveMark();
			void SynchronizeUpdateMark();
			void SynchronizeUpdateTag();

			void DisplayBackgroundBlack();
			void DisplayBackgroundWhite();

		private slots:
			void setMaxVScroll(int);
			void spectrumAndPeaks(bool);
			void sinusoidalSpectrum(bool);
			void residualSpectrum(bool);

		private:
			std::vector<Ruler*> mYRulers;
			VScrollGroup*       mVScrollBar;
			bool                mShowOnNewData;
			bool                mHasMasterData;
			bool                mHasFundamentalData;
			
			enum { MASTER=0, SINUSOIDAL=1, RESIDUAL=2 };

			void InitSMSFreqPlot();

			void SynchronizeVZoom();
			void CheckVisibility();
			void SetYRulersWidth(int w);

			void ConnectVScrollGroup();
			void SynchronizeYRulers();
			void SyncYRuler(int id);
			void HideDisplays();
			void ConnectDataTypeSignal();

		};
	}
}

#endif

