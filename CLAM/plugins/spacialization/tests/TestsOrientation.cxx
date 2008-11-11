#include "MiniCppUnit.hxx"
#include "AbsoluteCoordinates2RelativeAngles.hxx"


class TestsOrientation : public TestFixture<TestsOrientation>
{
public:
	typedef CLAM::AbsoluteCoordinates2RelativeAngles::Orientation Orientation;
	TEST_FIXTURE( TestsOrientation )
	{
		TEST_CASE( test );
	}
	void test()
	{
		Orientation orientation;
		ASSERT_EQUALS(0.0, orientation.azimuth );
	}
};

REGISTER_FIXTURE(TestsOrientation)


