
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

}
