#include "SpectralPeaksPresentation.hxx"
#include "SpectrumModel.hxx"
#include "SpectralPeaksModel.hxx"

namespace CLAMVM
{

	SpectrumPlusPeaksPresentation::SpectrumPlusPeaksPresentation()
	{
		SetSpectrum.Wrap( this, &SpectrumPlusPeaksPresentation::OnNewSpectrum );
		SetPartials.Wrap( this, &SpectrumPlusPeaksPresentation::OnNewPeakArray );
	}
	
	SpectrumPlusPeaksPresentation::~SpectrumPlusPeaksPresentation()
	{
	}

	void SpectrumPlusPeaksPresentation::AttachTo( SpectrumModel& specModel, SpectralPeaksModel& peaksModel )
	{
		specModel.ObjectPublished.Connect( SetSpectrum );
		
		peaksModel.ObjectPublished.Connect( SetPartials );
		
	}
	
	void SpectrumPlusPeaksPresentation::Detach()
	{
		SetSpectrum.Unbind();
		SetPartials.Unbind();
	}
	
}



