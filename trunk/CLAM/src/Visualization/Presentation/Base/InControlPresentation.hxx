#ifndef __INCONTROLPRESENTATION__
#define __INCONTROLPRESENTATION__

#include "Presentation.hxx"
#include "Signalv1.hxx"
#include "Slotv1.hxx"
#include "Slotv2.hxx"

namespace CLAMVM
{
	using SigSlot::Signalv1;
	using SigSlot::Slotv1;
	using SigSlot::Slotv2;

	class InputControlModel;
		
	typedef float             TControlData;

	class InControlPresentation : public Presentation
	{

	protected:
		virtual void OnNewValue( TControlData value ) = 0;
				
		virtual void OnNewRange( TControlData min, TControlData max ) = 0;

	public:
		InControlPresentation();
		virtual ~InControlPresentation();
				
		virtual void Show() = 0;
		virtual void Hide() = 0;

		virtual void AttachTo( InputControlModel& );
		virtual void Detach();

		Signalv1<TControlData>            SendValue;
		Slotv1<TControlData>              ReceiveValue;
		Slotv2<TControlData,TControlData> ReceiveValueRange;
	};
		
}

#endif // InControlPresentation.hxx
