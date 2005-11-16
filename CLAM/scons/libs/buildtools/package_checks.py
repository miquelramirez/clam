import sys
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

if sys.platform == 'win32' :
	package_checks['check_xerces_c'] = ThoroughPackageCheck( 'xerces-c', 'c++', 'xerces-c_2', xerces_test_code )
else :
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

if sys.platform == 'win32' :
	package_checks['check_pthread'] = ThoroughPackageCheck( 'pthread', 'c', 'pthreadVCE', pthread_test_code )
else :
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

float_fftw_wo_prefix_test_code = """\
#include <fftw.h>
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

package_checks['check_fftw_float_wo_prefix'] = ThoroughPackageCheck( 'fftw using floats with not prefixed binaries/headers', 'c', None, float_fftw_wo_prefix_test_code )

float_rfftw_wo_prefix_test_code = """\
#include <rfftw.h>

int main(int argc, char** argv )
{

	rfftw_create_plan(0,FFTW_FORWARD,0);
	return 0;
}		
"""

package_checks['check_rfftw_float_wo_prefix'] = ThoroughPackageCheck( 'rfftw using floats with not prefixed binaries/headers','c', None, float_rfftw_wo_prefix_test_code )

liboscpack_test_code = """\
#include <oscpack/ip/IpEndpointName.h>
#include <oscpack/ip/UdpSocket.h>
int main()
{
	UdpTransmitSocket socket( IpEndpointName("localhost", 9999) );
	return 0;
}
"""

package_checks['check_oscpack'] = ThoroughPackageCheck( 'oscpack', 'c++', None, liboscpack_test_code )

ladspa_test_code = """\
#include <ladspa.h>
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index)
{ return 0; }
int main()
{
	ladspa_descriptor(0);
	return 0;
}
"""

package_checks['check_ladspa'] = ThoroughPackageCheck( 'ladspa', 'c', None, ladspa_test_code )

libsndfile_test_code = """\
#include <sndfile.h>

int main()
{
	SF_INFO sfinfo;
	sf_open_fd(fileno(stdout),SFM_WRITE,&sfinfo,0);
	return 0;
}
"""

package_checks['check_libsndfile'] = ThoroughPackageCheck( 'libsndfile', 'c', None, libsndfile_test_code )

libogg_test_code = """\
#include <ogg/ogg.h>

int main( int argc, char** argv )
{
	oggpack_buffer b;

	oggpack_writeinit(&b);

	return 0;
}
"""

package_checks['check_libogg'] = ThoroughPackageCheck( 'libogg', 'c', None, libogg_test_code )

libvorbis_test_code = """\
#include <vorbis/vorbisenc.h>

int main( int argc, char** argv )
{
	vorbis_info vi;
	vorbis_info_init( &vi );
	vorbis_encode_setup_init( &vi );

	return 0;
}
"""

package_checks['check_libvorbis'] = ThoroughPackageCheck( 'libvorbis', 'c', None, libvorbis_test_code )

libvorbisfile_test_code = """\
#include <vorbis/vorbisfile.h>

int main( int argc, char** argv )
{
	OggVorbis_File vf;
	
	ov_test_open( &vf );

	return 0;
}

"""

package_checks['check_libvorbisfile'] = ThoroughPackageCheck( 'libvorbisfile', 'c', None, libvorbisfile_test_code )

libmad_test_code = """\
#include<mad.h>
int main()
{
	struct mad_stream s;
	mad_stream_init(&s);
	return 0;
}
"""

package_checks['check_libmad'] = ThoroughPackageCheck( 'libmad', 'c', None, libmad_test_code )

libasound_test_code = """\
#include<alsa/asoundlib.h>
int main()
{
	int card = 0;
	snd_card_next(&card);
	return 0;
}
"""

package_checks['check_libasound'] = ThoroughPackageCheck( 'libasound', 'c', None, libasound_test_code )

id3lib_test_code = """\
#include <id3.h>
#include <id3/tag.h>
int main()
{
	ID3_Tag myTag;
	return 0;
}
"""

package_checks['check_id3lib'] = ThoroughPackageCheck( 'id3lib', 'c++', None, id3lib_test_code )

fltk_test_code = """\
#include<FL/Fl_Gl_Window.H>
#include<FL/Fl_PNG_Image.H>
#include<FL/fl_draw.H>
int main()
{
	Fl_Gl_Window w(0,0);
	Fl_PNG_Image png("");
	return 0;
}
"""

package_checks['check_fltk'] = ThoroughPackageCheck( 'fltk', 'c++', None, fltk_test_code )

opengl_test_code = """\
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

int main( int argc, char** argv )
{
	glBegin(GL_LINES);
		glVertex2f(0.0,0.0);
		glVertex2f(1.0,0.0);
	glEnd();

	return 0;
}"""

package_checks['check_opengl'] = ThoroughPackageCheck( 'opengl', 'c', None, opengl_test_code )

glu_test_code = """\
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glu.h>

int main( int argc, char** argv )
{
	GLUquadricObj *sphereObj;
	sphereObj = gluNewQuadric();

	gluSphere(sphereObj, 1.0, 8, 8);

	return 0;
}"""

package_checks['check_glu'] = ThoroughPackageCheck( 'glu', 'c', None, glu_test_code )

libqt_test_code = r"""\
#include <qapplication.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	int argc = 2;
	char *argv[]={"Just","testing"};
	QApplication app(argc,argv);
	return 0;	
}
"""

package_checks['check_qt'] = ThoroughPackageCheck( 'qt', 'c++', None, libqt_test_code )

directx_test_code = r"""\
#include <windows.h>
#include <dsound.h>

int main()
{
	LPGUID mGUID = 0;
	LPDIRECTSOUND mDS;
	HRESULT hr = DirectSoundCreate( mGUID, &mDS, NULL );
	
	return 0;
}

"""

package_checks['check_directx'] = ThoroughPackageCheck( 'directx', 'c++', None, directx_test_code )

portmidi_test_code = """\
#include <portmidi.h>
#include <stdlib.h>

int main()
{
	PmStream* mHandleIn = NULL;
	PmError err = Pm_OpenInput( &mHandleIn, 0, NULL, 100, NULL, NULL, 0 );

	return 0;
}

"""

package_checks['check_portmidi'] = ThoroughPackageCheck( 'portmidi', 'c', None, portmidi_test_code )

jack_test_code = """\
#include <jack/jack.h>
#include <stdlib.h>
jack_client_t *client;
int main()
{
	
	client = jack_client_new ("foo");

	if ( client != NULL )
		jack_client_close (client);

	return 0;
}
"""

package_checks['check_jack'] = ThoroughPackageCheck( 'jack', 'c', 'jack', jack_test_code )
