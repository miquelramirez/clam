#include "StdioSpectralPeakArrayPresentation.hxx"
#include "SpectralPeaksModel.hxx"
#include <algorithm>
#include <iostream>


namespace CLAMVM
{
		StdioSpectralPeakArrayPresentation::StdioSpectralPeakArrayPresentation()
		{
				SetPartials.Wrap( this, &StdioSpectralPeakArrayPresentation::OnNewPartials );
		}
		
		StdioSpectralPeakArrayPresentation::~StdioSpectralPeakArrayPresentation()
		{
		}

		void StdioSpectralPeakArrayPresentation::Show()
		{
				std::cout << "DATA RETRIEVED: " << std::endl;
				
				for ( int i = 0; i < mPartialsToDraw.Size(); i++ )
				{
						std::cout << "Peak #" << i+1 << std::endl;
						std::cout << "Magnitude (dB): " << mPartialsToDraw[i].mMag << std::endl;
						std::cout << "Frequency (Hz): " << mPartialsToDraw[i].mFreq << std::endl;
						std::cout << "Phase    (rad):" << mPartialsToDraw[i].mPhase << std::endl;
				}
		}

		void StdioSpectralPeakArrayPresentation::Hide()
		{
		}

		void StdioSpectralPeakArrayPresentation::AttachTo( SpectralPeaksModel& peakModel )
		{

				peakModel.ObjectPublished.Connect( SetPartials );
		}

		void StdioSpectralPeakArrayPresentation::Detach()
		{
			SetPartials.Unbind();
		}

		void StdioSpectralPeakArrayPresentation::OnNewPartials( const Array<Partial>& array )
		{
				mPartialsToDraw.Resize( array.Size() );
				mPartialsToDraw.SetSize( array.Size() );

				std::copy( array.GetPtr(), array.GetPtr()+array.Size(), mPartialsToDraw.GetPtr() );
		}
}

