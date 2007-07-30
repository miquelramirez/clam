#ifndef __SMSTIMESTRETCHCONFIGURATOR__
#define __SMSTIMESTRETCHCONFIGURATOR__

#include "SMSConfigurator.hxx"
#include <CLAM/SegmentSMSTimeStretchConfig.hxx>

namespace QtSMS
{
	class Qt_Help_View;
	class Qt_SMS_BPF_Editor;
	
	class SMSTimeStretchConfigurator : public SMSConfigurator
	{
		Q_OBJECT
	public:
		SMSTimeStretchConfigurator();
		~SMSTimeStretchConfigurator();

		QWidget* GetParametersWidget();
		QWidget* GetHelpWidget();

		void Initialize( CLAM::ProcessingConfig& cfg );
		void SetConfig( const CLAM::ProcessingConfig& cfg );		
		const CLAM::ProcessingConfig& GetConfig();

	private:
		Qt_Help_View*              mHelpWidget;
		Qt_SMS_BPF_Editor*         mEditorWidget;
		CLAM::SegmentSMSTimeStretchConfig mConfig;
		static const char*         mHelpText;
		
	};
}

#endif


