#include "TestCases.hxx"
#include "SimpleProcessing.hxx"
#include "ControlAdapter.hxx"
#include "FakeControlWidget.hxx"

namespace CLAMDraft
{
	using CLAMVM::ControlAdapter;	

	void TestBindingProcessingAdapter()
	{
		SimpleProcessing       proc;
		SimpleProcessingConfig procCfg;
		ControlAdapter         adap;

		proc.Configure( procCfg );

		CLAM_ASSERT( adap.BindTo( proc.GetControl() ), "Binding Failed" );
		
	}

	void TestAttachPresentationAdapter()
	{
		ControlAdapter      adap;
		FakeControlWidget   fakeWidget;

		fakeWidget.AttachTo( adap );
	}

	void TestControlUpdating()
	{
		SimpleProcessing       proc;
		SimpleProcessingConfig procCfg;
		ControlAdapter         adap;
		FakeControlWidget      fakeWidget;		

		proc.Configure( procCfg );

		adap.BindTo( proc.GetControl() );

		adap.Publish();

		fakeWidget.AttachTo( adap );

		fakeWidget.SendValue.Emit( 10 );

		adap.Update();

		CLAM_ASSERT( proc.GetControl().GetLastValue() == 10 , 
					"Value was not received by model object" );

	}


	void TestSeveralControlsUpdating()
	{
		SimpleProcessing       proc;
		SimpleProcessingConfig procCfg;
		ControlAdapter         adap;
		FakeControlWidget      fakeWidget;		

		proc.Configure( procCfg );

		adap.BindTo( proc.GetControl() );

		fakeWidget.AttachTo( adap );


		for ( int i = 0; i < 11; i++ )
		{
			fakeWidget.SendValue.Emit( i );
		}

		adap.Update();

		CLAM_ASSERT( proc.GetControl().GetLastValue() == 10 , 
					"Value was not received by model object" );


	}

}

