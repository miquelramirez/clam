#ifndef __DUMMYPRODUCTS__
#define __DUMMYPRODUCTS__

#include "Component.hxx" // CLAM
#include "XMLStorage.hxx" // CLAM
#include "XMLAdapter.hxx" // CLAM

namespace CLAMTest
{

	class DummyProduct : public CLAM::Component
	{
	public:
		DummyProduct() {}
		virtual ~DummyProduct() {}
		virtual void dummyVirtual() =0;
		virtual const char * GetClassName() const {return "DummyProduct";}

	};

	class DummyProductFoo
		: public DummyProduct
	{
		std::string mFooContent;
	public:
		DummyProductFoo() {mFooContent="FooContent";}
		virtual ~DummyProductFoo() {}
		virtual void dummyVirtual() {}
		virtual const char * GetClassName() const {return "DummyProductFoo";}
		virtual void StoreOn(CLAM::Storage & storage) const
		{
			CLAM::XMLAdapter<std::string> adapter(mFooContent);
			storage.Store(adapter);			
		}
		virtual void LoadFrom(CLAM::Storage & storage)
		{
			CLAM::XMLAdapter<std::string> adapter(mFooContent);
			storage.Load(adapter);			
		}
		const std::string & fooContent() const
		{
			return mFooContent;
		}
	};

	class DummyProductBar
		: public DummyProduct
	{
		std::string mBarContent;
	public:
		DummyProductBar() {mBarContent="BarContent";}
		virtual ~DummyProductBar() {}
		virtual void dummyVirtual() {}
		virtual const char * GetClassName() const {return "DummyProductBar";}
		void StoreOn(CLAM::Storage & storage) const
		{
			CLAM::XMLAdapter<std::string> adapter(mBarContent);
			storage.Store(adapter);			
		}
		void LoadFrom(CLAM::Storage & storage)
		{
			CLAM::XMLAdapter<std::string> adapter(mBarContent);
			storage.Load(adapter);			
		}
		const std::string & barContent() const
		{
			return mBarContent;
		}
	};

  class A
  {
  public:
    virtual void foo() = 0;
    
    virtual ~A()
    {
    }
  };

  class B : public A
  {
  public:
    virtual void foo() {}
    
    virtual ~B()
    {
    }
  };


}

#endif

