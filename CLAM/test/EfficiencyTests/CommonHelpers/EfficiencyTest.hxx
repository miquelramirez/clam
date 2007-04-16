#ifndef EfficiencyTest_hxx
#define EfficiencyTest_hxx
#include <cppunit/extensions/HelperMacros.h>


class EfficiencyTest : public CppUnit::TestFixture
{
	unsigned _testimony;
protected:
	void start();
	void stop();
};


#endif//EfficiencyTest_hxx

