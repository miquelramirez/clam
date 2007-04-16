#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "similarityHelper.hxx"
#include "MelFilterBank.hxx"
#include "FFT.hxx"
#include "DataTypes.hxx"
#include <algorithm>
#include <iostream>

namespace CLAMTest
{


class MelFilterBankTest;

CPPUNIT_TEST_SUITE_REGISTRATION( MelFilterBankTest );

class MelFilterBankTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( MelFilterBankTest );
	CPPUNIT_TEST( test_Configuration );
	CPPUNIT_TEST( test_Do_FullRangeFilterBank );
	CPPUNIT_TEST( test_Do_SpeechRangeFilterBank );

	CPPUNIT_TEST_SUITE_END();

private:
	// back to back reference objects
	static CLAM::DataArray mFullRangeExpectedMelValues;
	static CLAM::DataArray mSpeechRangeExpectedMelValues;
	static CLAM::Spectrum  mFullRangeSpec;
	static CLAM::Spectrum  mSpeechRangeSpec;

	static bool mBack2BackInit;
	

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{ 
		if ( !mBack2BackInit )
		{
			mBack2BackInit = true;

			// Expected Mel filter bank output values setup
			
			CLAM::TData fullRangeValues[] = {
				8402.759, 5593.134, 1945.569, 1437.992, 1166.799, 
				1021.579, 852.393, 811.837, 718.220, 636.031, 704.546, 
				918.747, 965.266, 942.982, 1237.408, 1801.851, 1749.460, 
				2328.939, 2832.841, 2376.607
			};

			mFullRangeExpectedMelValues.Resize( 20 );
			mFullRangeExpectedMelValues.SetSize( 20 );

			std::copy( fullRangeValues, fullRangeValues + 20, 
				   mFullRangeExpectedMelValues.GetPtr() );
			
			CLAM::TData speechRangeValues[] = { 
				0.000, 80.481, 1297.095, 82.991, 902.328, 275.889, 
				531.991, 510.485, 338.979, 373.507, 445.152, 342.516, 
				290.438, 320.306, 374.704, 286.708, 317.216, 216.559, 
				298.209, 277.066 
			};

			mSpeechRangeExpectedMelValues.Resize( 20 );
			mSpeechRangeExpectedMelValues.SetSize( 20 );

			std::copy( speechRangeValues, speechRangeValues + 20,
				   mSpeechRangeExpectedMelValues.GetPtr() );

			// Input spectrums setup

			CLAM::SpecTypeFlags specFlags;
			specFlags.bMagPhase = 1;

			mFullRangeSpec.SetType(specFlags);
			mFullRangeSpec.SetSize(128);
			mFullRangeSpec.SetSpectralRange(22050);

			CLAM::DataArray& fullRangeMags = mFullRangeSpec.GetMagBuffer();

			fullRangeMags[0] = 3863.799805;
			fullRangeMags[1] = 12563.685547;
			fullRangeMags[2] = 2473.863037;
			fullRangeMags[3] = 1377.575806;
			fullRangeMags[4] = 985.318970;
			fullRangeMags[5] = 807.879456;
			fullRangeMags[6] = 653.121704;
			fullRangeMags[7] = 530.926941;
			fullRangeMags[8] = 484.075195;
			fullRangeMags[9] = 383.188141;
			fullRangeMags[10] = 314.185913;
			fullRangeMags[11] = 331.607544;
			fullRangeMags[12] = 367.209595;
			fullRangeMags[13] = 218.948563;
			fullRangeMags[14] = 274.610565;
			fullRangeMags[15] = 159.711609;
			fullRangeMags[16] = 197.033371;
			fullRangeMags[17] = 237.351715;
			fullRangeMags[18] = 177.763138;
			fullRangeMags[19] = 145.425201;
			fullRangeMags[20] = 44.067760;
			fullRangeMags[21] = 305.115448;
			fullRangeMags[22] = 151.477142;
			fullRangeMags[23] = 147.398636;
			fullRangeMags[24] = 97.936569;
			fullRangeMags[25] = 254.275620;
			fullRangeMags[26] = 252.690063;
			fullRangeMags[27] = 104.618202;
			fullRangeMags[28] = 238.678238;
			fullRangeMags[29] = 238.413589;
			fullRangeMags[30] = 106.487411;
			fullRangeMags[31] = 157.417999;
			fullRangeMags[32] = 196.605453;
			fullRangeMags[33] = 156.383759;
			fullRangeMags[34] = 195.228119;
			fullRangeMags[35] = 66.575058;
			fullRangeMags[36] = 80.477692;
			fullRangeMags[37] = 128.186066;
			fullRangeMags[38] = 155.196106;
			fullRangeMags[39] = 164.427521;
			fullRangeMags[40] = 186.805405;
			fullRangeMags[41] = 125.127724;
			fullRangeMags[42] = 109.505936;
			fullRangeMags[43] = 163.726730;
			fullRangeMags[44] = 149.201904;
			fullRangeMags[45] = 112.674171;
			fullRangeMags[46] = 103.977386;
			fullRangeMags[47] = 300.834686;
			fullRangeMags[48] = 161.853104;
			fullRangeMags[49] = 142.473419;
			fullRangeMags[50] = 255.723755;
			fullRangeMags[51] = 36.553200;
			fullRangeMags[52] = 41.701927;
			fullRangeMags[53] = 274.056519;
			fullRangeMags[54] = 336.059143;
			fullRangeMags[55] = 243.904984;
			fullRangeMags[56] = 184.314789;
			fullRangeMags[57] = 197.449814;
			fullRangeMags[58] = 85.940903;
			fullRangeMags[59] = 296.949280;
			fullRangeMags[60] = 131.487427;
			fullRangeMags[61] = 112.573540;
			fullRangeMags[62] = 204.185013;
			fullRangeMags[63] = 76.627083;
			fullRangeMags[64] = 168.378326;
			fullRangeMags[65] = 105.379623;
			fullRangeMags[66] = 101.306183;
			fullRangeMags[67] = 108.629196;
			fullRangeMags[68] = 95.293953;
			fullRangeMags[69] = 155.516388;
			fullRangeMags[70] = 253.814713;
			fullRangeMags[71] = 272.925171;
			fullRangeMags[72] = 252.411789;
			fullRangeMags[73] = 182.065308;
			fullRangeMags[74] = 146.791016;
			fullRangeMags[75] = 201.107864;
			fullRangeMags[76] = 65.739845;
			fullRangeMags[77] = 126.323753;
			fullRangeMags[78] = 145.190552;
			fullRangeMags[79] = 97.506729;
			fullRangeMags[80] = 126.490814;
			fullRangeMags[81] = 351.613800;
			fullRangeMags[82] = 179.530670;
			fullRangeMags[83] = 261.162109;
			fullRangeMags[84] = 95.853531;
			fullRangeMags[85] = 291.796692;
			fullRangeMags[86] = 256.122589;
			fullRangeMags[87] = 181.163788;
			fullRangeMags[88] = 140.050858;
			fullRangeMags[89] = 254.090790;
			fullRangeMags[90] = 129.594788;
			fullRangeMags[91] = 111.446365;
			fullRangeMags[92] = 295.206085;
			fullRangeMags[93] = 151.116455;
			fullRangeMags[94] = 266.566742;
			fullRangeMags[95] = 27.534611;
			fullRangeMags[96] = 51.423008;
			fullRangeMags[97] = 229.461517;
			fullRangeMags[98] = 272.580170;
			fullRangeMags[99] = 165.507751;
			fullRangeMags[100] = 42.286980;
			fullRangeMags[101] = 125.545723;
			fullRangeMags[102] = 120.389374;
			fullRangeMags[103] = 257.682861;
			fullRangeMags[104] = 189.390701;
			fullRangeMags[105] = 107.361794;
			fullRangeMags[106] = 315.615295;
			fullRangeMags[107] = 146.639496;
			fullRangeMags[108] = 22.579048;
			fullRangeMags[109] = 112.955009;
			fullRangeMags[110] = 175.849106;
			fullRangeMags[111] = 140.709534;
			fullRangeMags[112] = 65.522446;
			fullRangeMags[113] = 94.679405;
			fullRangeMags[114] = 75.674004;
			fullRangeMags[115] = 76.928978;
			fullRangeMags[116] = 73.280907;
			fullRangeMags[117] = 76.281586;
			fullRangeMags[118] = 81.804390;
			fullRangeMags[119] = 76.965645;
			fullRangeMags[120] = 71.684120;
			fullRangeMags[121] = 65.579536;
			fullRangeMags[122] = 77.021393;
			fullRangeMags[123] = 75.680740;
			fullRangeMags[124] = 79.639420;
			fullRangeMags[125] = 81.398521;
			fullRangeMags[126] = 70.366844;
			fullRangeMags[127] = 67.170120;

			mSpeechRangeSpec.SetType(specFlags);
			mSpeechRangeSpec.SetSize(128);
			mSpeechRangeSpec.SetSpectralRange(22050);

			CLAM::DataArray& speechMags = mSpeechRangeSpec.GetMagBuffer();

			speechMags[0] = 3863.799805;
			speechMags[1] = 12563.685547;
			speechMags[2] = 2473.863037;
			speechMags[3] = 1377.575806;
			speechMags[4] = 985.318970;
			speechMags[5] = 807.879456;
			speechMags[6] = 653.121704;
			speechMags[7] = 530.926941;
			speechMags[8] = 484.075195;
			speechMags[9] = 383.188141;
			speechMags[10] = 314.185913;
			speechMags[11] = 331.607544;
			speechMags[12] = 367.209595;
			speechMags[13] = 218.948563;
			speechMags[14] = 274.610565;
			speechMags[15] = 159.711609;
			speechMags[16] = 197.033371;
			speechMags[17] = 237.351715;
			speechMags[18] = 177.763138;
			speechMags[19] = 145.425201;
			speechMags[20] = 44.067760;
			speechMags[21] = 305.115448;
			speechMags[22] = 151.477142;
			speechMags[23] = 147.398636;
			speechMags[24] = 97.936569;
			speechMags[25] = 254.275620;
			speechMags[26] = 252.690063;
			speechMags[27] = 104.618202;
			speechMags[28] = 238.678238;
			speechMags[29] = 238.413589;
			speechMags[30] = 106.487411;
			speechMags[31] = 157.417999;
			speechMags[32] = 196.605453;
			speechMags[33] = 156.383759;
			speechMags[34] = 195.228119;
			speechMags[35] = 66.575058;
			speechMags[36] = 80.477692;
			speechMags[37] = 128.186066;
			speechMags[38] = 155.196106;
			speechMags[39] = 164.427521;
			speechMags[40] = 186.805405;
			speechMags[41] = 125.127724;
			speechMags[42] = 109.505936;
			speechMags[43] = 163.726730;
			speechMags[44] = 149.201904;
			speechMags[45] = 112.674171;
			speechMags[46] = 103.977386;
			speechMags[47] = 300.834686;
			speechMags[48] = 161.853104;
			speechMags[49] = 142.473419;
			speechMags[50] = 255.723755;
			speechMags[51] = 36.553200;
			speechMags[52] = 41.701927;
			speechMags[53] = 274.056519;
			speechMags[54] = 336.059143;
			speechMags[55] = 243.904984;
			speechMags[56] = 184.314789;
			speechMags[57] = 197.449814;
			speechMags[58] = 85.940903;
			speechMags[59] = 296.949280;
			speechMags[60] = 131.487427;
			speechMags[61] = 112.573540;
			speechMags[62] = 204.185013;
			speechMags[63] = 76.627083;
			speechMags[64] = 168.378326;
			speechMags[65] = 105.379623;
			speechMags[66] = 101.306183;
			speechMags[67] = 108.629196;
			speechMags[68] = 95.293953;
			speechMags[69] = 155.516388;
			speechMags[70] = 253.814713;
			speechMags[71] = 272.925171;
			speechMags[72] = 252.411789;
			speechMags[73] = 182.065308;
			speechMags[74] = 146.791016;
			speechMags[75] = 201.107864;
			speechMags[76] = 65.739845;
			speechMags[77] = 126.323753;
			speechMags[78] = 145.190552;
			speechMags[79] = 97.506729;
			speechMags[80] = 126.490814;
			speechMags[81] = 351.613800;
			speechMags[82] = 179.530670;
			speechMags[83] = 261.162109;
			speechMags[84] = 95.853531;
			speechMags[85] = 291.796692;
			speechMags[86] = 256.122589;
			speechMags[87] = 181.163788;
			speechMags[88] = 140.050858;
			speechMags[89] = 254.090790;
			speechMags[90] = 129.594788;
			speechMags[91] = 111.446365;
			speechMags[92] = 295.206085;
			speechMags[93] = 151.116455;
			speechMags[94] = 266.566742;
			speechMags[95] = 27.534611;
			speechMags[96] = 51.423008;
			speechMags[97] = 229.461517;
			speechMags[98] = 272.580170;
			speechMags[99] = 165.507751;
			speechMags[100] = 42.286980;
			speechMags[101] = 125.545723;
			speechMags[102] = 120.389374;
			speechMags[103] = 257.682861;
			speechMags[104] = 189.390701;
			speechMags[105] = 107.361794;
			speechMags[106] = 315.615295;
			speechMags[107] = 146.639496;
			speechMags[108] = 22.579048;
			speechMags[109] = 112.955009;
			speechMags[110] = 175.849106;
			speechMags[111] = 140.709534;
			speechMags[112] = 65.522446;
			speechMags[113] = 94.679405;
			speechMags[114] = 75.674004;
			speechMags[115] = 76.928978;
			speechMags[116] = 73.280907;
			speechMags[117] = 76.281586;
			speechMags[118] = 81.804390;
			speechMags[119] = 76.965645;
			speechMags[120] = 71.684120;
			speechMags[121] = 65.579536;
			speechMags[122] = 77.021393;
			speechMags[123] = 75.680740;
			speechMags[124] = 79.639420;
			speechMags[125] = 81.398521;
			speechMags[126] = 70.366844;
			speechMags[127] = 67.170120;


		}
	}

