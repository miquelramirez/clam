#ifndef _IFFTConfig_
#define _IFFTConfig_

namespace CLAM {

	#include "ProcessingConfig.hxx"

	/**
	 * Config class for the IFFT Processing
	 */
	class IFFTConfig: public ProcessingConfig {
	public:
		DYNAMIC_TYPE_USING_INTERFACE  (IFFTConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE(0,public,  std::string,Name);
		DYN_ATTRIBUTE (1,public,int, AudioSize);
	protected:
		/** Dynamic type initialization. */
		void DefaultInit(void);
	};

} // namespace CLAM

#endif
