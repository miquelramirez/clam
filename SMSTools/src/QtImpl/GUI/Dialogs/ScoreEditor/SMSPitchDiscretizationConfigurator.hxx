#ifndef __SMSPITCHDISCRETIZATIONCONFIGURATOR__
#define __SMSPITCHDISCRETIZATIONCONFIGURATOR__

#include "SMSConfigurator.hxx"
#include <CLAM/SegmentTransformationConfig.hxx>

class QLabel;

namespace QtSMS
{
	class Qt_Help_View;

	class SMSPitchDiscretizationConfigurator : public SMSConfigurator
	{
	public:
		SMSPitchDiscretizationConfigurator();
		~SMSPitchDiscretizationConfigurator();

		QWidget* GetParametersWidget();
		QWidget* GetHelpWidget();

		void Initialize(CLAM::ProcessingConfig&);
		void SetConfig(const CLAM::ProcessingConfig&);
		const CLAM::ProcessingConfig& GetConfig();

	private:
		QLabel*                           mLabelMsg;
		Qt_Help_View*                     mHelpWidget;
		CLAM::SegmentTransformationConfig mConfig;
		static const char*                mHelpText;
	};
}


#endif 

