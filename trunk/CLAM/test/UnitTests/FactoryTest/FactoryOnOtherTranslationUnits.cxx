#include "FactoryOnOtherTranslationUnits.hxx"


namespace CLAMTest
{

FactoryOfAs& FactoryOfAs::GetInstance()	
{
	static FactoryOfAs theInstance;
	return theInstance;
}

RegistratorStub RegistratorStub::sOnlyInstance;

RegistratorStub::RegistratorStub()
{
	mToFactory = &FactoryOfAs::GetInstance();
}

RegistratorStub& RegistratorStub::StaticInstance()
{
	return sOnlyInstance;
}

}

