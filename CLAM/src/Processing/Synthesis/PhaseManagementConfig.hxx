#ifndef _PhaseManagementConfig_
#define _PhaseManagementConfig_

#include "EPhaseGeneration.hxx"
#include "ProcessingConfig.hxx"

namespace CLAM {

	/**
	 * Config class for the PhaseManagement Processing
	 */
class PhaseManagementConfig:public ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE (PhaseManagementConfig,4,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
	DYN_ATTRIBUTE(1,public,TSize,MaxSines);
	DYN_ATTRIBUTE(2,public,TData,SamplingRate);
	DYN_ATTRIBUTE(3,public,EPhaseGeneration,Type);
public:
	~PhaseManagementConfig(){};
	void DefaultInit();
	void DefaultValues();
};

} // namespace CLAM

#endif
