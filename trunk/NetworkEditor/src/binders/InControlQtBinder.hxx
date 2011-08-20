#ifndef CLAM_InControlQtBinder_hxx
#define CLAM_InControlQtBinder_hxx

#include "QtBinder.hxx"

namespace CLAM { class InControlQtBinder; }

#include <CLAM/Network.hxx>

/**
	InControlQtBinder is a QtBinder subclass to bind slider like
	widgets with integer values to float incontrols by mapping
	200 steps into the processing in control range.

	The incontrol is identified either by naming the widget with
	the mangled incontrol names (prefix 'InControl__' or adding
	a custom string property to the widget named clamInControl
	containing 'theProcessingName.theInControlNAme'.

	Additional custom properties may change the behaviour.

	@todo document aditional custom properties

	@ingroup UiBinders
*/
class CLAM::InControlQtBinder : public CLAM::QtBinder
{
	QString _prefix;
	const char * _controlProperty;
public:
	InControlQtBinder();
	bool handles(QObject * uiElement);
	bool bind(QObject * uiElement, CLAM::Network & network, QStringList & errors);
private:
	void setPropertyIfPresent(QObject * uiElement, const char * property, int value);
};

namespace CLAM { class ControlSource; }
class ControlSourceSender : public QObject
{
	Q_OBJECT
	CLAM::ControlSource * _source;
public:
	ControlSourceSender(CLAM::ControlSource * processing)
		: _source(processing)
	{
	}
public slots:
	void send(int value);
};

#endif//CLAM_InControlQtBinder_hxx

