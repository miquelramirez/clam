#ifndef _OverlapAddConfig_
#define _OverlapAddConfig_

namespace CLAM {

	#include "ProcessingConfig.hxx"

	/**
	 * Config class for the OverlapAdd Processing
	 */
	class OverlapAddConfig: public CLAM::ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (OverlapAddConfig, 4, CLAM::ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, TSize, HopSize);
		DYN_ATTRIBUTE (2, public, TSize, FrameSize);
		DYN_ATTRIBUTE (3, public, TSize, BufferSize);

		~OverlapAddConfig(){};
	protected:
		void DefaultInit();
		void DefaultValues();
	};

} // namespace CLAM

#endif
