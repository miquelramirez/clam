#ifndef DummyConfigurations_hxx
#define DummyConfigurations_hxx

#include <CLAM/DynamicType.hxx>

namespace IpyclamTests {
	class DummyDTWithJustStrings;
	class DummyDTWithJustIntegers;
	class DummyDTWithEveryType;
	class DummyDTWithOptionalAttributes;
	class DummyTestsConfig;
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

class IpyclamTests::DummyDTWithEveryType : public CLAM::ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (DummyDTWithEveryType, 5, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, StringAttribute);
	DYN_ATTRIBUTE (1, public, int, IntAttribute);
	DYN_ATTRIBUTE (2, public, bool, BoolAttribute);
	DYN_ATTRIBUTE (3, public, float, FloatAttribute);
	DYN_ATTRIBUTE (4, public, double, DoubleAttribute);
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetStringAttribute("String Attribute");
		SetIntAttribute(42);
		SetBoolAttribute(0);
		SetFloatAttribute(3.140000104904175);
		SetDoubleAttribute(134.15);
	}
};

class IpyclamTests::DummyDTWithOptionalAttributes : public CLAM::ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (DummyDTWithOptionalAttributes, 3, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, StringAttribute);
	DYN_ATTRIBUTE (1, public, int, FirstInt);
	DYN_ATTRIBUTE (2, public, int, SecondInt);
	void DefaultInit(void)
	{
		AddAll();
		RemoveAttr_(2);
		UpdateData();
		SetStringAttribute("String Attribute");
		SetFirstInt(42);
	}
};

class IpyclamTests::DummyTestsConfig : public CLAM::ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (DummyTestsConfig, 2, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, ConfigParam1);
	DYN_ATTRIBUTE (1, public, std::string, ConfigParam2);
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetConfigParam1("default1");
		SetConfigParam2("default2");
	}
};

#endif //DummyConfigurations_hxx
