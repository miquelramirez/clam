#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "similarityHelper.hxx"
#include "FFT_rfftw.hxx"
#include "FFT_numrec.hxx"
#include "FFT_ooura.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "XMLStorage.hxx"

namespace CLAMTest
{
	class FFTFunctionalTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( FFTFunctionalTest );

	class FFTFunctionalTest
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( FFTFunctionalTest );
		CPPUNIT_TEST( test_FFTW_WithPowerOfTwoInput );
		CPPUNIT_TEST( test_NumRec_WithPowerOfTwoInput );
		CPPUNIT_TEST( test_Ooura_WithPowerOfTwoInput );
		CPPUNIT_TEST( test_FFTW_WithNonPowerOfTwoInput );
		CPPUNIT_TEST( test_NumRec_WithNonPowerOfTwoInput );
		CPPUNIT_TEST( test_Ooura_WithNonPowerOfTwoInput );
		CPPUNIT_TEST( test_NumRec_WithComplex );
		CPPUNIT_TEST( test_NumRec_WithPolar );
		CPPUNIT_TEST_SUITE_END();

	protected:

		static double         smEqualityThreshold;
		static bool           smBack2BackDataLoaded;
		static CLAM::Spectrum smReferenceP2Spectrum;
		static CLAM::Spectrum smReferenceNP2Spectrum;

		std::string mPathToTestData;

		CLAM::FFT_base& createFFTObject( const std::string& implementation )
		{
			if ( implementation == "FFTW" )
			{
				static CLAM::FFT_rfftw obj;

				return obj;
			}
			else if ( implementation == "NumRec" )
			{
				static CLAM::FFT_numrec obj;
				
				return obj;
			}
			else if ( implementation == "Ooura" )
			{
				static CLAM::FFT_ooura obj;

				return obj;
			}
			else
				return *(CLAM::FFT_base*)NULL;
		}

		void setupSine_F0400Hz_SR8kHz_1024samples( CLAM::Audio& object )
		{
			const CLAM::TSize samples = 1024;
			const CLAM::TSize sampleRate = 8000;
			const CLAM::TData sineFreq = 400.0f;

			object.SetSize( samples );
			object.SetSampleRate( sampleRate );
			object.SetBeginTime( CLAM::TTime(0.0) );
			object.SetEndTime( (CLAM::TTime(samples)/CLAM::TTime(sampleRate))*1000. );

			for ( CLAM::TSize i=0; i<samples; i++ )
				object.GetBuffer()[i]=0.625+0.5*sin(2.0*sineFreq*400.0*(((float)i)/sampleRate));
		}

		void setupSine_F0400Hz_SR8kHz_884samples( CLAM::Audio& object )
		{
			const CLAM::TSize samples = 884;
			const CLAM::TSize sampleRate = 8000;
			const CLAM::TData sineFreq = 400.0f;

			object.SetSize( samples );
			object.SetSampleRate( sampleRate );
			object.SetBeginTime( CLAM::TTime(0.0) );
			object.SetEndTime( CLAM::TTime(samples)/CLAM::TTime(sampleRate) *1000.);

			for ( CLAM::TSize i=0; i<samples; i++ )
				object.GetBuffer()[i]=0.625+0.5*sin(2.0*sineFreq*400.0*(((float)i)/sampleRate));
			
		}

		void setupSpectrumToStoreFFTOutput( CLAM::Spectrum& object )
		{
			CLAM::SpecTypeFlags specFlags;
			specFlags.bComplex = 1;
			specFlags.bMagPhase = 1;
			specFlags.bPolar = 1;
			specFlags.bMagPhaseBPF = 1;

			object.SetType( specFlags );
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
			char* pathToTestData = getenv("CLAM_TEST_DATA");

			if ( !pathToTestData )
			{
				mPathToTestData ="../../../../../CLAM-TestData/"; 
			}
			else
				mPathToTestData = pathToTestData;

			mPathToTestData += "spectralData/";
			loadBack2BackDataset( mPathToTestData );
		}

		void tearDown()
		{
		}

	private:
		
