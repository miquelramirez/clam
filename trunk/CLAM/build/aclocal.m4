dnl Some aditional checks for the CLAM library
dnl
dnl Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
dnl                         UNIVERSITAT POMPEU FABRA
dnl
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 2 of the License, or
dnl (at your option) any later version.
dnl
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

AC_DEFUN(CLAM_CHECK_DARWINESS,
[
	AC_MSG_CHECKING([system 'darwiness'])
	UNAME_RESULT=`uname`
	if test $UNAME_RESULT = "Darwin"
	then
		AC_MSG_RESULT(yes)
		IS_DARWIN="yes"
	else
		AC_MSG_RESULT(no)
		IS_DARWIN="no"
	fi
])

AC_DEFUN(CLAM_CHECK_PKGCONFIG,
[
	AC_MSG_CHECKING([pkg-config availability])
	WHICH_RESULTS=`which pkg-config`
	if test -x "$WHICH_RESULTS"
	then
		PKGCONFIG=$WHICH_RESULTS
		PKGCONFIG_AVAILABLE="yes"
		AC_MSG_RESULT(yes)
	else
		PKGCONFIG_AVAILABLE="no"
		AC_MSG_RESULT(no)
	fi
])

AC_DEFUN(CLAM_CHECK_CXX,
[
	AC_MSG_CHECKING(default g++ compiler)
	CXXVERSION=`
		g++ -v 2>&1 | grep 'gcc version' | \
		sed -e 's/.*gcc version \([[^ ]]*\).*/\1/'`
	AC_MSG_RESULT($CXXVERSION)
	CXXVERSION_MAJOR=`echo $CXXVERSION | cut -d'.' -f1`
	CXXVERSION_MINOR=`echo $CXXVERSION | cut -d'.' -f2`
	AC_MSG_CHECKING(if default g++ compiler is acceptable)
	if test $CXXVERSION_MAJOR = 3 ; then
		CXX=g++
		AC_MSG_RESULT(yes: $CXX=$CXXVERSION)
	elif test $CXXVERSION_MAJOR = 2 ; then
		if test $CXXVERSION_MINOR = 96 ; then
			AC_MSG_RESULT(no: $CXX=$CXXVERSION)
			AC_MSG_CHECKING([if prefered g++3 is available])
			if test x`which g++3` != x ; then
				CXX=g++3
				CXXVERSION=`
					g++3 -v 2>&1 | grep 'gcc version' | \
					sed -e 's/.*gcc version \([[^ ]]*\).*/\1/'`
				AC_MSG_RESULT(yes ($CXX=$CXXVERSION))
			elif test x`which g++-3.0` != x ; then
				CXX=g++-3.0
				CXXVERSION=`$CXX -v 2>&1 | grep version | cut -d' ' -f3`
				AC_MSG_RESULT(yes ($CXX=$CXXVERSION))
			fi			
		else
			AC_MSG_RESULT(yes ($CXX=$CXXVERSION))
		fi
	else
		AC_MSG_ERROR([
cannot handle g++ version ($CXXVERSION)]
		);
	fi
])

AC_DEFUN(CLAM_CHECK_UNICODE_SUPPORT,
[
	AC_MSG_CHECKING([for wchar_t support in libstdc++])

	dnl	UNICODE_SUPPORT_AVAILABLE="yes"
	AC_TRY_LINK([
	#include <string>
	],
	[
		const wchar_t str[] = {0xFABB,0x0000,0x12FF,0x33FF};
		std::basic_string<wchar_t> theString;
		theString.assign( (wchar_t*)str, 2);      

		return 0;
	],[UNICODE_SUPPORT_AVAILABLE="yes"],[UNICODE_SUPPORT_AVAILABLE="no"])

	if test $UNICODE_SUPPORT_AVAILABLE = "yes"
	then
		DEFINE_HAVE_STANDARD_UNICODE="HAVE_STANDARD_UNICODE"
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi
])

