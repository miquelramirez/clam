#include <qobject.h>
#include <iostream>


class EventLogger : public QObject
{
	Q_OBJECT
public:
	EventLogger(char * name)
	{
		_name = name;
	}
public slots:
	void printValue(int value)
	{
		std::cout << _name << ": changed to " << value << std::endl;
	}
	
private:
	char * _name;
	
};




