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

double_fftw_wo_prefix_test_code = """\
#include <fftw.h>
#include <stdio.h>

int main(int argc, char** argv )
{

	fftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(double))
	{
		fprintf(stderr, "expecting fftw to be using doubles, and it is using floats!\\n");
		return -1;
	}
	return 0;

}
"""

package_checks['check_fftw_double_wo_prefix'] = ThoroughPackageCheck( 'fftw using doubles without prefixed binaries/headers', 'c', None, double_fftw_wo_prefix_test_code )

double_fftw_w_prefix_test_code = """\
#include <dfftw.h>
#include <stdio.h>

int main(int argc, char** argv )
{

	fftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(double))
	{
		fprintf(stderr, "expecting fftw to be using doubles, and it is using floats!\\n");
		return -1;
	}
	return 0;

}
"""

package_checks['check_fftw_double_w_prefix'] = ThoroughPackageCheck( 'fftw using doubles with prefixed binaries/headers', 'c', None, double_fftw_w_prefix_test_code )

double_rfftw_wo_prefix_test_code = """\
#include <rfftw.h>

int main(int argc, char** argv )
{

	rfftw_create_plan(0,FFTW_FORWARD,0);
	return 0;
}
"""

package_checks['check_rfftw_double_wo_prefix'] = ThoroughPackageCheck( 'rfftw using doubles without prefixed binaries/headers', 'c', None, double_rfftw_wo_prefix_test_code )

double_rfftw_w_prefix_test_code = """\
#include <rfftw.h>

int main(int argc, char** argv )
{

	rfftw_create_plan(0,FFTW_FORWARD,0);
	return 0;
}
"""


package_checks['check_rfftw_double_w_prefix'] = ThoroughPackageCheck( 'rfftw using doubles with prefixed binaries/headers', 'c', None, double_rfftw_w_prefix_test_code )

float_fftw_w_prefix_test_code = """\
#include <sfftw.h>
#include <stdio.h>

int main(int argc, char** argv )
{

	fftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(float))
	{
		fprintf(stderr, "expecting fftw to be using floats, and it is using doubles!\\n");
		return -1;
	}
	return 0;

}
"""

package_checks['check_fftw_float_w_prefix'] = ThoroughPackageCheck( 'fftw using floats with prefixed binaries/headers', 'c', None, float_fftw_w_prefix_test_code )


float_rfftw_w_prefix_test_code = """\
#include <srfftw.h>

int main(int argc, char** argv )
{

	rfftw_create_plan(0,FFTW_FORWARD,0);
	return 0;
}
"""

package_checks['check_rfftw_float_w_prefix'] = ThoroughPackageCheck( 'rfftw using floats with prefixed binaries/headers', 'c', None, float_rfftw_w_prefix_test_code )

liboscpack_test_code = """\
#include <oscpack/ip/NetworkingUtils.h>
int main()
{
	InitializeNetworking();
	TerminateNetworking();
	return 0;
}
"""

package_checks['check_oscpack'] = ThoroughPackageCheck( 'oscpack', 'c++', None, liboscpack_test_code )


