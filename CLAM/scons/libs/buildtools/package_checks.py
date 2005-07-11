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

# pthreads test
pthread_test_code = """
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 2

void* print_hello(void *thread_id)
{
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	int rc, t;
	for(t=0;t<NUM_THREADS;t++)
	{
		rc = pthread_create(&threads[t], NULL, print_hello, (void *)t);
		if (rc)
		{
			exit(-1);
		}
	}

	pthread_exit(NULL);
}

"""

package_checks['check_pthread'] = ThoroughPackageCheck( 'pthread', 'c', None, pthread_test_code )


