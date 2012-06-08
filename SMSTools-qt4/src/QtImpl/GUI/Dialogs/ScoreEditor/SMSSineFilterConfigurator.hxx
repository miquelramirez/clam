#ifndef __SMSSINEFILTERCONFIGURATOR__
#define __SMSSINEFILTERCONFIGURATOR__

#include "SMSConfigurator.hxx"
#include <CLAM/SegmentTransformationConfig.hxx>

namespace QtSMS
{
	class Qt_Help_View;
	class Qt_SMS_BPF_Editor;

	class SMSSineFilterConfigurator : public SMSConfigurator
	{
		Q_OBJECT
	public:
		SMSSineFilterConfigurator();
		~SMSSineFilterConfigurator();

		QWidget* GetParametersWidget();
		QWidget* GetHelpWidget();

		void Initialize(CLAM::ProcessingConfig& cfg);
		void SetConfig(const CLAM::ProcessingConfig& cfg);
		const CLAM::ProcessingConfig& GetConfig();

	private:
		Qt_Help_View*                     mHelpWidget;
		Qt_SMS_BPF_Editor*                mEditorWidget;
		CLAM::SegmentTransformationConfig mConfig;
		static const char*                mHelpText;

	};
}


#endif

