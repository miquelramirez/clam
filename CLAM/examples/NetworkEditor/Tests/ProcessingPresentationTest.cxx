
#include <cppunit/extensions/HelperMacros.h>
#include "BaseLoggable.hxx" // also includes <sstream>
#include "ProcessingController.hxx"
#include "Oscillator.hxx"
#include "ProcessingConfig.hxx"
#include "Qt_ProcessingPresentation.hxx"
#include <string>
#include "Signalv1.hxx"

namespace CLAMTest 
{

class ProcessingPresentationTest;
CPPUNIT_TEST_SUITE_REGISTRATION( ProcessingPresentationTest );


class ProcessingPresentationTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ProcessingPresentationTest );
	
	CPPUNIT_TEST( testProcessingPresentationIsCreatedCorrectly_afterAttached );
	CPPUNIT_TEST( testSignalConfigureProcessingChangesProcessingConfiguration );
	
	CPPUNIT_TEST_SUITE_END();

	CLAM::Oscillator mProc;
	CLAMVM::ProcessingController mController;
	NetworkGUI::Qt_ProcessingPresentation mPresentation;
public:
	ProcessingPresentationTest()
		: mPresentation("foo")
	{
		mController.BindTo( mProc );
		mPresentation.AttachTo(mController);
	}

	void testProcessingPresentationIsCreatedCorrectly_afterAttached()
	{
		CPPUNIT_ASSERT_EQUAL( std::string("foo"), mPresentation.GetNameFromNetwork() );
		CPPUNIT_ASSERT_EQUAL( true, mPresentation.HasInPort("Input Phase Modulation") );
		CPPUNIT_ASSERT_EQUAL( true, mPresentation.HasInPort("Input Frequency Modulation") );
		CPPUNIT_ASSERT_EQUAL( true, mPresentation.HasOutPort("Audio Output") );
		
	}
	void testSignalConfigureProcessingChangesProcessingConfiguration()
	{
		CLAM::OscillatorConfig cfg;
		cfg.SetFrequency(880.0);
		cfg.SetAmplitude(0.3);
		cfg.SetSamplingRate(22050);

		SigSlot::Signalv1< const CLAM::ProcessingConfig &> SignalConfigureProcessing;
		SignalConfigureProcessing.Connect( mPresentation.SlotConfigureProcessing );

		SignalConfigureProcessing.Emit( cfg );
		
		CLAM::OscillatorConfig & cfg2 ((CLAM::OscillatorConfig&)(mProc.GetConfig()));
		CPPUNIT_ASSERT_EQUAL( 880.0f, cfg2.GetFrequency() );
		CPPUNIT_ASSERT_EQUAL( 0.3f, cfg2.GetAmplitude() );
		CPPUNIT_ASSERT_EQUAL( 22050.0f, cfg2.GetSamplingRate() );
	}

};

} // namespace CLAMTest
