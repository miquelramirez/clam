#ifndef __GT_FILTERBANKCONFIG__
#define __GT_FILTERBANKCONFIG__

#include "ProcessingConfig.hxx"

namespace CLAM
{
	class GT_FilterBankConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (GT_FilterBankConfig, 4, ProcessingConfig);

		DYN_ATTRIBUTE (0, public, TData, LowFreq);
		DYN_ATTRIBUTE (1, public, TData, HighFreq);
		DYN_ATTRIBUTE (2, public, int,   nChannels);
		DYN_ATTRIBUTE (3, public, int,   SampleRate);
		
	protected:
		void DefaultInit(void);
};

}

#endif // GT_FilterBankConfig.hxx
