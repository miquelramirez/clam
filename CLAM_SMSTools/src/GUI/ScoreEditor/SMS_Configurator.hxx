#ifndef __SMSCONFIGURATOR__
#define __SMSCONFIGURATOR__

#include <FL/Fl_Widget.H>
#include "Factory.hxx"
#include "Signalv0.hxx"
#include "Slotv0.hxx"

namespace CLAM
{
	class ProcessingConfig;
}

namespace CLAMVM
{
	class SMSConfigurator 
	{
	public:
		SMSConfigurator();
		virtual ~SMSConfigurator();

		/** Returns the editor widget for the SMS transformation*/
		virtual Fl_Widget* GetParametersWidget() = 0;

		/** Returns the help widget for the SMS transformation*/
		virtual Fl_Widget* GetHelpWidget() = 0;

		/** Initializes the provided configuration object to reasonable default values*/
		virtual void Initialize( CLAM::ProcessingConfig& cfg ) = 0;

		/** Returns a config object reflecting user changes */
		virtual const CLAM::ProcessingConfig& GetConfig() = 0;

		/** Sets the config object */
		virtual void SetConfig( const CLAM::ProcessingConfig& ) = 0;

		/** For notifying changes occured in wrapped configuration*/
		SigSlot::Signalv0  ConfigurationChanged;

	protected:
		
		SigSlot::Slotv0    UserEditedParameters;

		void OnChangeNotification();
	};

	typedef CLAM::Factory<SMSConfigurator> SMSConfiguratorFactory;

}

#endif // SMSConfigurator.hxx
