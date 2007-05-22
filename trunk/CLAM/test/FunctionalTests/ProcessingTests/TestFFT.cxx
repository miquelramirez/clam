#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "similarityHelper.hxx"
#include "FFT_base.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "XMLStorage.hxx"
#include "Factory.hxx"

namespace CLAMTest
{
	class FFTFunctionalTest
		: public CppUnit::TestFixture
	{
	protected:

		static double         smEqualityThreshold;
		static bool           smBack2BackDataLoaded;
		static CLAM::Spectrum smReferenceP2Spectrum;
		static CLAM::Spectrum smReferenceNP2Spectrum;

		std::string mPathToTestData;

		CLAM::FFT_base * mProcessing;
		virtual const std::string getProcessing()=0;
		void setupSine_F0400Hz_SR8kHz( CLAM::Audio& audio, int nSamples ) 
		{
			const CLAM::TSize sampleRate = 8000;
			const CLAM::TData sineFreq = 400.0f;

			audio.SetSize( nSamples );
			audio.SetSampleRate( sampleRate );
			audio.SetBeginTime( CLAM::TTime(0.0) );

			for ( CLAM::TSize i=0; i<nSamples; i++ )
				audio.GetBuffer()[i]=0.625+0.5*sin(2.0*sineFreq*400.0*(((float)i)/sampleRate));
		}

		void setupSine_F0400Hz_SR8kHz_1024samples( CLAM::Audio& audio )
		{
			setupSine_F0400Hz_SR8kHz(audio, 1024);
		}

		void setupSine_F0400Hz_SR8kHz_884samples( CLAM::Audio& audio )
		{
			setupSine_F0400Hz_SR8kHz(audio, 884);
		}

		void setupSpectrumToStoreFFTOutput( CLAM::Spectrum& spectrum )
		{
			CLAM::SpecTypeFlags specFlags;
			specFlags.bComplex = 1;
			specFlags.bMagPhase = 1;
			specFlags.bPolar = 1;
			specFlags.bMagPhaseBPF = 1;

			spectrum.SetType( specFlags );
		}

		static void loadBack2BackDataset( const std::string& pathToTestData )
		{
			if ( !smBack2BackDataLoaded )
			{

				CLAM::XMLStorage::Restore( smReferenceP2Spectrum,
							   pathToTestData + "OneSineSpectrum_RectWindow_P2.xml" );
				CLAM::XMLStorage::Restore( smReferenceNP2Spectrum,
							   pathToTestData + "OneSineSpectrum_RectWindow_NP2.xml" );

				smBack2BackDataLoaded = true;
			}
		}

	public:
		void setUp()
		{
			mProcessing = (CLAM::FFT_base*)CLAM::Factory<CLAM::Processing>::GetInstance().Create(getProcessing());
			mPathToTestData = GetTestDataDirectory("spectralData/");
			loadBack2BackDataset( mPathToTestData );
		}

		void tearDown()
		{
			delete mProcessing;
		}

	protected:
		void test_WithPowerOfTwoInput()
		{
			CLAM::Audio     input;
			CLAM::Spectrum  output;

			CLAM::FFTConfig processingConfig;

			setupSine_F0400Hz_SR8kHz_1024samples( input );
			setupSpectrumToStoreFFTOutput( output );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );
			processingConfig.SetAudioSize( input.GetSize() );

			mProcessing->Configure( processingConfig );
			mProcessing->Start();
			mProcessing->Do( input, output );
			mProcessing->Stop();

			double similarity = evaluateSimilarity( smReferenceP2Spectrum.GetMagBuffer(),
								output.GetMagBuffer() );
			CPPUNIT_ASSERT( smEqualityThreshold <= similarity );
			CPPUNIT_ASSERT( smReferenceP2Spectrum.GetSpectralRange() 
					== output.GetSpectralRange() );
		}
		
		
		void test_WithComplex()
		{
			CLAM::Audio     input;
			CLAM::SpecTypeFlags flg;
			flg.bMagPhase=0;
			flg.bComplex=1;
			CLAM::Spectrum  output;
			output.SetType(flg);

			CLAM::FFTConfig processingConfig;

			setupSine_F0400Hz_SR8kHz_1024samples( input );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );

