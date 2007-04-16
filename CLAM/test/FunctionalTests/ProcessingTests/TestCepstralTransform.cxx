#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "CepstralTransform.hxx"
#include "MelSpectrum.hxx"
#include "MelCepstrum.hxx"
#include "Array.hxx"
#include "similarityHelper.hxx"
#include "CLAM_Math.hxx"
#include <algorithm>
#include <iostream>

namespace CLAMTest
{


class CepstralTransformTest;

CPPUNIT_TEST_SUITE_REGISTRATION( CepstralTransformTest );

class CepstralTransformTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( CepstralTransformTest );
	CPPUNIT_TEST( test_Configuration );
	CPPUNIT_TEST( test_Do_20to13 );
	CPPUNIT_TEST( test_Do_20to1 );
	CPPUNIT_TEST_SUITE_END();

private:
	// Back 2 back testing data
	static CLAM::MelSpectrum mInputMelSpectrum;
	static CLAM::DataArray   m20To13ExpectedValues;
	static CLAM::DataArray   m20To1ExpectedValues;

	static bool              mBack2BackInit;

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{ 
		if ( !mBack2BackInit )
		{
			mBack2BackInit = true;
			
			CLAM::TData expected20to13Values[] = { 54.965, 0.265, 2.696, 1.462, 
							       1.551, 1.378, 0.311, 0.610, 
							       0.338, 0.526, -0.350, -0.042, -0.038 };

			m20To13ExpectedValues.Resize( 13 );
			m20To13ExpectedValues.SetSize( 13 );

			std::copy( expected20to13Values, expected20to13Values + 13,
				   m20To13ExpectedValues.GetPtr() );

			CLAM::TData expected20to1Values[] = { 54.965 };

			m20To1ExpectedValues.Resize( 1 );
			m20To1ExpectedValues.SetSize( 1 );

			std::copy( expected20to1Values, expected20to1Values + 1,
				   m20To1ExpectedValues.GetPtr() );

			mInputMelSpectrum.SetLowCutoff(0);
			mInputMelSpectrum.SetHighCutoff(22050);
			    
			CLAM::DataArray& melCoeffs = mInputMelSpectrum.GetCoefficients();
			melCoeffs.Resize(20);
			melCoeffs.SetSize(20);
			    
			melCoeffs[0] = 84023.047;
			melCoeffs[1] = 28551.820;
			melCoeffs[2] = 7897.233;
			melCoeffs[3] = 3400.617;
			melCoeffs[4] = 2650.711;
			melCoeffs[5] = 3125.628;
			melCoeffs[6] = 2647.331;
			melCoeffs[7] = 2451.074;
			melCoeffs[8] = 3440.129;
			melCoeffs[9] = 5310.415;
			melCoeffs[10] = 3585.410;
			melCoeffs[11] = 3259.937;
			melCoeffs[12] = 3304.838;
			melCoeffs[13] = 4684.439;
			melCoeffs[14] = 5098.067;
			melCoeffs[15] = 6528.634;
			melCoeffs[16] = 8278.005;
			melCoeffs[17] = 11910.106;
			melCoeffs[18] = 11997.375;
			melCoeffs[19] = 6802.410;



		}
	}

	/// Common clean up, executed after each test method
	void tearDown() { }

private:

	void test_Configuration()
	{
		CLAM::CepstralTransformConfig ctConfig;
		ctConfig.SetNumMelCoefficients(20);
		ctConfig.SetNumCepstrumCoefficients(13);

		CLAM::CepstralTransform proc;
		
		CPPUNIT_ASSERT_EQUAL( true,
				      proc.Configure( ctConfig ) );

	}

	void test_Do_20to13() 
	{

		CLAM::CepstralTransformConfig ctConfig;
		ctConfig.SetNumMelCoefficients(20);
		ctConfig.SetNumCepstrumCoefficients(13);
		
		CLAM::MelCepstrum melCeps;
		
		CLAM::CepstralTransform proc;

		proc.Configure( ctConfig );

		proc.Start();
		
		proc.Do( mInputMelSpectrum, melCeps );

		proc.Stop();

		double sim = evaluateSimilarity( m20To13ExpectedValues,
						 melCeps.GetCoefficients() );

		
		/*
		std::cout << sim;
		std::cout.flush();
		*/

		CPPUNIT_ASSERT( sim >= 0.999 );

	}

	void test_Do_20to1()
	{

		CLAM::CepstralTransformConfig ctConfig;
		ctConfig.SetNumMelCoefficients(20);
		ctConfig.SetNumCepstrumCoefficients(1);
		
		CLAM::MelCepstrum melCeps;
		
		CLAM::CepstralTransform proc;

		proc.Configure( ctConfig );

		proc.Start();
		
		proc.Do( mInputMelSpectrum, melCeps );

		proc.Stop();

		double sim = fabs( m20To1ExpectedValues[0] -  melCeps.GetCoefficients()[0] );

		sim /= m20To1ExpectedValues[0];
		
		sim = 1.0 - sim;

		/*
		std::cout << sim;
		std::cout.flush();
		*/
		
		CPPUNIT_ASSERT( sim >= 0.999 );


	}

};

	CLAM::MelSpectrum CepstralTransformTest::mInputMelSpectrum;
	CLAM::DataArray   CepstralTransformTest::m20To13ExpectedValues;
	CLAM::DataArray   CepstralTransformTest::m20To1ExpectedValues;

	bool            CepstralTransformTest::mBack2BackInit = false;

} // namespace CLAMTest

