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

AC_DEFUN(CLAM_LIB_FLTK,
[
AC_MSG_CHECKING([for fltk headers; looking relative to CLAM])
fltk_local=no
if test -d ../../fltk/include/FL/; then
	AC_MSG_RESULT(yes)
	found_fltk=yes
	pwd=`pwd`
	FLTK_VERSION=`$pwd/../../fltk/bin/fltk-config --api-version`
	if test $FLTK_VERSION = 1.1; then
	    FLAG_FLTK_INCLUDES=`$pwd/../../fltk/bin/fltk-config --use-gl --use-images --cxxflags`
	    FLAG_FLTK_LIBS=`$pwd/../../fltk/bin/fltk-config --use-gl --use-images --ldflags`
	else
	    FLTK_INCLUDES="../../fltk/include"
	    FLTK_LIB_PATH="/usr/X11R6/lib ../../fltk/lib"
	    FLAG_FLTK_INCLUDES="-I../../fltk/include"
	    FLAG_FLTK_LIB_PATH="-L/usr/X11R6/lib -L../../fltk/lib"
	fi
	fltk_local=yes
else
	AC_MSG_RESULT(no)
dnl	AC_MSG_CHECKING([for fltk headers; looking in standard locations...])
	AC_MSG_CHECKING([for fltk-config...])
	found_fltk=no
	FLTK_VERSION=`fltk-config --api-version`
    	if test $FLTK_VERSION = 1.1; then
		AC_MSG_RESULT(yes)
		found_fltk=yes
		FLAG_FLTK_INCLUDES=`fltk-config --use-gl --use-images --cxxflags`
		FLAG_FLTK_LIBS=`fltk-config --use-gl --use-images --ldflags`
	fi
dnl	for base in "/usr" \
dnl	            "/usr/local" \
dnl	            "/opt" 
dnl	do
dnl		if test -d $base/include/FL; then
dnl			AC_MSG_RESULT(yes)
dnl			found_fltk=yes
dnl			FLAG_FLTK_LIB_PATH="-L/usr/X11R6/lib -L$base/lib"
dnl			FLTK_LIB_PATH="/usr/X11R6/lib $base/lib"
dnl			break;
dnl		fi
dnl	done
fi
if test $found_fltk = yes; then
	AC_MSG_CHECKING([for fltk library (and other fltk required)...])
	OLD_FLAGS=$CXXFLAGS

	link_ok=no

dnl	FLTK_LIBS="fltk GL fltk_gl X11 Xext fltk_forms fltk_images z png jpeg"
dnl	for lib in $FLTK_LIBS
dnl	do
dnl		FLAG_FLTK_LIBS="$FLAG_FLTK_LIBS -l$lib"
dnl	done
	CXXFLAGS="$CXXFLAGS $FLAG_FLTK_INCLUDES $FLAG_FLTK_LIB_PATH $FLAG_FLTK_LIBS"
	AC_TRY_LINK([
		#include<FL/Fl_Window.H>
		#include<FL/Fl.H>
	],[
		Fl_Window w(100,100);
		Fl::run();
	],[
		link_ok=yes
	],[])

	if test $link_ok = no; then
		AC_MSG_ERROR([
The test program did not compile or link. Check your config.log for
details.]
		)
	else
		AC_MSG_RESULT(yes: [$FLTK_LIBS])
	fi

	AC_TRY_RUN([
		#include<FL/Fl_Window.H>
		#include<FL/Fl.H>
		int main()
		{
			Fl_Window w(100,100);
			Fl::run();
			return 0;
		}
	],[
		AC_MSG_RESULT(yes)
		DEFINE_HAVE_FLTK=HAVE_FLTK
		if test $fltk_local = yes; then
			FLTK_INCLUDES="\$(CLAM_PATH)/../fltk/include"
			FLTK_LIB_PATH="\$(CLAM_PATH)/../fltk/lib"
		fi
	],[
		AC_MSG_ERROR([
The test program did compile, but failed to link. This probably means that
the run-time linker is not able to find libxercesc. You might want to set
your LD_LIBRARY_PATH variable, or edit /etc/ld/ld.conf to point to the
right location.]
		)
	],[
		echo $ac_n "cross compiling; assumed OK... $ac_c"
	])

	CXXFLAGS=$OLD_FLAGS
else
	AC_MSG_ERROR([
No fltk headers found!]
	)
fi;
]
)

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
		XERCESC_LIBS=xerces-c
		FLAG_XERCESC_LIBS=-lxerces-c
		CXXFLAGS="$CXXFLAGS $FLAG_XERCESC_INCLUDES $FLAG_XERCESC_LIBS $FLAG_XERCESC_LIB_PATH"
		AC_TRY_RUN([
			#include<xercesc/dom/DOM_Document.hpp>
			int main()
			{
				DOM_Document::createDocument();
				return 0;
			}
		],[
			AC_MSG_RESULT(yes)
			DEFINE_HAVE_XERCESC=HAVE_XERCESC
		],[
			AC_TRY_LINK([
					#include<xercesc/dom/DOM_Document.hpp>
				],[
					DOM_Document::createDocument();
					return 0;
				],[
				AC_MSG_ERROR([
The test program did compile, but failed to link. This probably means that
the  run-time linker is not able to find libxercesc. You might want to set
your LD_LIBRARY_PATH variable, or edit /etc/ld/ld.conf to point to the
right location.]
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
the run-time linker is not able to find libfftw. You might want to set your
LD_LIBRARY_PATH variable, or edit /etc/ld/ld.conf to point to the right
location.]
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
might want to set your LD_LIBRARY_PATH variable, or edit /etc/ld/ld.conf to
point to the right location.]
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

