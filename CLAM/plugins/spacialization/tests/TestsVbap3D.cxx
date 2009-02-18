
#include "MiniCppUnit.hxx"
#include "Vbap3D.hxx"

class TestsVbap3D : public TestFixture<TestsVbap3D>
{
public:
	TEST_FIXTURE( TestsVbap3D )
	{
		TEST_CASE( test_front );
	}
	void test_front()
	{
		Vbap3D v;
		ASSERT_EQUALS( 1, v.findTriangle(0., 89.));
//		ASSERT_EQUALS( 0, v.findTriangle(0., -70.));
//		ASSERT_EQUALS( 3, v.findTriangle(45., -44.));
	}
};

REGISTER_FIXTURE(TestsVbap3D)


