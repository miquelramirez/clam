#include "WindowGeneratorConfig.hxx"

namespace CLAM
{
	void WindowGeneratorConfig::DefaultInit()
	{
		/* All Attributes are added */
		AddType();
		AddSize();
		AddMaxSize();
		AddUseTable();
		AddNormalize();
		AddInvert();

		UpdateData();
		DefaultValues();
	}

	void WindowGeneratorConfig::DefaultValues()
	{
		SetUseTable(true);
		SetSize(4097);
		SetNormalize(EWindowNormalize::eAnalysis);
		SetInvert(false);
		SetType(EWindowType::eHamming);
	}

}
