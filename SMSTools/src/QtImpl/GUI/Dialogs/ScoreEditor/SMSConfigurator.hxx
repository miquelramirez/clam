#ifndef __SMSCONFIGURATOR__
#define __SMSCONFIGURATOR__

#include <qobject.h>
#include <CLAM/Factory.hxx>

class QWidget;

namespace CLAM
{
	class ProcessingConfig;
}

namespace QtSMS
{
	class SMSConfigurator : public QObject
	{
		Q_OBJECT
	public:
		SMSConfigurator();
		virtual ~SMSConfigurator();

		/** Returns the editor widget for the SMS transformation*/
		virtual QWidget* GetParametersWidget() = 0;

		/** Returns the help widget for the SMS transformation*/
		virtual QWidget* GetHelpWidget() = 0;

		/** Initializes the provided configuration object to reasonable default values*/
		virtual void Initialize( CLAM::ProcessingConfig& cfg ) = 0;

		/** Returns a config object reflecting user changes */
		virtual const CLAM::ProcessingConfig& GetConfig() = 0;

		/** Sets the config object */
		virtual void SetConfig( const CLAM::ProcessingConfig& ) = 0;

	signals:
		/** For notifying changes occured in wrapped configuration*/
		void configurationChanged();

	};

	typedef CLAM::Factory<SMSConfigurator> SMSConfiguratorFactory;

}

#endif

