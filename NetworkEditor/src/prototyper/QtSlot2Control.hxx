#ifndef _QtSlot2Control_hxx_
#define _QtSlot2Control_hxx_
#include <iostream>
#include <CLAM/OutControl.hxx>
#include <CLAM/InControl.hxx>
#include <QtCore/QObject>


class QtSlot2Control : public QObject
{
	Q_OBJECT
public:
	QtSlot2Control(const char * name)
		: _sender(name)
		, _name(name)
	{
	}
	virtual ~QtSlot2Control(){}
	void linkControl(CLAM::InControl & receiver)
	{
		_sender.AddLink(receiver);
	}
public slots:
	void sendBooleanControl(bool value)
	{
		CLAM::TControlData mappedValue = CLAM::TControlData(value?0.0:1.0);
		_sender.SendControl(mappedValue);
	}
	void sendMappedControl(int value)
	{
		CLAM::TControlData mappedValue = CLAM::TControlData(value)/99.0;
		_sender.SendControl(mappedValue);
	}
	void sendControl(int value)
	{
		_sender.SendControl(value);
	}
private:
	CLAM::OutControl _sender;
	const char * _name;
	
};



#endif//_QtSlot2Control_hxx_


