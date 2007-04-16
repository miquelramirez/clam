#ifndef __FAKECOMPOSITECONTROLWIDGET__
#define __FAKECOMPOSITECONTROLWIDGET__

#include <Presentation.hxx>
#include "FakeControlWidget.hxx"
#include <vector>

namespace CLAMVM
{
		class ProcessingInControlSet;
}

namespace CLAMDraft
{
		using CLAMVM::ProcessingInControlSet;
		using CLAMVM::Presentation;

		class FakeCompositeControlWidget : public Presentation
		{
		private:
				FakeControlWidget        mControl1;
				FakeControlWidget        mControl2;
				FakeControlWidget        mControl3;
				FakeControlWidget        mControl4;
				FakeControlWidget        mControl5;
				FakeControlWidget        mControl6;

		protected:
		public:

				FakeCompositeControlWidget( );
				
				~FakeCompositeControlWidget();

				void SendTheseValues( const std::vector<float>& values );
			
				virtual void AttachTo( ProcessingInControlSet& );

				virtual void Detach();

				virtual void Show();

				virtual void Hide();

		};
		
}


#endif // FakeCompositeControlWidget.hxx