AC_DEFUN(CLAM_CHECK_LIBCXX_FEATURES,
[
dnl TODO: We should check if c++ and libc++ are available first...
	
	dnl Try the vector::at method.
	AC_MSG_CHECKING([for std::vector::at() method in libstdc++])
    AC_TRY_RUN([
#include<vector>
int main() {
	std::vector<int> v(1);
	v[0]=0;
	return v.at(0);
}
    ],vec_at_fails=no,vec_at_fails=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])

	if test "$vec_at_fails" = no ; then
    	AC_MSG_RESULT(yes)
		DEFINE_HAVE_STANDARD_VECTOR_AT=HAVE_STANDARD_VECTOR_AT
	else
    	AC_MSG_RESULT(no)
	fi

    dnl We now try to make the standard sstream header work
		dnl Really old versions of g++ do not have it.
    AC_MSG_CHECKING([for standard sstream header in libstdc++])
    AC_TRY_RUN([
#include<sstream>
int main() {
	std::stringstream ss;
	ss << "hi!" << std::ends;
	return 0;
}
    ],sstream_fails=no,sstream_fails=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])

	if test "$sstream_fails" = no ; then

    	AC_MSG_RESULT(yes)
				DEFINE_HAVE_STANDARD_SSTREAM=HAVE_STANDARD_SSTREAM
		DEFINE_HAVE_STANDARD_SSTREAM_STR=HAVE_STANDARD_SSTREAM_STR
		echo '#define STANDARD_SSTREAM' >> confdefs.h
	else

	    AC_MSG_RESULT(no)
		dnl We don't have the standard, so we check if older headers are provided
	    AC_MSG_CHECKING([for obsolete strstream header in libstdc++])
	    AC_TRY_RUN([
#include<strstream>
int main() {
	std::strstream ss;
	ss << "hi!" << std::ends;
}
	    ],strstream_fails=no,strstream_fails=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])

		if test "$strstream_fails" = no ; then

	    	AC_MSG_RESULT(yes)
				DEFINE_HAVE_STRSTREAM_SSTREAM=HAVE_STRSTREAM_SSTREAM
		dnl Bad luck. No known string stream header file found.
		else
			AC_MSG_ERROR([
No standard c++ library String Streams header file found!]
			)
		fi
	fi


	dnl It seems we have an lod string stream implementation. Lets check
	dnl if it has any peculiarity...
	AC_MSG_CHECKING([if stringstream::str() returns std::string in libstdc++])
    AC_TRY_RUN([
#ifdef STANDARD_SSTREAM
#include <sstream>
#else
#include <strstream>
#endif
#include<iostream>
int main() {
#ifdef STANDARD_SSTREAM
	std::stringstream ss("hi");
#else
    std::strstream ss;
	ss << "hi";
#endif
	if ( ss.str().c_str()[0] == 'h')
      return 0;
    return 1;
}
    ],str_string_fails=no,str_string_fails=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])

	if test "$str_string_fails" = no ; then
		AC_MSG_RESULT(yes)
		DEFINE_HAVE_STANDARD_SSTREAM_STR=HAVE_STANDARD_SSTREAM_STR
	else
		AC_MSG_RESULT(no)
	fi

])

dnl Begin of FLTK checking procedure
AC_DEFUN(CLAM_LIB_FLTK,
[
AC_MSG_CHECKING([fltk-config is known by the /usr/bin/which command...])

FLTKCONFIG=`which fltk-config`

if test -x "$FLTKCONFIG"
then
	AC_MSG_RESULT(yes)
else

	AC_MSG_RESULT(no)
	AC_MSG_CHECKING([fltk-config is in the sandbox...])
	FLTKCONFIG="../../fltk/bin/fltk-config"
	if test -x "$FLTKCONFIG"
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([The autoconf script has been unable to locate fltk-config script. This means that you have neither installed a suitable FLTK package or it is not present in your CLAM sandbox])
	fi;
fi;

AC_MSG_CHECKING([checking FLTK API version is 1.1 ...])

FLTK_API_VERSION=`$FLTKCONFIG --api-version`

if [[ "1.1" == "$FLTK_API_VERSION" ]]
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([Currently CLAM only supports FLTK API version 1.1])
fi;

FLTK_API_VERSION=`$FLTKCONFIG --version`
if [[ "1.1.4" == "$FLTK_API_VERSION" ]]
then
	RAW_FLTK_CFLAGS=`$FLTKCONFIG --use-gl --use-images --cxxflags`
	RAW_FLTK_LDFLAGS=`$FLTKCONFIG --use-gl --use-images --ldflags`
else
	RAW_FLTK_CFLAGS=`$FLTKCONFIG --cxxflags`
	RAW_FLTK_LDFLAGS=`$FLTKCONFIG --ldflags`
fi

for incpath in $RAW_FLTK_CFLAGS
	do
		if [[ ${incpath:0:2} == "-I" ]]
			then
				FLTK_INCLUDES="$FLTK_INCLUDES ${incpath#-I*}"
		fi
	done

for libpath in $RAW_FLTK_LDFLAGS
	do
		if [[ ${libpath:0:2} == "-L" ]]
			then
				FLTK_LIB_PATH="$FLTK_LIB_PATH ${libpath#-L*}"
		fi
	done

for binname in $RAW_FLTK_LDFLAGS
	do
		if [[ ${binname:0:2} == "-l" ]]
			then
				FLTK_LIBS="$FLTK_LIBS ${binname#-l*}"
		fi
	done

]
)
dnl End of FLTK checking procedure


