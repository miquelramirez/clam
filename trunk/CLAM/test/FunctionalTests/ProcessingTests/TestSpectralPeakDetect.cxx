#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "similarityHelper.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"
#include "SpectralPeakDetect.hxx"
#include "XMLStorage.hxx"

namespace CLAMTest
{
	class SpectralPeakDetectFunctionalTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( SpectralPeakDetectFunctionalTest );

	class SpectralPeakDetectFunctionalTest
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( SpectralPeakDetectFunctionalTest );
		CPPUNIT_TEST( test_ProofOfConcept );
		CPPUNIT_TEST_SUITE_END();

	protected:
		static double                     smSimilarityThreshold;
		static bool                       smBack2BackDataLoaded;
		static CLAM::SpectralPeakArray    smReferenceSpectralPeakArray;
		static CLAM::Spectrum             smReferenceSpectrum;

		std::string                       mPathToTestData;

	protected: // Auxiliary methods
		
		static void loadBack2BackDataset( const std::string& pathToTestData )
		{
			CLAM::XMLStorage   storageIface;

			if ( !smBack2BackDataLoaded )
			{
				
				storageIface.Restore( smReferenceSpectralPeakArray,
						      pathToTestData + "OneSineSpectrum_RectWindow_P2_Peaks.xml" );

				storageIface.Restore( smReferenceSpectrum,
						      pathToTestData + "OneSineSpectrum_RectWindow_P2.xml" );

				smBack2BackDataLoaded = true;
			}
		}

	public:

		void setUp()
		{
			mPathToTestData = "../../../../../CLAM-TestData/spectralData/";
			loadBack2BackDataset( mPathToTestData );
		}

		void tearDown()
		{
		}

	private:

		void test_ProofOfConcept()
		{
			CLAM::SpectralPeakArray detectedPeaks;
			detectedPeaks.SetScale( CLAM::EScale::eLog );

			CLAM::SpectralPeakDetectConfig processingConfig; // default
			CLAM::SpectralPeakDetect       processing;

			processing.Configure( processingConfig );

			processing.Attach( smReferenceSpectrum, detectedPeaks );
			
			processing.Start();

			processing.Do();

			processing.Stop();

			CLAM::DataArray flattenedReference;
			CLAM::DataArray flattenedResult;

			flattenSpectralPeakArray( smReferenceSpectralPeakArray,
						  flattenedReference );

			flattenSpectralPeakArray( detectedPeaks,
						  flattenedResult );

			double similarity = evaluateSimilarity( flattenedReference,
								flattenedResult );

			CPPUNIT_ASSERT( smSimilarityThreshold <= similarity );
		}
		
	};

	double SpectralPeakDetectFunctionalTest::smSimilarityThreshold = 0.9999;
	bool   SpectralPeakDetectFunctionalTest::smBack2BackDataLoaded = false;
	CLAM::Spectrum SpectralPeakDetectFunctionalTest::smReferenceSpectrum;
	CLAM::SpectralPeakArray SpectralPeakDetectFunctionalTest::smReferenceSpectralPeakArray;
}
