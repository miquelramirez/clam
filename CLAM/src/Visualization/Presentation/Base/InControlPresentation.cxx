#include "InControlPresentation.hxx"
#include "InputControlModel.hxx"

namespace CLAMVM
{
		InControlPresentation::InControlPresentation()
		{
				ReceiveValue.Wrap( this, &InControlPresentation::OnNewValue );
				ReceiveValueRange.Wrap( this, &InControlPresentation::OnNewRange );
		}

		InControlPresentation::~InControlPresentation()
		{
		}

		void InControlPresentation::AttachTo( InputControlModel& model )
		{
				SendValue.Connect( model.UpdateValue );
				model.ValuePublished.Connect( ReceiveValue );
				model.ValueRangePublished.Connect( ReceiveValueRange );
		}

		void InControlPresentation::Detach()
		{
				ReceiveValue.Unbind();
				ReceiveValueRange.Unbind();
		}
}
