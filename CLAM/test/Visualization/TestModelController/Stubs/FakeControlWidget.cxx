#include "FakeControlWidget.hxx"
#include "InputControlModel.hxx"
#include <iostream>

namespace CLAMDraft
{
		using CLAMVM::InputControlModel;

		FakeControlWidget::FakeControlWidget()
		{
				ReceiveValue.Wrap( this, &FakeControlWidget::OnNewValue );
		}

		FakeControlWidget::~FakeControlWidget()
		{
		}

		void FakeControlWidget::OnNewValue( TData value )
		{
				std::cout << "Model value: " << value  << std::endl;
		}

		void FakeControlWidget::AttachTo( InputControlModel& model )
		{
				model.ValuePublished.Connect( ReceiveValue );
				SendValue.Connect( model.UpdateValue );				
		}

		void FakeControlWidget::Detach()
		{
				ReceiveValue.Unbind();
		}
		
		void FakeControlWidget::Show()
		{
		}

		void FakeControlWidget::Hide()
		{
		}

}

