#ifndef __ERB_SPACE_GEN_CONFIG__
#define __ERB_SPACE_GEN_CONFIG__

#include "ProcessingConfig.hxx"
#include "DataTypes.hxx"

namespace CLAM
{
	class ERB_SpaceGenConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (ERB_SpaceGenConfig, 3,ProcessingConfig);
		DYN_ATTRIBUTE (0,public, TSize, NumFilter);
		DYN_ATTRIBUTE (1,public, TData, LowFreq);
		DYN_ATTRIBUTE (2,public, TData, HighFreq);
	protected:	
		void DefaultInit();
};


}

#endif // ERB_Space_Gen_Config.hxx
