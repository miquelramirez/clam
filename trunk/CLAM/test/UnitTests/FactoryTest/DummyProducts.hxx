#ifndef __DUMMYPRODUCTS__
#define __DUMMYPRODUCTS__

namespace CLAMTest
{

	class DummyProduct
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
	public:
		DummyProductFoo() {}
		virtual ~DummyProductFoo() {}
		virtual void dummyVirtual() {}
		virtual const char * GetClassName() const {return "DummyProductFoo";}
	};

	class DummyProductBar
		: public DummyProduct
	{
	public:
		DummyProductBar() {}
		virtual ~DummyProductBar() {}
		virtual void dummyVirtual() {}
		virtual const char * GetClassName() const {return "DummyProductBar";}
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
