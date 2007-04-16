#ifndef __FAKECONTROLWIDGET__
#define __FAKECONTROLWIDGET__

#include <Presentation.hxx>
#include <Slotv1.hxx>
#include <Signalv1.hxx>
#include <DataTypes.hxx>

namespace CLAMVM
{
	class InputControlModel;
}

namespace CLAMDraft
{
		
		using CLAMVM::Presentation;
		using SigSlot::Slotv1;
		using SigSlot::Signalv1;
		using CLAM::TData;
		using CLAMVM::InputControlModel;

		class FakeControlWidget 
				: public Presentation
		{
		protected:

				virtual void OnNewValue( TData value );

		public:
				
				virtual void AttachTo( InputControlModel& );
				
				virtual void Detach();
				
				virtual void Show();

				virtual void Hide();
				
				FakeControlWidget();

				~FakeControlWidget();
				
				Slotv1<TData>   ReceiveValue;
				Signalv1<TData> SendValue;

				
		};
}


#endif // FakeControlWidget.hxx