	/// Common clean up, executed after each test method
	void tearDown() 
	{ 
	}

private:
	
	void test_Configuration()
	{
		CLAM::MelFilterBankConfig mfbConfig;
		mfbConfig.SetSpectrumSize(256);
		mfbConfig.SetSpectralRange(22050);
		mfbConfig.SetLowCutoff(300);
		mfbConfig.SetHighCutoff(3400);
		mfbConfig.SetNumBands(20);

		CLAM::MelFilterBank proc;

		CPPUNIT_ASSERT_EQUAL( true,
				      proc.Configure( mfbConfig ) );

	}

	void test_Do_FullRangeFilterBank()
	{
		CLAM::MelFilterBankConfig mfbConfig;
		mfbConfig.SetSpectrumSize( mFullRangeSpec.GetSize());
		mfbConfig.SetSpectralRange(mFullRangeSpec.GetSpectralRange());
		mfbConfig.SetLowCutoff(0);
		mfbConfig.SetHighCutoff(22050);
		mfbConfig.SetNumBands(20);
		    
		CLAM::MelSpectrum melSpec;
		    
		CLAM::MelFilterBank proc;

		proc.Configure( mfbConfig );

		proc.Start( );
		
		proc.Do( mFullRangeSpec, melSpec );

		proc.Stop();

		double sim = evaluateSimilarity( mFullRangeExpectedMelValues,
						 melSpec.GetCoefficients() );


		/*
		std::cout << sim;
		std::cout.flush();
		*/
		
		CPPUNIT_ASSERT( sim >= 0.999 );
	}

