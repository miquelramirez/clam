#ifndef __SMSPITCHDISCRETIZATIONCONFIGURATOR__
#define __SMSPITCHDISCRETIZATIONCONFIGURATOR__


#include "SMS_Configurator.hxx"
#include "SMSTransformationConfig.hxx"

class Fl_Widget;
class Fl_Help_View;

namespace CLAMVM
{
	class SMSPitchDiscretizationConfigurator : public SMSConfigurator
	{
	public:
		SMSPitchDiscretizationConfigurator();
		virtual ~SMSPitchDiscretizationConfigurator();

		virtual Fl_Widget* GetParametersWidget();
		virtual Fl_Widget* GetHelpWidget();

		virtual void Initialize( CLAM::ProcessingConfig& );
		virtual void SetConfig( const CLAM::ProcessingConfig& );
		virtual const CLAM::ProcessingConfig& GetConfig();

	protected:
		
		void SetHelpWidgetText();

	protected:
		
		Fl_Help_View*                 mHelpWidget;
		CLAM::SMSTransformationConfig mConfig;
		static const char*            mHelpText;
		
		
	};
}


#endif // SMSPitchDiscretizationConfigurator.hxx
