#ifndef __SMSODDEVENHARMONICRATIO__
#define __SMSODDEVENHARMONICRATIO__

#include "SMS_Configurator.hxx"
#include "SMSTransformationConfig.hxx"

class Fl_Widget;
class Fl_Help_View;

namespace CLAMVM
{
	class Fl_SMS_BPF_Editor;

	class SMSOddEvenHarmonicRatioConfigurator : public SMSConfigurator
	{
	public:
		SMSOddEvenHarmonicRatioConfigurator();
		virtual ~SMSOddEvenHarmonicRatioConfigurator();

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
		CLAM::SMSTransformationConfig mConfig;
		static const char*            mHelpText;
		
		
	};
}


#endif // SMSOddEvenHarmonicRatio.hxx
