#ifndef __FACTORYONOTHERTRANSLATIONUNITS__
#define __FACTORYONOTHERTRANSLATIONUNITS__

#include "Factory.hxx" // CLAM
#include "DummyProducts.hxx"

namespace CLAMTest
{

  typedef CLAM::Factory< A > FactoryOfAs;

class RegistratorStub
{

public:

  RegistratorStub( );

  const FactoryOfAs* AddressOfFactory() const
  {
    return mToFactory;
  }

  static RegistratorStub& StaticInstance();

private:

  FactoryOfAs* mToFactory;
  
  static RegistratorStub sOnlyInstance;

};


}

#endif 

