#ifndef __FACTORYONOTHERTRANSLATIONUNITS__
#define __FACTORYONOTHERTRANSLATIONUNITS__

#include "Factory.hxx" // CLAM
#include "DummyProducts.hxx"

namespace CLAMTest
{

class FactoryOfAs : public CLAM::Factory< A >
{
public:
	static FactoryOfAs& GetInstance();	
};

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

