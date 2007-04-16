#include "FactoryOnOtherTranslationUnits.hxx"


namespace CLAMTest
{

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