			processingConfig.SetAudioSize( input.GetSize() );

			mProcessing->Configure( processingConfig );
			
			mProcessing->Start();
			mProcessing->Do(input,output);
			mProcessing->Stop();

			flg.bMagPhase=1;
			output.SetTypeSynchronize(flg);
			double similarity = evaluateSimilarity( smReferenceP2Spectrum.GetMagBuffer(),
								output.GetMagBuffer() );

			CPPUNIT_ASSERT( smEqualityThreshold <= similarity );
			CPPUNIT_ASSERT( smReferenceP2Spectrum.GetSpectralRange() 
					== output.GetSpectralRange() );
		}

		void test_WithBPF()
		{
			CLAM::Audio     input;
			CLAM::SpecTypeFlags flg;
			flg.bMagPhase=0;
			flg.bMagPhaseBPF=1;
			CLAM::Spectrum  output;
			output.SetType(flg);

			CLAM::FFTConfig processingConfig;

			setupSine_F0400Hz_SR8kHz_1024samples( input );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );

			processingConfig.SetAudioSize( input.GetSize() );

			
			mProcessing->Configure( processingConfig );
			mProcessing->Start();
			mProcessing->Do(input,output);
			mProcessing->Stop();

			flg.bMagPhase=1;
			output.SetTypeSynchronize(flg);
			double similarity = evaluateSimilarity( smReferenceP2Spectrum.GetMagBuffer(),
								output.GetMagBuffer() );

			CPPUNIT_ASSERT( smReferenceP2Spectrum.GetSpectralRange() 
					== output.GetSpectralRange() );
			CPPUNIT_ASSERT( smReferenceP2Spectrum.GetSize() 
					== output.GetSize() );
			CPPUNIT_ASSERT( smEqualityThreshold <= similarity );
		}

		void test_WithPolar()
		{
			CLAM::Audio     input;
			CLAM::SpecTypeFlags flg;
			flg.bMagPhase=0;
			flg.bPolar=1;
			CLAM::Spectrum  output;
			output.SetType(flg);

			CLAM::FFTConfig processingConfig;

			setupSine_F0400Hz_SR8kHz_1024samples( input );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );

			processingConfig.SetAudioSize( input.GetSize() );

			mProcessing->Configure( processingConfig );
			
			mProcessing->Start();
			mProcessing->Do(input,output);
			mProcessing->Stop();

			flg.bMagPhase=1;
			output.SetTypeSynchronize(flg);
			double similarity = evaluateSimilarity( smReferenceP2Spectrum.GetMagBuffer(),
								output.GetMagBuffer() );

			CPPUNIT_ASSERT( smEqualityThreshold <= similarity );
			CPPUNIT_ASSERT( smReferenceP2Spectrum.GetSpectralRange() 
					== output.GetSpectralRange() );
		}

		void test_WithNonPowerOfTwoInput()
		{
			CLAM::Audio     input;
			CLAM::Spectrum  output;

			CLAM::FFTConfig processingConfig;

			setupSine_F0400Hz_SR8kHz_884samples( input );
			setupSpectrumToStoreFFTOutput( output );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );

			processingConfig.SetAudioSize( input.GetSize() );

			mProcessing->Configure( processingConfig );
			
			mProcessing->Start();
			mProcessing->Do( input, output );
			mProcessing->Stop();

			double similarity = evaluateSimilarity( smReferenceNP2Spectrum.GetMagBuffer(),
								output.GetMagBuffer() );

			CPPUNIT_ASSERT( smEqualityThreshold <= similarity );
			CPPUNIT_ASSERT( smReferenceP2Spectrum.GetSpectralRange() 
					== output.GetSpectralRange() );
		}

		void test_WithNonPowerOfTwoInput_fails()
		{
			CLAM::Audio     input;
			CLAM::Spectrum  output;

			CLAM::FFTConfig processingConfig;

			setupSine_F0400Hz_SR8kHz_884samples( input );
			setupSpectrumToStoreFFTOutput( output );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );

			processingConfig.SetAudioSize( input.GetSize() );

			CPPUNIT_ASSERT_EQUAL( false, mProcessing->Configure( processingConfig ) );
		}
	};

	double FFTFunctionalTest::smEqualityThreshold = 0.99999999999; // More accurate as close to 1
	bool FFTFunctionalTest::smBack2BackDataLoaded = false;
	CLAM::Spectrum FFTFunctionalTest::smReferenceP2Spectrum;
	CLAM::Spectrum FFTFunctionalTest::smReferenceNP2Spectrum;

