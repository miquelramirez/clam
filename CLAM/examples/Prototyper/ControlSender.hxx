#ifndef _ControlSender_hxx_
#define _ControlSender_hxx_
#include <qobject.h>
#include <iostream>
#include "OutControl.hxx"
#include "InControl.hxx"


class ControlSender : public QObject
{
	Q_OBJECT
public:
	ControlSender(const char * name)
		: _sender(name)
		, _name(name)
	{
	}
	virtual ~ControlSender(){}
	void linkControl(CLAM::InControl & receiver)
	{
		_sender.AddLink(& receiver);
	}
public slots:
	void sendControl(int value)
	{
		std::cout << _name << ": Sending " << value << std::endl;
		_sender.SendControl(value);
	}
private:
	CLAM::OutControl _sender;
	const char * _name;
	
};


#endif//_ControlSender_hxx_


