#ifndef __DUMMYFDFWIDGET__
#define __DUMMYFDFWIDGET__

#include "ProcessingControlsPresentation.hxx"
#include "ProcessingInControlSet.hxx"
#include "InputControlModel.hxx"
#include "Signalv1.hxx"
#include "Slotv2.hxx"

namespace CLAMVM
{
	using SigSlot::Signalv1;
	using SigSlot::Slotv2;
	
	class DummyFDFWidget 
		: public ProcessingInControlsPresentation
	{
	public:
		DummyFDFWidget();

		virtual ~DummyFDFWidget();

		void Show();
		void Hide();

		void AttachTo( ProcessingInControlSet& s );
		void Detach();

		Signalv1<CLAM::TControlData>    SendGain;
		Signalv1<CLAM::TControlData>    SendLowCutoff;
		Signalv1<CLAM::TControlData>    SendHighCutoff;
		Signalv1<CLAM::TControlData>    SendStopBand;
		Signalv1<CLAM::TControlData>    SendPassBand;


		Slotv2<CLAM::TControlData, CLAM::TControlData> ReceiveLowCutoffRange;
		Slotv2<CLAM::TControlData, CLAM::TControlData> ReceiveHighCutoffRange;
		Slotv2<CLAM::TControlData, CLAM::TControlData> ReceiveGainRange;
		
	protected:

		void _OnNewLowCutoffRange( CLAM::TControlData min, CLAM::TControlData max );
		void _OnNewHighCutoffRange( CLAM::TControlData min, CLAM::TControlData max );
		void _OnNewGainRange( CLAM::TControlData min, CLAM::TControlData max );

	};
}

#endif // DummyFDFWidget.hxx

