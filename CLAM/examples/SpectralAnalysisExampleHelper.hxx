#ifndef __SPECTRAL_ANALYSIS_EXAMPLE_HELPER__
#define __SPECTRAL_ANALYSIS_EXAMPLE_HELPER__

#include <string>
#include "Audio.hxx"
#include "DataTypes.hxx"
#include "ProcessingComposite.hxx"
#include "ProcessingConfig.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include "FFT_rfftw.hxx"
#include "FFT_ooura.hxx"
#include "FFT_numrec.hxx"
#include "WindowGenerator.hxx"
#include "ZeroPadder.hxx"
#include "CircularShift.hxx"
#include "AudioMultiplier.hxx"
#include "CircularStreamImpl.hxx"
#include "StreamBuffer.hxx"
#include "Text.hxx"
#include "Filename.hxx"
#include <list>

// forward declarations
namespace CLAM
{
	class Segment;
}

namespace CLAMExamples
{

	class SpectralAnalysisSettings
		: public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( SpectralAnalysisSettings, 8, CLAM::ProcessingConfig );
		/** Required attribute by CLAM::ProcessingConfig */
		DYN_ATTRIBUTE( 0, public, std::string,   Name );
		/** Given in milliseconds */
		DYN_ATTRIBUTE( 1, public, CLAM::TData,         AnalysisWindowWidth );
		/** Given as a percentage between [0..100] */
		DYN_ATTRIBUTE( 2, public, CLAM::TData,         OverlapFactor );
		/** The function name e.g. 'BlackmanHarris92' */
		DYN_ATTRIBUTE( 3, public, std::string,         WindowFunction );
		/** The zero padding factor to be enforced*/
		DYN_ATTRIBUTE( 4, public, unsigned,            ZeroPaddingFactor );
		/** The FFT algorithm to be used */
		DYN_ATTRIBUTE( 5, public, std::string,         FFTAlgorithm );
		/** Output spectrum desired format */
		DYN_ATTRIBUTE( 6, public, CLAM::SpecTypeFlags, OutputSpectrumFormat );
		/** For putting some notes*/
		DYN_ATTRIBUTE( 7, public, CLAM::Text,          Notes );

	protected: // Attributes
		CLAM::TData   mSampleRate;

	protected: // methods
		void DefaultInit();

	public:

		void SetSampleRate( CLAM::TData sr )
		{
			mSampleRate = sr;
		}

		CLAM::TData GetSampleRate( )
		{
			return mSampleRate;
		}
	};

	class SpectralAnalysis
		: public CLAM::ProcessingData
	{
		DYNAMIC_TYPE_USING_INTERFACE( SpectralAnalysis, 3, CLAM::ProcessingData );
		DYN_ATTRIBUTE( 0, public, CLAM::Filename,              AnalyzedAudio );
		DYN_ATTRIBUTE( 1, public, SpectralAnalysisSettings,    AnalysisSettings );
		DYN_CONTAINER_ATTRIBUTE( 2, public, std::list< CLAM::Spectrum >, ResultingFrames, Frame );
	protected:

		void DefaultInit();

	public:
		typedef std::list< CLAM::Spectrum > FramesContainer;
	};

	class SpectralFourierAnalyzer
		: public CLAM::ProcessingComposite
	{
	protected: // attributes
		SpectralAnalysisSettings  mConfig;

		/** Internal data objects */
		CLAM::Audio         mFFTInput;
		CLAM::Audio         mAcquiredSamples;
		CLAM::Audio         mAnalysisWindowSamples;
		CLAM::Audio         mShiftedSamples;
		CLAM::Audio         mWindowedAudioData;
		CLAM::Spectrum      mResultingSpectrum;

		
		CLAM::TSize         mSamplesToAcquire;
		CLAM::TSize         mEffectiveFFTInputSize;
		CLAM::TSize         mHopSize;

		/** Spectal Fourier analysis Processings*/
		CLAM::FFT_base*        mpFFTAlgorithm;
		CLAM::CircularShift    mCircularShift;
		CLAM::WindowGenerator  mWindowGen;
		CLAM::AudioMultiplier  mWindowApplier;
		CLAM::ZeroPadder       mZeroPadder;
		
		/** Ports */
		CLAM::InPortTmpl< CLAM::Audio >                        mInput;
		CLAM::OutPortTmpl< CLAMExamples::SpectralAnalysis  >   mOutput;

		CLAM::AudioStreamBuffer< CLAM::CircularStreamImpl<CLAM::TData> >*  mStreamingBuffer;
		CLAM::WriteStreamRegion*  mDataProvider;
		CLAM::ReadStreamRegion*   mDataConsumer;

	protected: // methods;
		
		/** Convenience methods */
		void AttachChildren();
		bool ConfigureChildren();
		void ConfigureData();
		
		bool ConcreteConfigure( const CLAM::ProcessingConfig& );
		bool ConcreteStart();
		bool ConcreteStop();

	public:

		SpectralFourierAnalyzer();
		virtual ~SpectralFourierAnalyzer();

		const char* GetClassName() const
		{
			return "SpectralFourierAnalyzer";
		}

		const CLAM::ProcessingConfig& GetConfig() const
		{
			return mConfig;
		}

		bool Do();
	};

} // end of namespace CLAMExamples 

#endif // SpectralAnalysisExampleHelper.hxx