AC_DEFUN(CLAM_LIB_XERCESC,
[
	AC_MSG_CHECKING(XERCESCROOT environment variable defined?)
	if test "x$XERCESCROOT" != x ; then
		# We first try to find the include directory.
		AC_MSG_RESULT(yes)
		AC_MSG_CHECKING([for xercesc headers, using XERCESCROOT environment variable...])
		XERCESC_LIB_PATH=${XERCESCROOT}/lib
		FLAG_XERCESC_LIB_PATH=-L${XERCESCROOT}/lib
		found_dom=no
		if test -d $XERCESCROOT/include/dom ; then
			found_dom=yes
			XERCESC_INCLUDES=$XERCESCROOT/include
			FLAG_XERCESC_INCLUDES=-I$XERCESCROOT/include
		elif test -d $XERCESCROOT/include/xercesc/dom; then
			found_dom=yes
			XERCESC_INCLUDES="$XERCESCROOT/include"
			FLAG_XERCESC_INCLUDES=-I$XERCESCROOT/include
		else
			AC_MSG_ERROR([
No xerces header directories found!  If you don't want to compile with XML support, run
configure with the --disable-xml option.]
			)
		fi
	else # XERCESCROOT
		AC_MSG_RESULT(no)
		AC_MSG_CHECKING([for xercesc headers; looking relative to CLAM])
		xerces_local=no
		if test -d ../../xercesc/include/xercesc/dom; then
			AC_MSG_RESULT(yes)
			found_dom=yes
			XERCESC_INCLUDES="\$(CLAM_PATH)/../xercesc/include"
			FLAG_XERCESC_INCLUDES="-I../../xercesc/include"
			XERCESC_LIB_PATH="\$(CLAM_PATH)/../xercesc/lib"
			FLAG_XERCESC_LIB_PATH="-L../../xercesc/lib"
			xerces_local=yes
		elif test -d ../../xercesc/include/dom; then
			AC_MSG_RESULT(yes)
			found_dom=yes
			XERCESC_INCLUDES="\$(CLAM_PATH)/../xercesc/include"
			FLAG_XERCESC_INCLUDES="-I../../xercesc/include"
			XERCESC_LIB_PATH="\$(CLAM_PATH)/../xercesc/lib"
			FLAG_XERCESC_LIB_PATH="-L../../xercesc/lib"
			xerces_local=yes
		else
			AC_MSG_RESULT(no)
			AC_MSG_CHECKING([for xercesc headers; looking in standard locations...])
			found_dom=no
			for base in "/usr/include" \
			"/usr/local/include" \
			"/opt/include" \
			"/usr/"; do
				if test -d $base/dom; then
					AC_MSG_RESULT(yes)
					found_dom=yes
					break;
				fi
				if test -d $base/xercesc/dom; then
					AC_MSG_RESULT(yes)
					found_dom=yes
					XERCESC_INCLUDES=$base/xercesc
					FLAG_XERCESC_INCLUDES=-I$base/xercesc
					break;
				fi
			done
			XERCESC_LIB_PATH=
		fi		
	fi
	if test $found_dom = yes; then
		AC_MSG_CHECKING([for xercesc library...])
		OLD_FLAGS=$CXXFLAGS
		XERCESC_LIBS="xerces-c pthread"
		FLAG_XERCESC_LIBS="-lxerces-c -lpthread"
		CXXFLAGS="$CXXFLAGS $FLAG_XERCESC_INCLUDES $FLAG_XERCESC_LIBS $FLAG_XERCESC_LIB_PATH"
		AC_TRY_RUN([
			#include<xercesc/util/PlatformUtils.hpp>
			int main()
			{
				namespace xercesc=XERCES_CPP_NAMESPACE;
				xercesc::XMLPlatformUtils::Initialize();
				return 0;
			}
		],[
			AC_MSG_RESULT(yes)
			DEFINE_HAVE_XERCESC=HAVE_XERCESC
		],[
			AC_TRY_LINK([
					#include<xercesc/util/PlatformUtils.hpp>
				],[
					namespace xercesc=XERCES_CPP_NAMESPACE;
					xercesc::XMLPlatformUtils::Initialize();
					return 0;
				],[
				AC_MSG_ERROR([
The test program did compile, but failed to link. This probably means that
the  run-time linker is not able to find libxercesc. You might want to set
your LD_LIBRARY_PATH (DYLD_LIBRARY_PATH on Mac OS X) variable, or edit 
/etc/ld/ld.conf to point to the right location.]
					)
				],[
				AC_MSG_ERROR([
The test program did not compile or link. Check your config.log for
details.])
			])
		],[echo $ac_n "cross compiling; assumed OK... $ac_c"
		])
		CXXFLAGS=$OLD_FLAGS
	else
		AC_MSG_ERROR([
No xerces headers found! If you don't want to compile with XML support,
run configure with the --disable-xml option.]
		)
	fi;
])




