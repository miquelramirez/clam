#ifndef _SinTrackingConfig_
#define _SinTrackingConfig_

namespace CLAM {

	#include "ProcessingConfig.hxx"

	/**
	 * Config class for the SinTracking Processing
	 */
	class SinTrackingConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SinTrackingConfig, 3,ProcessingConfig);
		/** Frequency deviation in percentage allowed for a peak to be continued (default: 20). */
		DYN_ATTRIBUTE (0, public, TData, Threshold);
		/** Maximum number of sines which can be tracked at once (default: 50). */
		DYN_ATTRIBUTE (1, public, TSize, nMaxSines);
		/**
		* Determines if an algorithm specialized for inharmonic input 
		* is used or one specialized harmonic input is used. Default: false.
		*/
		DYN_ATTRIBUTE (2, public, bool, IsHarmonic);

		void DefaultInit();
		void DefaultValues();
	};
	
} // namespace CLAM

#endif
