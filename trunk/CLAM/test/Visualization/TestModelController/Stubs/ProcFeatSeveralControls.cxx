#include "ProcFeatSeveralControls.hxx"

namespace CLAMDraft
{
		void ProcFeatSeveralControlsConfig::DefaultInit()
		{
				AddAll();
				UpdateData();
		}

		ProcFeatSeveralControls::ProcFeatSeveralControls()
				: mInCtl_0("Control#1", this, &ProcFeatSeveralControls::InCtl0_cb ),
				  mInCtl_1("Control#2", this, &ProcFeatSeveralControls::InCtl1_cb ),
				  mInCtl_2("Control#3", this, &ProcFeatSeveralControls::InCtl2_cb ),
				  mInCtl_3("Control#4", this, &ProcFeatSeveralControls::InCtl3_cb ),
				  mInCtl_4("Control#5", this, &ProcFeatSeveralControls::InCtl4_cb ),
				  mInCtl_5("Control#6", this, &ProcFeatSeveralControls::InCtl5_cb ),
				  mTimesCalled0( 0 ), mTimesCalled1( 0 ), mTimesCalled2( 0 ), mTimesCalled3( 0 ),
				  mTimesCalled4( 0 ), mTimesCalled5( 0 )
		{
		}

		ProcFeatSeveralControls::ProcFeatSeveralControls( const ProcFeatSeveralControlsConfig& cfg )
				: mInCtl_0("Control#1", this, &ProcFeatSeveralControls::InCtl0_cb ),
				  mInCtl_1("Control#2", this, &ProcFeatSeveralControls::InCtl1_cb ),
				  mInCtl_2("Control#3", this, &ProcFeatSeveralControls::InCtl2_cb ),
				  mInCtl_3("Control#4", this, &ProcFeatSeveralControls::InCtl3_cb ),
				  mInCtl_4("Control#5", this, &ProcFeatSeveralControls::InCtl4_cb ),
				  mInCtl_5("Control#6", this, &ProcFeatSeveralControls::InCtl5_cb ),
				  mTimesCalled0( 0 ), mTimesCalled1( 0 ), mTimesCalled2( 0 ), mTimesCalled3( 0 ),
				  mTimesCalled4( 0 ), mTimesCalled5( 0 )
		{
				Configure( cfg );
		}

		ProcFeatSeveralControls::~ProcFeatSeveralControls()
		{
		}

		bool ProcFeatSeveralControls::Do()
		{
				return true;
		}

		bool ProcFeatSeveralControls::ConcreteConfigure( const ProcessingConfig& cfg )
		{
				CopyAsConcreteConfig( mConfig, cfg );

				return true;
		}

		bool ProcFeatSeveralControls::ConcreteStart()
		{
				mInCtl_0.DoControl( 0 );
				mInCtl_1.DoControl( 0 );
				mInCtl_2.DoControl( 0 );
				mInCtl_3.DoControl( 0 );
				mInCtl_4.DoControl( 0 );
				mInCtl_5.DoControl( 0 );

				return true;
		}

		bool ProcFeatSeveralControls::ConcreteStop()
		{
				return true;
		}

		int ProcFeatSeveralControls::GetControlTimesCalled( int id )
		{
			if ( id == 0 ) return mTimesCalled0;
			if ( id == 1 ) return mTimesCalled1;
			if ( id == 2 ) return mTimesCalled2;
			if ( id == 3 ) return mTimesCalled3;
			if ( id == 4 ) return mTimesCalled4;
			if ( id == 5 ) return mTimesCalled5;

			return 0;
		}

		float ProcFeatSeveralControls::GetControlLastValue( int id )
		{
			if ( id == 0 ) return mInCtl_0.GetLastValue();
			if ( id == 1 ) return mInCtl_1.GetLastValue();
			if ( id == 2 ) return mInCtl_2.GetLastValue();
			if ( id == 3 ) return mInCtl_3.GetLastValue();
			if ( id == 4 ) return mInCtl_4.GetLastValue();
			if ( id == 5 ) return mInCtl_5.GetLastValue();

			return -1.0f;

		}

		int ProcFeatSeveralControls::InCtl0_cb( TControlData value )
		{
				mTimesCalled0++;

				return 0;
		}

		int ProcFeatSeveralControls::InCtl1_cb( TControlData value )
		{
				mTimesCalled1++;

				return 0;
		}

		int ProcFeatSeveralControls::InCtl2_cb( TControlData value )
		{
				mTimesCalled2++;

				return 0;
		}

		int ProcFeatSeveralControls::InCtl3_cb( TControlData value )
		{
				mTimesCalled3++;

				return 0;
		}

		int ProcFeatSeveralControls::InCtl4_cb( TControlData value )
		{
				mTimesCalled4++;

				return 0;
		}

		int ProcFeatSeveralControls::InCtl5_cb( TControlData value )
		{
				mTimesCalled5++;

				return 0;
		}
}

