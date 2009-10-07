#include "ControlFade.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "ControlFade",
		"category", "Controls",
		"description", "ControlFade",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ControlFade> reg = metadata;
}

void ControlFadeConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	SetFadeTimeInMs( 0.0 );
}
ControlFade::ControlFade()
	: _inControlValue( "Input Value", this , &ControlFade::InControlValueCallback )
	, _inControlDelay( "Delay Time in Ms", this , &ControlFade::InControlDelayCallback )
	, _outControlValue( "Output Value", this )
	, _bufferTime(0)
	, _counterTime(0)
	, _initValue(0)
	, _lastValue(0)
	, _delayTime(0)
{
	Configure( _config );	
}

ControlFade::ControlFade( const ControlFadeConfig& cfg ) 
	: _inControlValue( "Input Values", this , &ControlFade::InControlValueCallback )
	, _inControlDelay( "Delay Time in Ms", this , &ControlFade::InControlDelayCallback )
	, _outControlValue( "Output Value", this )
	, _bufferTime(0)
	, _counterTime(0)
	, _initValue(0)
	, _lastValue(0)
	, _delayTime(0)
{ 
	Configure( cfg );
}

bool ControlFade::ConcreteConfigure( const ProcessingConfig& cfg )
{
	CopyAsConcreteConfig( _config, cfg );
	_inControlDelay.DoControl(_config.GetFadeTimeInMs());
	_bufferTime = 1000. * (float)BackendBufferSize() / (float)BackendSampleRate();
	_initValue=_lastValue=0;
//	std::cout<<"Buffer Time: "<<_bufferTime<<std::endl;
	_counterTime=0;
	return true; 		
}

void ControlFade::InControlDelayCallback(const TControlData & value)
{
		_delayTime=value;
}
void ControlFade::InControlValueCallback(const TControlData & value)
{
	_counterTime=0;
	_initValue=_lastValue;
	_lastValue=_inControlValue.GetLastValue();
}

bool ControlFade::Do()
{
	if (_initValue==_lastValue)
		return true;
	double a=(_inControlValue.GetLastValue() - _initValue)/_delayTime;
	float newValue=a * _counterTime + _initValue;
	_counterTime += _bufferTime;
	if (((newValue <= _lastValue) and (_lastValue > _initValue)) or ((newValue>=_lastValue) and (_lastValue<_initValue)))
	{
//		std::cout<<"DoControl! value: "<<newValue<<std::endl;
		_outControlValue.SendControl(newValue);
	}
	if (((newValue >= _lastValue) and (_lastValue > _initValue)) or ((newValue<=_lastValue) and (_lastValue<_initValue)))
	{
		_initValue = _lastValue;
		_outControlValue.SendControl(_lastValue);
//		std::cout<<"lastvalue: "<<_lastValue<<std::endl;
	}
	return true;
}
}

