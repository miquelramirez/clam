#include "SpectrumPresentation.hxx"
#include "SpectrumModel.hxx"

namespace CLAMVM
{
		SpectrumPresentation::SpectrumPresentation()
		{
				SetSpectrum.Wrap( this, &SpectrumPresentation::OnNewSpectrum );
		}

		SpectrumPresentation::~SpectrumPresentation()
		{
		}

		void SpectrumPresentation::AttachTo( SpectrumModel& model )
		{				
				model.ObjectPublished.Connect( SetSpectrum );
		}

		void SpectrumPresentation::Detach()
		{
				SetSpectrum.Unbind();
		}
}
