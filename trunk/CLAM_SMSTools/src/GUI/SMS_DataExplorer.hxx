#ifndef __SMS_DATAEXPLORER__
#define __SMS_DATAEXPLORER__

#include "Slotv2.hxx"
#include "Slotv1.hxx"
#include "Slotv0.hxx"
#include "Signalv1.hxx"
#include "DataTypes.hxx"
#include "LogMagSpectrumAdapter.hxx"
#include "AudioAdapter.hxx"
#include "SinTracksAdapter.hxx"
#include "SpectralPeakArrayAdapter.hxx"

#include <FL/Fl_Widget.H>

class Fl_Smart_Tile;

namespace CLAM
{
	class Audio;
	class Frame;
	class Segment;
}

namespace CLAMVM
{
	class Fl_SMS_Browsable_Playable_Audio;
	class Fl_SMS_Spectrum;
	class Fl_SMS_SinTracks_Browser;
	class Fl_SMS_SpectrumAndPeaks;

	class SMS_DataExplorer
	{
	public:
		
		SMS_DataExplorer();
		~SMS_DataExplorer();

		SigSlot::Slotv1< const CLAM::Segment& >   NewSegment;
		SigSlot::Slotv2< const CLAM::Frame&, bool >     NewFrame;
		SigSlot::Slotv1< const CLAM::Audio& >     NewInputAudio;
		SigSlot::Slotv1< const CLAM::Audio& >     NewSynthesizedAudio;
		SigSlot::Slotv1< const CLAM::Audio& >     NewSynthesizedSinusoidal;
		SigSlot::Slotv1< const CLAM::Audio& >     NewSynthesizedResidual;
		SigSlot::Signalv1< double >               SelectedTime;

		SigSlot::Slotv0                           ShowInputAudio;
		SigSlot::Slotv0                           ShowSinTracks;
		SigSlot::Slotv0                           ShowSpectrumAndPeaks;
		SigSlot::Slotv0                           ShowSinusoidalSpectrum;
		SigSlot::Slotv0                           ShowResidualSpectrum;
		SigSlot::Slotv0                           ShowSynthesizedAudio;
		SigSlot::Slotv0                           ShowSynthesizedSinusoidal;
		SigSlot::Slotv0                           ShowSynthesizedResidual;

		void SetCanvas( Fl_Smart_Tile* pCanvas );
		void CloseAll();

	protected:
		
		// "Proxy" Slot that is always connected to SelectedFrame signal 
		SigSlot::Slotv1< double >                 SelectedTimeChanged; 

		void OnNewSegment( const CLAM::Segment& );
		void OnNewFrame( const CLAM::Frame& , bool fullAnalysisData );
		void OnNewInputAudio( const CLAM::Audio& );
		void OnNewSynthesizedAudio( const CLAM::Audio& );
		void OnNewSynthesizedSinusoidal( const CLAM::Audio& );
		void OnNewSynthesizedResidual( const CLAM::Audio& );
		void OnSelectedTimeChanged( double time  );

		void OnShowInputAudio();
		void OnShowSinTracks();
		void OnShowSpectrumAndPeaks();
		void OnShowSinusoidalSpectrum();
		void OnShowResidualSpectrum();
		void OnShowSynthesizedAudio();
		void OnShowSynthesizedSinusoidal();
		void OnShowSynthesizedResidual();

		// Smart Tile detach callback methods
		void Detach();
		static void sDetachCb( Fl_Widget*, void* );

	private:
		
		// Adapters
		LogMagSpectrumAdapter        mSpectrumAdapter;
		LogMagSpectrumAdapter        mSinusoidalSpectrumAdapter;
		LogMagSpectrumAdapter        mResidualSpectrumAdapter;

		SinTracksAdapter             mSinusoidalTracksAdapter;
		
		SpectralPeakArrayAdapter     mPeakArrayAdapter;

		AudioAdapter                 mOriginalAudioAdapter;
		AudioAdapter                 mSynthesizedAudioAdapter;
		AudioAdapter                 mSynthesizedSinusoidalAdapter;
		AudioAdapter                 mSynthesizedResidualAdapter;

		// Widgets
		Fl_SMS_Browsable_Playable_Audio*            mpOriginalAudioWidget;
		Fl_SMS_Browsable_Playable_Audio*            mpSynthesizedAudioWidget;
		Fl_SMS_Browsable_Playable_Audio*            mpSynthesizedResidualWidget;
		Fl_SMS_Browsable_Playable_Audio*            mpSynthesizedSinusoidalWidget;
		Fl_SMS_SpectrumAndPeaks*         mpSpectrumAndPeaksWidget;
		Fl_SMS_Spectrum*                 mpSinusoidalSpectrum;
		Fl_SMS_Spectrum*                 mpResidualSpectrum;
		Fl_SMS_SinTracks_Browser*        mpSegmentSinTracks;
		Fl_Smart_Tile*                   mpCanvas;

		double                           mCurrentFrameCenterTime;
	};

}

#endif // SMS_DataExplorer.hxx
