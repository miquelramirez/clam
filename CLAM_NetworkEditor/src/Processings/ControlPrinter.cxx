#include "ControlPrinter.hxx"
#include "Factory.hxx"
#include <iostream>

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<CLAM::ControlPrinter> regtControlPrinter( "ControlPrinter" );
	}

	void ControlPrinterConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetIdentifier( "ControlPrinter" );
	}

	ControlPrinter::ControlPrinter()
	: mInControl( "Control In", this )
	{
		Configure( mConfig );	
	}
	
	ControlPrinter::ControlPrinter( const ControlPrinterConfig& cfg )
	: mInControl( "Control In", this )
	{ 
		Configure( cfg );
	}
		
	bool ControlPrinter::ConcreteConfigure( const ProcessingConfig& cfg )
	{ 
		CopyAsConcreteConfig( mConfig, cfg );
		return true; 
	}
	
	bool ControlPrinter::Do()
	{
		std::cout 	<< mConfig.GetIdentifier() 
					<< ": " 
					<< mInControl.GetLastValue()
					<< std::endl;
		return true;
	}
		

		
}
