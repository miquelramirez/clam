#ifndef _OverlapAddConfig_
#define _OverlapAddConfig_

#include "ProcessingConfig.hxx"

namespace CLAM 
{
	/**
	 * Config class for the OverlapAdd Processing
	 */
	class OverlapAddConfig: public CLAM::ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (OverlapAddConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, TSize, HopSize);
		DYN_ATTRIBUTE (1, public, TSize, FrameSize);
	protected:
		void DefaultInit();
		void DefaultValues();
	};

} // namespace CLAM

#endif
