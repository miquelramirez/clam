#ifndef __SMSRESIDUALGAINCONFIGURATOR__
#define __SMSRESIDUALGAINCONFIGURATOR__


#include "SMS_Configurator.hxx"
#include "SMSTransformationConfig.hxx"

class Fl_Widget;
class Fl_Help_View;

namespace CLAMVM
{
	class Fl_SMS_BPF_Editor;
	

	class SMSResidualGainConfigurator : public SMSConfigurator
	{
	public:
		SMSResidualGainConfigurator();
		virtual ~SMSResidualGainConfigurator();

		virtual Fl_Widget* GetParametersWidget();
		virtual Fl_Widget* GetHelpWidget();

		virtual void Initialize( CLAM::ProcessingConfig& );
		virtual void SetConfig( const CLAM::ProcessingConfig& );
		virtual const CLAM::ProcessingConfig& GetConfig();

	protected:
		
		void SetHelpWidgetText();

	protected:
		
		Fl_Help_View*                 mHelpWidget;
		Fl_SMS_BPF_Editor*     mEditorWidget;
		CLAM::SMSTransformationConfig mConfig;
		static const char*            mHelpText;
		
		
	};
}


#endif // SMSResidualGainConfigurator.hxx
