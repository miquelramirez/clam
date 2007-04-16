#include "DummyTDWidget.hxx"
#include <iostream>

namespace CLAMVM
{
	DummyTDWidget::DummyTDWidget()
	{
	}

	DummyTDWidget::~DummyTDWidget()
	{
	}

	void DummyTDWidget::EmitValue( TControlData value )
	{
		SendValue.Emit( value );	
	}

	void DummyTDWidget::Show()
	{
	}

	void DummyTDWidget::Hide()
	{
	}

	void DummyTDWidget::OnNewValue( TControlData value )
	{
		std::cout << "New value received: " << value << std::endl;
	}

	void DummyTDWidget::OnNewRange( TControlData min, TControlData max )
	{
		std::cout << "New range received: " << std::endl;
		std::cout << "\t + min: " << min << std::endl;
		std::cout << "\t + max: " << max << std::endl;
	}
}

