#include "LogMagSpectrumAdapter.hxx"
#include "SpectrumConversions.hxx"
#include "GlobalEnums.hxx"
#include "Spectrum.hxx"
#include <algorithm>

namespace CLAMVM
{
	using CLAM::SpecTypeFlags;
	using CLAM::EScale;
	using CLAM::TSize;
	using CLAM::Linear2LogMagnitude;
	using CLAM::Complex2MagPhase;
	using CLAM::Polar2MagPhase;
	using CLAM::BPF2MagPhase;

	LogMagSpectrumAdapter::~LogMagSpectrumAdapter()
	{
	}

	void LogMagSpectrumAdapter::TrivialCaseConversion()
	{
		CLAM_DEBUG_ASSERT( mObserved->HasMagBuffer(), 
				   "The Spectrum object has no magnitude data or isn't properly configured!"  );
		CLAM_DEBUG_ASSERT( mObserved->HasPhaseBuffer(), 
				   "The Spectrum object has no phase data or isn't properly configured!" );
				
		if ( mObserved->GetScale() == EScale::eLinear )
			Linear2LogMagnitude( mObserved->GetMagBuffer(), mMagBuffer );
		else
		{
			TSize buffSizeMag =  mObserved->GetMagBuffer().Size();
			mMagBuffer.Resize( buffSizeMag );
			mMagBuffer.SetSize( buffSizeMag );
						
			std::copy( mObserved->GetMagBuffer().GetPtr(), 
				   mObserved->GetMagBuffer().GetPtr()+buffSizeMag, 
				   mMagBuffer.GetPtr() );
						
		}
				
		TSize buffSizePhase = mObserved->GetPhaseBuffer().Size();
		mPhaseBuffer.Resize( buffSizePhase );
		mPhaseBuffer.SetSize( buffSizePhase );
				
		std::copy( mObserved->GetPhaseBuffer().GetPtr(),
			   mObserved->GetPhaseBuffer().GetPtr()+buffSizePhase,
			   mPhaseBuffer.GetPtr() );
				
	}
		

	void LogMagSpectrumAdapter::TranslateSpectralData()
	{
		// Let's check if the observed object accomplishes the requirements
		// stated in the class' name

		SpecTypeFlags flags;
		mObserved->GetType(flags);

		if ( flags.bMagPhase )
		{
			TrivialCaseConversion();
		}
		else if ( flags.bComplex )
		{
			CLAM_DEBUG_ASSERT( mObserved->HasComplexArray(), 
					   "The Spectrum object is not consistent, expected buffer not found"  );

			Complex2MagPhase( mObserved->GetComplexArray(),
					  mMagBuffer, mPhaseBuffer );
			Linear2LogMagnitude( mMagBuffer );
		}
		else if ( flags.bPolar )
		{
			CLAM_DEBUG_ASSERT( mObserved->HasPolarArray(), 
					   "The Spectrum object is not consistent, expected buffer not found"  );

			Polar2MagPhase( mObserved->GetPolarArray(),
					mMagBuffer, mPhaseBuffer );
			Linear2LogMagnitude( mMagBuffer );
		}
		else if ( flags.bMagPhaseBPF )
		{
			CLAM_DEBUG_ASSERT( mObserved->HasMagBPF(), 
					   "The Spectrum object is not consistent, expected buffer not found"  );

			CLAM_DEBUG_ASSERT( mObserved->HasPhaseBPF(), 
					   "The Spectrum object is not consistent, expected buffer not found"  );

			BPF2MagPhase( mObserved->GetMagBPF(), mObserved->GetPhaseBPF(),
				      mMagBuffer, mPhaseBuffer, 512, mObserved->GetSpectralRange() );
						
			Linear2LogMagnitude( mMagBuffer );
		}
	
	}

	bool LogMagSpectrumAdapter::Publish()
	{
		if ( !mObserved )
			return false;

		TranslateSpectralData();

		if ( mMagBuffer.Size() > 0 )
			MagnitudePublished.Emit( mMagBuffer );
		if ( mPhaseBuffer.Size() > 0 )
			PhasePublished.Emit( mPhaseBuffer );
		
		SpectralRangePublished.Emit( mObserved->GetSpectralRange() );
		ObjectPublished.Emit( mMagBuffer,  mObserved->GetSpectralRange() );

		return true;
	}

}
