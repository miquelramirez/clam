#include "ControlPrinterTyped.hxx"
#include <iostream>
#include <sstream>
#include "ProcessingFactory.hxx"


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
		SetNumberOfInputs(1.);
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
	InControlBase * ControlPrinterTyped::createControl(const CLAM::EnumTypedTypes & type, const std::string & name)
	{
		if (type==EnumTypedTypes::eString)
			return new TypedInControl<std::string> (name,this);
		if (type==EnumTypedTypes::eFloat)
			return new FloatInControl (name,this);
		if (type==EnumTypedTypes::eDouble)
			return new TypedInControl<double> (name,this);
		if (type==EnumTypedTypes::eInt)
			return new TypedInControl<int> (name,this);
		// TODO: Decide whether ASSERTing (contract) or throw (control) 
		return 0;
	}
	void ControlPrinterTyped::ResizeControls(unsigned nInputs, const std::list<std::string> & names)
	{

		unsigned int type=EnumTypedTypes::eFloat;
		if (_config.HasTypedType())
			type=EnumTypedTypes(_config.GetTypedType());
		ClearControls();
		for (std::list<std::string>::const_iterator it = names.begin(); it != names.end(); it++)
			mInControls.push_back(createControl(type,*it));
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

		unsigned nInputs = _config.GetNumberOfInputs();
		if (nInputs == 0)
		{
			_config.SetNumberOfInputs(1);
			nInputs = 1;
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
		//mInControls.Clear();
		ClearControls();
		GetInControls().Clear();
	}
}

