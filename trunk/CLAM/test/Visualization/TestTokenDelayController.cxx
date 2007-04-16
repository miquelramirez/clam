#include "Err.hxx"
#include "TokenDelay.hxx"
#include "Spectrum.hxx"
#include "TokenDelayController.hxx"
#include "DummyTDWidget.hxx"
#include "Assert.hxx"
#include <iostream>

using CLAM::Err;
using CLAM::TokenDelay;
using CLAM::TokenDelayConfig;
using CLAM::Spectrum;
using CLAMVM::SpectralTokenDelayController;
using CLAMVM::DummyTDWidget;


// Just check that binding and attaching are working right
void TestBasicUse()
{
	TokenDelayConfig cfg;
	cfg.SetMaxDelay( 3 );
	SpectralTokenDelayController controller;
	TokenDelay<Spectrum> specDelay;
	DummyTDWidget widget;

	specDelay.Configure( cfg );

	specDelay.Start();

	controller.BindTo( specDelay );
	widget.AttachTo( controller.Retrieve("Delay Control") );

	controller.Publish();

	specDelay.Stop();
}

void TestUseCase_ValueSending()
{
	TokenDelayConfig cfg;
	cfg.SetMaxDelay( 3 );
	SpectralTokenDelayController controller;
	TokenDelay<Spectrum> specDelay;
	DummyTDWidget widget;

	specDelay.Configure( cfg );

	specDelay.Start();

	controller.BindTo( specDelay );
	widget.AttachTo( controller.Retrieve("Delay Control") );

	// required to synchronize the Controller and the Widget
	controller.Publish();	
	
	// Some value sending
	widget.EmitValue( 1 );
	controller.Update();
	CLAM_ASSERT( specDelay.GivenDelay() == 1 , 
		     "Test Use Case: Value Sending FAILED - the token delay object did not receive the value" );

	widget.EmitValue( 2 );
	controller.Update();
	CLAM_ASSERT( specDelay.GivenDelay() == 2 , 
		     "Test Use Case: Value Sending FAILED - the token delay object did not receive the value" );


	widget.EmitValue( 5 );
	controller.Update();
	CLAM_ASSERT( specDelay.GivenDelay() == 2 , 
		     "Test Use Case: Value Sending FAILED - the ControlAdapter did not clamp the control value to the valid range" );


	specDelay.Stop();
}

int main( int argc, char** argv )
{

	std::cout << "TestTokenDelayController PASSED!" << std::endl;

	try
	{
		TestBasicUse();
		std::cout << "Basic Use Case test PASSED!" << std::endl;
		TestUseCase_ValueSending();
		std::cout << "Use Case test: Value Sending PASSED!" << std::endl;
	}
	catch ( CLAM::Err& e )
	{
		std::cerr << "A CLAM controlled error has occured" << std::endl;
		e.Print();
		std::cerr << "<==== END OF ERROR MESSAGE" << std::endl;

		return -1;
	}
	catch( std::exception& e )
	{
		std::cerr << "An standard library error has occured" << std::endl;
		std::cerr << e.what() << std::endl;
		std::cerr << "<==== END OF ERROR MESSAGE" << std::endl;

		return -1;
	}
	catch( ... )
	{
		std::cerr << "An unknown error has occured!" << std::endl;
		return -1;
	}
	

	return 0;
}