AC_DEFUN(CLAM_LIB_FFTW,
[
AC_MSG_CHECKING([for fftw headers; looking relative to CLAM])
fftw_local=no
if test -d ../../fftw/include; then
	AC_MSG_RESULT(yes)
	found_fftw=yes
	FFTW_INCLUDES="../../fftw/include"
	FFTW_LIB_PATH="../../fftw/lib"
	FLAG_FFTW_INCLUDES="-I../../fftw/include"
	FLAG_FFTW_LIB_PATH="-L../../fftw/lib"
	fftw_local=yes
else
	AC_MSG_RESULT(no)
	AC_MSG_CHECKING([for fftw headers; looking in standard locations...])
	found_fftw=no
	for base in "/usr/include" \
	            "/usr/local/include" \
	            "/opt/include"
	do
		if test -r "$base/rfftw.h"; then
			AC_MSG_RESULT(yes)
			found_fftw=yes
			break;
		fi
	done
	FFTW_LIB_PATH=
fi
if test $found_fftw = yes; then
	AC_MSG_CHECKING([for fftw library...])
	OLD_FLAGS=$CXXFLAGS

	link_ok=no

	FFTW_LIBS="fftw"
	FLAG_FFTW_LIBS="$FLAG_FFTW_LIBS -lfftw"
	CFLAGS="$CFLAGS $FLAG_FFTW_INCLUDES $FLAG_FFTW_LIB_PATH $FLAG_FFTW_LIBS"
	AC_TRY_LINK([
		#include<fftw.h>
	],[
		fftw_sizeof_fftw_real();
		return 0;
	],[
		link_ok=yes
	],[])

	if test $link_ok = no; then
		AC_MSG_ERROR([
The test program did not compile or link. Check your config.log for
details.]
		)
	else
		AC_MSG_RESULT(yes: [$FFTW_LIBS])
	fi
	 
	AC_TRY_RUN([
		#include<fftw.h>
		int main()
		{
			fftw_sizeof_fftw_real();
			return 0;
		}
	],[
		AC_MSG_RESULT(yes)
		DEFINE_HAVE_FFTW=HAVE_FFTW
		if test $fftw_local = yes; then
			FFTW_INCLUDES="\$(CLAM_PATH)/../fftw/include"
			FFTW_LIB_PATH="\$(CLAM_PATH)/../fftw/lib"
		fi
	],[
				AC_MSG_ERROR([
The test program did compile, but failed to link. This probably means that
the run-time linker is not able to find libfftw. You might want to set
your LD_LIBRARY_PATH (DYLD_LIBRARY_PATH on Mac OS X) variable, or edit 
/etc/ld/ld.conf to point to the right location.]
					)
	],[
		echo $ac_n "cross compiling; assumed OK... $ac_c"
	])

	CXXFLAGS=$OLD_FLAGS
else
	AC_MSG_ERROR([
No fftw headers found!]
	)
fi;
]
)

dnl Darwin checking procs
AC_DEFUN(CLAM_LIB_OGGVORBIS_DARWIN,
[
	AC_MSG_NOTICE([Checking that Ogg/Vorbis libs are in CLAM sandbox])
		
	OGGVORBIS_IN_SANDBOX="no"
	
	AC_CHECK_FILE( ../../oggvorbis/include/vorbis/vorbisfile.h,
			[OGGVORBIS_IN_SANDBOX="yes"], [OGGVORBIS_IN_SANDBOX="no"] )
	
	if test $OGGVORBIS_IN_SANDBOX = "no"
	then
		AC_MSG_ERROR([Ogg/Vorbis libraries are not in local CLAM Sandbox: follow MacOS X installation instructions])
	fi

	OGGVORBIS_LIBS="ogg vorbis vorbisenc vorbisfile"
	OGGVORBIS_LIB_PATH="\$(CLAM_PATH)/../oggvorbis/lib"
	OGGVORBIS_INCLUDES="\$(CLAM_PATH)/../oggvorbis/include"
])

AC_DEFUN( CLAM_LIB_SNDFILE_DARWIN,
[
	AC_MSG_NOTICE([Checking that libsndfile is in CLAM sandbox])
		
	SNDFILE_IN_SANDBOX="no"
	
	AC_CHECK_FILE( ../../libsndfile/include/sndfile.h,
			[SNDFILE_IN_SANDBOX="yes"], [SNDFILE_IN_SANDBOX="no"] )
	
	if test $SNDFILE_IN_SANDBOX = "no"
	then
		AC_MSG_ERROR([libsndfile are not in local CLAM Sandbox: follow MacOS X installation instructions])
	fi

	SNDFILE_LIBS="sndfile"
	SNDFILE_LIB_PATH="\$(CLAM_PATH)/../libsndfile/lib"
	SNDFILE_INCLUDES="\$(CLAM_PATH)/../libsndfile/include"
	
])

