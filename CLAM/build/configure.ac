AC_INIT([test],[0.1],[clam@iua.upf.es])

AC_PREREQ([2.50])

AC_MSG_CHECKING([setenv])
which_setenv=`which setenv`
if test x$which_setenv = x
then
	setenv() { export "$1=$2"; }
	unsetenv() { unset "$1"; export "$1"; }
	AC_MSG_RESULT([using export])
else
	AC_MSG_RESULT([$which setenv])
fi

AC_PROG_CC()

AC_PROG_CPP()

AC_PROG_CXX()

AC_PROG_CXXCPP()

cat>cpp_preprocessor_variable.cxx<<EOF
#include <xercesc/util/XercesVersion.hpp>
DEFINED_PREPROCESSOR_VARIABLE=XERCES_CPP_NAMESPACE
EOF
SET_DEFINED_PREPROCESSOR_VARIABLE=`$CXXCPP cpp_preprocessor_variable.cxx | grep DEFINED_PREPROCESSOR_VARIABLE`
eval $SET_DEFINED_PREPROCESSOR_VARIABLE
XERCES_CPP_NAMESPACE=$DEFINED_PREPROCESSOR_VARIABLE
#####################################################################
# Checking sed presence
#
# source: acv/sed_presence.acv 
#####################################################################
AC_MSG_CHECKING([sed present])
sed=`which sed`
if test x$sed = x
then
	AC_MSG_RESULT([no])
	AC_MSG_FAILURE([sed not found])
else
	AC_MSG_RESULT([yes])
	AC_MSG_CHECKING([sed working])
	testresult=`echo "-Ia -Ib -Ic" | sed -e 's/ -I/ /g' | sed -e 's/^-I/ /' | sed -e 's/^ //'`
	if test "$testresult" = "a b c"
	then
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
		AC_MSG_FAILURE([sed not working as expected])
	fi
fi
#####################################################################

AC_LANG_CPLUSPLUS()

AC_MSG_CHECKING([default g++ compiler])
CXXVERSION=`g++ -v 2>&1 | grep 'gcc version' | sed -e 's/.*gcc version \([[^ ]]*\).*/\1/'`
AC_MSG_RESULT($CXXVERSION)
CXXVERSION_MAJOR=`echo $CXXVERSION | cut -d'.' -f1`
CXXVERSION_MINOR=`echo $CXXVERSION | cut -d'.' -f2`
AC_MSG_CHECKING([if default g++ compiler is acceptable])
if test $CXXVERSION_MAJOR = 3 || test $CXXVERSION_MAJOR = 4
then
	CXX=g++
	AC_MSG_RESULT([yes: $CXX version is $CXXVERSION])
else
	if test $CXXVERSION_MAJOR = 2
	then
		if test $CXXVERSION_MINOR = 96
		then
			AC_MSG_RESULT([no: $CXX version is $CXXVERSION])
			AC_MSG_CHECKING([if prefered g++3 is available])
			if test x`which g++3` != x
			then
				CXX=g++3
				CXXVERSION=`g++3 -v 2>&1 | grep 'gcc version' | sed -e 's/.*gcc version \([[^ ]]*\).*/\1/'`
				AC_MSG_RESULT([yes: $CXX version is $CXXVERSION])
			else
				if test x`which g++-3.0` != x
				then
					CXX=g++-3.0
					CXXVERSION=`$CXX -v 2>&1 | grep version | cut -d' ' -f3`
					AC_MSG_RESULT([yes: $CXX version is $CXXVERSION])
				fi			
			fi
		else
			AC_MSG_RESULT([yes: $CXX version is $CXXVERSION])
		fi
	else
		AC_MSG_ERROR([cannot handle g++ version $CXXVERSION]);
	fi
fi


AC_MSG_CHECKING([for wchar_t support in libstdc++])
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


dnl Try the vector::at method.
AC_MSG_CHECKING([for std::vector::at() method in libstdc++])
AC_TRY_RUN([
#include<vector>
int main() {
	std::vector<int> v(1);
	v[0]=0;
	return v.at(0);
}
],[vec_at_fails=no],[vec_at_fails=yes],[echo $ac_n "cross compiling; assumed OK... $ac_c"])

if test x$vec_at_fails = xno
then
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
],[sstream_fails=no],[sstream_fails=yes],
[echo $ac_n "cross compiling; assumed OK... $ac_c"]
)

if test x$sstream_fails = xno
then
	AC_MSG_RESULT([yes])
	DEFINE_HAVE_STANDARD_SSTREAM=HAVE_STANDARD_SSTREAM
	DEFINE_HAVE_STANDARD_SSTREAM_STR=HAVE_STANDARD_SSTREAM_STR
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
],[strstream_fails=no],[strstream_fails=yes],
[echo $ac_n "cross compiling; assumed OK... $ac_c"])

	if test "$strstream_fails" = no
	then
		AC_MSG_RESULT([yes])
		DEFINE_HAVE_STRSTREAM_SSTREAM=HAVE_STRSTREAM_SSTREAM
		dnl Bad luck. No known string stream header file found.
	else
		AC_MSG_ERROR(
[No standard c++ library String Streams header file found!]
		)
	fi
fi


dnl It seems we have an old string stream implementation. Lets check
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
	if ( ss.str().c_str()[0] == 'h') return 0;
	return 1;
}
],[str_string_fails=no],[str_string_fails=yes],
[echo $ac_n "cross compiling; assumed OK... $ac_c"]
)

if test x$str_string_fails = xno
then
	AC_MSG_RESULT(yes)
	DEFINE_HAVE_STANDARD_SSTREAM_STR=HAVE_STANDARD_SSTREAM_STR
else
	AC_MSG_RESULT(no)
fi


AC_SUBST(DEFINE_HAVE_STANDARD_SSTREAM_STR)
AC_SUBST(DEFINE_HAVE_STANDARD_SSTREAM)
AC_SUBST(DEFINE_HAVE_STANDARD_UNICODE)

AC_MSG_CHECKING([dll extension])
UNAME_RESULT=`uname`
if test $UNAME_RESULT = "Darwin"
then
	dllext='dylib'
else
	dllext='so'
