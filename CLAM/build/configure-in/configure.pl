sub parse_acv_file
{
	local $filename = shift;
	open INFILE,$filename or die "opening $filename";
	$line = 0;

	while (<INFILE>)
	{
		$line++;
		foreach $pat (
			'\$\$\w+\[\$\$\w+\]',
			'\$\$\w+\{\$\$\w+\}',
			'\$\$\w+'
		)
		{
			while (m/($pat)/)
			{
				my $var = $1;
				$var =~ s/\$\$/\$/g;
				my $val = eval($var);
				if (!defined $val)
				{
					my $tmp = $var;
					my $extra = "";
					while ($tmp =~ m/(\$\w+)/)
					{
						my $hvar = $1;
						my $hval = eval($hvar);
						$extra = $extra."- $hvar = $hval\n";
						$tmp =~ s/\$\w+//;
					}
					die "$filename:$line: error: variable $var not defined\n$extra";
				}
#				print "[$var=$val]\n";
				s/$pat/$val/;
			}
		}
		print OUTFILE $_;
	}	
}

sub ac
{
	my $func = shift;
	my $sep = "";
	my $arg;
	print OUTFILE "AC_$func(";
	while ($_ = shift)
	{
		print OUTFILE $sep,'[',$_,']';
		$sep = ",";
	}
	print OUTFILE ")\n\n";
}

sub ac_check_feature_enabled
{
	local $feature = shift;
	local $feature_esc = $feature;
	$feature_esc =~ s/-/_/g;
	local $default = shift;

	
	if ($default eq "") { $default = "yes"; }

	&parse_acv_file("acv/check_feature_enable.acv");
}

sub ac_check_package_enabled
{
	local $package = shift;
	local $package_esc = $package;
	$package_esc =~ s/-/_/g;
	local $default = shift;

	
	if ($default eq "") { $default = "yes"; }

	&parse_acv_file("acv/check_package_enable.acv");
}

sub ac_package_enabled_start
{
	local $package = shift;
	local $package_esc = $package;
	$package_esc =~ s/-/_/g;
	&parse_acv_file("acv/package_enabled_start.acv");
}

sub ac_package_enabled_end
{
	local $package = shift;
	local $package_esc = $package;
	$package_esc =~ s/-/_/g;
	&parse_acv_file("acv/package_enabled_end.acv");
}

sub ac_disabled_package_warning
{
	local $package = shift;
	local $package_esc = $package;
	local $uc_package = uc($package);
	$package_esc =~ s/-/_/g;

	&parse_acv_file("acv/disabled_package_warning.acv");
}

sub ac_sandbox_location
{
	if ($checked_sandbox_location) { return; }
	$checked_sandbox_location = 1;

	&ac_check_feature_enabled('sandbox');
	&parse_acv_file("acv/sandbox_location.acv");
}

sub ac_cplusplus_tests
{
	&parse_acv_file("acv/cplusplus_tests.acv");
}

sub ac_sandbox_find
{
	local $package = shift;
	local $package_esc = $package;
	$package_esc =~ s/-/_/g;

	ac_sandbox_location();

	if ($headers{$package} eq "")
	{
		die "configure.pl: no headers for $package specified\n";
	}
	&parse_acv_file("acv/package_in_sandbox.acv");
}

sub ac_standard_find
{
	local $package = shift;
	local $package_esc = $package;
	$package_esc =~ s/-/_/g;

	if (! defined $headers{$package})
	{
		die "configure.pl: no headers for $package specified\n";
	}
	if (! defined $libs{$package})
	{
		die "configure.pl: no libs for $package specified\n";
	}
	&parse_acv_file("acv/standard_find.acv");
}

sub ac_standard_qt_find
{
	local $package = shift;
	local $package_esc = $package;
	$package_esc =~ s/-/_/g;

	if (! defined $headers{$package})
	{
		die "configure.pl: no headers for $package specified\n";
	}
	if (! defined $libs{$package})
	{
		die "configure.pl: no libs for $package specified\n";
	}
	&parse_acv_file("acv/standard_qt_find.acv");
}