	void test_Do_SpeechRangeFilterBank()
	{
		CLAM::MelFilterBankConfig mfbConfig;
		mfbConfig.SetSpectrumSize(mSpeechRangeSpec.GetSize());
		mfbConfig.SetSpectralRange(mSpeechRangeSpec.GetSpectralRange());
		mfbConfig.SetLowCutoff(300);
		mfbConfig.SetHighCutoff(3400);
		mfbConfig.SetNumBands(20);
		
		CLAM::MelSpectrum melSpec;
		
		CLAM::MelFilterBank proc;

		proc.Configure( mfbConfig );

		proc.Start();
		
		proc.Do( mSpeechRangeSpec, melSpec );

		proc.Stop();

		double sim = evaluateSimilarity( mSpeechRangeExpectedMelValues,
						 melSpec.GetCoefficients() );
		/*
		std::cout << sim;
		std::cout.flush();
		*/

		CPPUNIT_ASSERT( sim >= 0.999  );		
	}
	

};

	CLAM::DataArray MelFilterBankTest::mFullRangeExpectedMelValues;
	CLAM::DataArray MelFilterBankTest::mSpeechRangeExpectedMelValues;
	CLAM::Spectrum  MelFilterBankTest::mFullRangeSpec;
	CLAM::Spectrum  MelFilterBankTest::mSpeechRangeSpec;

	bool            MelFilterBankTest::mBack2BackInit = false;
	



} // namespace CLAMTest

