#include "DescriptorComputationConfig.hxx"

namespace CLAM
{
	DescriptorComputationConfig::~DescriptorComputationConfig()
	{
	}

	void DescriptorComputationConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
	}

}
