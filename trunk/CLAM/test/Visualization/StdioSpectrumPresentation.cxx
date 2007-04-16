#include "StdioSpectrumPresentation.hxx"
#include "SpectrumModel.hxx"
#include <algorithm>
#include <iostream>

namespace CLAMVM
{
		StdioSpectrumPresentation::StdioSpectrumPresentation()
				: mSpectralRange( 22050 )
		{
				SetMagnitudeBins.Wrap( this, &StdioSpectrumPresentation::OnNewMagBins );
				SetPhaseBins.Wrap( this, &StdioSpectrumPresentation::OnNewPhaseBins );
				SetSpectralRange.Wrap( this, &StdioSpectrumPresentation::OnNewSpecRng );
		}

		StdioSpectrumPresentation::~StdioSpectrumPresentation()
		{
		}

		void StdioSpectrumPresentation::Show()
		{
				std::cout << "DATA RETRIEVED:" << std::endl;
				
				std::cout << "Highest magnitude value (dB): ";
				std::cout << *std::max_element(mBinsMagnitude.GetPtr(),mBinsMagnitude.GetPtr()+mBinsMagnitude.Size() );
				std::cout << std::endl;
				std::cout << "Lowest magnitude value (dB): ";
				std::cout << *std::min_element(mBinsMagnitude.GetPtr(),mBinsMagnitude.GetPtr()+mBinsMagnitude.Size() );
				std::cout << std::endl;

				std::cout << "Highest phase angle ( radians ):";
				std::cout << *std::max_element( mBinsPhase.GetPtr(),mBinsPhase.GetPtr()+mBinsPhase.Size() );
				std::cout << std::endl;				
				std::cout << "Lowest phase angle ( radians ):";
				std::cout << *std::max_element( mBinsPhase.GetPtr(),mBinsPhase.GetPtr()+mBinsPhase.Size() );
				std::cout << std::endl;

				std::cout << "Spectral range :" << mSpectralRange << std::endl;
 		}

		void StdioSpectrumPresentation::Hide()
		{
		}

		void StdioSpectrumPresentation::AttachTo( SpectrumModel& model )
		{
				
				model.MagnitudePublished.Connect( SetMagnitudeBins );
				model.PhasePublished.Connect( SetPhaseBins );
				model.SpectralRangePublished.Connect( SetSpectralRange );
		}

		void StdioSpectrumPresentation::Detach()
		{
				SetMagnitudeBins.Unbind();
				SetPhaseBins.Unbind();
				SetSpectralRange.Unbind();

		}

		void StdioSpectrumPresentation::OnNewMagBins( const DataArray& array )
		{
				mBinsMagnitude.Resize( array.Size() );
				mBinsMagnitude.SetSize( array.Size() );

				std::copy( array.GetPtr(), array.GetPtr() + array.Size(), mBinsMagnitude.GetPtr() );
		}

		void StdioSpectrumPresentation::OnNewPhaseBins( const DataArray& array )
		{
				mBinsPhase.Resize( array.Size() );
				mBinsPhase.SetSize( array.Size() );

				std::copy( array.GetPtr(), array.GetPtr() + array.Size(), mBinsPhase.GetPtr() );
				
		}
		
		void StdioSpectrumPresentation::OnNewSpecRng( TData specRange )
		{
				mSpectralRange = specRange;
		}

}

