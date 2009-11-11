#ifndef OscAccumulator_hxx
#define OscAccumulator_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/InControl.hxx>
#include <vector>
#include <sstream>

namespace CLAM
{

class OscAccumulatorConfig : public CLAM::ProcessingConfig
{ 
    DYNAMIC_TYPE_USING_INTERFACE( OscAccumulatorConfig, 1, ProcessingConfig );
    DYN_ATTRIBUTE( 0, public, int, NumberOfPorts);
	
protected:
    void DefaultInit()
    {
          AddAll();
          UpdateData();
          SetNumberOfPorts(1);
    };
};

class OscAccumulator : public CLAM::Processing
{
	typedef OscAccumulatorConfig Config;
	Config _config;
	
	struct Accumulate {
		FloatInControl* incontrol;
		FloatOutControl* outcontrol;
		TControlData last_value;
		TControlData total;
	};
	
	typedef std::vector<Accumulate> Accumulates;
	Accumulates _accumulates;

public:
	OscAccumulator(const Config& config = Config()) 
	{
		Configure( config );
		
		for( int i = 0; i < _config.GetNumberOfPorts(); ++i)
		{
			std::stringstream number("");
			number << i;
			Accumulate acc= {
				new FloatInControl("Input " + number.str(), this),
				new FloatOutControl("Output " + number.str(), this),
				0, 0};
			_accumulates.push_back(acc);
		}
	}
	~OscAccumulator() 
	{
		for(Accumulates::iterator it=_accumulates.begin(); it!=_accumulates.end(); ++it) {
			delete it->incontrol;
			delete it->outcontrol;
		}
		_accumulates.clear();
	}
	
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}
	bool Do()
	{
		for(Accumulates::iterator it=_accumulates.begin(); it!=_accumulates.end(); ++it) {
			TControlData value = it->incontrol->GetLastValue();
			if (it->last_value == value)
				continue;
			it->last_value = value;
			it->total += value;
			it->outcontrol->SendControl(it->total);
		}

		return true;
	}
	const char* GetClassName() const
	{
		return "OscAccumulator";
	}
protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);
		return true; 
	}

};

} //namespace

#endif
