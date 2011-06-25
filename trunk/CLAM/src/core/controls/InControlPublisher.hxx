#ifndef _INCONTROL_PUBLISHER_HXX_
#define _INCONTROL_PUBLISHER_HXX_

#include "InControl.hxx"
#include <string>

// TODO: Refactoring...
// Extract common interface from InControlPublisher, InControl and create a common base class
namespace CLAM
{
class InControlPublisher : public FloatInControl
{
	FloatInControl* _published; //TODO should be a list, in the future

public:
	InControlPublisher() 
		: FloatInControl( "InControlPublisher", 0 ) 
	{
		_published=NULL;
	} 

	InControlPublisher( const std::string& name, Processing* father )
		: FloatInControl( name, father ) 
	{
		_published=NULL;
	}

	void PublishInControl( FloatInControl& in )
	{
		_published = &in;
	}
	void DoControl(const TControlData & val)
	{
		if(_published)
			_published->DoControl(val);
		else
			FloatInControl::DoControl(val);
	}
	const TControlData& GetLastValue() const 
	{ 
		if(_published)
			return _published->GetLastValue();
		return _lastValue; 
	}

};

} // namespace CLAM

#endif

