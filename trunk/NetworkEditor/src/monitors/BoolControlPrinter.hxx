#ifndef BoolControlPrinter_hxx
#define BoolControlPrinter_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>


class BoolControlPrinter : public CLAM::Processing
{
public:
	class Config : public CLAM::ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Config, 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, unsigned, NInputs );
		void DefaultInit(void)
		{
			AddAll();
			UpdateData();
			SetNInputs(1);
		}
	};
private:
	typedef CLAM::InControl<bool> BoolControl;
	typedef std::vector<BoolControl*> BoolControls;
	BoolControls _inputs;
	Config _config;
public:
	BoolControlPrinter(const Config & c=Config())
	{
		Configure(c);
	}
	const CLAM::ProcessingConfig & GetConfig() const { return _config; }
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		if (not _config.HasNInputs())
		{
			_config.AddNInputs();
			_config.UpdateData();
			_config.SetNInputs(1);
		}
		unsigned nInputs = _config.GetNInputs();
		ResizeControls(nInputs);
		for (unsigned i=0; i<nInputs; i++)
			_inputs[i]->DoControl(0);
		return true;
	}
	~BoolControlPrinter()
	{
		ResizeControls(0);
	}
	const char * GetClassName() const { return "BoolControlPrinter"; }
	void ControlCallback(unsigned i, const bool & value)
	{
	}
	bool Do()
	{
		return true;
	}
private:
	void ResizeControls(unsigned newSize)
	{
		unsigned previousSize = _inputs.size();
		for (unsigned i=newSize; i<previousSize; i++)
			delete _inputs[i];
		_inputs.resize(newSize);
		for (unsigned i=previousSize; i<newSize; i++)
		{
			std::ostringstream os;
			os << i+1;
			_inputs[i]= new BoolControl(i,os.str(),this,&BoolControlPrinter::ControlCallback);
		}
	}
};




#endif//BoolControlPrinter_hxx
