#ifndef DummyConfigurations_hxx
#define DummyConfigurations_hxx

#include <CLAM/DynamicType.hxx>

namespace IpyclamTests {
	class DummyDTWithJustStrings;
	class DummyDTWithJustIntegers;
}

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

class IpyclamTests::DummyDTWithJustIntegers : public CLAM::ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (DummyDTWithJustIntegers, 2, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, int, FirstInt);
	DYN_ATTRIBUTE (1, public, int, SecondInt);
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetFirstInt(42);
		SetSecondInt(15);
	}
};

#endif //DummyConfigurations_hxx
