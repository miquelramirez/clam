#ifndef __INPUTCONTROLMODEL__
#define __INPUTCONTROLMODEL__

#include "Signalv1.hxx"
#include "Signalv2.hxx"
#include "Slotv1.hxx"
#include "OutControl.hxx"

namespace CLAMVM
{
	using SigSlot::Signalv1;
	using SigSlot::Signalv2;
	using SigSlot::Slotv1;

	class InputControlModel
	{
	protected:

		virtual void DispatchControlValue( CLAM::TControlData value ) = 0;

	public:

		InputControlModel( );

		virtual ~InputControlModel();

		// Signals
		Signalv1< CLAM::TControlData >                      ValuePublished;
		Signalv2< CLAM::TControlData, CLAM::TControlData >  ValueRangePublished;
				
		// Slots
		Slotv1< CLAM::TControlData>                         UpdateValue;

	};
}

#endif // InputControlModel.hxx
