#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "Spectrum.hxx" // CLAM
#include "SpecTypeFlags.hxx" // CLAM
#include "SpectrumConfig.hxx" // CLAM

namespace CLAMTest
{

class SpectrumConversionsTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SpectrumConversionsTest );

class SpectrumConversionsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SpectrumConversionsTest );
	CPPUNIT_TEST( testDefaultSpectrum );
	CPPUNIT_TEST( testMagPhase2BPF2MagPhase );
	CPPUNIT_TEST( testComplex2BPF2Complex );
	CPPUNIT_TEST( testComplex2BPF2ComplexWithDifferentSpectrums );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:

	void testDefaultSpectrum()
	{
		CLAM::Spectrum spec;
		CPPUNIT_ASSERT( CLAM::TData(22050) == spec.GetSpectralRange() );
		CPPUNIT_ASSERT( 0 == spec.GetSize() );
		CPPUNIT_ASSERT( 0 == spec.GetBPFSize() );

		CLAM::SpecTypeFlags typeFlags;
		spec.GetType( typeFlags );
		CPPUNIT_ASSERT( true == typeFlags.bMagPhase );
		CPPUNIT_ASSERT( false == typeFlags.bComplex );
		CPPUNIT_ASSERT( false == typeFlags.bPolar );
		CPPUNIT_ASSERT( false == typeFlags.bMagPhaseBPF );

	}




	void testMagPhase2BPF2MagPhase()
	{
		CLAM::Spectrum spec;
		CLAM::SpectrumConfig config;
		config.SetSize(22050);
		spec.Configure(config);

		CLAM::DataArray & mag = spec.GetMagBuffer();
		CLAM::DataArray & phase = spec.GetPhaseBuffer();
		for(int i=0; i<22050; i++)
		{
			mag[i] = CLAM::TData(0.1);
			phase[i] = CLAM::TData(-0.1);
		}
		CLAM::SpecTypeFlags bpfAndMagphaseFlag;
		bpfAndMagphaseFlag.bMagPhaseBPF = true;
		bpfAndMagphaseFlag.bMagPhase=false;
		spec.SetTypeSynchronize(bpfAndMagphaseFlag);

		CLAM::SpecTypeFlags magphaseFlag;
	
		CPPUNIT_ASSERT_EQUAL(22050, spec.GetBPFSize() );

		CLAM::BPF & magBpf = spec.GetMagBPF();
		CLAM::BPF & phaseBpf = spec.GetPhaseBPF();
		for(int i=0; i<22050; i++)
		{
			CPPUNIT_ASSERT( CLAM::TData(0.1) == magBpf.GetValue(i) );
			CPPUNIT_ASSERT( CLAM::TData(-0.1) == phaseBpf.GetValue(i) );
		}
		spec.SetTypeSynchronize(magphaseFlag);
		for(int i=0; i<22050; i++)
		{
			CPPUNIT_ASSERT( mag[i] == CLAM::TData(0.1));
			CPPUNIT_ASSERT( phase[i] == CLAM::TData(-0.1));
		}
	}
	void testComplex2BPF2Complex()
	{	
		CLAM::Spectrum spec;
		CLAM::SpectrumConfig config;
		config.SetSize(22050);
		CLAM::SpecTypeFlags complexFlag;
		complexFlag.bMagPhase=false;
		complexFlag.bComplex=true;
		config.SetType(complexFlag);
		spec.Configure(config);

		CLAM::Array<CLAM::Complex> & complexArray = spec.GetComplexArray();
		for(int i=0; i<22050; i++)
		{
			complexArray[i].SetReal(CLAM::TData(1));
			complexArray[i].SetImag(CLAM::TData(0));
		}
		CLAM::SpecTypeFlags bpfFlag;
		bpfFlag.bMagPhase=false;
		bpfFlag.bMagPhaseBPF = true;
		spec.SetTypeSynchronize(bpfFlag);

			
		CPPUNIT_ASSERT_EQUAL(22050, spec.GetBPFSize() );

		CLAM::BPF & magBpf = spec.GetMagBPF();
		CLAM::BPF & phaseBpf = spec.GetPhaseBPF();
		for(int i=0; i<22050; i++)
		{
			CPPUNIT_ASSERT( CLAM::TData(1) == magBpf.GetValue(i) );
			CPPUNIT_ASSERT( CLAM::TData(0) == phaseBpf.GetValue(i) );
		}
		spec.SetTypeSynchronize(complexFlag);
		for(int i=0; i<22050; i++)
		{
			CPPUNIT_ASSERT( complexArray[i].Real() == CLAM::TData(1));
			CPPUNIT_ASSERT( complexArray[i].Imag() == CLAM::TData(0));
		}
	}	
	void testComplex2BPF2ComplexWithDifferentSpectrums()
	{	
		CLAM::Spectrum complexSpec;
		CLAM::SpectrumConfig config;
		config.SetSize(22050);
		CLAM::SpecTypeFlags complexFlag;
		complexFlag.bMagPhase=false;
		complexFlag.bComplex=true;
		config.SetType(complexFlag);
		complexSpec.Configure(config);

		CLAM::Spectrum BPFSpec;
		CLAM::SpectrumConfig config2;
		config2.SetSize(22050);
		CLAM::SpecTypeFlags bpfFlag;
		bpfFlag.bMagPhase=false;
		bpfFlag.bMagPhaseBPF=true;
		config2.SetType(bpfFlag);
		BPFSpec.Configure(config2);

		CLAM::Array<CLAM::Complex> & complexArray = complexSpec.GetComplexArray();
		for(int i=0; i<22050; i++)
		{
			complexArray[i].SetReal(CLAM::TData(1));
			complexArray[i].SetImag(CLAM::TData(0));
		}
	
		BPFSpec.SynchronizeTo(complexSpec);

			
		CPPUNIT_ASSERT_EQUAL(22050, BPFSpec.GetBPFSize() );

		CLAM::BPF & magBpf = BPFSpec.GetMagBPF();
		CLAM::BPF & phaseBpf = BPFSpec.GetPhaseBPF();
		for(int i=0; i<22050; i++)
		{
			CPPUNIT_ASSERT( CLAM::TData(1) == magBpf.GetValue(i) );
			CPPUNIT_ASSERT( CLAM::TData(0) == phaseBpf.GetValue(i) );
		}
		complexSpec.SynchronizeTo(BPFSpec);
		for(int i=0; i<22050; i++)
		{
			CPPUNIT_ASSERT( complexArray[i].Real() == CLAM::TData(1));
			CPPUNIT_ASSERT( complexArray[i].Imag() == CLAM::TData(0));
		}
	}	
	void testMagPhase2Complex2MagPhase()
	{

	}

	void testMagPhase2Polar2MagPhase()
	{

	}

	void testPolar2Complex2Polar()
	{

	}
};






} // namespace CLAMTest

