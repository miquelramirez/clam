#ifndef __SMSTIMESTRETCHCONFIGURATOR__
#define __SMSTIMESTRETCHCONFIGURATOR__

#include "SMS_Configurator.hxx"
#include "SMSTimeStretchConfig.hxx"

class Fl_Widget;
class Fl_Help_View;

namespace CLAMVM
{
	class Fl_SMS_BPF_Editor;

	class SMSTimeStretchConfigurator : public SMSConfigurator
	{
	public:
		SMSTimeStretchConfigurator();
		virtual ~SMSTimeStretchConfigurator();

		virtual Fl_Widget* GetParametersWidget();
		virtual Fl_Widget* GetHelpWidget();

		virtual void Initialize( CLAM::ProcessingConfig& cfg );
		virtual void SetConfig( const CLAM::ProcessingConfig& cfg );		
		virtual const CLAM::ProcessingConfig& GetConfig();

	protected:
		
		void SetHelpWidgetText();

	protected:
		
		Fl_Help_View*                 mHelpWidget;
		Fl_SMS_BPF_Editor*            mEditorWidget;
		CLAM::SMSTimeStretchConfig	  mConfig;
		static const char*            mHelpText;
		
		
	};
}


#endif // SMSTimeStretchConfigurator.hxx
