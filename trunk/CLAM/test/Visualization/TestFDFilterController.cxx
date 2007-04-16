#include "Err.hxx"
#include "FDFilterGen.hxx"
#include "FDFilterController.hxx"
#include "DummyFDFWidget.hxx"
#include "Assert.hxx"
#include <iostream>

using CLAM::Err;
using CLAM::FDFilterGen;
using CLAM::FDFilterGenConfig;
using CLAMVM::FDFilterController;
using CLAMVM::DummyFDFWidget;

void TestBasicUse()
{
	FDFilterGenConfig cfg;
	cfg.SetSpectralRange( 22050 );
	FDFilterController controller;
	DummyFDFWidget widget;
	FDFilterGen filter;

	filter.Configure( cfg );

	filter.Start();

	controller.BindTo( filter );
	widget.AttachTo( controller );

	filter.Stop();
}

void TestUseCase_ValueSending()
{
	FDFilterGenConfig cfg;
	cfg.SetSpectralRange( 22050 );
	FDFilterController controller;
	DummyFDFWidget widget;
	FDFilterGen filter;

	filter.Configure( cfg );

	filter.Start();

	controller.BindTo( filter );
	widget.AttachTo( controller );

	widget.SendGain.Emit( -25 );
	controller.Update();
	CLAM_ASSERT( filter.Gain.GetLastValue() == -25, "Filter did not receive gain value!" );
	widget.SendLowCutoff.Emit( 300 );
	controller.Update();
	CLAM_ASSERT( filter.LowCutOff.GetLastValue() == 300 , "Filter did not receive low cutoff freq!" );
	widget.SendHighCutoff.Emit( 1600 );
	controller.Update();
	CLAM_ASSERT( filter.HighCutOff.GetLastValue() == 1600, "Filter did not receive high cutoff freq!" );
	widget.SendStopBand.Emit( -3 );
	controller.Update();
	CLAM_ASSERT( filter.StopBandSlope.GetLastValue() == -3, "Filter did not receive stop band slope!");
	widget.SendPassBand.Emit( 3 );
	controller.Update();
	CLAM_ASSERT( filter.PassBandSlope.GetLastValue() == 3, "Filter did not receive pass band slope!");

	filter.Stop();
	
}

int main( int argc, char** argv )
{

	std::cout << "FDFilterController Test" << std::endl;

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

