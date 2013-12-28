#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "SpectralPeakArray.hxx" // CLAM

namespace CLAMTest { class SpectralPeaksTest; }


CPPUNIT_TEST_SUITE_REGISTRATION( CLAMTest::SpectralPeaksTest );

class CLAMTest::SpectralPeaksTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SpectralPeaksTest );

	CPPUNIT_TEST(test_asString_withEmptyArray);

	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	typedef CLAM::SpectralPeakArray SpectralPeaks;

	template <class T> std::string asString(const CLAM::Array<T> & array)
	{
		std::ostringstream os;
		os << "[";
		os << "]";
		return os.str();
	}

	std::string asString(const SpectralPeaks & peaks)
	{
		std::ostringstream os;
		os << "[";
		os << "]";
		return os.str();
	}

	void test_asString_withEmptyArray()
	{
		CLAM::Array<float> data;
		CPPUNIT_ASSERT_EQUAL(
			std::string(
			"[]"
			), asString(data));
	}


};


