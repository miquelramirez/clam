#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "similarityHelper.hxx"
#include "XMLStorage.hxx"

#ifdef USE_FFTW
#include "IFFT_rfftw.hxx"

namespace CLAMTest
{
	class IFFTFunctionalTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( IFFTFunctionalTest );

	class IFFTFunctionalTest 
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( IFFTFunctionalTest );
		CPPUNIT_TEST( test_FFTW_WithPowerOfTwoSignalSpectrum );
		CPPUNIT_TEST( test_FFTW_WithNonPowerOfTwoSignalSpectrum );
		CPPUNIT_TEST_SUITE_END();

	protected:

		static double          smEqualityThreshold;
		static bool            smBack2BackDataLoaded;
		static CLAM::Spectrum  smReferenceP2Spectrum;
		static CLAM::Spectrum  smReferenceNP2Spectrum;

		std::string            mPathToTestData;

		
	protected: // Auxiliary methods
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
			mPathToTestData = GetTestDataDirectory("spectralData/");
			loadBack2BackDataset( mPathToTestData );
		}

		void tearDown()
		{
		}

	private:

		void test_FFTW_WithPowerOfTwoSignalSpectrum()
		{
			// The signal that should be
			CLAM::Audio  expectedOutput;
			// The signal synthesized by the IFFT processing
			CLAM::Audio  actualOutput;

			setupSine_F0400Hz_SR8kHz_1024samples( expectedOutput );

			actualOutput.SetSize( expectedOutput.GetSize() );
			actualOutput.SetSampleRate( expectedOutput.GetSampleRate() );

			CLAM::IFFTConfig processingConfig;
			CLAM::IFFT_rfftw processing;

			processingConfig.SetAudioSize( expectedOutput.GetSize() );

			processing.Configure( processingConfig );

			//(*processing.GetInPorts().Begin())->Attach( smReferenceP2Spectrum );
			//(*processing.GetOutPorts().Begin())->Attach( actualOutput );
			
			processing.Start();

			processing.Do( smReferenceP2Spectrum, actualOutput );

			processing.Stop();

			double similarity = evaluateSimilarity( expectedOutput.GetBuffer(),
								actualOutput.GetBuffer() );

			CPPUNIT_ASSERT( smEqualityThreshold <= similarity );
		}

		void test_FFTW_WithNonPowerOfTwoSignalSpectrum()
		{
			// The signal that should be
			CLAM::Audio  expectedOutput;
			// The signal synthesized by the IFFT processing
			CLAM::Audio  actualOutput;

			setupSine_F0400Hz_SR8kHz_884samples( expectedOutput );

			actualOutput.SetSize( expectedOutput.GetSize() );
			actualOutput.SetSampleRate( expectedOutput.GetSampleRate() );

			CLAM::IFFTConfig processingConfig;
			CLAM::IFFT_rfftw processing;

			processingConfig.SetAudioSize( expectedOutput.GetSize() );

			processing.Configure( processingConfig );

			//(*processing.GetInPorts().Begin())->Attach( smReferenceNP2Spectrum );
			//(*processing.GetOutPorts().Begin())->Attach( actualOutput );

			processing.Start();

			processing.Do( smReferenceNP2Spectrum, actualOutput );

			processing.Stop();

			double similarity = evaluateSimilarity( expectedOutput.GetBuffer(),
								actualOutput.GetBuffer() );

			CPPUNIT_ASSERT( smEqualityThreshold <= similarity );
		}
		
	};
	
	double IFFTFunctionalTest::smEqualityThreshold = 0.9999;
	bool IFFTFunctionalTest::smBack2BackDataLoaded = false;
	CLAM::Spectrum IFFTFunctionalTest::smReferenceP2Spectrum;
	CLAM::Spectrum IFFTFunctionalTest::smReferenceNP2Spectrum;	
}

#endif


