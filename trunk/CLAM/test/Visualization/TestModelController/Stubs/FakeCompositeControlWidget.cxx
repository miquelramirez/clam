#include "FakeCompositeControlWidget.hxx"
#include "ProcessingInControlSet.hxx"

namespace CLAMDraft
{
		using CLAMVM::ProcessingInControlSet;

		FakeCompositeControlWidget::FakeCompositeControlWidget()
		{
		}

		FakeCompositeControlWidget::~FakeCompositeControlWidget()
		{
		}

		void FakeCompositeControlWidget::AttachTo( ProcessingInControlSet& set  )
		{
				mControl1.AttachTo( set.Retrieve( "Control#1" ) );
				mControl2.AttachTo( set.Retrieve( "Control#2" ) );
				mControl3.AttachTo( set.Retrieve( "Control#3" ) );
				mControl4.AttachTo( set.Retrieve( "Control#4" ) );
				mControl5.AttachTo( set.Retrieve( "Control#5" ) );
				mControl6.AttachTo( set.Retrieve( "Control#6" ) );
		}

		void FakeCompositeControlWidget::Detach()
		{
				mControl1.Detach();
				mControl2.Detach();
				mControl3.Detach();
				mControl4.Detach();
				mControl5.Detach();
				mControl6.Detach();
		}

		void FakeCompositeControlWidget::SendTheseValues( const std::vector<TData>& values )
		{
				mControl1.SendValue.Emit( values[0] );
				mControl2.SendValue.Emit( values[1] );
				mControl3.SendValue.Emit( values[2] );
				mControl4.SendValue.Emit( values[3] );
				mControl5.SendValue.Emit( values[4] );
				mControl6.SendValue.Emit( values[5] );
				
		}

		void FakeCompositeControlWidget::Show()
		{
		}

		void FakeCompositeControlWidget::Hide()
		{
		}
}

