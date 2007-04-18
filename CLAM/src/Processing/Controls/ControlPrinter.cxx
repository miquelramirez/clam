#include "ControlPrinter.hxx"
#include "Factory.hxx"
#include <iostream>
#include <sstream>

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
		SetNumberOfInputs(1.);
		SetGuiOnly(false);
	}

	ControlPrinter::ControlPrinter()
		: mGuiOnly(false)
	{
		Configure( mConfig );	
	}
	
	ControlPrinter::ControlPrinter( const ControlPrinterConfig& cfg )
		: mGuiOnly(false)
	{ 
		Configure( cfg );
	}
	ControlPrinter::~ControlPrinter()
	{
		RemoveOldControls();
	}
		
	bool ControlPrinter::ConcreteConfigure( const ProcessingConfig& cfg )
	{ 
		RemoveOldControls();
		CopyAsConcreteConfig( mConfig, cfg );
		if (!mConfig.HasNumberOfInputs())
		{
			mConfig.AddNumberOfInputs();
			mConfig.UpdateData();
			mConfig.SetNumberOfInputs(1.);
		}
		int nInputs = int(mConfig.GetNumberOfInputs());
		if (nInputs < 1) nInputs = 1;
		if (nInputs == 1) 
		{
			// preserve old port name 
			std::list<std::string> names;
			names.push_back("In Control");
			mInControls.Resize(1, names, this);
		} 
		else 
		{
			// multi-port names share user-configured identifier
			mInControls.Resize(nInputs,
				mConfig.GetIdentifier(), this);
		}

		if (!mConfig.HasGuiOnly())
		{
			mConfig.AddGuiOnly();
			mConfig.UpdateData();
		}
		if (mConfig.GetGuiOnly())
			mGuiOnly = true;

		return true; 
	}
	
	bool ControlPrinter::Do()
	{
		if (mGuiOnly)
			return true;
		
		std::string separator = "";
		std::stringstream values;
		for (int i = 0; i < mInControls.Size(); i++) 
		{
			values << separator << mInControls[i].GetLastValue();
			separator = ", ";
		}
		std::cout << mConfig.GetIdentifier() << ": " 
				<< values.str() << std::endl;
		return true;
	}

	void ControlPrinter::RemoveOldControls()
	{
		mInControls.Clear();
		GetInControls().Clear();
	}
}

