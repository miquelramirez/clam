#ifndef _SpectralEnvelopeExtractConfig_
#define _SpectralEnvelopeExtractConfig_

#include "ProcessingConfig.hxx"
#include "GlobalEnums.hxx"

namespace CLAM {

	/**
	 * Config class for the SpectralEnvelopeExtract Processing
	 */
	class SpectralEnvelopeExtractConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectralEnvelopeExtractConfig, 2,ProcessingConfig);
		/** Name of the SpectralEnvelopeExtract object*/
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/** Type of interpolation for resultant Spectrum BPF*/
		DYN_ATTRIBUTE (1, public, EInterpolation, InterpolationType);
		
	protected:
		void DefaultInit();
	public:
	  /** Initialize configuration object with default values (name 'SpectralEnvelopeExtract', 513 bands, Magnitude treshold set to -80 and MaxSpectralPeaks setted to 100
	   */
		void DefaultValues();
		~SpectralEnvelopeExtractConfig(){};
		
 	}; 

} // namespace CLAM

#endif
