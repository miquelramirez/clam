#ifndef __SMSRESIDUALGAINCONFIGURATOR__
#define __SMSRESIDUALGAINCONFIGURATOR__

#include "SMSConfigurator.hxx"
#include "SegmentTransformationConfig.hxx"

namespace QtSMS
{
	class Qt_Help_View;
	class Qt_SMS_BPF_Editor;
	
	class SMSResidualGainConfigurator : public SMSConfigurator
	{
		Q_OBJECT
	public:
		SMSResidualGainConfigurator();
		~SMSResidualGainConfigurator();

		QWidget* GetParametersWidget();
		QWidget* GetHelpWidget();

		void Initialize( CLAM::ProcessingConfig& );
		void SetConfig( const CLAM::ProcessingConfig& );
		const CLAM::ProcessingConfig& GetConfig();

	private:
		Qt_Help_View*                     mHelpWidget;
		Qt_SMS_BPF_Editor*                mEditorWidget;
		CLAM::SegmentTransformationConfig mConfig;
		static const char*                mHelpText;

	};
}


#endif 

