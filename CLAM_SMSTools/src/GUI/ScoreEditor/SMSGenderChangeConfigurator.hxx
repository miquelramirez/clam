#ifndef __SMSGENDERCHANGECONFIGURATOR__
#define __SMSGENDERCHANGECONFIGURATOR__


#include "SMS_Configurator.hxx"
#include "SMSTransformationConfig.hxx"
#include "Slotv1.hxx"
#include "Signalv1.hxx"

class Fl_Widget;
class Fl_Help_View;

namespace CLAMVM
{
	class Fl_SMS_Gender_Selector;

	class SMSGenderChangeConfigurator : public SMSConfigurator
	{
	public:
		SMSGenderChangeConfigurator();
		virtual ~SMSGenderChangeConfigurator();

		virtual Fl_Widget* GetParametersWidget();
		virtual Fl_Widget* GetHelpWidget();

		virtual void Initialize( CLAM::ProcessingConfig& cfg );
		virtual void SetConfig( const CLAM::ProcessingConfig& );
		virtual const CLAM::ProcessingConfig& GetConfig();

		SigSlot::Slotv1<int>   SetGender;
		SigSlot::Signalv1<int> GenderChanged;

	protected:
		
		void OnGenderSet( int gender );
		void SetHelpWidgetText();

	protected:
		
		Fl_Help_View*                 mHelpWidget;
		Fl_SMS_Gender_Selector*       mpGenderSelector;
		CLAM::SMSTransformationConfig mConfig;
		static const char*            mHelpText;
		
		
	};
}


#endif // SMSGenderChangeConfigurator.hxx
