#ifndef BoolControlSender_hxx
#define BoolControlSender_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>


class BoolControlSender : public CLAM::Processing
{
public:
	class Config : public CLAM::ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Config, 1, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, unsigned, NOutputs );
		void DefaultInit(void)
		{
			AddAll();
			UpdateData();
			SetNOutputs(1);
		}
	};
private:
	typedef CLAM::OutControl<bool> BoolInControl;
	typedef std::vector<BoolInControl*> BoolControls;
	BoolControls _outputs;
	Config _config;
public:
	BoolControlSender(const Config & c=Config())
	{
		Configure(c);
	}
	const CLAM::ProcessingConfig & GetConfig() const { return _config; }
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		if (not _config.HasNOutputs())
		{
			_config.AddNOutputs();
			_config.UpdateData();
			_config.SetNOutputs(1);
		}
		unsigned nOutputs = _config.GetNOutputs();
		ResizeControls(nOutputs);
		return true;
	}
	~BoolControlSender()
	{
		ResizeControls(0);
	}
	const char * GetClassName() const { return "BoolControlSender"; }
	void ControlCallback(unsigned i, const bool & value)
	{
	}
	bool Do()
	{
		return true;
	}
	void SendControl(unsigned index, bool value)
	{
		_outputs[index]->SendControl(value);
	}
private:
	void ResizeControls(unsigned newSize)
	{
		unsigned previousSize = _outputs.size();
		for (unsigned i=newSize; i<previousSize; i++)
			delete _outputs[i];
		_outputs.resize(newSize);
		for (unsigned i=previousSize; i<newSize; i++)
		{
			std::ostringstream os;
			os << i+1;
			_outputs[i]= new BoolInControl(os.str(),this);
		}
	}
};




#endif//BoolControlSender_hxx
