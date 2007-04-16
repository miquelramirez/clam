#include "TestCases.hxx"
#include "ProcFeatSeveralControls.hxx"
#include "ProcessingController.hxx"
#include "FakeCompositeControlWidget.hxx"
#include <Assert.hxx>

namespace CLAMDraft
{
	using CLAMVM::ProcessingController;
	
		void TestBindingProcessingAdapter()
		{
			ProcFeatSeveralControls        proc;
			ProcFeatSeveralControlsConfig  procCfg;
			ProcessingController           modCont;
			
			proc.Configure( procCfg );

			CLAM_ASSERT( modCont.BindTo( proc ), "Binding has failed!");

		}
		
		void TestAttachPresentationAdapter( )
		{
			ProcFeatSeveralControls        proc;
			ProcFeatSeveralControlsConfig  procCfg;

			ProcessingController        modCont;
			FakeCompositeControlWidget     compoWidget;
			proc.Configure( procCfg );
			modCont.BindTo( proc );
			compoWidget.AttachTo( modCont );
				
		}

		bool AssertTimesCalled( ProcFeatSeveralControls& stubProc, int expectedTimes  )
		{
			if ( stubProc.GetControlTimesCalled( 0 ) != expectedTimes ) return false;
			if ( stubProc.GetControlTimesCalled( 1 ) != expectedTimes ) return false;
			if ( stubProc.GetControlTimesCalled( 2 ) != expectedTimes ) return false;
			if ( stubProc.GetControlTimesCalled( 3 ) != expectedTimes ) return false;
			if ( stubProc.GetControlTimesCalled( 4 ) != expectedTimes ) return false;
			if ( stubProc.GetControlTimesCalled( 5 ) != expectedTimes ) return false;

			return true;
		}

		bool AssertValuesReceived(	ProcFeatSeveralControls& stubProc, 
									std::vector<float>& values )
		{
			if ( stubProc.GetControlLastValue( 0 ) != values[0] ) return false;
			if ( stubProc.GetControlLastValue( 1 ) != values[1] ) return false;
			if ( stubProc.GetControlLastValue( 2 ) != values[2] ) return false;
			if ( stubProc.GetControlLastValue( 3 ) != values[3] ) return false;
			if ( stubProc.GetControlLastValue( 4 ) != values[4] ) return false;
			if ( stubProc.GetControlLastValue( 5 ) != values[5] ) return false;

			return true;

		}

		void TestControlUpdating()
		{
			ProcFeatSeveralControls        proc;
			ProcFeatSeveralControlsConfig  procCfg;
			ProcessingController        modCont;
			FakeCompositeControlWidget     compoWidget;


			modCont.BindTo( proc );
			
			compoWidget.AttachTo( modCont );

			std::vector<float> values;
			values.resize( 6 );
			values[0] = 33.0f;
			values[1] = 21.0f;
			values[2] = 15.0f;
			values[3] = 100.0f;
			values[4] = 1000.0f;
			values[5] = 10000.0f;

			compoWidget.SendTheseValues( values );
			modCont.Update();

			CLAM_ASSERT( AssertTimesCalled( proc, 1 ), "Times called mismatch!" );
			CLAM_ASSERT( AssertValuesReceived( proc, values ), "Values received mismatch!" );

		}

		void TestSeveralControlsUpdating()
		{
			ProcFeatSeveralControls        proc;

			ProcFeatSeveralControlsConfig  procCfg;
			ProcessingController        modCont;
			FakeCompositeControlWidget     compoWidget;

			modCont.BindTo( proc );
			
			compoWidget.AttachTo( modCont );

			std::vector<float> values;
			values.resize( 6 );
			values[0] = 33.0f;
			values[1] = 21.0f;
			values[2] = 15.0f;
			values[3] = 100.0f;
			values[4] = 1000.0f;
			values[5] = 10000.0f;

			for ( int i = 0; i < 11; i++ )
			{
					values[0]+=i;
					values[1]+=i;
					values[2]+=i;
					values[3]+=i;
					values[4]+=i;
					values[5]+=i;

					compoWidget.SendTheseValues( values );	
					
					modCont.Update();
			}

			CLAM_ASSERT( AssertTimesCalled( proc, 11 ), "Times called mismatch!" );
			CLAM_ASSERT( AssertValuesReceived( proc, values ), "Values received mismatch!" );
	
		}
}

