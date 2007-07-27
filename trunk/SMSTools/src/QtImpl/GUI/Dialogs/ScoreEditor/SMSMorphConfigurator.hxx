#ifndef __SMSMORPHCONFIGURATOR__
#define __SMSMORPHCONFIGURATOR__

#include "SMSConfigurator.hxx"
#include <CLAM/SegmentSMSMorphConfig.hxx>

namespace QtSMS
{
	class Qt_Help_View;
	class Qt_SMS_Morph_Control;
	
	class SMSMorphConfigurator : public SMSConfigurator
	{
		Q_OBJECT 
	public:
		SMSMorphConfigurator();
		~SMSMorphConfigurator();

		QWidget* GetParametersWidget();
		QWidget* GetHelpWidget();

		void Initialize( CLAM::ProcessingConfig& );
		void SetConfig( const CLAM::ProcessingConfig& );
		const CLAM::ProcessingConfig& GetConfig();

	private:
		Qt_Help_View*         mHelpWidget;
		Qt_SMS_Morph_Control* mMorphEditor;
		CLAM::SegmentSMSMorphConfig  mConfig;
		static const char*    mHelpText;

		void SetupConfigObject();
	};
}

#endif


