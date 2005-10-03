#ifndef __SMSGENDERCHANGECONFIGURATOR__
#define __SMSGENDERCHANGECONFIGURATOR__

#include "SMSConfigurator.hxx"
#include "SegmentTransformationConfig.hxx"

namespace QtSMS
{
	class Qt_Help_View;
	class Qt_SMS_Gender_Selector;

	class SMSGenderChangeConfigurator : public SMSConfigurator
	{
		Q_OBJECT
	public:
		SMSGenderChangeConfigurator();
		~SMSGenderChangeConfigurator();

		QWidget* GetParametersWidget();
		QWidget* GetHelpWidget();

		void Initialize( CLAM::ProcessingConfig& cfg );
		void SetConfig( const CLAM::ProcessingConfig& );
		const CLAM::ProcessingConfig& GetConfig();
		
	private slots:
		void onGenderChanged(int);

	private:
		Qt_Help_View*                     mHelpWidget;
		Qt_SMS_Gender_Selector*           mGenderSelector;
		CLAM::SegmentTransformationConfig mConfig;
		static const char*                mHelpText;
		
	};
}


#endif 

