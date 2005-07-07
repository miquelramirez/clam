from thorough_check import *

package_checks = dict()


# xerces-c package check
xerces_test_code = """
#include <xercesc/util/PlatformUtils.hpp>

int main( int argc, char** argv )
{
	namespace xercesc=XERCES_CPP_NAMESPACE;

	xercesc::XMLPlatformUtils::Initialize();

	return 0;
}
"""

package_checks['check_xerces_c'] = ThoroughPackageCheck( 'xerces-c', 'c++', 'xerces-c', xerces_test_code )

# libxml++ package-check
xmlpp_test_code = """
#include <libxml++/libxml++.h>

int main( int argc, char** argv )
{
	xmlpp::Document document;

	return 0;
}
"""

package_checks['check_xmlpp'] = ThoroughPackageCheck( 'libxml++', 'c++', 'xml++-2.6', xmlpp_test_code )


