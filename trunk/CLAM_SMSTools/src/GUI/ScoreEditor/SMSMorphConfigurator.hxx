#ifndef __SMSMORPHCONFIGURATOR__
#define __SMSMORPHCONFIGURATOR__


#include "SMS_Configurator.hxx"
#include "SMSMorphConfig.hxx"

class Fl_Widget;
class Fl_Help_View;

namespace CLAMVM
{
	class Fl_SMS_BPF_Editor;
	
	class SMSMorphConfigurator : public SMSConfigurator
	{
	public:
		SMSMorphConfigurator();
		virtual ~SMSMorphConfigurator();

		virtual Fl_Widget* GetParametersWidget();
		virtual Fl_Widget* GetHelpWidget();

		virtual void Initialize( CLAM::ProcessingConfig& );
		virtual void SetConfig( const CLAM::ProcessingConfig& );
		virtual const CLAM::ProcessingConfig& GetConfig();

	protected:
		
		void SetHelpWidgetText();
		void SetupConfigObject();

	protected:
		
		Fl_Help_View*                 mHelpWidget;
		Fl_SMS_BPF_Editor*            mEditorWidget;
		CLAM::SMSMorphConfig          mConfig;
		static const char*            mHelpText;
		
		
	};
}


#endif // SMSMorphConfigurator.hxx