fi
AC_MSG_RESULT([$dllext])
AC_MSG_WARN([TODO: improve dll extension test])
#####################################################################
# Check if sandbox is enabled
#
# source: acv/check_feature_enable.acv 
#####################################################################
AC_MSG_CHECKING([sandbox enabled])
#
AC_ARG_ENABLE(
	[sandbox],AC_HELP_STRING([--enable-sandbox],
	[use sandbox (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_sandbox=yes
	else
		AC_MSG_RESULT([no])
		enabled_sandbox=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_sandbox=yes
])
found_sandbox=no
#####################################################################

#####################################################################
# Check sandbox location
#
# source: acv/sandbox_location.acv 
#####################################################################
if test x$enabled_sandbox = xyes
then
	if test x$sandbox_location = x
	then
		AC_MSG_CHECKING([[CLAM sandbox location]])
		sandbox_location=`cd ../.. ; pwd`;
		AC_MSG_RESULT([$sandbox_location])
	fi
fi
#####################################################################

#####################################################################
# Check if pkg-config is enabled
#
# source: acv/check_feature_enable.acv 
#####################################################################
AC_MSG_CHECKING([pkg-config enabled])
#
AC_ARG_ENABLE(
	[pkg-config],AC_HELP_STRING([--enable-pkg-config],
	[use pkg-config (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_pkg_config=yes
	else
		AC_MSG_RESULT([no])
		enabled_pkg_config=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_pkg_config=yes
])
found_pkg_config=no
#####################################################################

#####################################################################
# Check pkg-config availability
#
# source: acv/pkg_config_availability.acv 
#####################################################################
if test x$enabled_pkg_config = xyes
then
	AC_MSG_CHECKING([pkg-config availability])
	WHICH_RESULTS=`which pkg-config`
	if test x"$WHICH_RESULTS" = x
	then
		AC_MSG_RESULT([no])
		pkg_config=""
		pkg_config_status=unavailable
	else
		AC_MSG_RESULT([yes: $WHICH_RESULTS])
		pkg_config=$WHICH_RESULTS
		pkg_config_status=enabled
	fi
else
	pkg_config_status=disabled
fi
#####################################################################

AC_LANG_CPLUSPLUS()

#####################################################################
# Check if fftw is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([fftw usage specified])
#
AC_ARG_WITH(
	[fftw],AC_HELP_STRING([--with-fftw],
	[use fftw (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_fftw=yes
	else
		AC_MSG_RESULT([no])
		enabled_fftw=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_fftw=yes
])
found_fftw=no
#####################################################################

#####################################################################
# IF fftw is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_fftw = xyes
then
#####################################################################

#####################################################################
# Look for fftw in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([fftw in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_fftw = xno
	then
		found=1
		for file in fftw.h
		do
			AC_MSG_CHECKING([$file for fftw in sandbox])
			if test -r $sandbox_location/fftw/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_fftw="$sandbox_location/fftw/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[fftw header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in fftw rfftw
			do
				AC_MSG_CHECKING([lib$file for fftw in sandbox])
				if test -r $sandbox_location/fftw/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_fftw="$libs_in_sandbox_fftw$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_fftw="$sandbox_location/fftw/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all fftw lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_fftw=yes
				found_in_sandbox_fftw=yes
			else
				AC_MSG_RESULT([no])
				found_fftw=no
			fi
		else
			AC_MSG_RESULT([no])
			found_fftw=no
		fi
	fi
fi
if test x$found_in_sandbox_fftw = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_fftw"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_fftw
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_fftw
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for fftw])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include<fftw.h>
#include<rfftw.h>
int main()
{
	fftw_create_plan(0,FFTW_FORWARD,0);
	rfftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(double))
	{
		return -1;
	}
	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_fftw 
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<fftw.h>
#include<rfftw.h>
int main()
{
	fftw_create_plan(0,FFTW_FORWARD,0);
	rfftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(double))
	{
		return -1;
	}
	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_fftw=$tmp_libs
			found_fftw=yes
			AC_MSG_RESULT([$libs_fftw])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding fftw
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_fftw = xyes
    then
    if test x$found_fftw = xno
	then
	AC_MSG_CHECKING([fftw has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists fftw
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_fftw=yes
		include_dirs_fftw=`$pkg_config fftw --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_fftw=`$pkg_config fftw --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_fftw=`$pkg_config fftw --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_fftw=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding fftw
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_fftw = xyes
then
  if test x$found_fftw = xno
	then
	found=1
	for file in fftw.h
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for fftw in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[fftw header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"fftw rfftw" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_fftw
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_fftw
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for fftw])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<fftw.h>
#include<rfftw.h>
int main()
{
	fftw_create_plan(0,FFTW_FORWARD,0);
	rfftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(double))
	{
		return -1;
	}
	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in fftw rfftw 
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include<fftw.h>
#include<rfftw.h>
int main()
{
	fftw_create_plan(0,FFTW_FORWARD,0);
	rfftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(double))
	{
		return -1;
	}
	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_fftw=$tmp_libs
					found_fftw=yes
					AC_MSG_RESULT([$libs_fftw])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_fftw
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in dfftw drfftw
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include<fftw.h>
#include<rfftw.h>
int main()
{
	fftw_create_plan(0,FFTW_FORWARD,0);
	rfftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(double))
	{
		return -1;
	}
	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_fftw=$tmp_libs
						found_fftw=yes
						AC_MSG_RESULT([$libs_fftw])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_fftw=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_fftw=no
	fi
    fi
fi

fi
#####################################################################
# Check if sfftw is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([sfftw usage specified])
#
AC_ARG_WITH(
	[sfftw],AC_HELP_STRING([--with-sfftw],
	[use sfftw (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_sfftw=yes
	else
		AC_MSG_RESULT([no])
		enabled_sfftw=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_sfftw=yes
])
found_sfftw=no
#####################################################################

#####################################################################
# IF sfftw is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_sfftw = xyes
then
#####################################################################

#####################################################################
# Look for sfftw in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([sfftw in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_sfftw = xno
	then
		found=1
		for file in fftw.h
		do
			AC_MSG_CHECKING([$file for sfftw in sandbox])
			if test -r $sandbox_location/fftw/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_sfftw="$sandbox_location/fftw/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[sfftw header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in sfftw srfftw
			do
				AC_MSG_CHECKING([lib$file for sfftw in sandbox])
				if test -r $sandbox_location/fftw/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_sfftw="$libs_in_sandbox_sfftw$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_sfftw="$sandbox_location/fftw/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all sfftw lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_sfftw=yes
				found_in_sandbox_sfftw=yes
			else
				AC_MSG_RESULT([no])
				found_sfftw=no
			fi
		else
			AC_MSG_RESULT([no])
			found_sfftw=no
		fi
	fi
fi
if test x$found_in_sandbox_sfftw = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_sfftw"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_sfftw
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_sfftw
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for sfftw])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include<sfftw.h>
#include<srfftw.h>
int main()
{
	fftw_create_plan(0,FFTW_FORWARD,0);
	rfftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(float))
	{
		return -1;
	}
	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_sfftw 
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<sfftw.h>
#include<srfftw.h>
int main()
{
	fftw_create_plan(0,FFTW_FORWARD,0);
	rfftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(float))
	{
		return -1;
	}
	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_sfftw=$tmp_libs
			found_sfftw=yes
			AC_MSG_RESULT([$libs_sfftw])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding sfftw
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_sfftw = xyes
    then
    if test x$found_sfftw = xno
	then
	AC_MSG_CHECKING([sfftw has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists sfftw
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_sfftw=yes
		include_dirs_sfftw=`$pkg_config sfftw --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_sfftw=`$pkg_config sfftw --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_sfftw=`$pkg_config sfftw --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_sfftw=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding sfftw
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_sfftw = xyes
then
  if test x$found_sfftw = xno
	then
	found=1
	for file in fftw.h
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for sfftw in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[sfftw header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"sfftw srfftw" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_sfftw
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_sfftw
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for sfftw])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<sfftw.h>
#include<srfftw.h>
int main()
{
	fftw_create_plan(0,FFTW_FORWARD,0);
	rfftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(float))
	{
		return -1;
	}
	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in sfftw srfftw 
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include<sfftw.h>
#include<srfftw.h>
int main()
{
	fftw_create_plan(0,FFTW_FORWARD,0);
	rfftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(float))
	{
		return -1;
	}
	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_sfftw=$tmp_libs
					found_sfftw=yes
					AC_MSG_RESULT([$libs_sfftw])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_sfftw
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in 
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include<sfftw.h>
#include<srfftw.h>
int main()
{
	fftw_create_plan(0,FFTW_FORWARD,0);
	rfftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(float))
	{
		return -1;
	}
	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_sfftw=$tmp_libs
						found_sfftw=yes
						AC_MSG_RESULT([$libs_sfftw])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_sfftw=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_sfftw=no
	fi
    fi
fi

fi
#####################################################################
# Check if xerces is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([xerces usage specified])
#
AC_ARG_WITH(
	[xerces],AC_HELP_STRING([--with-xerces],
	[use xerces (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_xerces=yes
	else
		AC_MSG_RESULT([no])
		enabled_xerces=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_xerces=yes
])
found_xerces=no
#####################################################################

#####################################################################
# IF xerces is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_xerces = xyes
then
#####################################################################

#####################################################################
# Look for xerces in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([xerces in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_xerces = xno
	then
		found=1
		for file in xercesc/util/PlatformUtils.hpp
		do
			AC_MSG_CHECKING([$file for xerces in sandbox])
			if test -r $sandbox_location/xercesc/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_xerces="$sandbox_location/xercesc/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[xerces header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in xerces-c
			do
				AC_MSG_CHECKING([lib$file for xerces in sandbox])
				if test -r $sandbox_location/xercesc/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_xerces="$libs_in_sandbox_xerces$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_xerces="$sandbox_location/xercesc/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all xerces lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_xerces=yes
				found_in_sandbox_xerces=yes
			else
				AC_MSG_RESULT([no])
				found_xerces=no
			fi
		else
			AC_MSG_RESULT([no])
			found_xerces=no
		fi
	fi
fi
if test x$found_in_sandbox_xerces = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_xerces"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_xerces
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_xerces
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for xerces])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include<xercesc/util/PlatformUtils.hpp>
int main()
{
	namespace xercesc=XERCES_CPP_NAMESPACE;
	xercesc::XMLPlatformUtils::Initialize();
	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_xerces 
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<xercesc/util/PlatformUtils.hpp>
int main()
{
	namespace xercesc=XERCES_CPP_NAMESPACE;
	xercesc::XMLPlatformUtils::Initialize();
	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_xerces=$tmp_libs
			found_xerces=yes
			AC_MSG_RESULT([$libs_xerces])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding xerces
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_xerces = xyes
    then
    if test x$found_xerces = xno
	then
	AC_MSG_CHECKING([xerces has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists xerces
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_xerces=yes
		include_dirs_xerces=`$pkg_config xerces --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_xerces=`$pkg_config xerces --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_xerces=`$pkg_config xerces --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_xerces=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding xerces
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_xerces = xyes
then
  if test x$found_xerces = xno
	then
	found=1
	for file in xercesc/util/PlatformUtils.hpp
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for xerces in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[xerces header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"xerces-c" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_xerces
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_xerces
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for xerces])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<xercesc/util/PlatformUtils.hpp>
int main()
{
	namespace xercesc=XERCES_CPP_NAMESPACE;
	xercesc::XMLPlatformUtils::Initialize();
	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in xerces-c 
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include<xercesc/util/PlatformUtils.hpp>
int main()
{
	namespace xercesc=XERCES_CPP_NAMESPACE;
	xercesc::XMLPlatformUtils::Initialize();
	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_xerces=$tmp_libs
					found_xerces=yes
					AC_MSG_RESULT([$libs_xerces])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_xerces
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in 
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include<xercesc/util/PlatformUtils.hpp>
int main()
{
	namespace xercesc=XERCES_CPP_NAMESPACE;
	xercesc::XMLPlatformUtils::Initialize();
	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_xerces=$tmp_libs
						found_xerces=yes
						AC_MSG_RESULT([$libs_xerces])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_xerces=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_xerces=no
	fi
    fi
fi

fi
#####################################################################
# Check if xmlpp is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([xmlpp usage specified])
#
AC_ARG_WITH(
	[xmlpp],AC_HELP_STRING([--with-xmlpp],
	[use xmlpp (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_xmlpp=yes
	else
		AC_MSG_RESULT([no])
		enabled_xmlpp=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_xmlpp=yes
])
found_xmlpp=no
#####################################################################

#####################################################################
# IF xmlpp is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_xmlpp = xyes
then
#####################################################################

#####################################################################
# Look for xmlpp in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([xmlpp in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_xmlpp = xno
	then
		found=1
		for file in libxml++/libxml++.h
		do
			AC_MSG_CHECKING([$file for xmlpp in sandbox])
			if test -r $sandbox_location/libxml++/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_xmlpp="$sandbox_location/libxml++/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[xmlpp header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in xml++-2.6
			do
				AC_MSG_CHECKING([lib$file for xmlpp in sandbox])
				if test -r $sandbox_location/libxml++/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_xmlpp="$libs_in_sandbox_xmlpp$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_xmlpp="$sandbox_location/libxml++/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all xmlpp lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_xmlpp=yes
				found_in_sandbox_xmlpp=yes
			else
				AC_MSG_RESULT([no])
				found_xmlpp=no
			fi
		else
			AC_MSG_RESULT([no])
			found_xmlpp=no
		fi
	fi
fi
if test x$found_in_sandbox_xmlpp = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_xmlpp"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_xmlpp
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_xmlpp
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for xmlpp])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include <libxml++/libxml++.h>

int main( int argc, char** argv )
{
	xmlpp::Document document;

	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_xmlpp 
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include <libxml++/libxml++.h>

int main( int argc, char** argv )
{
	xmlpp::Document document;

	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_xmlpp=$tmp_libs
			found_xmlpp=yes
			AC_MSG_RESULT([$libs_xmlpp])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding libxml++-2.6
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_xmlpp = xyes
    then
    if test x$found_xmlpp = xno
	then
	AC_MSG_CHECKING([libxml++-2.6 has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists libxml++-2.6
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_xmlpp=yes
		include_dirs_xmlpp=`$pkg_config libxml++-2.6 --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_xmlpp=`$pkg_config libxml++-2.6 --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_xmlpp=`$pkg_config libxml++-2.6 --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_xmlpp=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding xmlpp
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_xmlpp = xyes
then
  if test x$found_xmlpp = xno
	then
	found=1
	for file in libxml++/libxml++.h
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for xmlpp in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[xmlpp header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"xml++-2.6" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_xmlpp
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_xmlpp
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for xmlpp])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include <libxml++/libxml++.h>

int main( int argc, char** argv )
{
	xmlpp::Document document;

	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in xml++-2.6 
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include <libxml++/libxml++.h>

int main( int argc, char** argv )
{
	xmlpp::Document document;

	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_xmlpp=$tmp_libs
					found_xmlpp=yes
					AC_MSG_RESULT([$libs_xmlpp])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_xmlpp
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in 
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include <libxml++/libxml++.h>

int main( int argc, char** argv )
{
	xmlpp::Document document;

	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_xmlpp=$tmp_libs
						found_xmlpp=yes
						AC_MSG_RESULT([$libs_xmlpp])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_xmlpp=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_xmlpp=no
	fi
    fi
fi

fi
#####################################################################
# Check if fltk is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([fltk usage specified])
#
AC_ARG_WITH(
	[fltk],AC_HELP_STRING([--with-fltk],
	[use fltk (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_fltk=yes
	else
		AC_MSG_RESULT([no])
		enabled_fltk=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_fltk=yes
])
found_fltk=no
#####################################################################

#####################################################################
# IF fltk is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_fltk = xyes
then
#####################################################################

#####################################################################
# Look for fltk in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([fltk in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_fltk = xno
	then
		found=1
		for file in FL/gl.h
		do
			AC_MSG_CHECKING([$file for fltk in sandbox])
			if test -r $sandbox_location/fltk/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_fltk="$sandbox_location/fltk/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[fltk header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in fltk fltk_images fltk_forms fltk_gl
			do
				AC_MSG_CHECKING([lib$file for fltk in sandbox])
				if test -r $sandbox_location/fltk/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_fltk="$libs_in_sandbox_fltk$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_fltk="$sandbox_location/fltk/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all fltk lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_fltk=yes
				found_in_sandbox_fltk=yes
			else
				AC_MSG_RESULT([no])
				found_fltk=no
			fi
		else
			AC_MSG_RESULT([no])
			found_fltk=no
		fi
	fi
fi
if test x$found_in_sandbox_fltk = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_fltk"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_fltk
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_fltk
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for fltk])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include<FL/Fl_Gl_Window.H>
#include<FL/Fl_PNG_Image.H>
#include<FL/fl_draw.H>
int main()
{
	Fl_Gl_Window w(0,0);
	Fl_PNG_Image png("");
	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_fltk GL
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<FL/Fl_Gl_Window.H>
#include<FL/Fl_PNG_Image.H>
#include<FL/fl_draw.H>
int main()
{
	Fl_Gl_Window w(0,0);
	Fl_PNG_Image png("");
	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_fltk=$tmp_libs
			found_fltk=yes
			AC_MSG_RESULT([$libs_fltk])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding fltk
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_fltk = xyes
    then
    if test x$found_fltk = xno
	then
	AC_MSG_CHECKING([fltk has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists fltk
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_fltk=yes
		include_dirs_fltk=`$pkg_config fltk --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_fltk=`$pkg_config fltk --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_fltk=`$pkg_config fltk --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_fltk=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding fltk
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_fltk = xyes
then
  if test x$found_fltk = xno
	then
	found=1
	for file in FL/gl.h
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for fltk in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[fltk header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"fltk fltk_images fltk_forms fltk_gl" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_fltk
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_fltk
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for fltk])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<FL/Fl_Gl_Window.H>
#include<FL/Fl_PNG_Image.H>
#include<FL/fl_draw.H>
int main()
{
	Fl_Gl_Window w(0,0);
	Fl_PNG_Image png("");
	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in fltk fltk_images fltk_forms fltk_gl GL
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include<FL/Fl_Gl_Window.H>
#include<FL/Fl_PNG_Image.H>
#include<FL/fl_draw.H>
int main()
{
	Fl_Gl_Window w(0,0);
	Fl_PNG_Image png("");
	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_fltk=$tmp_libs
					found_fltk=yes
					AC_MSG_RESULT([$libs_fltk])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_fltk
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in 
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include<FL/Fl_Gl_Window.H>
#include<FL/Fl_PNG_Image.H>
#include<FL/fl_draw.H>
int main()
{
	Fl_Gl_Window w(0,0);
	Fl_PNG_Image png("");
	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_fltk=$tmp_libs
						found_fltk=yes
						AC_MSG_RESULT([$libs_fltk])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_fltk=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_fltk=no
	fi
    fi
fi

fi
#####################################################################
# Check if qt is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([qt usage specified])
#
AC_ARG_WITH(
	[qt],AC_HELP_STRING([--with-qt],
	[use qt (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_qt=yes
	else
		AC_MSG_RESULT([no])
		enabled_qt=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_qt=yes
])
found_qt=no
#####################################################################

#####################################################################
# IF qt is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_qt = xyes
then
#####################################################################

#####################################################################
# Standard routine for finding qt
#
# source: acv/standard_qt_find.acv 
#####################################################################

if test x$enabled_qt = xyes
then
	if test x$found_qt = xno
	then
		AC_ARG_WITH([qtdir], [  --with-qtdir=DIR        Qt installation directory [default=$QTDIR]], QTDIR=$withval)
		dirs="$sandbox_location/qt3/include /usr/include/qt3"
		if test x$QTDIR != x
		then
			QTDIR=`echo $QTDIR | sed -e 's=/$=='`
			dirs="$QTDIR/include $dirs"
		fi
		for sub1 in /usr/lib /usr/share /usr/local /usr/local/lib /usr/local/share /opt
		do
			for sub2 in qt3 qt31 qt32 qt33
			do
				dirs="$dirs $sub1/$sub2/include"
			done
		done
		for dir in $dirs
		do
			found=1
			for file in qapplication.h
			do
				if test ! -f $dir/$file
				then
					found=0
					break 1
				fi
			done
			if test $found = 1
			then
				break 1
			fi
		done
		AC_MSG_CHECKING([[qt header(s) found]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes: $dir])
			include_dirs_qt=$dir
			lib_dirs_qt=`echo $dir | sed -e 's=/include=/lib='`
			if test x"$dir" == x"$QTDIR/include"
			then
				setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_qt"
			        setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
				AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])

			fi
			AC_MSG_RESULT([yes: $dir, assuming lib-dir $lib_dirs_qt])
			if test x"qt-mt" != x
			then
				LIBS=
				old_CXXFLAGS=$CXXFLAGS
				old_LDFLAGS=$LDFLAGS
				for tmp in $include_dirs_qt
				do
					CXXFLAGS="$CXXFLAGS -I$tmp"
				done
				for tmp in $lib_dirs_qt
				do
					LDFLAGS="$LDFLAGS -L$tmp"
				done
				AC_MSG_CHECKING([libraries needed for qt])
				AC_COMPILE_IFELSE(
				[
					AC_LANG_SOURCE(
					[[
#include<qapplication.h>
int main()
{
	int argc = 2;
	char *argv[]={"Just","testing"};
	QApplication app(argc,argv);
	return 0;	
}

					]]	
				)],[
					links_ok=0
					tmp_libs=""
					sep=""
					for tmp in qt-mt
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
							[[
#include<qapplication.h>
int main()
{
	int argc = 2;
	char *argv[]={"Just","testing"};
	QApplication app(argc,argv);
	return 0;	
}
;
							]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_qt=$tmp_libs
						found_qt=yes
						AC_MSG_RESULT([$libs_qt])
					else
						# trying alternative libs
						LDFLAGS=$old_LDFLAGS
						tmp_libs=""
						sep=""
						for tmp in $lib_dirs_qt
						do
							LDFLAGS="$LDFLAGS -L$tmp"
						done	
						for tmp in qt
						do
							LDFLAGS="$LDFLAGS -l$tmp"
							tmp_libs="$tmp_libs$sep$tmp"
							sep=" "
							AC_LINK_IFELSE(
							[
								AC_LANG_SOURCE(
								[[
									#include<qapplication.h>
int main()
{
	int argc = 2;
	char *argv[]={"Just","testing"};
	QApplication app(argc,argv);
	return 0;	
}
;
								]])	
							],[
								links_ok=yes
							],[
							]
							)
							if test x$links_ok = xyes; then
								break 2
							fi
						done
						if test x$links_ok = xyes; then
							libs_qt=$tmp_libs
							found_qt=yes
							AC_MSG_RESULT([$libs_qt])
						else
							AC_MSG_RESULT([linking failed])
						fi
					fi
				],[
					AC_MSG_RESULT([compilation failed])
				])
				CXXFLAGS=$old_CXXFLAGS
				LDFLAGS=$old_LDFLAGS
			else
				# no libs to checks
				found_qt=yes
			fi
		else
			AC_MSG_RESULT([no])
			found_qt=no
		fi
	else
		AC_MSG_RESULT([no])
	fi
fi
fi
#####################################################################
# Check if sndfile is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([sndfile usage specified])
#
AC_ARG_WITH(
	[sndfile],AC_HELP_STRING([--with-sndfile],
	[use sndfile (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_sndfile=yes
	else
		AC_MSG_RESULT([no])
		enabled_sndfile=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_sndfile=yes
])
found_sndfile=no
#####################################################################

#####################################################################
# IF sndfile is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_sndfile = xyes
then
#####################################################################

#####################################################################
# Look for sndfile in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([sndfile in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_sndfile = xno
	then
		found=1
		for file in sndfile.h
		do
			AC_MSG_CHECKING([$file for sndfile in sandbox])
			if test -r $sandbox_location/libsndfile/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_sndfile="$sandbox_location/libsndfile/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[sndfile header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in sndfile
			do
				AC_MSG_CHECKING([lib$file for sndfile in sandbox])
				if test -r $sandbox_location/libsndfile/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_sndfile="$libs_in_sandbox_sndfile$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_sndfile="$sandbox_location/libsndfile/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all sndfile lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_sndfile=yes
				found_in_sandbox_sndfile=yes
			else
				AC_MSG_RESULT([no])
				found_sndfile=no
			fi
		else
			AC_MSG_RESULT([no])
			found_sndfile=no
		fi
	fi
fi
if test x$found_in_sandbox_sndfile = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_sndfile"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_sndfile
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_sndfile
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for sndfile])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include <sndfile.h>
#include <stdio.h>
int main()
{
	SF_INFO sfinfo;
	sf_open_fd(fileno(stdout),SFM_WRITE,&sfinfo,0);
	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_sndfile 
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include <sndfile.h>
#include <stdio.h>
int main()
{
	SF_INFO sfinfo;
	sf_open_fd(fileno(stdout),SFM_WRITE,&sfinfo,0);
	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_sndfile=$tmp_libs
			found_sndfile=yes
			AC_MSG_RESULT([$libs_sndfile])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding sndfile
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_sndfile = xyes
    then
    if test x$found_sndfile = xno
	then
	AC_MSG_CHECKING([sndfile has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists sndfile
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_sndfile=yes
		include_dirs_sndfile=`$pkg_config sndfile --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_sndfile=`$pkg_config sndfile --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_sndfile=`$pkg_config sndfile --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_sndfile=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding sndfile
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_sndfile = xyes
then
  if test x$found_sndfile = xno
	then
	found=1
	for file in sndfile.h
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for sndfile in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[sndfile header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"sndfile" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_sndfile
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_sndfile
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for sndfile])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include <sndfile.h>
#include <stdio.h>
int main()
{
	SF_INFO sfinfo;
	sf_open_fd(fileno(stdout),SFM_WRITE,&sfinfo,0);
	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in sndfile 
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include <sndfile.h>
#include <stdio.h>
int main()
{
	SF_INFO sfinfo;
	sf_open_fd(fileno(stdout),SFM_WRITE,&sfinfo,0);
	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_sndfile=$tmp_libs
					found_sndfile=yes
					AC_MSG_RESULT([$libs_sndfile])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_sndfile
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in 
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include <sndfile.h>
#include <stdio.h>
int main()
{
	SF_INFO sfinfo;
	sf_open_fd(fileno(stdout),SFM_WRITE,&sfinfo,0);
	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_sndfile=$tmp_libs
						found_sndfile=yes
						AC_MSG_RESULT([$libs_sndfile])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_sndfile=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_sndfile=no
	fi
    fi
fi

fi
#####################################################################
# Check if oggvorbis is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([oggvorbis usage specified])
#
AC_ARG_WITH(
	[oggvorbis],AC_HELP_STRING([--with-oggvorbis],
	[use oggvorbis (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_oggvorbis=yes
	else
		AC_MSG_RESULT([no])
		enabled_oggvorbis=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_oggvorbis=yes
])
found_oggvorbis=no
#####################################################################

#####################################################################
# IF oggvorbis is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_oggvorbis = xyes
then
#####################################################################

#####################################################################
# Look for oggvorbis in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([oggvorbis in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_oggvorbis = xno
	then
		found=1
		for file in vorbis/vorbisfile.h ogg/ogg.h
		do
			AC_MSG_CHECKING([$file for oggvorbis in sandbox])
			if test -r $sandbox_location/oggvorbis/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_oggvorbis="$sandbox_location/oggvorbis/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[oggvorbis header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in vorbisenc vorbisfile ogg vorbis
			do
				AC_MSG_CHECKING([lib$file for oggvorbis in sandbox])
				if test -r $sandbox_location/oggvorbis/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_oggvorbis="$libs_in_sandbox_oggvorbis$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_oggvorbis="$sandbox_location/oggvorbis/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all oggvorbis lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_oggvorbis=yes
				found_in_sandbox_oggvorbis=yes
			else
				AC_MSG_RESULT([no])
				found_oggvorbis=no
			fi
		else
			AC_MSG_RESULT([no])
			found_oggvorbis=no
		fi
	fi
fi
if test x$found_in_sandbox_oggvorbis = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_oggvorbis"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_oggvorbis
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_oggvorbis
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for oggvorbis])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>
int main()
{
	vorbis_info vi;
	OggVorbis_File vf;
	oggpack_buffer b;
	vorbis_info_init(&vi);
	vorbis_encode_setup_init(&vi);
	ov_test_open(&vf);
	oggpack_writeinit(&b);
	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_oggvorbis 
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>
int main()
{
	vorbis_info vi;
	OggVorbis_File vf;
	oggpack_buffer b;
	vorbis_info_init(&vi);
	vorbis_encode_setup_init(&vi);
	ov_test_open(&vf);
	oggpack_writeinit(&b);
	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_oggvorbis=$tmp_libs
			found_oggvorbis=yes
			AC_MSG_RESULT([$libs_oggvorbis])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding oggvorbis
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_oggvorbis = xyes
    then
    if test x$found_oggvorbis = xno
	then
	AC_MSG_CHECKING([oggvorbis has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists oggvorbis
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_oggvorbis=yes
		include_dirs_oggvorbis=`$pkg_config oggvorbis --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_oggvorbis=`$pkg_config oggvorbis --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_oggvorbis=`$pkg_config oggvorbis --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_oggvorbis=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding oggvorbis
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_oggvorbis = xyes
then
  if test x$found_oggvorbis = xno
	then
	found=1
	for file in vorbis/vorbisfile.h ogg/ogg.h
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for oggvorbis in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[oggvorbis header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"vorbisenc vorbisfile ogg vorbis" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_oggvorbis
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_oggvorbis
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for oggvorbis])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>
int main()
{
	vorbis_info vi;
	OggVorbis_File vf;
	oggpack_buffer b;
	vorbis_info_init(&vi);
	vorbis_encode_setup_init(&vi);
	ov_test_open(&vf);
	oggpack_writeinit(&b);
	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in vorbisenc vorbisfile ogg vorbis 
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>
int main()
{
	vorbis_info vi;
	OggVorbis_File vf;
	oggpack_buffer b;
	vorbis_info_init(&vi);
	vorbis_encode_setup_init(&vi);
	ov_test_open(&vf);
	oggpack_writeinit(&b);
	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_oggvorbis=$tmp_libs
					found_oggvorbis=yes
					AC_MSG_RESULT([$libs_oggvorbis])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_oggvorbis
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in 
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>
int main()
{
	vorbis_info vi;
	OggVorbis_File vf;
	oggpack_buffer b;
	vorbis_info_init(&vi);
	vorbis_encode_setup_init(&vi);
	ov_test_open(&vf);
	oggpack_writeinit(&b);
	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_oggvorbis=$tmp_libs
						found_oggvorbis=yes
						AC_MSG_RESULT([$libs_oggvorbis])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_oggvorbis=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_oggvorbis=no
	fi
    fi
fi

fi
#####################################################################
# Check if ladspa is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([ladspa usage specified])
#
AC_ARG_WITH(
	[ladspa],AC_HELP_STRING([--with-ladspa],
	[use ladspa (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_ladspa=yes
	else
		AC_MSG_RESULT([no])
		enabled_ladspa=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_ladspa=yes
])
found_ladspa=no
#####################################################################

#####################################################################
# IF ladspa is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_ladspa = xyes
then
#####################################################################

#####################################################################
# Look for ladspa in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([ladspa in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_ladspa = xno
	then
		found=1
		for file in ladspa.h
		do
			AC_MSG_CHECKING([$file for ladspa in sandbox])
			if test -r $sandbox_location/ladspa/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_ladspa="$sandbox_location/ladspa/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[ladspa header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in 
			do
				AC_MSG_CHECKING([lib$file for ladspa in sandbox])
				if test -r $sandbox_location/ladspa/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_ladspa="$libs_in_sandbox_ladspa$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_ladspa="$sandbox_location/ladspa/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all ladspa lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_ladspa=yes
				found_in_sandbox_ladspa=yes
			else
				AC_MSG_RESULT([no])
				found_ladspa=no
			fi
		else
			AC_MSG_RESULT([no])
			found_ladspa=no
		fi
	fi
fi
if test x$found_in_sandbox_ladspa = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_ladspa"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_ladspa
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_ladspa
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for ladspa])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include <ladspa.h>
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index)
{ return 0; }
int main()
{
	ladspa_descriptor(0);
	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_ladspa 
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include <ladspa.h>
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index)
{ return 0; }
int main()
{
	ladspa_descriptor(0);
	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_ladspa=$tmp_libs
			found_ladspa=yes
			AC_MSG_RESULT([$libs_ladspa])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding ladspa
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_ladspa = xyes
    then
    if test x$found_ladspa = xno
	then
	AC_MSG_CHECKING([ladspa has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists ladspa
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_ladspa=yes
		include_dirs_ladspa=`$pkg_config ladspa --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_ladspa=`$pkg_config ladspa --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_ladspa=`$pkg_config ladspa --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_ladspa=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding ladspa
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_ladspa = xyes
then
  if test x$found_ladspa = xno
	then
	found=1
	for file in ladspa.h
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for ladspa in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[ladspa header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_ladspa
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_ladspa
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for ladspa])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include <ladspa.h>
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index)
{ return 0; }
int main()
{
	ladspa_descriptor(0);
	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in  
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include <ladspa.h>
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index)
{ return 0; }
int main()
{
	ladspa_descriptor(0);
	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_ladspa=$tmp_libs
					found_ladspa=yes
					AC_MSG_RESULT([$libs_ladspa])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_ladspa
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in 
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include <ladspa.h>
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index)
{ return 0; }
int main()
{
	ladspa_descriptor(0);
	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_ladspa=$tmp_libs
						found_ladspa=yes
						AC_MSG_RESULT([$libs_ladspa])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_ladspa=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_ladspa=no
	fi
    fi
fi

fi
#####################################################################
# Check if portmidi is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([portmidi usage specified])
#
AC_ARG_WITH(
	[portmidi],AC_HELP_STRING([--with-portmidi],
	[use portmidi (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_portmidi=yes
	else
		AC_MSG_RESULT([no])
		enabled_portmidi=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_portmidi=yes
])
found_portmidi=no
#####################################################################

#####################################################################
# IF portmidi is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_portmidi = xyes
then
#####################################################################

#####################################################################
# Look for portmidi in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([portmidi in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_portmidi = xno
	then
		found=1
		for file in portmidi.h
		do
			AC_MSG_CHECKING([$file for portmidi in sandbox])
			if test -r $sandbox_location/portmidi/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_portmidi="$sandbox_location/portmidi/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[portmidi header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in porttime portmidi
			do
				AC_MSG_CHECKING([lib$file for portmidi in sandbox])
				if test -r $sandbox_location/portmidi/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_portmidi="$libs_in_sandbox_portmidi$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_portmidi="$sandbox_location/portmidi/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all portmidi lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_portmidi=yes
				found_in_sandbox_portmidi=yes
			else
				AC_MSG_RESULT([no])
				found_portmidi=no
			fi
		else
			AC_MSG_RESULT([no])
			found_portmidi=no
		fi
	fi
fi
if test x$found_in_sandbox_portmidi = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_portmidi"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_portmidi
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_portmidi
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for portmidi])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include<portmidi.h>
int main()
{
	Pm_CountDevices();
	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_portmidi 
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<portmidi.h>
int main()
{
	Pm_CountDevices();
	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_portmidi=$tmp_libs
			found_portmidi=yes
			AC_MSG_RESULT([$libs_portmidi])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding portmidi
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_portmidi = xyes
    then
    if test x$found_portmidi = xno
	then
	AC_MSG_CHECKING([portmidi has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists portmidi
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_portmidi=yes
		include_dirs_portmidi=`$pkg_config portmidi --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_portmidi=`$pkg_config portmidi --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_portmidi=`$pkg_config portmidi --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_portmidi=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding portmidi
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_portmidi = xyes
then
  if test x$found_portmidi = xno
	then
	found=1
	for file in portmidi.h
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for portmidi in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[portmidi header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"porttime portmidi" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_portmidi
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_portmidi
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for portmidi])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<portmidi.h>
int main()
{
	Pm_CountDevices();
	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in porttime portmidi 
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include<portmidi.h>
int main()
{
	Pm_CountDevices();
	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_portmidi=$tmp_libs
					found_portmidi=yes
					AC_MSG_RESULT([$libs_portmidi])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_portmidi
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in portmidi
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include<portmidi.h>
int main()
{
	Pm_CountDevices();
	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_portmidi=$tmp_libs
						found_portmidi=yes
						AC_MSG_RESULT([$libs_portmidi])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_portmidi=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_portmidi=no
	fi
    fi
fi

fi
#####################################################################
# Check if portaudio is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([portaudio usage specified])
#
AC_ARG_WITH(
	[portaudio],AC_HELP_STRING([--with-portaudio],
	[use portaudio (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_portaudio=yes
	else
		AC_MSG_RESULT([no])
		enabled_portaudio=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_portaudio=yes
])
found_portaudio=no
#####################################################################

#####################################################################
# IF portaudio is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_portaudio = xyes
then
#####################################################################

#####################################################################
# Look for portaudio in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([portaudio in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_portaudio = xno
	then
		found=1
		for file in portaudio.h
		do
			AC_MSG_CHECKING([$file for portaudio in sandbox])
			if test -r $sandbox_location/portaudio/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_portaudio="$sandbox_location/portaudio/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[portaudio header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in portaudio
			do
				AC_MSG_CHECKING([lib$file for portaudio in sandbox])
				if test -r $sandbox_location/portaudio/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_portaudio="$libs_in_sandbox_portaudio$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_portaudio="$sandbox_location/portaudio/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all portaudio lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_portaudio=yes
				found_in_sandbox_portaudio=yes
			else
				AC_MSG_RESULT([no])
				found_portaudio=no
			fi
		else
			AC_MSG_RESULT([no])
			found_portaudio=no
		fi
	fi
fi
if test x$found_in_sandbox_portaudio = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_portaudio"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_portaudio
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_portaudio
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for portaudio])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include<portaudio.h>
int main()
{
	Pa_CountDevices();
	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_portaudio 
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<portaudio.h>
int main()
{
	Pa_CountDevices();
	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_portaudio=$tmp_libs
			found_portaudio=yes
			AC_MSG_RESULT([$libs_portaudio])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding portaudio
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_portaudio = xyes
    then
    if test x$found_portaudio = xno
	then
	AC_MSG_CHECKING([portaudio has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists portaudio
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_portaudio=yes
		include_dirs_portaudio=`$pkg_config portaudio --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_portaudio=`$pkg_config portaudio --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_portaudio=`$pkg_config portaudio --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_portaudio=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding portaudio
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_portaudio = xyes
then
  if test x$found_portaudio = xno
	then
	found=1
	for file in portaudio.h
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for portaudio in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[portaudio header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"portaudio" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_portaudio
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_portaudio
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for portaudio])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<portaudio.h>
int main()
{
	Pa_CountDevices();
	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in portaudio 
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include<portaudio.h>
int main()
{
	Pa_CountDevices();
	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_portaudio=$tmp_libs
					found_portaudio=yes
					AC_MSG_RESULT([$libs_portaudio])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_portaudio
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in 
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include<portaudio.h>
int main()
{
	Pa_CountDevices();
	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_portaudio=$tmp_libs
						found_portaudio=yes
						AC_MSG_RESULT([$libs_portaudio])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_portaudio=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_portaudio=no
	fi
    fi
fi

fi
#####################################################################
# Check if alsa is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([alsa usage specified])
#
AC_ARG_WITH(
	[alsa],AC_HELP_STRING([--with-alsa],
	[use alsa (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_alsa=yes
	else
		AC_MSG_RESULT([no])
		enabled_alsa=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_alsa=yes
])
found_alsa=no
#####################################################################

#####################################################################
# IF alsa is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_alsa = xyes
then
#####################################################################

#####################################################################
# Look for alsa in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([alsa in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_alsa = xno
	then
		found=1
		for file in alsa/asoundlib.h
		do
			AC_MSG_CHECKING([$file for alsa in sandbox])
			if test -r $sandbox_location/alsa/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_alsa="$sandbox_location/alsa/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[alsa header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in asound
			do
				AC_MSG_CHECKING([lib$file for alsa in sandbox])
				if test -r $sandbox_location/alsa/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_alsa="$libs_in_sandbox_alsa$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_alsa="$sandbox_location/alsa/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all alsa lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_alsa=yes
				found_in_sandbox_alsa=yes
			else
				AC_MSG_RESULT([no])
				found_alsa=no
			fi
		else
			AC_MSG_RESULT([no])
			found_alsa=no
		fi
	fi
fi
if test x$found_in_sandbox_alsa = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_alsa"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_alsa
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_alsa
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for alsa])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include<alsa/asoundlib.h>
int main()
{
	int card = 0;
	snd_card_next(&card);
	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_alsa 
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<alsa/asoundlib.h>
int main()
{
	int card = 0;
	snd_card_next(&card);
	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_alsa=$tmp_libs
			found_alsa=yes
			AC_MSG_RESULT([$libs_alsa])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding alsa
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_alsa = xyes
    then
    if test x$found_alsa = xno
	then
	AC_MSG_CHECKING([alsa has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists alsa
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_alsa=yes
		include_dirs_alsa=`$pkg_config alsa --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_alsa=`$pkg_config alsa --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_alsa=`$pkg_config alsa --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_alsa=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding alsa
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_alsa = xyes
then
  if test x$found_alsa = xno
	then
	found=1
	for file in alsa/asoundlib.h
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for alsa in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[alsa header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"asound" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_alsa
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_alsa
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for alsa])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<alsa/asoundlib.h>
int main()
{
	int card = 0;
	snd_card_next(&card);
	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in asound 
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include<alsa/asoundlib.h>
int main()
{
	int card = 0;
	snd_card_next(&card);
	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_alsa=$tmp_libs
					found_alsa=yes
					AC_MSG_RESULT([$libs_alsa])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_alsa
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in 
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include<alsa/asoundlib.h>
int main()
{
	int card = 0;
	snd_card_next(&card);
	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_alsa=$tmp_libs
						found_alsa=yes
						AC_MSG_RESULT([$libs_alsa])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_alsa=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_alsa=no
	fi
    fi
fi

fi
#####################################################################
# Check if mad is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([mad usage specified])
#
AC_ARG_WITH(
	[mad],AC_HELP_STRING([--with-mad],
	[use mad (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_mad=yes
	else
		AC_MSG_RESULT([no])
		enabled_mad=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_mad=yes
])
found_mad=no
#####################################################################

#####################################################################
# IF mad is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_mad = xyes
then
#####################################################################

#####################################################################
# Look for mad in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([mad in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_mad = xno
	then
		found=1
		for file in mad.h
		do
			AC_MSG_CHECKING([$file for mad in sandbox])
			if test -r $sandbox_location/libmad/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_mad="$sandbox_location/libmad/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[mad header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in mad
			do
				AC_MSG_CHECKING([lib$file for mad in sandbox])
				if test -r $sandbox_location/libmad/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_mad="$libs_in_sandbox_mad$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_mad="$sandbox_location/libmad/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all mad lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_mad=yes
				found_in_sandbox_mad=yes
			else
				AC_MSG_RESULT([no])
				found_mad=no
			fi
		else
			AC_MSG_RESULT([no])
			found_mad=no
		fi
	fi
fi
if test x$found_in_sandbox_mad = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_mad"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_mad
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_mad
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for mad])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include<mad.h>
int main()
{
	struct mad_stream s;
	mad_stream_init(&s);
	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_mad m
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<mad.h>
int main()
{
	struct mad_stream s;
	mad_stream_init(&s);
	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_mad=$tmp_libs
			found_mad=yes
			AC_MSG_RESULT([$libs_mad])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding mad
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_mad = xyes
    then
    if test x$found_mad = xno
	then
	AC_MSG_CHECKING([mad has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists mad
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_mad=yes
		include_dirs_mad=`$pkg_config mad --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_mad=`$pkg_config mad --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_mad=`$pkg_config mad --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_mad=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding mad
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_mad = xyes
then
  if test x$found_mad = xno
	then
	found=1
	for file in mad.h
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for mad in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[mad header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"mad" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_mad
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_mad
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for mad])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include<mad.h>
int main()
{
	struct mad_stream s;
	mad_stream_init(&s);
	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in mad m
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include<mad.h>
int main()
{
	struct mad_stream s;
	mad_stream_init(&s);
	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_mad=$tmp_libs
					found_mad=yes
					AC_MSG_RESULT([$libs_mad])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_mad
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in 
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include<mad.h>
int main()
{
	struct mad_stream s;
	mad_stream_init(&s);
	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_mad=$tmp_libs
						found_mad=yes
						AC_MSG_RESULT([$libs_mad])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_mad=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_mad=no
	fi
    fi
fi

fi
#####################################################################
# Check if id3 is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([id3 usage specified])
#
AC_ARG_WITH(
	[id3],AC_HELP_STRING([--with-id3],
	[use id3 (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_id3=yes
	else
		AC_MSG_RESULT([no])
		enabled_id3=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_id3=yes
])
found_id3=no
#####################################################################

#####################################################################
# IF id3 is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_id3 = xyes
then
#####################################################################

#####################################################################
# Look for id3 in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([id3 in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_id3 = xno
	then
		found=1
		for file in id3/tag.h
		do
			AC_MSG_CHECKING([$file for id3 in sandbox])
			if test -r $sandbox_location/id3lib/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_id3="$sandbox_location/id3lib/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[id3 header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in id3
			do
				AC_MSG_CHECKING([lib$file for id3 in sandbox])
				if test -r $sandbox_location/id3lib/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_id3="$libs_in_sandbox_id3$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_id3="$sandbox_location/id3lib/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all id3 lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_id3=yes
				found_in_sandbox_id3=yes
			else
				AC_MSG_RESULT([no])
				found_id3=no
			fi
		else
			AC_MSG_RESULT([no])
			found_id3=no
		fi
	fi
fi
if test x$found_in_sandbox_id3 = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_id3"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_id3
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_id3
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for id3])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include <id3/tag.h>
int main()
{
	ID3_Tag myTag;
	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_id3 z
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include <id3/tag.h>
int main()
{
	ID3_Tag myTag;
	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_id3=$tmp_libs
			found_id3=yes
			AC_MSG_RESULT([$libs_id3])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding id3
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_id3 = xyes
    then
    if test x$found_id3 = xno
	then
	AC_MSG_CHECKING([id3 has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists id3
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_id3=yes
		include_dirs_id3=`$pkg_config id3 --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_id3=`$pkg_config id3 --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_id3=`$pkg_config id3 --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_id3=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding id3
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_id3 = xyes
then
  if test x$found_id3 = xno
	then
	found=1
	for file in id3/tag.h
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for id3 in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[id3 header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"id3" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_id3
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_id3
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for id3])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include <id3/tag.h>
int main()
{
	ID3_Tag myTag;
	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in id3 z
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include <id3/tag.h>
int main()
{
	ID3_Tag myTag;
	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_id3=$tmp_libs
					found_id3=yes
					AC_MSG_RESULT([$libs_id3])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_id3
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in 
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include <id3/tag.h>
int main()
{
	ID3_Tag myTag;
	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_id3=$tmp_libs
						found_id3=yes
						AC_MSG_RESULT([$libs_id3])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_id3=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_id3=no
	fi
    fi
fi

fi
#####################################################################
# Check if oscpack is going to be used
#
# source: acv/check_package_enable.acv 
#####################################################################
AC_MSG_CHECKING([oscpack usage specified])
#
AC_ARG_WITH(
	[oscpack],AC_HELP_STRING([--with-oscpack],
	[use oscpack (default is yes)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_oscpack=yes
	else
		AC_MSG_RESULT([no])
		enabled_oscpack=no
	fi
],[
	AC_MSG_RESULT([yes (default)])
	enabled_oscpack=yes
])
found_oscpack=no
#####################################################################

#####################################################################
# IF oscpack is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_oscpack = xyes
then
#####################################################################

#####################################################################
# Look for oscpack in the sandbox
#
# source: acv/package_in_sandbox.acv 
#####################################################################
AC_MSG_CHECKING([oscpack in sandbox])
if test x$sandbox_location = x
then
	if test x$enabled_sandbox = xno
	then
		AC_MSG_RESULT([no - sandbox disabled])	
	else
		AC_MSG_RESULT([no - sandbox not found])	
	fi
else
	AC_MSG_RESULT([searching headers])	
	if test x$found_oscpack = xno
	then
		found=1
		for file in oscpack/ip/UdpSocket.h
		do
			AC_MSG_CHECKING([$file for oscpack in sandbox])
			if test -r $sandbox_location/oscpack/include/$file
			then
				AC_MSG_RESULT([yes])
				include_dirs_oscpack="$sandbox_location/oscpack/include"
			else
				AC_MSG_RESULT([no])
				found=0
			fi
		done
		AC_MSG_CHECKING([[oscpack header(s) found in sandbox]])
		if test $found = 1
		then
			AC_MSG_RESULT([yes])
			sep=""
			found=1
			libdirset=no
			for file in oscpack
			do
				AC_MSG_CHECKING([lib$file for oscpack in sandbox])
				if test -r $sandbox_location/oscpack/lib/lib$file.$dllext
					then
					AC_MSG_RESULT([yes])
					libs_in_sandbox_oscpack="$libs_in_sandbox_oscpack$sep$file"
					if test x$libdirset = xno
					then
						lib_dirs_oscpack="$sandbox_location/oscpack/lib"
						libdirset=yes
					fi
					sep=" "
				else
					AC_MSG_RESULT([no])
					found=0
				fi
			done
			AC_MSG_CHECKING([[all oscpack lib(s) found in sandbox]])
			if test $found = 1
			then
				AC_MSG_RESULT([yes - adding to library path])
				found_oscpack=yes
				found_in_sandbox_oscpack=yes
			else
				AC_MSG_RESULT([no])
				found_oscpack=no
			fi
		else
			AC_MSG_RESULT([no])
			found_oscpack=no
		fi
	fi
fi
if test x$found_in_sandbox_oscpack = xyes
then
	setenv DYLD_LIBRARY_PATH "$DYLD_LIBRARY_PATH:$lib_dirs_oscpack"
	setenv LD_LIBRARY_PATH "$DYLD_LIBRARY_PATH"
	AC_MSG_NOTICE([LD_LIBRARY_PATH / DYLD_LIBRARY_PATH is now $DYLD_LIBRARY_PATH])
	LIBS=
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_oscpack
	do
		CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_oscpack
	do
		LDFLAGS="$LDFLAGS -L$tmp"
	done
	AC_MSG_CHECKING([libraries needed for oscpack])
	AC_COMPILE_IFELSE(
	[
		AC_LANG_SOURCE(
[[
#include <oscpack/ip/IpEndpointName.h>
#include <oscpack/ip/UdpSocket.h>
int main()
{
	UdpTransmitSocket socket( IpEndpointName("localhost", 9999) );
	bool b=socket.IsBound();
	return 0;
}

]]	
	)],[
		links_ok=0
		tmp_libs=""
		sep=""
		for tmp in $libs_in_sandbox_oscpack pthread
		do
			LDFLAGS="$LDFLAGS -l$tmp"
			tmp_libs="$tmp_libs$sep$tmp"
			sep=" "
			AC_LINK_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include <oscpack/ip/IpEndpointName.h>
#include <oscpack/ip/UdpSocket.h>
int main()
{
	UdpTransmitSocket socket( IpEndpointName("localhost", 9999) );
	bool b=socket.IsBound();
	return 0;
}
;
]])	
			],[
				links_ok=yes
			],[
			]
			)
			if test x$links_ok = xyes; then
				break 2
			fi
		done
		if test x$links_ok = xyes; then
			libs_oscpack=$tmp_libs
			found_oscpack=yes
			AC_MSG_RESULT([$libs_oscpack])
		else
			AC_MSG_RESULT([linking failed])
		fi
	],[
		AC_MSG_RESULT([compilation failed])
	])
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
fi
#####################################################################
# pkgconfig routine for finding oscpack
#
# source: acv/pkg_config_find.acv 
#####################################################################
if test x$enabled_oscpack = xyes
    then
    if test x$found_oscpack = xno
	then
	AC_MSG_CHECKING([oscpack has pkg-config])
	if test x$pkg_config_status = xenabled
	    then
	    $pkg_config --exists oscpack
	    if test $? = 0
		then
		AC_MSG_RESULT([yes])
		found_oscpack=yes
		include_dirs_oscpack=`$pkg_config oscpack --cflags-only-I | sed -e 's/ -I/ /g' |  sed -e 's/^-I/ /g' | sed -e 's/^ //'`
		libs_oscpack=`$pkg_config oscpack --libs-only-l | sed -e 's/ -l/ /g' |  sed -e 's/^-l/ /g' | sed -e 's/^ //'`
		lib_dirs_oscpack=`$pkg_config oscpack --libs-only-L | sed -e 's/ -L/ /g' | sed -e 's/^-L/ /g' | sed -e 's/^ //'`
	    else
		AC_MSG_RESULT([no])
		found_oscpack=no
	    fi
	else
	    if test x$pkg_config_status = xdisabled
		then
		AC_MSG_RESULT([no: pkg-config disabled])
	    else
		AC_MSG_RESULT([no: pkg-config is not installed])
	    fi
	fi
    fi
fi
#####################################################################

#####################################################################
# Standard routine for finding oscpack
#
# source: acv/standard_find.acv 
#####################################################################
if test x$enabled_oscpack = xyes
then
  if test x$found_oscpack = xno
	then
	found=1
	for file in oscpack/ip/UdpSocket.h
	do
		AC_CHECK_HEADER($file,[filefound=1],[filefound=0])
		AC_MSG_CHECKING([$file for oscpack in standard locations])
		if test $filefound = 1
		then
			AC_MSG_RESULT([yes])
		else
	    AC_MSG_RESULT([no])
	    found=0
		fi
	done
	AC_MSG_CHECKING([[oscpack header(s) found in standard locations]])
	if test $found = 1
	then
		AC_MSG_RESULT([yes])
		if test x"oscpack" != x
		then
			LIBS=
			old_CXXFLAGS=$CXXFLAGS
			old_LDFLAGS=$LDFLAGS
			for tmp in $include_dirs_oscpack
			do
				CXXFLAGS="$CXXFLAGS -I$tmp"
			done
			for tmp in $lib_dirs_oscpack
			do
				LDFLAGS="$LDFLAGS -L$tmp"
			done
			AC_MSG_CHECKING([libraries needed for oscpack])
			AC_COMPILE_IFELSE(
			[
				AC_LANG_SOURCE(
[[
#include <oscpack/ip/IpEndpointName.h>
#include <oscpack/ip/UdpSocket.h>
int main()
{
	UdpTransmitSocket socket( IpEndpointName("localhost", 9999) );
	bool b=socket.IsBound();
	return 0;
}

]]	
			)],[
				links_ok=0
				tmp_libs=""
				sep=""
				for tmp in oscpack pthread
				do
					LDFLAGS="$LDFLAGS -l$tmp"
					tmp_libs="$tmp_libs$sep$tmp"
					sep=" "
					AC_LINK_IFELSE(
					[
						AC_LANG_SOURCE(
[[
#include <oscpack/ip/IpEndpointName.h>
#include <oscpack/ip/UdpSocket.h>
int main()
{
	UdpTransmitSocket socket( IpEndpointName("localhost", 9999) );
	bool b=socket.IsBound();
	return 0;
}
;
]])	
					],[
						links_ok=yes
					],[
					]
					)
					if test x$links_ok = xyes; then
						break 2
					fi
				done
				if test x$links_ok = xyes; then
					libs_oscpack=$tmp_libs
					found_oscpack=yes
					AC_MSG_RESULT([$libs_oscpack])
				else
					# trying alternative libs
					LDFLAGS=$old_LDFLAGS
					tmp_libs=""
					sep=""
					for tmp in $lib_dirs_oscpack
					do
						LDFLAGS="$LDFLAGS -L$tmp"
					done
		for tmp in 
					do
						LDFLAGS="$LDFLAGS -l$tmp"
						tmp_libs="$tmp_libs$sep$tmp"
						sep=" "
						AC_LINK_IFELSE(
						[
							AC_LANG_SOURCE(
[[
#include <oscpack/ip/IpEndpointName.h>
#include <oscpack/ip/UdpSocket.h>
int main()
{
	UdpTransmitSocket socket( IpEndpointName("localhost", 9999) );
	bool b=socket.IsBound();
	return 0;
}
;
]])	
						],[
							links_ok=yes
						],[
						]
						)
						if test x$links_ok = xyes; then
							break 2
						fi
					done
					if test x$links_ok = xyes; then
						libs_oscpack=$tmp_libs
						found_oscpack=yes
						AC_MSG_RESULT([$libs_oscpack])
					else
						AC_MSG_RESULT([linking failed])
					fi
				fi
			],[
				AC_MSG_RESULT([compilation failed])
			])
			CXXFLAGS=$old_CXXFLAGS
			LDFLAGS=$old_LDFLAGS
		else
			# no libs to checks
			found_oscpack=yes
		fi
	else
	    AC_MSG_RESULT([no])
	    found_oscpack=no
	fi
    fi
fi

fi
#####################################################################
# Check if exit_on_failure is enabled
#
# source: acv/check_feature_enable.acv 
#####################################################################
AC_MSG_CHECKING([exit_on_failure enabled])
#
AC_ARG_ENABLE(
	[exit_on_failure],AC_HELP_STRING([--enable-exit_on_failure],
	[use exit_on_failure (default is no)]),[
	if test x$enableval = xyes
	then
		AC_MSG_RESULT([yes])
		enabled_exit_on_failure=yes
	else
		AC_MSG_RESULT([no])
		enabled_exit_on_failure=no
	fi
],[
	AC_MSG_RESULT([no (default)])
	enabled_exit_on_failure=no
])
found_exit_on_failure=no
#####################################################################

#####################################################################
# IF fftw is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_fftw = xyes
then
#####################################################################

#####################################################################
# Default test, to test fftw
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_fftw
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_fftw
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_fftw
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_fftw
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with fftw])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include<fftw.h>
#include<rfftw.h>
int main()
{
	fftw_create_plan(0,FFTW_FORWARD,0);
	rfftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(double))
	{
		return -1;
	}
	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for fftw failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of fftw altogether, running configure
with the
  --without-fftw
option.
		])
	else
		result_fftw=fail
	fi
else
	result_fftw=ok
fi
#####################################################################

fi
#####################################################################
# IF sfftw is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_sfftw = xyes
then
#####################################################################

#####################################################################
# Default test, to test sfftw
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_sfftw
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_sfftw
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_sfftw
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_sfftw
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with sfftw])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include<sfftw.h>
#include<srfftw.h>
int main()
{
	fftw_create_plan(0,FFTW_FORWARD,0);
	rfftw_create_plan(0,FFTW_FORWARD,0);
	if (fftw_sizeof_fftw_real()!=sizeof(float))
	{
		return -1;
	}
	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for sfftw failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of sfftw altogether, running configure
with the
  --without-sfftw
option.
		])
	else
		result_sfftw=fail
	fi
else
	result_sfftw=ok
fi
#####################################################################

fi
#####################################################################
# IF xerces is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_xerces = xyes
then
#####################################################################

#####################################################################
# Default test, to test xerces
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_xerces
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_xerces
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_xerces
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_xerces
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with xerces])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include<xercesc/util/PlatformUtils.hpp>
int main()
{
	namespace xercesc=XERCES_CPP_NAMESPACE;
	xercesc::XMLPlatformUtils::Initialize();
	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for xerces failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of xerces altogether, running configure
with the
  --without-xerces
option.
		])
	else
		result_xerces=fail
	fi
else
	result_xerces=ok
fi
#####################################################################

fi
#####################################################################
# IF xmlpp is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_xmlpp = xyes
then
#####################################################################

#####################################################################
# Default test, to test xmlpp
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_xmlpp
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_xmlpp
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_xmlpp
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_xmlpp
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with xmlpp])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include <libxml++/libxml++.h>

int main( int argc, char** argv )
{
	xmlpp::Document document;

	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for xmlpp failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of xmlpp altogether, running configure
with the
  --without-xmlpp
option.
		])
	else
		result_xmlpp=fail
	fi
else
	result_xmlpp=ok
fi
#####################################################################

fi
#####################################################################
# IF fltk is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_fltk = xyes
then
#####################################################################

#####################################################################
# Default test, to test fltk
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_fltk
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_fltk
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_fltk
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_fltk
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with fltk])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include<FL/Fl_Gl_Window.H>
#include<FL/Fl_PNG_Image.H>
#include<FL/fl_draw.H>
int main()
{
	Fl_Gl_Window w(0,0);
	Fl_PNG_Image png("");
	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for fltk failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of fltk altogether, running configure
with the
  --without-fltk
option.
		])
	else
		result_fltk=fail
	fi
else
	result_fltk=ok
fi
#####################################################################

fi
#####################################################################
# IF qt is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_qt = xyes
then
#####################################################################

#####################################################################
# Default test, to test qt
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_qt
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_qt
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_qt
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_qt
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with qt])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include<qapplication.h>
int main()
{
	int argc = 2;
	char *argv[]={"Just","testing"};
	QApplication app(argc,argv);
	return 0;	
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for qt failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of qt altogether, running configure
with the
  --without-qt
option.
		])
	else
		result_qt=fail
	fi
else
	result_qt=ok
fi
#####################################################################

fi
#####################################################################
# IF sndfile is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_sndfile = xyes
then
#####################################################################

#####################################################################
# Default test, to test sndfile
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_sndfile
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_sndfile
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_sndfile
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_sndfile
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with sndfile])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include <sndfile.h>
#include <stdio.h>
int main()
{
	SF_INFO sfinfo;
	sf_open_fd(fileno(stdout),SFM_WRITE,&sfinfo,0);
	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for sndfile failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of sndfile altogether, running configure
with the
  --without-sndfile
option.
		])
	else
		result_sndfile=fail
	fi
else
	result_sndfile=ok
fi
#####################################################################

fi
#####################################################################
# IF oggvorbis is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_oggvorbis = xyes
then
#####################################################################

#####################################################################
# Default test, to test oggvorbis
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_oggvorbis
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_oggvorbis
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_oggvorbis
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_oggvorbis
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with oggvorbis])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>
int main()
{
	vorbis_info vi;
	OggVorbis_File vf;
	oggpack_buffer b;
	vorbis_info_init(&vi);
	vorbis_encode_setup_init(&vi);
	ov_test_open(&vf);
	oggpack_writeinit(&b);
	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for oggvorbis failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of oggvorbis altogether, running configure
with the
  --without-oggvorbis
option.
		])
	else
		result_oggvorbis=fail
	fi
else
	result_oggvorbis=ok
fi
#####################################################################

fi
#####################################################################
# IF ladspa is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_ladspa = xyes
then
#####################################################################

#####################################################################
# Default test, to test ladspa
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_ladspa
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_ladspa
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_ladspa
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_ladspa
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with ladspa])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include <ladspa.h>
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index)
{ return 0; }
int main()
{
	ladspa_descriptor(0);
	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for ladspa failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of ladspa altogether, running configure
with the
  --without-ladspa
option.
		])
	else
		result_ladspa=fail
	fi
else
	result_ladspa=ok
fi
#####################################################################

fi
#####################################################################
# IF portmidi is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_portmidi = xyes
then
#####################################################################

#####################################################################
# Default test, to test portmidi
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_portmidi
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_portmidi
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_portmidi
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_portmidi
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with portmidi])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include<portmidi.h>
int main()
{
	Pm_CountDevices();
	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for portmidi failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of portmidi altogether, running configure
with the
  --without-portmidi
option.
		])
	else
		result_portmidi=fail
	fi
else
	result_portmidi=ok
fi
#####################################################################

fi
#####################################################################
# IF portaudio is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_portaudio = xyes
then
#####################################################################

#####################################################################
# Default test, to test portaudio
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_portaudio
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_portaudio
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_portaudio
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_portaudio
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with portaudio])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include<portaudio.h>
int main()
{
	Pa_CountDevices();
	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for portaudio failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of portaudio altogether, running configure
with the
  --without-portaudio
option.
		])
	else
		result_portaudio=fail
	fi
else
	result_portaudio=ok
fi
#####################################################################

fi
#####################################################################
# IF alsa is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_alsa = xyes
then
#####################################################################

#####################################################################
# Default test, to test alsa
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_alsa
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_alsa
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_alsa
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_alsa
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with alsa])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include<alsa/asoundlib.h>
int main()
{
	int card = 0;
	snd_card_next(&card);
	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for alsa failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of alsa altogether, running configure
with the
  --without-alsa
option.
		])
	else
		result_alsa=fail
	fi
else
	result_alsa=ok
fi
#####################################################################

fi
#####################################################################
# IF mad is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_mad = xyes
then
#####################################################################

#####################################################################
# Default test, to test mad
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_mad
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_mad
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_mad
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_mad
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with mad])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include<mad.h>
int main()
{
	struct mad_stream s;
	mad_stream_init(&s);
	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for mad failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of mad altogether, running configure
with the
  --without-mad
option.
		])
	else
		result_mad=fail
	fi
else
	result_mad=ok
fi
#####################################################################

fi
#####################################################################
# IF id3 is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_id3 = xyes
then
#####################################################################

#####################################################################
# Default test, to test id3
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_id3
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_id3
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_id3
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_id3
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with id3])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include <id3/tag.h>
int main()
{
	ID3_Tag myTag;
	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for id3 failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of id3 altogether, running configure
with the
  --without-id3
option.
		])
	else
		result_id3=fail
	fi
else
	result_id3=ok
fi
#####################################################################

fi
#####################################################################
# IF oscpack is enabled THEN
#
# source: acv/package_enabled_start.acv 
#####################################################################
if test x$enabled_oscpack = xyes
then
#####################################################################

#####################################################################
# Default test, to test oscpack
#
# source: acv/default_test.acv 
#####################################################################
failure=no
if test $found_oscpack
then
	old_CXXFLAGS=$CXXFLAGS
	old_LDFLAGS=$LDFLAGS
	for tmp in $include_dirs_oscpack
	  do
	  CXXFLAGS="$CXXFLAGS -I$tmp"
	done
	for tmp in $lib_dirs_oscpack
	  do
	  LDFLAGS="$LDFLAGS -L$tmp"
	done
	for tmp in $libs_oscpack
	  do
	  LDFLAGS="$LDFLAGS -l$tmp"
	done
	AC_MSG_CHECKING([compiling, linking and running with oscpack])
	AC_RUN_IFELSE(
	    [
		AC_LANG_SOURCE(
		    [[
			    #include <oscpack/ip/IpEndpointName.h>
#include <oscpack/ip/UdpSocket.h>
int main()
{
	UdpTransmitSocket socket( IpEndpointName("localhost", 9999) );
	bool b=socket.IsBound();
	return 0;
}

			    ]])	
		],[
		AC_MSG_RESULT(ok)
		],[
			AC_MSG_RESULT(failed)
			failure=yes
		]
	)
	CXXFLAGS=$old_CXXFLAGS
	LDFLAGS=$old_LDFLAGS
else
	failure=yes
fi
if test x$failure = xyes
then
	if test x$enabled_exit_on_failure = xyes
	then
		AC_MSG_FAILURE([
Configure-checks for oscpack failed. Exiting because you run with the
flag --enable-exit_on_failure.
Please check the configure output to see which checks failed, and the
config.log file for more details.
Note that you can disable the use of oscpack altogether, running configure
with the
  --without-oscpack
option.
		])
	else
		result_oscpack=fail
	fi
else
	result_oscpack=ok
fi
#####################################################################

fi
AC_MSG_RESULT(
[==================== Results of package tests ==================== 
Please see below for more information
==================================================================
[Package-name] [enabled] [check-result] [srcdeps variables]
------------------------------------------------------------------])
if test x$enabled_fftw = xno
then
  enabledstr='NO   '
else
  if test x$enabled_fftw = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_fftw = xfail
then
  resultstr='FAILED  '
else
  if test x$result_fftw = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_fftw = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_fftw = xyes; then
        HAS_FFTW=1
else
        HAS_FFTW=0
fi
tmp1=`echo 'fftw' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[fftw$tmp2 $enabledstr   $resultstr     HAS_FFTW=$HAS_FFTW , USE_FFTW])
if test x$enabled_sfftw = xno
then
  enabledstr='NO   '
else
  if test x$enabled_sfftw = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_sfftw = xfail
then
  resultstr='FAILED  '
else
  if test x$result_sfftw = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_sfftw = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_sfftw = xyes; then
        HAS_SFFTW=1
else
        HAS_SFFTW=0
fi
tmp1=`echo 'sfftw' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[sfftw$tmp2 $enabledstr   $resultstr     HAS_SFFTW=$HAS_SFFTW , USE_SFFTW])
if test x$enabled_xerces = xno
then
  enabledstr='NO   '
else
  if test x$enabled_xerces = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_xerces = xfail
then
  resultstr='FAILED  '
else
  if test x$result_xerces = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_xerces = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_xerces = xyes; then
        HAS_XERCES=1
else
        HAS_XERCES=0
fi
tmp1=`echo 'xerces' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[xerces$tmp2 $enabledstr   $resultstr     HAS_XERCES=$HAS_XERCES , USE_XERCES])
if test x$enabled_xmlpp = xno
then
  enabledstr='NO   '
else
  if test x$enabled_xmlpp = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_xmlpp = xfail
then
  resultstr='FAILED  '
else
  if test x$result_xmlpp = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_xmlpp = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_xmlpp = xyes; then
        HAS_XMLPP=1
else
        HAS_XMLPP=0
fi
tmp1=`echo 'xmlpp' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[xmlpp$tmp2 $enabledstr   $resultstr     HAS_XMLPP=$HAS_XMLPP , USE_XMLPP])
if test x$enabled_fltk = xno
then
  enabledstr='NO   '
else
  if test x$enabled_fltk = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_fltk = xfail
then
  resultstr='FAILED  '
else
  if test x$result_fltk = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_fltk = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_fltk = xyes; then
        HAS_FLTK=1
else
        HAS_FLTK=0
fi
tmp1=`echo 'fltk' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[fltk$tmp2 $enabledstr   $resultstr     HAS_FLTK=$HAS_FLTK , USE_FLTK])
if test x$enabled_qt = xno
then
  enabledstr='NO   '
else
  if test x$enabled_qt = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_qt = xfail
then
  resultstr='FAILED  '
else
  if test x$result_qt = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_qt = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_qt = xyes; then
        HAS_QT=1
else
        HAS_QT=0
fi
tmp1=`echo 'qt' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[qt$tmp2 $enabledstr   $resultstr     HAS_QT=$HAS_QT , USE_QT])
if test x$enabled_sndfile = xno
then
  enabledstr='NO   '
else
  if test x$enabled_sndfile = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_sndfile = xfail
then
  resultstr='FAILED  '
else
  if test x$result_sndfile = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_sndfile = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_sndfile = xyes; then
        HAS_SNDFILE=1
else
        HAS_SNDFILE=0
fi
tmp1=`echo 'sndfile' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[sndfile$tmp2 $enabledstr   $resultstr     HAS_SNDFILE=$HAS_SNDFILE , USE_SNDFILE])
if test x$enabled_oggvorbis = xno
then
  enabledstr='NO   '
else
  if test x$enabled_oggvorbis = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_oggvorbis = xfail
then
  resultstr='FAILED  '
else
  if test x$result_oggvorbis = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_oggvorbis = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_oggvorbis = xyes; then
        HAS_OGGVORBIS=1
else
        HAS_OGGVORBIS=0
fi
tmp1=`echo 'oggvorbis' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[oggvorbis$tmp2 $enabledstr   $resultstr     HAS_OGGVORBIS=$HAS_OGGVORBIS , USE_OGGVORBIS])
if test x$enabled_ladspa = xno
then
  enabledstr='NO   '
else
  if test x$enabled_ladspa = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_ladspa = xfail
then
  resultstr='FAILED  '
else
  if test x$result_ladspa = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_ladspa = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_ladspa = xyes; then
        HAS_LADSPA=1
else
        HAS_LADSPA=0
fi
tmp1=`echo 'ladspa' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[ladspa$tmp2 $enabledstr   $resultstr     HAS_LADSPA=$HAS_LADSPA , USE_LADSPA])
if test x$enabled_portmidi = xno
then
  enabledstr='NO   '
else
  if test x$enabled_portmidi = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_portmidi = xfail
then
  resultstr='FAILED  '
else
  if test x$result_portmidi = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_portmidi = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_portmidi = xyes; then
        HAS_PORTMIDI=1
else
        HAS_PORTMIDI=0
fi
tmp1=`echo 'portmidi' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[portmidi$tmp2 $enabledstr   $resultstr     HAS_PORTMIDI=$HAS_PORTMIDI , USE_PORTMIDI])
if test x$enabled_portaudio = xno
then
  enabledstr='NO   '
else
  if test x$enabled_portaudio = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_portaudio = xfail
then
  resultstr='FAILED  '
else
  if test x$result_portaudio = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_portaudio = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_portaudio = xyes; then
        HAS_PORTAUDIO=1
else
        HAS_PORTAUDIO=0
fi
tmp1=`echo 'portaudio' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[portaudio$tmp2 $enabledstr   $resultstr     HAS_PORTAUDIO=$HAS_PORTAUDIO , USE_PORTAUDIO])
if test x$enabled_alsa = xno
then
  enabledstr='NO   '
else
  if test x$enabled_alsa = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_alsa = xfail
then
  resultstr='FAILED  '
else
  if test x$result_alsa = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_alsa = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_alsa = xyes; then
        HAS_ALSA=1
else
        HAS_ALSA=0
fi
tmp1=`echo 'alsa' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[alsa$tmp2 $enabledstr   $resultstr     HAS_ALSA=$HAS_ALSA , USE_ALSA])
if test x$enabled_mad = xno
then
  enabledstr='NO   '
else
  if test x$enabled_mad = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_mad = xfail
then
  resultstr='FAILED  '
else
  if test x$result_mad = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_mad = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_mad = xyes; then
        HAS_MAD=1
else
        HAS_MAD=0
fi
tmp1=`echo 'mad' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[mad$tmp2 $enabledstr   $resultstr     HAS_MAD=$HAS_MAD , USE_MAD])
if test x$enabled_id3 = xno
then
  enabledstr='NO   '
else
  if test x$enabled_id3 = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_id3 = xfail
then
  resultstr='FAILED  '
else
  if test x$result_id3 = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_id3 = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_id3 = xyes; then
        HAS_ID3=1
else
        HAS_ID3=0
fi
tmp1=`echo 'id3' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[id3$tmp2 $enabledstr   $resultstr     HAS_ID3=$HAS_ID3 , USE_ID3])
if test x$enabled_oscpack = xno
then
  enabledstr='NO   '
else
  if test x$enabled_oscpack = xyes
  then
    enabledstr='YES  '
  else
    enabledstr='ERROR'
  fi
fi

if test x$result_oscpack = xfail
then
  resultstr='FAILED  '
else
  if test x$result_oscpack = xok
  then
    resultstr='SUCCESS '
  else
    if test x$enabled_oscpack = xno
    then
      resultstr='DISABLED'
    else
      resultstr='ERROR   '
    fi
  fi
fi
if test x$found_oscpack = xyes; then
        HAS_OSCPACK=1
else
        HAS_OSCPACK=0
fi
tmp1=`echo 'oscpack' | sed s/././g`
tmp2=`echo '            ' | sed s/$tmp1//`
AC_MSG_RESULT(
[oscpack$tmp2 $enabledstr   $resultstr     HAS_OSCPACK=$HAS_OSCPACK , USE_OSCPACK])
AC_MSG_RESULT(
[==================================================================
If you disabled checking for a specific package, or the test failed,
certain examples and tests may not compile. Typically running
'make depend' may give you an error that says:
 srcdeps: configuration specifies USE_[package]=1, but HAS_[package]=0
If a check failed, please read the configure output to see why it failed,
and the config.log file for more details. If you want configure to exit
immediately when a package test fails, use the --enable-exit_on_failure
flag
==================================================================])

# OBSOLETE
# OBSOLETE
# OBSOLETE
# OBSOLETE
# OBSOLETE
# OBSOLETE
# OBSOLETE
# OBSOLETE
# OBSOLETE
# OBSOLETE
# OBSOLETE
# OBSOLETE
# OBSOLETE
# OBSOLETE
# OBSOLETE
# substs for fftw
FFTW_INCLUDES=$include_dirs_fftw
FFTW_LIBS=$libs_fftw
FFTW_LIB_PATH=$lib_dirs_fftw
#echo FFTW_INCLUDES=$include_dirs_fftw
#echo FFTW_LIBS=$libs_fftw
#echo FFTW_LIB_PATH=$lib_dirs_fftw
if test x$found_fftw = xyes; then
	HAS_FFTW=1
else
	HAS_FFTW=0
fi
AC_SUBST(FFTW_INCLUDES)
AC_SUBST(FFTW_LIB_PATH)
AC_SUBST(FFTW_LIBS)
AC_SUBST(HAS_FFTW)
# substs for sfftw
SFFTW_INCLUDES=$include_dirs_sfftw
SFFTW_LIBS=$libs_sfftw
SFFTW_LIB_PATH=$lib_dirs_sfftw
#echo SFFTW_INCLUDES=$include_dirs_sfftw
#echo SFFTW_LIBS=$libs_sfftw
#echo SFFTW_LIB_PATH=$lib_dirs_sfftw
if test x$found_sfftw = xyes; then
	HAS_SFFTW=1
else
	HAS_SFFTW=0
fi
AC_SUBST(SFFTW_INCLUDES)
AC_SUBST(SFFTW_LIB_PATH)
AC_SUBST(SFFTW_LIBS)
AC_SUBST(HAS_SFFTW)
# substs for xerces
XERCES_INCLUDES=$include_dirs_xerces
XERCES_LIBS=$libs_xerces
XERCES_LIB_PATH=$lib_dirs_xerces
#echo XERCES_INCLUDES=$include_dirs_xerces
#echo XERCES_LIBS=$libs_xerces
#echo XERCES_LIB_PATH=$lib_dirs_xerces
if test x$found_xerces = xyes; then
	HAS_XERCES=1
else
	HAS_XERCES=0
fi
AC_SUBST(XERCES_INCLUDES)
AC_SUBST(XERCES_LIB_PATH)
AC_SUBST(XERCES_LIBS)
AC_SUBST(HAS_XERCES)
# substs for xmlpp
XMLPP_INCLUDES=$include_dirs_xmlpp
XMLPP_LIBS=$libs_xmlpp
XMLPP_LIB_PATH=$lib_dirs_xmlpp
#echo XMLPP_INCLUDES=$include_dirs_xmlpp
#echo XMLPP_LIBS=$libs_xmlpp
#echo XMLPP_LIB_PATH=$lib_dirs_xmlpp
if test x$found_xmlpp = xyes; then
	HAS_XMLPP=1
else
	HAS_XMLPP=0
fi
AC_SUBST(XMLPP_INCLUDES)
AC_SUBST(XMLPP_LIB_PATH)
AC_SUBST(XMLPP_LIBS)
AC_SUBST(HAS_XMLPP)
# substs for fltk
FLTK_INCLUDES=$include_dirs_fltk
FLTK_LIBS=$libs_fltk
FLTK_LIB_PATH=$lib_dirs_fltk
#echo FLTK_INCLUDES=$include_dirs_fltk
#echo FLTK_LIBS=$libs_fltk
#echo FLTK_LIB_PATH=$lib_dirs_fltk
if test x$found_fltk = xyes; then
	HAS_FLTK=1
else
	HAS_FLTK=0
fi
AC_SUBST(FLTK_INCLUDES)
AC_SUBST(FLTK_LIB_PATH)
AC_SUBST(FLTK_LIBS)
AC_SUBST(HAS_FLTK)
# substs for qt
QT_INCLUDES=$include_dirs_qt
QT_LIBS=$libs_qt
QT_LIB_PATH=$lib_dirs_qt
#echo QT_INCLUDES=$include_dirs_qt
#echo QT_LIBS=$libs_qt
#echo QT_LIB_PATH=$lib_dirs_qt
if test x$found_qt = xyes; then
	HAS_QT=1
else
	HAS_QT=0
fi
AC_SUBST(QT_INCLUDES)
AC_SUBST(QT_LIB_PATH)
AC_SUBST(QT_LIBS)
AC_SUBST(HAS_QT)
# substs for sndfile
SNDFILE_INCLUDES=$include_dirs_sndfile
SNDFILE_LIBS=$libs_sndfile
SNDFILE_LIB_PATH=$lib_dirs_sndfile
#echo SNDFILE_INCLUDES=$include_dirs_sndfile
#echo SNDFILE_LIBS=$libs_sndfile
#echo SNDFILE_LIB_PATH=$lib_dirs_sndfile
if test x$found_sndfile = xyes; then
	HAS_SNDFILE=1
else
	HAS_SNDFILE=0
fi
AC_SUBST(SNDFILE_INCLUDES)
AC_SUBST(SNDFILE_LIB_PATH)
AC_SUBST(SNDFILE_LIBS)
AC_SUBST(HAS_SNDFILE)
# substs for oggvorbis
OGGVORBIS_INCLUDES=$include_dirs_oggvorbis
OGGVORBIS_LIBS=$libs_oggvorbis
OGGVORBIS_LIB_PATH=$lib_dirs_oggvorbis
#echo OGGVORBIS_INCLUDES=$include_dirs_oggvorbis
#echo OGGVORBIS_LIBS=$libs_oggvorbis
#echo OGGVORBIS_LIB_PATH=$lib_dirs_oggvorbis
if test x$found_oggvorbis = xyes; then
	HAS_OGGVORBIS=1
else
	HAS_OGGVORBIS=0
fi
AC_SUBST(OGGVORBIS_INCLUDES)
AC_SUBST(OGGVORBIS_LIB_PATH)
AC_SUBST(OGGVORBIS_LIBS)
AC_SUBST(HAS_OGGVORBIS)
# substs for ladspa
LADSPA_INCLUDES=$include_dirs_ladspa
LADSPA_LIBS=$libs_ladspa
LADSPA_LIB_PATH=$lib_dirs_ladspa
#echo LADSPA_INCLUDES=$include_dirs_ladspa
#echo LADSPA_LIBS=$libs_ladspa
#echo LADSPA_LIB_PATH=$lib_dirs_ladspa
if test x$found_ladspa = xyes; then
	HAS_LADSPA=1
else
	HAS_LADSPA=0
fi
AC_SUBST(LADSPA_INCLUDES)
AC_SUBST(LADSPA_LIB_PATH)
AC_SUBST(LADSPA_LIBS)
AC_SUBST(HAS_LADSPA)
# substs for portmidi
PORTMIDI_INCLUDES=$include_dirs_portmidi
PORTMIDI_LIBS=$libs_portmidi
PORTMIDI_LIB_PATH=$lib_dirs_portmidi
#echo PORTMIDI_INCLUDES=$include_dirs_portmidi
#echo PORTMIDI_LIBS=$libs_portmidi
#echo PORTMIDI_LIB_PATH=$lib_dirs_portmidi
if test x$found_portmidi = xyes; then
	HAS_PORTMIDI=1
else
	HAS_PORTMIDI=0
fi
AC_SUBST(PORTMIDI_INCLUDES)
AC_SUBST(PORTMIDI_LIB_PATH)
AC_SUBST(PORTMIDI_LIBS)
AC_SUBST(HAS_PORTMIDI)
# substs for portaudio
PORTAUDIO_INCLUDES=$include_dirs_portaudio
PORTAUDIO_LIBS=$libs_portaudio
PORTAUDIO_LIB_PATH=$lib_dirs_portaudio
#echo PORTAUDIO_INCLUDES=$include_dirs_portaudio
#echo PORTAUDIO_LIBS=$libs_portaudio
#echo PORTAUDIO_LIB_PATH=$lib_dirs_portaudio
if test x$found_portaudio = xyes; then
	HAS_PORTAUDIO=1
else
	HAS_PORTAUDIO=0
fi
AC_SUBST(PORTAUDIO_INCLUDES)
AC_SUBST(PORTAUDIO_LIB_PATH)
AC_SUBST(PORTAUDIO_LIBS)
AC_SUBST(HAS_PORTAUDIO)
# substs for alsa
ALSA_INCLUDES=$include_dirs_alsa
ALSA_LIBS=$libs_alsa
ALSA_LIB_PATH=$lib_dirs_alsa
#echo ALSA_INCLUDES=$include_dirs_alsa
#echo ALSA_LIBS=$libs_alsa
#echo ALSA_LIB_PATH=$lib_dirs_alsa
if test x$found_alsa = xyes; then
	HAS_ALSA=1
else
	HAS_ALSA=0
fi
AC_SUBST(ALSA_INCLUDES)
AC_SUBST(ALSA_LIB_PATH)
AC_SUBST(ALSA_LIBS)
AC_SUBST(HAS_ALSA)
# substs for mad
MAD_INCLUDES=$include_dirs_mad
MAD_LIBS=$libs_mad
MAD_LIB_PATH=$lib_dirs_mad
#echo MAD_INCLUDES=$include_dirs_mad
#echo MAD_LIBS=$libs_mad
#echo MAD_LIB_PATH=$lib_dirs_mad
if test x$found_mad = xyes; then
	HAS_MAD=1
else
	HAS_MAD=0
fi
AC_SUBST(MAD_INCLUDES)
AC_SUBST(MAD_LIB_PATH)
AC_SUBST(MAD_LIBS)
AC_SUBST(HAS_MAD)
# substs for id3
ID3_INCLUDES=$include_dirs_id3
ID3_LIBS=$libs_id3
ID3_LIB_PATH=$lib_dirs_id3
#echo ID3_INCLUDES=$include_dirs_id3
#echo ID3_LIBS=$libs_id3
#echo ID3_LIB_PATH=$lib_dirs_id3
if test x$found_id3 = xyes; then
	HAS_ID3=1
else
	HAS_ID3=0
fi
AC_SUBST(ID3_INCLUDES)
AC_SUBST(ID3_LIB_PATH)
AC_SUBST(ID3_LIBS)
AC_SUBST(HAS_ID3)
# substs for oscpack
OSCPACK_INCLUDES=$include_dirs_oscpack
OSCPACK_LIBS=$libs_oscpack
OSCPACK_LIB_PATH=$lib_dirs_oscpack
#echo OSCPACK_INCLUDES=$include_dirs_oscpack
#echo OSCPACK_LIBS=$libs_oscpack
#echo OSCPACK_LIB_PATH=$lib_dirs_oscpack
if test x$found_oscpack = xyes; then
	HAS_OSCPACK=1
else
	HAS_OSCPACK=0
fi
AC_SUBST(OSCPACK_INCLUDES)
AC_SUBST(OSCPACK_LIB_PATH)
AC_SUBST(OSCPACK_LIBS)
AC_SUBST(HAS_OSCPACK)
GL_INCLUDES=
GL_LIB_PATH=
GL_LIBS=GL
HAS_GL=1

AC_MSG_NOTICE([TODO: OpenGL tests - now assuming it is present])
AC_SUBST(GL_INCLUDES)
AC_SUBST(GL_LIB_PATH)
AC_SUBST(GL_LIBS)
AC_SUBST(HAS_GL)

AC_OUTPUT(system-posix.cfg)
AC_OUTPUT(packages-posix.cfg)