AC_DEFUN(CLAM_LIB_MAD_DARWIN,
[
	AC_MSG_NOTICE([Checking that Underbit's libmad is in CLAM sandbox])
		
	LIBMAD_IN_SANDBOX="no"
	
	AC_CHECK_FILE( ../../libmad/include/mad.h,
			[LIBMAD_IN_SANDBOX="yes"], [LIBMAD_IN_SANDBOX="no"] )
	
	if test $LIBMAD_IN_SANDBOX = "no"
	then
		AC_MSG_ERROR([libmad cannot be found on local CLAM Sandbox: follow MacOS X installation instructions])
	fi

	LIBMAD_LIBS="mad"
	LIBMAD_LIB_PATH="\$(CLAM_PATH)/../libmad/lib"
	LIBMAD_INCLUDES="\$(CLAM_PATH)/../libmad/include"
	
])

AC_DEFUN(CLAM_LIB_ID3LIB_DARWIN,
[
	AC_MSG_NOTICE([Checking that id3lib is in CLAM sandbox])
		
	ID3LIB_IN_SANDBOX="no"
	
	AC_CHECK_FILE( ../../id3lib/include/id3/tag.h,
			[ID3LIB_IN_SANDBOX="yes"], [ID3LIB_IN_SANDBOX="no"] )
	
	if test $ID3LIB_IN_SANDBOX = "no"
	then
		AC_MSG_ERROR([id3lib cannot be found on local CLAM Sandbox: follow MacOS X installation instructions])
	fi

	ID3LIB_LIBS="id3 z"
	ID3LIB_LIB_PATH="\$(CLAM_PATH)/../id3lib/lib"
	ID3LIB_INCLUDES="\$(CLAM_PATH)/../id3lib/include"
	
])

dnl End Of Darwin checking procs

dnl Start of Vorbis I SDK checking procedure
AC_DEFUN(CLAM_LIB_OGGVORBIS,
[
	AC_MSG_NOTICE([Checking that Vorbis I SDK is installed])
	AC_CHECK_LIB(vorbisfile,
		     ov_open,
		     [LIBVORBISFILE_PRESENT="yes";OGGVORBIS_LIBS="vorbisfile"],
		     [LIBVORBISFILE_PRESENT="no"],
		     -lvorbis -logg)
	
	AC_CHECK_LIB(vorbisenc,
		     vorbis_encode_init,
		     [LIBVORBISENC_PRESENT="yes";OGGVORBIS_LIBS="$OGGVORBIS_LIBS vorbisenc"],
		     [LIBVORBISENC_PRESENT="no"],
		     -lvorbis -logg)
	
	AC_CHECK_HEADER(vorbis/vorbisfile.h,
			[HDRVORBISFILE_PRESENT="yes"],
			[HDRVORBISFILE_PRESENT="no"] )
	
	AC_CHECK_HEADER(vorbis/vorbisenc.h,
			[HDRVORBISENC_PRESENT="yes"],
			[HDRVORBISENC_PRESENT="no"] )
	
	if test $LIBVORBISFILE_PRESENT = no || test $HDRVORBISFILE_PRESENT = no;
	then
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([libvorbisfile.so seems not to be present on your system. Please install Xiph.org Vorbis I development libraries])
	fi
	if test $HDRVORBISFILE_PRESENT = no || test $HDRVORBISENC_PRESENT = no;
	then
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([Vorbis SDK headers were not found. Please install Xiph.org Vorbis I development libraries])
	fi

	OGGVORBIS_LIBS="vorbis ogg $OGGVORBIS_LIBS"
	OGGVORBIS_LIB_PATH=""
	OGGVORBIS_INCLUDES=""
	
	AC_PATH_TOOL( OGGVORBIS_LIB_PATH,
		      libvorbis.so,
		      [],
		      [/usr/lib:/usr/local/lib:/opt/lib])

	OGGVORBIS_LIB_PATH=${OGGVORBIS_LIB_PATH%/libvorbis.so}

	if test $OGGVORBIS_LIB_PATH = "/usr/lib";
	then
		OGGVORBIS_LIB_PATH=""
	fi

	AC_PATH_TOOL( OGGVORBIS_INCLUDES,
		      vorbis/vorbisfile.h,
		      [],
		      [/usr/include:/usr/local/include])

	OGGVORBIS_INCLUDES=${OGGVORBIS_INCLUDES%/vorbis/vorbisfile.h}
	
	if test $OGGVORBIS_INCLUDES = "/usr/include" || test $OGGVORBIS_INCLUDES = "/usr/local/include";
	then
		OGGVORBIS_INCLUDES=""
	fi

])
dnl End of Vorbis I SDK checking procedure


dnl Start of LADSPA SDK checking procedure
AC_DEFUN( CLAM_LIB_LADSPA,
[
	AC_MSG_NOTICE([Checking that LADSPA SDK is installed])

	HDRLADSPA_PRESENT="no"

	AC_CHECK_HEADER(ladspa.h,
			[HDRLADSPA_PRESENT="yes"],
			[HDRLADSPA_PRESENT="no"])

	if test $HDRLADSPA_PRESENT = no
	then
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([LADSPA SDK has not been installed in your system!])
	fi

	AC_PATH_TOOL( LADSPA_INCLUDES,
		      ladspa.h,
		      [],
		      [/usr/include:/usr/local/include] )

	LADSPA_INCLUDES=${LADSPA_INCLUDES%/ladspa.h}

	if test $LADSPA_INCLUDES = "/usr/include" || test $LADSPA_INCLUDES="/usr/local/include"
	then
		LADSPA_INCLUDES=""
	fi

])
dnl End of LADSPA SDK checking procedure

dnl Start of libid3 checking procedure
AC_DEFUN( CLAM_LIB_ID3LIB,
[
	AC_MSG_NOTICE([Checking that id3lib is installed])

	AC_CHECK_HEADER(id3/tag.h,
			[HDRID3_PRESENT="yes"],
			[HDRID3_PRESENT="no"])


	if test $HDRID3_PRESENT = no;
	then
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([libid3 seems not to be installed on your system!])
	fi

	ID3_INCLUDES=""

	AC_PATH_TOOL( ID3_INCLUDES,
		      id3/tag.h,
		      [],
		      [/usr/include:/usr/local/include])

	ID3LIB_INCLUDES=${ID3_INCLUDES%/id3/tag.h}

	if test $ID3_INCLUDES = "/usr/include" || test $ID3_INCLUDES="/usr/local/include"
	then
		ID3_INCLUDES=""
	fi


	CXXFLAGS="-I$ID3_INCLUDES -lid3 -lz"

	LIBID3_PRESENT="no"

	AC_TRY_LINK(
	[
		#include <id3/tag.h>
		
		int main( int argc, char** argv )
		{
			ID3_Tag myTag;

			return 0;
		}
	], 
	[
		LIBID3_PRESENT="yes"
	],
	[
		LIBID3_PRESENT="no"
	]
	);

	if test $LIBID3_PRESENT="yes";
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([Seems that id3lib, http://id3lib.sourceforge.net is not installed. Please make necessary steps to install it on your system.])
	fi
	
	ID3LIB_LIBS="id3 z"
	ID3LIB_LIB_PATH=""


	AC_PATH_TOOL( ID3_LIB_PATH,
		      libid3.so,
		      [],
		      [/usr/lib:/usr/local:/opt/lib])

	ID3_LIB_PATH=${ID3_LIB_PATH%/libid3.so}

	if test $ID3_LIB_PATH="/usr/lib"
	then
		ID3_LIB_PATH=""
	fi




])

dnl End of libid3 checking procedure

dnl Start of Underbit's libmad checking procedure
AC_DEFUN(CLAM_LIB_MAD,
[
	AC_MSG_NOTICE([Checking that Underbit's MPEG Audio Decoder library is installed])
	
	$PKGCONFIG --exists mad
	
	if test "$?" -eq 0;
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([Seems that libmad is not installed in your system. If you haven't installed it yet, please do so. If you have, then check that the mad.pc file location is somewhere inside the PKG_CONFIG_PATH environment variable.])
	fi;

	AC_MSG_NOTICE([Checking that libmad version is acceptable (above 0.14.2)])
	
	$PKGCONFIG --atleast-version=0.14.2 mad
	
	if test "$?" -eq 0;
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([The currently installed libmad version is inferior to 0.14.2. Please update you currently installed version to a more recent release])
	fi;

	LIBMAD_INCLUDES__0=`$PKGCONFIG --cflags mad`
	LIBMAD_LIBS_PATH_0=`$PKGCONFIG --libs-only-L mad`
	LIBMAD_LIBS_0=`$PKGCONFIG --libs-only-l mad`

	for incpath in $LIBMAD_INCLUDES_0
	do
		if [[ ${incpath:0:2} == "-I" ]]
	 	   then
			LIBMAD_INCLUDES="$LIBMAD_INCLUDES ${incpath#-I*}"
		fi
	done

	for libpath in $LIBMAD_LIBS_PATH_0
	do
		if [[ ${libpath:0:2} == "-L" ]]
		   then
			LIBMAD_LIB_PATH="$LIBMAD_LIB_PATH ${libpath#-L*}"
		fi
	done

	for binname in $LIBMAD_LIBS_0
	do
		if [[ ${binname:0:2} == "-l" ]]
		   then
			LIBMAD_LIBS="$LIBMAD_LIBS ${binname#-l}"
		fi
	done

])
dnl end of Underbit's libmad checking procedure

dnl Start of Underbit's libmad checking procedure without pkg-config
AC_DEFUN(CLAM_LIB_MAD_NO_PKGCONFIG,
[
	AC_MSG_NOTICE([Checking that Underbit's MPEG Audio Decoder library is installed])

	LIBMADPRESENT="no"
	HDRMADPRESENT="no"
	
	AC_CHECK_LIB( mad,
		      mad_stream_init,
		      [LIBMADPRESENT="yes"],[LIBMADPRESENT="no"])

	AC_CHECK_HEADER( mad.h,
			[HDRMADPRESENT="yes"],[HDRMADPRESENT="no"],
			[/usr/include:/usr/local/include])

	if test $LIBMADPRESENT = "no" || test $HDRMADPRESENT = "no";
	then
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([libmad seems not to be present on your system!])
	fi

	LIBMAD_LIBS="mad"
	LIBMAD_LIB_PATH=""
	LIBMAD_INCLUDES=""
	
	AC_PATH_TOOL( LIBMAD_LIB_PATH,
		      libmad.so,
		      [],
		      [/usr/lib:/usr/local/lib:/opt/lib])

	LIBMAD_LIB_PATH=${LIBMAD_LIB_PATH%/libmad.so}

	if test $LIBMAD_LIB_PATH = "/usr/lib";
	then
		LIBMAD_LIB_PATH=""
	fi

	AC_PATH_TOOL( LIBMAD_INCLUDES,
		      mad.h,
		      [],
		      [/usr/include:/usr/local/include])

	LIBMAD_INCLUDES=${LIBMAD_INCLUDES%/mad.h}
	
	if test $LIBMAD_INCLUDES = "/usr/include" || test $LIBMAD_INCLUDES = "/usr/local/include";
	then
		LIBMAD_INCLUDES=""
	fi		
])
dnl End of libmad checking procedure

dnl Start of libsndfile checking procedure
AC_DEFUN(CLAM_LIB_SNDFILE,
[
AC_MSG_CHECKING([libsndfile is installed...])

$PKGCONFIG --exists sndfile

if test "$?" -eq 0;
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([Seems that libsndfile is not installed in your system. If you haven't installed it yet, please do so. If you have, then check that the sndfile.pc file location is in the PKG_CONFIG_PATH environment variable.
		])
fi;

AC_MSG_CHECKING([libsndfile version is acceptable...])

$PKGCONFIG --atleast-version=1.0.3 sndfile

if test "$?" -eq 0;
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([Currently installed libsndfile version is inferior to 1.0.3. Please update your current libsndfile installation to a more recent release.
		])
fi;

SNDFILE_INCLUDE_PATH_0=`$PKGCONFIG --cflags sndfile`
SNDFILE_LIBS_PATH_0=`$PKGCONFIG --libs-only-L sndfile`
SNDFILE_LIBS_0=`$PKGCONFIG --libs-only-l sndfile `

for incpath in $SNDFILE_INCLUDE_PATH_0
	do
		if [[ ${incpath:0:2} == "-I" ]]
	 	   then
			SNDFILE_INCLUDES="$SNDFILE_INCLUDES ${incpath#-I*}"
		fi
	done

for libpath in $SNDFILE_LIBS_PATH_0
	do
		if [[ ${libpath:0:2} == "-L" ]]
		   then
			SNDFILE_LIB_PATH="$SNDFILE_LIB_PATH ${libpath#-L*}"
		fi
	done

for binname in $SNDFILE_LIBS_0
	do
		if [[ ${binname:0:2} == "-l" ]]
		   then
			SNDFILE_LIBS="$SNDFILE_LIBS ${binname#-l}"
		fi
	done

]
) 
dnl End of libsndfile checking procedure

dnl Start of libsndfile checking procedure without pkg-config
AC_DEFUN(CLAM_LIB_SNDFILE_NO_PKGCONFIG,
[
	AC_MSG_NOTICE([Checking that libsndfile is installed])
	AC_CHECK_LIB(sndfile,
		     sf_open,
		     [LIBSNDFILE_PRESENT="yes";SNDFILE_LIBS="sndfile"],
		     [LIBSNDFILE_PRESENT="no"])
	
	
	AC_CHECK_HEADER(sndfile.h,
			[HDRSNDFILE_PRESENT="yes"],
			[HDRSNDFILE_PRESENT="no"] )
	
	if test $LIBSNDFILE_PRESENT = no || test $HDRSNDFILE_PRESENT = no;
	then
		AC_MSG_RESULT(no)
		AC_MSG_ERROR([libsndfile seems not to be present on your system!])
	fi

	SNDFILE_LIB_PATH=""
	SNDFILE_INCLUDES=""
	
	AC_PATH_TOOL( SNDFILE_LIB_PATH,
		      libsndfile.so,
		      [],
		      [/usr/lib:/usr/local/lib:/opt/lib])

	SNDFILE_LIB_PATH=${SNDFILE_LIB_PATH%/libsndfile.so}

	if test $SNDFILE_LIB_PATH = "/usr/lib";
	then
		SNDFILE_LIB_PATH=""
	fi

	AC_PATH_TOOL( SNDFILE_INCLUDES,
		      sndfile.h,
		      [],
		      [/usr/include:/usr/local/include])

	SNDFILE_INCLUDES=${SNDFILE_INCLUDES%/sndfile.h}
	
	if test $SNDFILE_INCLUDES = "/usr/include" || test $SNDFILE_INCLUDES = "/usr/local/include";
	then
		SNDFILE_INCLUDES=""
	fi


])
dnl End of libsndfile checking procedure without pkg-config

AC_DEFUN(CLAM_LIB_FFTWOLD,
[
AC_MSG_CHECKING([for fftw headers; looking relative to CLAM])
fltk_local=no
if test -d ../../fftw/include/; then
	AC_MSG_RESULT(yes)
	found_fftw=yes
	FFTW_INCLUDES="../../fftw/include"
	FFTW_LIB_PATH="../../fftw/lib"
	FLAG_FFTW_INCLUDES="-I../../fftw/include"
	FLAG_FFTW_LIB_PATH="-L../../fftw/lib"
	fltk_local=yes
else
	AC_MSG_RESULT(no)
	AC_MSG_CHECKING([for fftw headers; looking in standard locations...])
	found_fltk=no
	for base in "/usr/include" \
	            "/usr/local/include" \
	            "/opt/include" \
	            "/usr/"
	do
		if test -f $base/fftw.h; then
			AC_MSG_RESULT(yes)
			found_fftw=yes
			break;
		fi
	done
	FFTW_LIB_PATH=
fi
	echo FLAG_FFTW_LIB_PATH
	if test "$usedouble" = no; then
		AC_MSG_CHECKING([for single precision fftw library])
    		OLD_LIBS=$LIBS
		AC_SEARCH_LIBS(fftw_sizeof_fftw_real,sfftw,
			[AC_CHECK_HEADER(srfftw.h,
				[ 
					DEFINE_HAVE_FFTW=HAVE_FFTW
					FFTW_LIBS="srfftw sfftw"
				])
			],
			[AC_MSG_CHECKING([for double precision fftw library])
			AC_SEARCH_LIBS(fftw_sizeof_fftw_real,fftw,
				[AC_CHECK_HEADER(rfftw.h,
					[
						DEFINE_HAVE_FFTW=HAVE_FFTW
						FFTW_LIBS="rfftw fftw"
					])
				],
				[AC_MSG_ERROR
				(
[fftw not found]
				)]
				,-lfftw -lm)
			],-lsfftw -lm
		)
	else
		AC_MSG_CHECKING([for double precision fftw library])
	
	AC_SEARCH_LIBS(fftw_sizeof_fftw_real,fftw,
    	[AC_CHECK_HEADER(rfftw.h,
		[ 
		DEFINE_HAVE_FFTW=HAVE_FFTW
		FFTW_LIBS="rfftw fftw"
		]
		)
	],
	AC_MSG_ERROR([
fftw library (double precision) not found. Maybe you want to configure
with  --disable-double?]) 		
	,-lfftw -lm
    )
	fi
	LIBS=$OLD_LIBS
])




AC_DEFUN(CLAM_LIB_QT,
[
AC_MSG_CHECKING([for qt headers; looking relative to CLAM])
qt_local=no
if test -d ../../qt/include//; then
	AC_MSG_RESULT(yes)
	found_qt=yes
	QT_INCLUDES="../../qt/include"
	QT_LIB_PATH="../../qt/lib"
	FLAG_QT_INCLUDES="-I../../qt/include"
	FLAG_QT_LIB_PATH="-L../../qt/lib"
	qt_local=yes
else
	AC_MSG_RESULT(no)
	AC_MSG_CHECKING([for qt headers; looking in standard locations...])
	found_qt=no
	for base in "/usr" \
	            "/usr/local" \
	            "/opt"
	do
		if test -d $base/include/qt3; then
			AC_MSG_RESULT(yes)
			QT_INCLUDES="$base/include/qt3"
			QT_LIB_PATH="$base/lib"
			FLAG_QT_INCLUDES="-I$base/include/qt3"
			FLAG_QT_LIB_PATH="-L$base/lib"
			found_qt=yes
			break;
		fi
	done
	
	if test -d $QTDIR/include/; then
		AC_MSG_RESULT(yes)
		QT_INCLUDES="$QTDIR/include"
		QT_LIB_PATH="$QTDIR/lib"
		FLAG_QT_INCLUDES="-I$QTDIR/include"
		FLAG_QT_LIB_PATH="-L$QTDIR/lib"
		found_qt=yes
		break;
	fi
fi

if test $found_qt = yes; then
	AC_MSG_CHECKING([for qt library (and other qt required)...])
	OLD_FLAGS=$CXXFLAGS

	link_ok=no

	QT_LIBS=""
	FLAG_QT_LIBS=""
	for lib in qt-mt qt; do
		QT_LIBS="$lib"
		FLAG_QT_LIBS="-l$lib"
		CXXFLAGS="$CXXFLAGS $FLAG_QT_INCLUDES $FLAG_QT_LIB_PATH $FLAG_QT_LIBS"
		AC_TRY_LINK([
				#include<qapplication.h>
			],[
				int argc = 2;
				char *argv[]={"Just","testing"};
				QApplication app(argc,argv);
			],[
				link_ok=yes
			],[])
		test "$link_ok"="yes" && break
	done

	if test $link_ok = no; then
		AC_MSG_ERROR([
The test program did not compile or link. Check your config.log for
details.]
		)
	else
		AC_MSG_RESULT(yes: [$QT_LIBS])
	fi

	AC_TRY_RUN([
		#include<qapplication.h>
		int main(int argc, char** argv)
		{
			QApplication app(argc,argv);
			return 0;
		}
	],[
		AC_MSG_RESULT(yes)
		DEFINE_HAVE_QT=HAVE_QT
		if test $qt_local = yes; then
			QT_INCLUDES="\$(CLAM_PATH)/../qt/include"
			QT_LIB_PATH="\$(CLAM_PATH)/../qt/lib"
		fi
	],[
		AC_MSG_ERROR([
The test program did compile and to link, but failed to run. This
probably  means that the run-time linker is not able to find libqt.so. You
might want to set your LD_LIBRARY_PATH (DYLD_LIBRARY_PATH on Mac OS X) 
variable, or edit /etc/ld/ld.conf to point to the right location.]
		)
	],[
		echo $ac_n "cross compiling; assumed OK... $ac_c"
	])

	CXXFLAGS=$OLD_FLAGS
else
	AC_MSG_WARN([
No qt headers found!]
	)
fi
])

