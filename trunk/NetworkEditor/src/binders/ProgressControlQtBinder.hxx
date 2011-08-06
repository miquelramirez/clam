#ifndef CLAM_ProgressControlQtBinder_hxx
#define CLAM_ProgressControlQtBinder_hxx


#include "QtBinder.hxx"
#include "ProgressControl.hxx"
#include "ProgressControlWidget.hxx"
#include <CLAM/Network.hxx>

namespace CLAM { class ProgressControlQtBinder; }


class CLAM::ProgressControlQtBinder : public QtBinder
{
	const char * _prefix;
public:
	ProgressControlQtBinder()
		: _prefix("ProgressControl__")
	{}
	
	bool handles(QObject * uiElement)
	{
		if (not isSubClassOf<QWidget>(uiElement)) return false;
		if (not uiElement->objectName().startsWith(_prefix)) return false;
		return true;
	}
	bool bind(QObject * uiElement, CLAM::Network & network, QStringList & errors)
	{
		if (not isSubClassOf<ProgressControlWidget>(uiElement))
			error(errors, QString(
				"ProgressControlQtBinder: %1 expected a ProgressControlWidget bot got a %1")
				.arg(uiElement->objectName())
				.arg(uiElement->metaObject()->className())
				);
		ProgressControlWidget * widget = qobject_cast<ProgressControlWidget*>(uiElement);

		if (not widget->objectName().startsWith(_prefix))
			error(errors, QString(
				"ProgressControlQtBinder: '%1' does not starts with prefix '%2'")
				.arg(widget->objectName())
				.arg(_prefix)
				);
		std::string processingName = widget2NetworkName(_prefix,widget->objectName());
		std::cout << "* Progress Control (2) " << processingName 
			<< " to " << widget->metaObject()->className()
			<< std::endl;

		CLAM::Processing & receiver = network.GetProcessing(processingName);

		ProgressControl * sender = new ProgressControl;
		std::string senderName = network.GetUnusedName("PrototyperProgressControl");
		network.AddProcessing(senderName, sender);
		widget->SetProcessing(sender);

		// TODO: chekc that the  controls exist in receiver
		std::string seekSend = "Progress Jump";
		std::string seekRecv =  "Current Time Position (%)";
		std::string progressSend = "Current Time Position";
		std::string progressRecv = "Progress Update";
		ConnectControls(*sender, seekSend, receiver, seekRecv);
		ConnectControls(receiver, progressSend, *sender, progressRecv);
		return true;
	}
};


#endif//CLAM_ProgressControlQtBinder_hxx

