#include "ControlSelector.hxx"
#include <iostream>
#include <sstream>
#include "ProcessingFactory.hxx"


namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "ControlSelector",
		"category", "Controls",
		"description", "ControlSelector",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ControlSelector> reg = metadata;
}

	void ControlSelectorConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetNumberOfInputs(2);
		SetDefaultIndex(1);
	}

	ControlSelector::ControlSelector()
		: _indexControl(0,"In Control Index to send",this,&ControlSelector::InControlCallback)
		, _outputControl("Output value",this)
	{
		Configure( _config );	
	}
	
	ControlSelector::ControlSelector( const ControlSelectorConfig& cfg )
		: _indexControl(0,"In Control Index to send",this,&ControlSelector::InControlCallback)
		, _outputControl("Output value",this)
	{ 
		Configure( cfg );
	}
	ControlSelector::~ControlSelector()
	{
		RemoveOldControls();
	}
		
	bool ControlSelector::ConcreteConfigure( const ProcessingConfig& cfg )
	{ 
		RemoveOldControls();

		CopyAsConcreteConfig( _config, cfg );

		_config.AddAll();
		_config.UpdateData();


		_indexControl.DoControl(_config.GetDefaultIndex());

		int nInputs = int(_config.GetNumberOfInputs());
		if (nInputs == 0)
			return AddConfigErrorMessage("Cannot work without any control input");
		CreateInControls(nInputs);
		return true; 
	}


	void ControlSelector::CreateInControls(unsigned numberOfControls)
	{
		for (unsigned i=1; i<=numberOfControls; i++)
		{
			std::ostringstream controlName;
			controlName<<"In Control Value"<<i;
			_inControls.push_back(new FloatInControl(i,controlName.str(),this,&ControlSelector::InControlCallback));
			std::cout<<"created control "<<i<<std::endl;
		}

	}
	void ControlSelector::RemoveOldControls()
	{
		for (unsigned i=0; i<_inControls.size();i++)
		{
			delete _inControls[i];
		}
		_inControls.clear();
	}

	void ControlSelector::InControlCallback(unsigned controlId, const TControlData & value)
	{
		if (not IsConfigured()) return;
		const unsigned index=_indexControl.GetLastValue();
		if (controlId==0 or controlId==index+1) // index change or proper input
		{
			if (index>=_inControls.size())
			{
				std::cout<<"WARNING: control selector receiving an index number greater than the number of input controls, ignoring it"<<std::endl;
				return;
			}
			_outputControl.SendControl(_inControls[index]->GetLastValue());
		}
	}
	
	bool ControlSelector::Do()
	{
		return true;
	}

}

