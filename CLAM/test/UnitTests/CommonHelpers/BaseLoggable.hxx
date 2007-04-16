#ifndef _BaseLoggable_hxx_
#define _BaseLoggable_hxx_

#include <sstream> // class BaseLoggable needs stringstream

namespace CLAMTest
{

/**
 * From this class should derive all the TestFixutures that wants to
 * use the Log String testing pattern.
 * Of course the fixture tearDown() must call ClearLog()
 */
class BaseLoggable
{
	std::stringstream mLog;
public:
	std::string GetLog() const { return mLog.str(); }
	void ClearLog() { mLog.str( std::string() ); }
	std::stringstream& ToLog() { return mLog; }
};

} // namespace CLAMTest

#endif

