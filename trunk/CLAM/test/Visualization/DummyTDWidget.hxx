#ifndef __DUMMYTDWIDGET__
#define __DUMMYTDWIDGET__

#include "InControlPresentation.hxx"

namespace CLAMVM
{
	class DummyTDWidget : public InControlPresentation
	{
	public:

		DummyTDWidget();
		~DummyTDWidget();

		void EmitValue( TControlData value );

		virtual void Show();
		virtual void Hide();

	protected:

		virtual void OnNewValue( TControlData value );

		virtual void OnNewRange( TControlData min, TControlData max );
	};

}

#endif // DummyTDWidget.hxx

