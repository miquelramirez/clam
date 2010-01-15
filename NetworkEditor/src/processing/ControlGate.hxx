#ifndef ControlGate_hxx
#define ControlGate_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>


class ControlGate : public CLAM::Processing
{
	CLAM::InControl<float> _input;
	CLAM::OutControl<bool> _output;
	unsigned _count;
public:
	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE (Config, 3, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, float, Threshold); ///< The value that opens the gate
		DYN_ATTRIBUTE (1, public, float, HysteresisWidth); ///< Negative offset to close the gate once open
		DYN_ATTRIBUTE (2, public, bool, Inverted); ///< Inverts the output value
		void DefaultInit(void)
		{
			AddAll();
			UpdateData();
			SetThreshold(0);
			SetHysteresisWidth(0);
			SetInverted(false);
		}
	};
private:
	Config _config;
	bool _lastOutput;
public:
	ControlGate()
		: _input("Input", this)
		, _output("Output", this)
	{
	}
	~ControlGate()
	{
	}
	const char * GetClassName() const { return "ControlGate"; }
	const CLAM::ProcessingConfig & GetConfig() const { return _config; }
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		_input.DoControl(0);
		_lastOutput= 0 >= _config.GetThreshold();
		return true;
	}
	bool Do()
	{
		float currentInput = _input.GetLastValue();
		bool output = _lastOutput ? 
			currentInput > _config.GetThreshold() - _config.GetHysteresisWidth():
			currentInput > _config.GetThreshold() ;
		_lastOutput = output;
		if (_config.GetInverted()) output = not output;
		_output.SendControl(output);
		return true;
	}
};




#endif//ControlGate_hxx