sub ac_pkg_config_availability
{
	if ($checked_pkg_config_availability)
	{
		return;
	}
	
	$checked_pkg_config_availability = 1;

	&ac_check_feature_enabled('pkg-config');

	&parse_acv_file("acv/pkg_config_availability.acv");
}

sub ac_pkg_config_find
{
	&ac_pkg_config_availability();

	local $package = shift;
	local $package_esc = $package;
	$package_esc =~ s/-/_/g;

	&parse_acv_file("acv/pkg_config_find.acv");

}

sub ac_default_find
{
	local $package = shift;
	local $package_esc = $package;
	$package_esc =~ s/-/_/g;

	&ac_check_package_enabled($package,'yes');
	&ac_package_enabled_start($package);
	&ac_sandbox_find($package);
	&ac_pkg_config_find($package);
	&ac_standard_find($package);
	&ac_package_enabled_end($package);
}

sub ac_qt_find
{
	local $package = shift;
	local $package_esc = $package;
	$package_esc =~ s/-/_/g;

	&ac_check_package_enabled($package,'yes');
	&ac_package_enabled_start($package);
	&ac_standard_qt_find($package);
	&ac_package_enabled_end($package);
}

sub ac_default_test
{
	local $package = shift;
	local $package_esc = $package;
	$package_esc =~ s/-/_/g;

	&ac_package_enabled_start($package);
	&parse_acv_file("acv/default_test.acv");
	&ac_package_enabled_end($package);
}

sub ac_get_cpp_preprocessor_variable
{
	local $header = shift;
	local $variable = shift;
	
	&parse_acv_file("acv/get_cpp_preprocessor_variable.acv");
}

sub ac_sed
{
	&parse_acv_file("acv/sed_presence.acv");
}

sub ac_introspect
{
	my $package = shift;
	my $package_esc = $package;
	$package_esc =~ s/-/_/g;

	print OUTFILE<<EOF;
AC_MSG_NOTICE([found_$package_esc = \$found_$package_esc])
AC_MSG_NOTICE([include_dirs_$package_esc = \$include_dirs_$package_esc])
AC_MSG_NOTICE([lib_dirs_$package_esc = \$lib_dirs_$package_esc])
AC_MSG_NOTICE([libs_$package_esc = \$libs_$package_esc])

EOF
}

sub ac_dllextension
{
	&parse_acv_file("acv/dllextension.acv");
}

sub ac_package_substs
{
	local $package = shift;
	local $package_esc = $package;
	$package_esc =~ s/-/_/g;
	local $uc_package_esc = uc($package_esc);		
	local $uc_package_esc_INCLUDES = $uc_package_esc."_INCLUDES";
	local $uc_package_esc_LIBS = $uc_package_esc."_LIBS";
	local $uc_package_esc_LIB_PATH = $uc_package_esc."_LIB_PATH";

	&parse_acv_file("acv/package_substs.acv");
}

# important: qt should come before qwt
@packagedlibs = ('fftw','sfftw','xerces','fltk','qt','qwt','sndfile','oggvorbis','ladspa','portmidi','portaudio','alsa','mad','id3', 'oscpack');

if ($ARGV[0] eq '-u')
{
open OUTFILE,">$ARGV[1]" or die "Failed to open $ARGV[1] for writing\n";
foreach $f (@packagedlibs)
{
	my $package = $f;
	my $package_esc = $package;
	$package_esc =~ s/-/_/g;
	my $uc_package_esc = uc($package_esc);		
	print OUTFILE "USE_$uc_package_esc=0\n";
}
close OUTFILE;
exit(0);
}

#if ($ARGV[0] eq '-p')
#{
#open OUTFILE,">$ARGV[1]";
#foreach $f (@packagedlibs)
#{
#	my $package = $f;
#	my $package_esc = $package;
#	$package_esc =~ s/-/_/g;
#	my $uc_package_esc = uc($package_esc);		
#	print OUTFILE "HAS_$uc_package_esc=\@HAS_$uc_package_esc\@\n";
#}
#close OUTFILE;
#exit(0);
#}

if ($ARGV[0] eq '')
{
	die "Missing outfile\n";
}

open OUTFILE,">$ARGV[0]" or die "Failed to open $ARGV[0] for writing\n";

