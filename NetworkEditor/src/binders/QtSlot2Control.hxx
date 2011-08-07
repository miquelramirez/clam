#ifndef _QtSlot2Control_hxx_
#define _QtSlot2Control_hxx_
#include <iostream>
#include <CLAM/OutControl.hxx>
#include <CLAM/InControl.hxx>
#include <QtCore/QObject>


class QtSlot2Control : public QObject
{
	Q_OBJECT

	CLAM::FloatOutControl _sender;
	const char * _name;
	float _controlMinBound;
	float _controlMaxBound;
	int _widgetMin;
	int _widgetMax;
public:
	QtSlot2Control(const char * name, float min=0.f, float max=1.f, float wMin=0, float wMax=200)
		: _sender(name)
		, _name(name)
		, _controlMinBound(min)
		, _controlMaxBound(max)
		, _widgetMin(wMin)
		, _widgetMax(wMax)
	{

	}
	virtual ~QtSlot2Control(){}
	void linkControl(CLAM::InControlBase & receiver)
	{
		_sender.AddLink(receiver);
	}
public slots:
	void sendBooleanControl(bool value)
	{
		CLAM::TControlData mappedValue = CLAM::TControlData(value?0.0:1.0);
		_sender.SendControl(mappedValue);
	}
	float map(float value)
	{
		return (value-_widgetMin)
			* (_controlMaxBound-_controlMinBound)
			/ (_widgetMax-_widgetMin)
			+ _controlMinBound;
	}
	void sendMappedControl(int value)
	{
		std::cout << "Mapping " << map(value) << std::endl;
		_sender.SendControl(map(value));
	}
	void sendControl(int value)
	{
		_sender.SendControl(value);
	}
	
};



#endif//_QtSlot2Control_hxx_


