#include "DummyFDFWidget.hxx"
#include <iostream>

namespace CLAMVM
{
	DummyFDFWidget::DummyFDFWidget()
	{
		ReceiveLowCutoffRange.Wrap( this, &DummyFDFWidget::_OnNewLowCutoffRange );
		ReceiveHighCutoffRange.Wrap( this, &DummyFDFWidget::_OnNewHighCutoffRange );
		ReceiveGainRange.Wrap( this, &DummyFDFWidget::_OnNewGainRange );
	}

	DummyFDFWidget::~DummyFDFWidget()
	{
	}

	void DummyFDFWidget::Show()
	{
	}

	void DummyFDFWidget::Hide( )
	{
	}

	void DummyFDFWidget::AttachTo( ProcessingInControlSet& cs )
	{
		InputControlModel& lcm = cs.Retrieve( "Low Cutoff Frequency");
		InputControlModel& hcm = cs.Retrieve( "High Cutoff Frequency");
		InputControlModel& gcm = cs.Retrieve( "Gain" );
		InputControlModel& sbcm = cs.Retrieve( "Stop Band Slope" );
		InputControlModel& pbcm = cs.Retrieve( "Pass Band Slope" );

		lcm.ValueRangePublished.Connect( ReceiveLowCutoffRange );
		hcm.ValueRangePublished.Connect( ReceiveHighCutoffRange );
		gcm.ValueRangePublished.Connect( ReceiveGainRange );

		SendGain.Connect( gcm.UpdateValue );
		SendLowCutoff.Connect( lcm.UpdateValue );
		SendHighCutoff.Connect( hcm.UpdateValue );
		SendStopBand.Connect( sbcm.UpdateValue );
		SendPassBand.Connect( pbcm.UpdateValue );
	}

	void DummyFDFWidget::Detach()
	{
	}

	void DummyFDFWidget::_OnNewLowCutoffRange( CLAM::TControlData min, CLAM::TControlData max )
	{
		std::cout << "New low cutoff range:" << std::endl;
		std::cout << "\t + { " << min << ", " << max << std::endl;
	}

	void DummyFDFWidget::_OnNewHighCutoffRange( CLAM::TControlData min, CLAM::TControlData max )
	{
		std::cout << "New high cutoff range:" << std::endl;
		std::cout << "\t + { " << min << ", " << max << std::endl;
	}

	void DummyFDFWidget::_OnNewGainRange( CLAM::TControlData min, CLAM::TControlData max )
	{
		std::cout << "New gain range:" << std::endl;
		std::cout << "\t + { " << min << ", " << max << std::endl;		
	}
}