		void test_FFTW_WithPowerOfTwoInput()
		{
			CLAM::Audio     input;
			CLAM::Spectrum  output;

			CLAM::FFTConfig processingConfig;
			CLAM::FFT_base&      processing = createFFTObject( "FFTW" );

			setupSine_F0400Hz_SR8kHz_1024samples( input );
			setupSpectrumToStoreFFTOutput( output );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );

			processingConfig.SetAudioSize( input.GetSize() );

			processing.Attach( input, output );
			
			processing.Configure( processingConfig );
			
			processing.Start();
			
			processing.Do();

			processing.Stop();

			double similarity = evaluateSimilarity( smReferenceP2Spectrum.GetMagBuffer(),
								output.GetMagBuffer() );

			CPPUNIT_ASSERT( smEqualityThreshold <= similarity );
			CPPUNIT_ASSERT( smReferenceP2Spectrum.GetSpectralRange() 
					== output.GetSpectralRange() );
			// "Output spectrum magnitude buffer suspicious!" );
		}
		
		void test_NumRec_WithPolar()
		{
			CLAM::Audio     input;
			CLAM::SpecTypeFlags flg;
			flg.bMagPhase=0;
			flg.bPolar=1;
			CLAM::Spectrum  output;
			output.SetType(flg);

			CLAM::FFTConfig processingConfig;
			CLAM::FFT_base&      processing = createFFTObject( "NumRec" );

			setupSine_F0400Hz_SR8kHz_1024samples( input );
			//setupSpectrumToStoreFFTOutput( output );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );

			processingConfig.SetAudioSize( input.GetSize() );

			processing.Attach( input, output );
			
			processing.Configure( processingConfig );
			
			processing.Start();
			
			processing.Do();

			processing.Stop();

			flg.bMagPhase=1;
			output.SetTypeSynchronize(flg);
			double similarity = evaluateSimilarity( smReferenceP2Spectrum.GetMagBuffer(),
								output.GetMagBuffer() );

			CPPUNIT_ASSERT( smEqualityThreshold <= similarity );
			CPPUNIT_ASSERT( smReferenceP2Spectrum.GetSpectralRange() 
					== output.GetSpectralRange() );

			// "Output spectrum magnitude buffer suspicious!" );
			
		}
		void test_NumRec_WithComplex()
		{
			CLAM::Audio     input;
			CLAM::SpecTypeFlags flg;
			flg.bMagPhase=0;
			flg.bComplex=1;
			CLAM::Spectrum  output;
			output.SetType(flg);

			CLAM::FFTConfig processingConfig;
			CLAM::FFT_base&      processing = createFFTObject( "NumRec" );

			setupSine_F0400Hz_SR8kHz_1024samples( input );
			//setupSpectrumToStoreFFTOutput( output );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );

			processingConfig.SetAudioSize( input.GetSize() );

			processing.Attach( input, output );
			
			processing.Configure( processingConfig );
			
			processing.Start();
			
			processing.Do();

			processing.Stop();

			flg.bMagPhase=1;
			output.SetTypeSynchronize(flg);
			double similarity = evaluateSimilarity( smReferenceP2Spectrum.GetMagBuffer(),
								output.GetMagBuffer() );

			CPPUNIT_ASSERT( smEqualityThreshold <= similarity );
			CPPUNIT_ASSERT( smReferenceP2Spectrum.GetSpectralRange() 
					== output.GetSpectralRange() );

			// "Output spectrum magnitude buffer suspicious!" );
			
		}
		
		void test_NumRec_WithPowerOfTwoInput()
		{
			CLAM::Audio     input;
			CLAM::Spectrum  output;

			CLAM::FFTConfig processingConfig;
			CLAM::FFT_base&      processing = createFFTObject( "NumRec" );

			setupSine_F0400Hz_SR8kHz_1024samples( input );
			setupSpectrumToStoreFFTOutput( output );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );

			processingConfig.SetAudioSize( input.GetSize() );

			processing.Attach( input, output );
			
			processing.Configure( processingConfig );
			
			processing.Start();
			
			processing.Do();

			processing.Stop();

			double similarity = evaluateSimilarity( smReferenceP2Spectrum.GetMagBuffer(),
								output.GetMagBuffer() );

			CPPUNIT_ASSERT( smEqualityThreshold <= similarity );
			CPPUNIT_ASSERT( smReferenceP2Spectrum.GetSpectralRange() 
					== output.GetSpectralRange() );

			// "Output spectrum magnitude buffer suspicious!" );
			
		}

		void test_Ooura_WithPowerOfTwoInput()
		{
			CLAM::Audio     input;
			CLAM::Spectrum  output;

			CLAM::FFTConfig processingConfig;
			CLAM::FFT_base&      processing = createFFTObject( "Ooura" );

			setupSine_F0400Hz_SR8kHz_1024samples( input );
			setupSpectrumToStoreFFTOutput( output );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );

			processingConfig.SetAudioSize( input.GetSize() );

			processing.Attach( input, output );
			
			processing.Configure( processingConfig );
			
			processing.Start();
			
			processing.Do();

			processing.Stop();

			double similarity = evaluateSimilarity( smReferenceP2Spectrum.GetMagBuffer(),
								output.GetMagBuffer() );

			CPPUNIT_ASSERT( smEqualityThreshold <= similarity );
			CPPUNIT_ASSERT( smReferenceP2Spectrum.GetSpectralRange() 
					== output.GetSpectralRange() );

			// "Output spectrum magnitude buffer suspicious!" );

		}

		void test_FFTW_WithNonPowerOfTwoInput()
		{
			CLAM::Audio     input;
			CLAM::Spectrum  output;

			CLAM::FFTConfig processingConfig;
			CLAM::FFT_base&      processing = createFFTObject( "FFTW" );

			setupSine_F0400Hz_SR8kHz_884samples( input );
			setupSpectrumToStoreFFTOutput( output );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );

			processingConfig.SetAudioSize( input.GetSize() );

			processing.Attach( input, output );
			
			processing.Configure( processingConfig );
			
			processing.Start();
			
			processing.Do();

			processing.Stop();

			double similarity = evaluateSimilarity( smReferenceNP2Spectrum.GetMagBuffer(),
								output.GetMagBuffer() );

			CPPUNIT_ASSERT( smEqualityThreshold <= similarity );
			CPPUNIT_ASSERT( smReferenceP2Spectrum.GetSpectralRange() 
					== output.GetSpectralRange() );

			// "Output spectrum magnitude buffer suspicious!" );
			
		}

		void test_NumRec_WithNonPowerOfTwoInput()
		{
			CLAM::Audio     input;
			CLAM::Spectrum  output;

			CLAM::FFTConfig processingConfig;
			CLAM::FFT_base&      processing = createFFTObject( "NumRec" );

			setupSine_F0400Hz_SR8kHz_884samples( input );
			setupSpectrumToStoreFFTOutput( output );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );

			processingConfig.SetAudioSize( input.GetSize() );

			processing.Attach( input, output );
			
			CPPUNIT_ASSERT_EQUAL( false, processing.Configure( processingConfig ) );
			
		}

		void test_Ooura_WithNonPowerOfTwoInput()
		{
			CLAM::Audio     input;
			CLAM::Spectrum  output;

			CLAM::FFTConfig processingConfig;
			CLAM::FFT_base&      processing = createFFTObject( "Ooura" );

			setupSine_F0400Hz_SR8kHz_884samples( input );
			setupSpectrumToStoreFFTOutput( output );

			output.SetSize( CLAM::TSize(input.GetSize()/2 + 1) );

			processingConfig.SetAudioSize( input.GetSize() );

			processing.Attach( input, output );
			
			CPPUNIT_ASSERT_EQUAL( false, processing.Configure( processingConfig ) );	
		}
	};

	double FFTFunctionalTest::smEqualityThreshold = 0.9999;
	bool FFTFunctionalTest::smBack2BackDataLoaded = false;
	CLAM::Spectrum FFTFunctionalTest::smReferenceP2Spectrum;
	CLAM::Spectrum FFTFunctionalTest::smReferenceNP2Spectrum;
}
