#include "ControlPrinterTyped.hxx"
#include <iostream>
#include <sstream>
#include "ProcessingFactory.hxx"

#include <CLAM/MIDIMessage.hxx>

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "ControlPrinterTyped",
		"category", "Controls",
		"description", "ControlPrinterTyped",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ControlPrinterTyped> reg = metadata;
}

	void ControlPrinterTypedConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetIdentifier( "ControlPrinterTyped" );
		SetTypesMask("f");
		SetGuiOnly(true);
	}

	ControlPrinterTyped::ControlPrinterTyped()
	{
		Configure( _config );	
	}
	
	ControlPrinterTyped::ControlPrinterTyped( const ControlPrinterTypedConfig& cfg )
	{ 
		Configure( cfg );
	}
	ControlPrinterTyped::~ControlPrinterTyped()
	{
		RemoveOldControls();
	}

	const unsigned int ControlPrinterTyped::GetInputsNumber() const
	{
		unsigned nInputs;
		std::string typespec;
		typespec=_config.GetTypesMask();
		for (nInputs=0; nInputs<typespec.size();nInputs++)
		{
			const char type = typespec[nInputs];
			if (type != 's' and type != 'i' 
				and type != 'f' and type != 'd'
				and type != 'h' and type != 'M')
				return 0; // return 0 if there is any non-compatible type
		}
		return nInputs;
	}

	void ControlPrinterTyped::ResizeControls(unsigned nInputs, const std::string & baseName)
	{
		std::list<std::string> names;
		if (nInputs == 1) 
		{
			names.push_back("In Control");
			ResizeControls(nInputs, names);
			return;
		}
		for (unsigned i=0; i<nInputs; i++)
		{
			std::ostringstream os;
			os << baseName << "_" << i;
			names.push_back(os.str());
		}
		ResizeControls(nInputs, names);
	}
	InControlBase * ControlPrinterTyped::createControl(const std::string & type, const std::string & name)
	{
		if (type=="s")
			return new InControl<std::string> (name,this);
		if (type=="f")
			return new FloatInControl (name,this);
		if (type=="d")
			return new InControl<double> (name,this);
		if (type=="i")
			return new InControl<int> (name,this);
		if (type=="M")
			return new InControl<MIDI::Message> (name,this);
		// TODO: Decide whether ASSERTing (contract) or throw (control) 
		return 0;
	}
	void ControlPrinterTyped::ResizeControls(unsigned nInputs, const std::list<std::string> & names)
	{
		ClearControls();
		unsigned i=0;
		for (std::list<std::string>::const_iterator it = names.begin(); it != names.end(); it++)
		{
			std::string type;
			type=_config.GetTypesMask()[i];
			mInControls.push_back(createControl(type,*it));
			i++;
		}
	}
	void ControlPrinterTyped::ClearControls()
	{
		for (InControls::iterator it = mInControls.begin(); it != mInControls.end(); it++)
			delete *it;
		mInControls.clear();
	}

	bool ControlPrinterTyped::ConcreteConfigure( const ProcessingConfig& cfg )
	{ 
		RemoveOldControls();

		CopyAsConcreteConfig( _config, cfg );

		_config.AddAll();
		_config.UpdateData();

		unsigned nInputs = GetInputsNumber();
		if (nInputs == 0)
		{
			AddConfigErrorMessage("No proper OSCTypeSpec setup. Use: 'f' for float, 'd' for double, 'i' for integer, 'h' for integer 64, 'M' for MIDI Message.");
			return false;
		}
		std::string baseName = nInputs==1 ?  "In Control" : _config.GetIdentifier();
		ResizeControls(nInputs, baseName);

		return true; 
	}
	
	bool ControlPrinterTyped::Do()
	{
		if (_config.GetGuiOnly())
			return true;
		
		std::string separator = "";
		std::stringstream values;
		for (unsigned i = 0; i < mInControls.size(); i++) 
		{
			values<<separator<<mInControls[i]->GetLastValueAsString();
			separator = ", ";
		}
		std::cout << _config.GetIdentifier() << ": " 
				<< values.str() << std::endl;
		return true;
	}

	void ControlPrinterTyped::RemoveOldControls()
	{
		ClearControls();
		GetInControls().Clear();
	}
}

