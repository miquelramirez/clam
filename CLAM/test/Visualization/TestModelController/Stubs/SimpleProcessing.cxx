#include "SimpleProcessing.hxx"

namespace CLAMDraft
{

		void SimpleProcessingConfig::DefaultInit()
		{
				AddAll();
				UpdateData();
		}


		SimpleProcessing::SimpleProcessing()
				: mInControl( "InControl", this, &SimpleProcessing::InControl_cb )
		{
		}

		SimpleProcessing::SimpleProcessing( const SimpleProcessingConfig& cfg )
				: mInControl( "InControl", this, &SimpleProcessing::InControl_cb )
		{
				Configure( cfg );
		}

		SimpleProcessing::~SimpleProcessing()
		{
		}

		bool SimpleProcessing::Do()
		{
				return true;
		}

		bool SimpleProcessing::ConcreteConfigure( const ProcessingConfig& cfg )
		{
				CopyAsConcreteConfig( mConfig, cfg );

				return true;
		}

		bool SimpleProcessing::ConcreteStart()
		{
				// Control initialization?
				mInControl.DoControl( 0 );

				return true;
		}

		bool SimpleProcessing::ConcreteStop()
		{
				return true;
		}

		int SimpleProcessing::InControl_cb( TControlData value )
		{
				return 0;
		}
}