&ac('INIT','test','0.1','clam@iua.upf.es');
&ac('PREREQ','2.50');


&parse_acv_file("acv/portable_setenv.acv");

&ac('PROG_CC');
&ac('PROG_CPP');
&ac('PROG_CXX');
&ac('PROG_CXXCPP');

ac_get_cpp_preprocessor_variable(
	'xercesc/util/XercesVersion.hpp',
	'XERCES_CPP_NAMESPACE');

$sandbox{'qt'} = 'qt';
$headers{'qt'} = 'qapplication.h';
$libs{'qt'} = 'qt-mt';
$alt_libs{'qt'} = 'qt';
$ext_libs{'qt'} = '';
$source{'qt'} = <<EOF;
#include<qapplication.h>
int main()
{
	int argc = 2;
	char *argv[]={"Just","testing"};
	QApplication app(argc,argv);
	return 0;	
}
EOF

$sandbox{'qwt'} = 'qwt';
$headers{'qwt'} = 'qwt/qwt_math.h';
$libs{'qwt'} = 'qwt';
$alt_libs{'qwt'} = '';
$ext_libs{'qwt'} = '';
$source{'qwt'} = <<EOF;
#include<qwt/qwt_math.h>
int main()
{
	if (qwtCeil125(1.1)==2.0)
	{
		return 0;
	}
	return -1;
}
EOF

$sandbox{'fftw'} = 'fftw';
$headers{'fftw'} = 'fftw.h';
$libs{'fftw'} = 'fftw rfftw';
$alt_libs{'fftw'} = 'dfftw drfftw';
$ext_libs{'fftw'} = '';
$source{'fftw'} = <<EOF;
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
EOF

$sandbox{'sfftw'} = 'fftw';
$headers{'sfftw'} = 'fftw.h';
$libs{'sfftw'} = 'sfftw srfftw';
$alt_libs{'sfftw'} = '';
$ext_libs{'sfftw'} = '';
$source{'sfftw'} = <<EOF;
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
EOF

$sandbox{'mad'} = 'libmad';
$headers{'mad'} = 'mad.h';
$libs{'mad'} = 'mad';
$alt_libs{'mad'} = '';
$ext_libs{'mad'} = 'm';
$source{'mad'} = <<EOF;
#include<mad.h>
int main()
{
	struct mad_stream s;
	mad_stream_init(&s);
	return 0;
}
EOF

$sandbox{'id3'} = 'id3lib';
$headers{'id3'} = 'id3/tag.h';
$libs{'id3'} = 'id3';
$alt_libs{'id3'} = '';
$ext_libs{'id3'} = 'z';
$source{'id3'} = <<EOF;
#include <id3/tag.h>
int main()
{
	ID3_Tag myTag;
	return 0;
}
EOF

$sandbox{'xerces'} = 'xercesc';
$headers{'xerces'} = 'xercesc/util/PlatformUtils.hpp';
$libs{'xerces'} = 'xerces-c';
$alt_libs{'xerces'} = '';
$ext_libs{'xerces'} = '';
$source{'xerces'} = <<EOF;
#include<xercesc/util/PlatformUtils.hpp>
int main()
{
	namespace xercesc=XERCES_CPP_NAMESPACE;
	xercesc::XMLPlatformUtils::Initialize();
	return 0;
}
EOF

$sandbox{'fltk'} = 'fltk';
$headers{'fltk'} = 'FL/gl.h';
$libs{'fltk'} = 'fltk fltk_images fltk_forms fltk_gl';
$ext_libs{'fltk'} = 'GL';
$alt_libs{'fltk'} = '';
$source{'fltk'} = <<EOF;
#include<FL/Fl_Gl_Window.H>
#include<FL/Fl_PNG_Image.H>
#include<FL/fl_draw.H>
int main()
{
	Fl_Gl_Window w(0,0);
	Fl_PNG_Image png("");
	return 0;
}
EOF

$sandbox{'portmidi'} = 'portmidi';
$headers{'portmidi'} = 'portmidi.h';
$libs{'portmidi'} = 'porttime portmidi';
$alt_libs{'portmidi'} = 'portmidi';
$ext_libs{'portmidi'} = '';
$source{'portmidi'} = <<EOF;
#include<portmidi.h>
int main()
{
	Pm_CountDevices();
	return 0;
}
EOF