#ifdef USE_FFTW3
	class FFTW3FunctionalTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( FFTW3FunctionalTest );
	class FFTW3FunctionalTest : public FFTFunctionalTest
	{
		CPPUNIT_TEST_SUITE( FFTW3FunctionalTest );
		CPPUNIT_TEST( test_WithPowerOfTwoInput );
		CPPUNIT_TEST( test_WithNonPowerOfTwoInput );
		CPPUNIT_TEST( test_WithComplex );
		CPPUNIT_TEST( test_WithPolar );
		CPPUNIT_TEST( test_WithBPF );
		CPPUNIT_TEST_SUITE_END();

	protected:
		virtual const std::string getProcessing()
		{
			return "FFT_fftw3";
		}
	};
#endif
#ifdef USE_FFTW
	class FFTWFunctionalTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( FFTWFunctionalTest );
	class FFTWFunctionalTest : public FFTFunctionalTest
	{
		CPPUNIT_TEST_SUITE( FFTWFunctionalTest );
		CPPUNIT_TEST( test_WithPowerOfTwoInput );
		CPPUNIT_TEST( test_WithNonPowerOfTwoInput );
		CPPUNIT_TEST( test_WithComplex );
		CPPUNIT_TEST( test_WithPolar );
		CPPUNIT_TEST( test_WithBPF );
		CPPUNIT_TEST_SUITE_END();

	protected:
		virtual const std::string getProcessing()
		{
			return "FFT_rfftw";
		}
	};
#endif
	class FFTOouraFunctionalTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( FFTOouraFunctionalTest );
	class FFTOouraFunctionalTest : public FFTFunctionalTest
	{
		CPPUNIT_TEST_SUITE( FFTOouraFunctionalTest );
		CPPUNIT_TEST( test_WithPowerOfTwoInput );
		CPPUNIT_TEST( test_WithNonPowerOfTwoInput_fails );
		CPPUNIT_TEST( test_WithComplex );
		CPPUNIT_TEST( test_WithPolar );
		CPPUNIT_TEST( test_WithBPF );
		CPPUNIT_TEST_SUITE_END();

	protected:
		virtual const std::string getProcessing()
		{
			return "FFT_ooura";
		}
	};

	class FFTNumRecFunctionalTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( FFTNumRecFunctionalTest );
	class FFTNumRecFunctionalTest : public FFTFunctionalTest
	{
		CPPUNIT_TEST_SUITE( FFTNumRecFunctionalTest );
		CPPUNIT_TEST( test_WithPowerOfTwoInput );
		CPPUNIT_TEST( test_WithNonPowerOfTwoInput_fails );
		CPPUNIT_TEST( test_WithComplex );
		CPPUNIT_TEST( test_WithPolar );
		CPPUNIT_TEST( test_WithBPF );
		CPPUNIT_TEST_SUITE_END();

	protected:
		virtual const std::string getProcessing()
		{
			return "FFT_numrec";
		}
	};


}

