#include "SMSTransformationConfig.hxx"

namespace CLAM
{
	void SMSTransformationConfig::DefaultInit()
	{
		AddAmount();
		UpdateData();
		DefaultValues();
	}

	void SMSTransformationConfig::DefaultValues()
	{
		SetAmount(0);
	}

}
