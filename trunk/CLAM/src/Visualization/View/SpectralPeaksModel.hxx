#ifndef __SPECTRALPEAKSMODEL__
#define __SPECTRALPEAKSMODEL__

#include "DataTypes.hxx"
#include "Array.hxx"
#include "Partial.hxx"
#include "Signalv1.hxx"

namespace CLAMVM
{
	using SigSlot::Signalv1;
	using CLAM::Array;


	class SpectralPeaksModel
	{
	public:
				
		SpectralPeaksModel(  );
				
		virtual ~SpectralPeaksModel();
								
		// Signals
		Signalv1< const Array<Partial>& >       ObjectPublished;

	};

}

#endif // SpectralPeakArrayAspect.hxx