$sandbox{'portaudio'} = 'portaudio';
$headers{'portaudio'} = 'portaudio.h';
$libs{'portaudio'} = 'portaudio';
$alt_libs{'portaudio'} = '';
$ext_libs{'portaudio'} = '';
$source{'portaudio'} = <<EOF;
#include<portaudio.h>
int main()
{
	Pa_CountDevices();
	return 0;
}
EOF

$sandbox{'alsa'} = 'alsa';
$headers{'alsa'} = 'alsa/asoundlib.h';
$libs{'alsa'} = 'asound';
$alt_libs{'alsa'} = '';
$ext_libs{'alsa'} = '';
$source{'alsa'} = <<EOF;
#include<alsa/asoundlib.h>
int main()
{
	int card = 0;
	snd_card_next(&card);
	return 0;
}
EOF

$sandbox{'ladspa'} = 'ladspa';
$headers{'ladspa'} = 'ladspa.h';
$libs{'ladspa'} = '';
$alt_libs{'ladspa'} = '';
$ext_libs{'ladspa'} = '';
$source{'ladspa'} = <<EOF;
#include <ladspa.h>
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index)
{ return 0; }
int main()
{
	ladspa_descriptor(0);
	return 0;
}
EOF

$sandbox{'sndfile'} = 'libsndfile';
$headers{'sndfile'} = 'sndfile.h';
$libs{'sndfile'} = 'sndfile';
$alt_libs{'sndfile'} = '';
$ext_libs{'sndfile'} = '';
$source{'sndfile'} = <<EOF;
#include <sndfile.h>
#include <stdio.h>
int main()
{
	SF_INFO sfinfo;
	sf_open_fd(fileno(stdout),SFM_WRITE,&sfinfo,0);
	return 0;
}
EOF

$sandbox{'oggvorbis'} = 'oggvorbis';
$headers{'oggvorbis'} = 'vorbis/vorbisfile.h ogg/ogg.h';
$libs{'oggvorbis'} = 'vorbisenc vorbisfile ogg vorbis';
$alt_libs{'oggvorbis'} = '';
$ext_libs{'oggvorbis'} = '';
$source{'oggvorbis'} = <<EOF;
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
EOF


$sandbox{'oscpack'} = 'oscpack';
$headers{'oscpack'} = 'oscpack/ip/NetworkingUtils.h';
$libs{'oscpack'} = 'oscpack';
$alt_libs{'oscpack'} = '';
$ext_libs{'oscpack'} = 'pthread';
$source{'oscpack'} = <<EOF;
#include <oscpack/ip/NetworkingUtils.h>
int main()
{
	InitializeNetworking();
	TerminateNetworking();
	return 0;
}
EOF


&ac_sed();

&ac_cplusplus_tests();
&ac_dllextension();

&ac_sandbox_location();
&ac_pkg_config_availability();

&ac('LANG_CPLUSPLUS');

foreach $f (@packagedlibs)
{ 
	if ($f eq 'qt')
	{
		&ac_qt_find($f);	
	}elsif ($f eq 'qwt')
	{
		&parse_acv_file("acv/add_qt_include_path.acv");
		&ac_default_find($f);
		&parse_acv_file("acv/add_qt_include_path_done.acv");
	}else{
		&ac_default_find($f);
	}
}
foreach $f (@packagedlibs) { 
	if ($f eq 'qwt')
	{
		&parse_acv_file("acv/add_qt_include_path.acv");
		&ac_default_test($f);
		&parse_acv_file("acv/add_qt_include_path_done.acv");
	}else{
		&ac_default_test($f);
	}
}
#foreach $f (@packagedlibs) { &ac_introspect($f); }
foreach $f (@packagedlibs) { &ac_disabled_package_warning($f); }
foreach $f (@packagedlibs) { &ac_package_substs($f); }

&parse_acv_file("acv/opengl.acv");

print OUTFILE<<EOF;
AC_OUTPUT(system-posix.cfg)
AC_OUTPUT(packages-posix.cfg)
EOF
