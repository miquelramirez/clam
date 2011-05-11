#ifndef DummyConfigurations_hxx
#define DummyConfigurations_hxx

#include <CLAM/DynamicType.hxx>

namespace IpyclamTests { class DummyDTWithJustStrings; }


class IpyclamTests::DummyDTWithJustStrings : public CLAM::ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (DummyDTWithJustStrings, 2, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, UnString);
	DYN_ATTRIBUTE (1, public, std::string, UnAltreString);
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetUnString("ValorPerDefecte");
		SetUnAltreString("Un altre valor per defecte");
	}
};

#endif//DummyConfigurations_hxx
