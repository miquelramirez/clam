#ifndef __SMSMORPHCONFIGURATOR__
#define __SMSMORPHCONFIGURATOR__


#include "SMS_Configurator.hxx"
#include "SMSMorphConfig.hxx"
#include "Signalv0.hxx"
#include "Slotv1.hxx"

class Fl_Widget;
class Fl_Help_View;

namespace CLAMVM
{
	class Fl_SMS_Morph_Control;
	
	class SMSMorphConfigurator : public SMSConfigurator
	{
	protected:
		class UserDefinedParams
		{
		protected:
			bool   mUserActivatedFrameInterpolation;
			bool   mUserDefinedGlobalEnvelope;
			bool   mUserDefinedSinAmpEnvelope;
			bool   mUserDefinedSinFreqEnvelope;
			bool   mUserDefinedResAmpEnvelope;
			bool   mUserDefinedPitchHybEnvelope;
			bool   mUserDefinedTimeSyncEnvelope;
			bool   mUserDefinedSinShapeW1Envelope;
			bool   mUserDefinedSinShapeW2Envelope;
			bool   mUserDefinedResShapeW1Envelope;
			bool   mUserDefinedResShapeW2Envelope;
			bool   mUserDefinedSinShapeEnvelope;
			bool   mUserDefinedResShapeEnvelope;
			bool   mOnlyGlobalEnvelope;

		public:
			UserDefinedParams(  );
			~UserDefinedParams();

			void Reset();

			bool UserOnlyDefinedGlobalEnvelope() const
			{
				return mOnlyGlobalEnvelope;
			}

			bool UserActivatedFrameInterpolation() const
			{
				return mUserActivatedFrameInterpolation;
			}

			bool UserDefinedGlobalEnvelope() const
			{
				return mUserDefinedGlobalEnvelope;
			}

			bool UserDefinedSinAmpEnvelope() const
			{
				return mUserDefinedSinAmpEnvelope;
			}
			
			bool UserDefinedSinFreqEnvelope() const
			{
				return mUserDefinedSinFreqEnvelope;
			}

			bool UserDefinedResAmpEnvelope() const
			{
				return mUserDefinedResAmpEnvelope;
			}

			bool UserDefinedPitchHybEnvelope() const
			{
				return mUserDefinedPitchHybEnvelope;
			}

			bool UserDefinedTimeSyncEnvelope() const
			{
				return mUserDefinedTimeSyncEnvelope;
			}

			bool UserDefinedSinShapeW1Envelope() const
			{
				return mUserDefinedSinShapeW1Envelope;
			}

			bool UserDefinedSinShapeW2Envelope() const
			{
				return mUserDefinedSinShapeW2Envelope;
			}

			bool UserDefinedResShapeW1Envelope() const
			{
				return mUserDefinedResShapeW1Envelope;
			}

			bool UserDefinedResShapeW2Envelope() const
			{
				return mUserDefinedResShapeW2Envelope;
			}

			bool UserDefinedSinShapeEnvelope() const
			{
				return mUserDefinedSinShapeEnvelope;
			}

			bool UserDefinedResShapeEnvelope() const
			{
				return mUserDefinedResShapeEnvelope;
			}

			SigSlot::Slotv1<bool> FrameInterpolationListener;
			SigSlot::Slotv0       GlobalEnvelopeListener;
			SigSlot::Slotv0       PitchHybEnvelopeListener;
			SigSlot::Slotv0       SinAmpEnvelopeListener;
			SigSlot::Slotv0       SinFreqEnvelopeListener;
			SigSlot::Slotv0       ResAmpEnvelopeListener;
			SigSlot::Slotv0       TimeSyncEnvelopeListener;
			SigSlot::Slotv0       SinShapeW1EnvelopeListener;
			SigSlot::Slotv0       SinShapeW2EnvelopeListener;
			SigSlot::Slotv0       ResShapeW1EnvelopeListener;
			SigSlot::Slotv0       ResShapeW2EnvelopeListener;
			SigSlot::Slotv0       SinShapeEnvelopeListener;
			SigSlot::Slotv0       ResShapeEnvelopeListener;
			SigSlot::Signalv0     UserHasActed;

		protected:

			void OnFrameInterpolationChanged( bool newState );
			void OnGlobalEnvelopeChanged();
			void OnPitchHybEnvelopeChanged();
			void OnSinAmpEnvelopeChanged();
			void OnSinFreqEnvelopeChanged();
			void OnResAmpEnvelopeChanged();
			void OnTimeSyncEnvelopeChanged();

			void OnSinShapeW1EnvelopeChanged();
			void OnSinShapeW2EnvelopeChanged();
			void OnResShapeW1EnvelopeChanged();
			void OnResShapeW2EnvelopeChanged();

			void OnSinShapeEnvelopeChanged();
			void OnResShapeEnvelopeChanged();
		};
		

	public:
		SMSMorphConfigurator();
		virtual ~SMSMorphConfigurator();

		virtual Fl_Widget* GetParametersWidget();
		virtual Fl_Widget* GetHelpWidget();

		virtual void Initialize( CLAM::ProcessingConfig& );
		virtual void SetConfig( const CLAM::ProcessingConfig& );
		virtual const CLAM::ProcessingConfig& GetConfig();

	protected:
		
		void SetHelpWidgetText();
		void SetupConfigObject();

	protected:
		
		Fl_Help_View*                 mHelpWidget;
		Fl_SMS_Morph_Control*         mpMorphEditor;
		CLAM::SMSMorphConfig          mConfig;
		static const char*            mHelpText;
		UserDefinedParams             mUserListener;

		UserDefinedParams&            UserListener()
		{
			return mUserListener;
		}

	};
}


#endif // SMSMorphConfigurator.hxx
