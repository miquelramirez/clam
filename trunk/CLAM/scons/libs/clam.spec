# Copyright (c) 2007 oc2pus
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments to toni@links2linux.de

# Packmangroup: Multimedia
# Packmanpackagename: clam
# Packman: Toni Graffy
# Packmandepends: alsa jack ladspa libsndfile mad

# norootforbuild

%define svn_revision ~svn10065
%define CLAM_version			1.1.1%{svn_revision}
%define NetworkEditor_version	1.1.1%{svn_revision}
%define SMSTools_version		0.4.8%{svn_revision}
%define Annotator_version		0.3.9%{svn_revision}
# /usr/lib/qt3 in SuSe
%define Qt3dir                  /usr/lib/qt-3.3
# /usr/lib/qt4 in FC<6
%define Qt4dir                  /usr

Summary:			CLAM - C++ Library for Audio and Music
Name:				clam
Version:			%{CLAM_version}
Release:			1
License:			GPL
URL:				http://clam.iua.upf.edu
Group:				System/Libraries
Source0:			CLAM-%{CLAM_version}.tar.gz
Source1:			NetworkEditor-%{NetworkEditor_version}.tar.gz
Source2:			SMSTools-%{SMSTools_version}.tar.gz
Source3:			Annotator-%{Annotator_version}.tar.gz
#Patch0:			%{name}-networkeditor-SConstruct.diff
#Patch1:			%{name}-scons-libs-modconf-audioio.py.diff
BuildRoot:		%{_tmppath}/%{name}-%{version}-%{release}-buildroot
#BuildRequires:	alsa-devel # SuSe
BuildRequires:	alsa-lib-devel
BuildRequires:	doxygen
BuildRequires:	fftw-devel
BuildRequires:	fftw2-devel
BuildRequires:	gcc-c++
#BuildRequires:	ghostscript-library # SuSe
BuildRequires:	graphviz
BuildRequires:	id3lib-devel
#BuildRequires:	jack-devel # SuSe
BuildRequires:	jack-audio-connection-kit-devel
BuildRequires:	libjpeg-devel
BuildRequires:	libogg-devel
BuildRequires:	libsndfile-devel
BuildRequires:	libvorbis-devel
#BuildRequires:	mad-devel # SuSe
BuildRequires:	libmad-devel
#BuildRequires:	Mesa-devel # SuSe
BuildRequires:	pkgconfig
#BuildRequires:	portaudio-devel
#BuildRequires:	qt3-devel # SuSe
BuildRequires:	qt-devel
#BuildRequires:	update-desktop-files
#BuildRequires:	te_latex
#BuildRequires:	Xerces-c-devel # SuSe
BuildRequires:	xerces-c-devel
BuildRequires:	scons
BuildRequires:  ladspa-devel
Requires:			alsa
Requires:			fftw
Requires:			fftw2
Requires:			jack
Requires:			mad
#Requires:			portaudio
#Requires:			Xerces-c

%description
CLAM stands for C++ Library for Audio and Music and in Catalan means something
like a "continuous sound produced by a large number of people as to show
approval or disapproval of a given event" It is the best name we could find
after long discussions and it is certainly much better than its original name
(MTG-Classes).

CLAM is a framework to develop audio and music applications in C++.

It provides tools to perform advanced analysis, transformations and synthesis,
visualization for audio and music related objects, and other tools that are
useful to abstract target platform for most tasks in an audio applications
such as audio devices, file formats, threading...

##########################################
##########################################
#debug_package

##########################################
##########################################
%package devel
Summary:	CLAM - C++ Library for Audio and Music - development files
Group:		Development/Libraries/C and C++
Version:	%{CLAM_version}
Requires:	%{name} = %{CLAM_version}
Requires:	fftw-devel
Requires:	gcc-c++
Requires:	id3lib-devel
Requires:	jack-devel
Requires:	ladspa-devel
Requires:	libogg-devel
Requires:	libsndfile-devel
Requires:	libvorbis-devel
Requires:	mad-devel
Requires:	pkgconfig
#Requires:	portaudio-devel

%description devel
CLAM is a framework to develop audio and music applications in C++.
It provides tools to perform advanced analysis, transformations and synthesis,
visualization for audio and music related objects, and other tools that are
useful to abstract target platform for most tasks in an audio applications
such as audio devices, file formats, threading...

This package contains the files needed to develop with the following libraries:
clam-core, clam-processing, clam-audioio and clam-vmqt.

##########################################
##########################################
%package doc
Summary:	Contains the clam-framework documentation and some example programs
Group:		Documentation/Other
Version:	%{CLAM_version}

%description doc
This package contains the framework documentation and some example programs.

##########################################
##########################################
%package networkeditor
Summary:	The Network Editor is the application for editing CLAM networks
Group:		Productivity/Multimedia/Sound/Utilites
Version:	%{NetworkEditor_version}
Requires:	%{name} = %{CLAM_version}

%description networkeditor
CLAM Network Editor, prototyping tool for CLAM.

The CLAM Network Editor is a tool for editing CLAM processing networks.
Those processing networks can become the processing core of an audio
application by using the CLAM::NetworkPlayer class in such program, or by
using the CLAM Prototyper to link the network to a Qt Designer interface.

This packages provides both the Network Editor and the Prototyper.
It also provides a plugin for Qt Designer which adds widgets to display
several kinds of CLAM audio objects from a running network.

##########################################
##########################################
%package networkeditor-examples
Summary:	Examples for the CLAM Network Editor
Group:		Productivity/Multimedia/Sound/Utilites
Version:	%{NetworkEditor_version}
Requires:	%{name} = %{CLAM_version}

%description networkeditor-examples
This package provides examples of networks, some of them with their user
interface to be executed also by the Prototyper.

Those examples include, among others, some sinusoidal plus residual
modelling (SMS) transformations and tonal (chords, notes) analysis.

##########################################
##########################################
%package smstools
Summary:	CLAM SMSTools, SMS based audio authoring tool
Group:		Productivity/Multimedia/Sound/Utilites
Version:	%{SMSTools_version}
Requires:	%{name} = %{CLAM_version}
Requires:	qt3-devel
Requires:	qt-devel

%description smstools
CLAM SMSTools, SMS based audio authoring tool.

SmsTools is an audio authoring application which allows to analyze the audio
using Spectral Model Synthesis (SMS) and perform pipelined transformations on
such representation.

SMS representation makes easy to perform quality effects such as gender change,
pitch shift, time streetch, morphing...

##########################################
##########################################
%package smstools-examples
Summary:	Examples for CLAM SMSTools
Group:		Productivity/Multimedia/Sound/Utilites
Version:	%{SMSTools_version}
Requires:	%{name} = %{CLAM_version}

%description smstools-examples
This package provides examples of smstools.

##########################################
##########################################
%package annotator
Summary:	The CLAM Annotator - A tool that can be used to visualize, check and modify music information extracted from audio
Group:		Productivity/Multimedia/Sound/Utilites
Version:	%{Annotator_version}
Requires:	%{name} = %{CLAM_version}

%description annotator
The CLAM Annotator is a tool that can be used to visualize, check and 
modify music information extracted from audio: low level features, note
segmentation, chords, structure... The tool is intended to be useful for
the music information retrieval research whenever you need to:

* Supervise and correct the results of automated audio feature extraction
algorithms.

* Generate manually edited annotations of audio as training examples or ground
truth for those algorithms.

The CLAM Annotator is not tied to a fixed description scheme (the set of 
attributes). You can define the working attribute set by providing an XML 
description schema definition. A sample description scheme and an extractor
program are provided with the Annotator but you could implement your own
one with different algorithms and attributes. Also tools are provided to 
select and join attributes from the output of several description sources.

Other key features are:
* Attribute types:
Free text, Enumeration, Bounded float, Bounded integer, Frame Division,
Segmentations (Sizeless, Gapless, Overlapless or Free), ...
* Several time segmentations can be defined at once
* Each segment can have its own attributes
* Auralization for the segmentations (click on the beggining)
* Auralization for the low-level descriptor (magnitude to freq)

##########################################
##########################################
%package annotator-examples
Summary:	The CLAM Annotator - examples
Group:		Productivity/Multimedia/Sound/Utilites
Version:	%{Annotator_version}
Requires:	%{name} = %{CLAM_version}

%description annotator-examples
This package contains example projects for CLAM Annotator to do.

##########################################
##########################################
%prep
%setup -q -n CLAM-%{CLAM_version} -a1 -a2 -a3

%build
%__install -dm 755 %{buildroot}%{_prefix}

# Setup for parallel builds
numprocs=`egrep -c ^cpu[0-9]+ /proc/stat || :`
if [ "$numprocs" = "0" ]; then
	numprocs=1
fi
# needs further investigation :(
numprocs=1

#################
# build core-libs
#################

scons -j$numprocs configure \
	prefix=%{buildroot}%{_prefix} \
	release=1 \
	double=0 \
	xmlbackend=xercesc \
	with_ladspa=1 \
	with_jack=1 \
	with_fftw=1 \
	with_fftw3=1 \
	with_nr_fft=1 \
	with_sndfile=1 \
	with_oggvorbis=1 \
	with_mad=1 \
	with_id3=1 \
	with_alsa=1 \
	with_portaudio=1 \
	with_portmidi=1 \

scons -j$numprocs \
	prefix=%{buildroot}%{_prefix}

scons -j$numprocs \
	install \
	prefix=%{buildroot}%{_prefix}


# and now doxygenate CLAM stuff
doxygen ./doxygen.cfg

#####################
# build NetworkEditor
#####################
cd NetworkEditor-%{NetworkEditor_version}

export QTDIR=%{Qt4dir}
scons -j$numprocs \
	prefix=%{buildroot}%{_prefix} \
	clam_prefix=%{buildroot}%{_prefix} \
	clam_sconstools=%{buildroot}%{_datadir}/%{name}/sconstools \
	qt_plugins_install_path=/lib/qt4/plugins/designer \
	release=1
cd ..

################
# build SMSTools
################
cd SMSTools-%{SMSTools_version}
export QTDIR=%{Qt3dir}
scons -j$numprocs \
	prefix=%{buildroot}%{_prefix} \
	clam_prefix=%{buildroot}%{_prefix} \
	clam_sconstools=%{buildroot}%{_datadir}/%{name}/sconstools \
	release=1
cd ..

################
# build Annotator
################
cd Annotator-%{Annotator_version}
export QTDIR=%{Qt4dir}
scons -C./vmqt \
	prefix=%{buildroot}%{_prefix} \
	clam_prefix=%{buildroot}%{_prefix} \
	clam_sconstools=%{buildroot}%{_datadir}/%{name}/sconstools \
	release=1
scons -j$numprocs \
	prefix=%{buildroot}%{_prefix} \
	clam_prefix=%{buildroot}%{_prefix} \
	clam_sconstools=%{buildroot}%{_datadir}/%{name}/sconstools \
	release=1
cd ..

%install
%__install -dm 755 %{buildroot}%{_datadir}/pixmaps

# Setup for parallel builds
numprocs=`egrep -c ^cpu[0-9]+ /proc/stat || :`
if [ "$numprocs" = "0" ]; then
	numprocs=1
fi

################
# install NetworkEditor
################
cd NetworkEditor-%{NetworkEditor_version}
export QTDIR=%{Qt4dir}
scons -j$numprocs \
	install \
	prefix=%{buildroot}%{_prefix}

# icon
%__install -m 644 resources/NetworkEditor-icon.png \
	%{buildroot}%{_datadir}/pixmaps/%{name}-networkeditor.png
cd ..

##################
# install SMSTools
##################
cd SMSTools-%{SMSTools_version}
export QTDIR=%{Qt3dir}
scons -j$numprocs \
	install \
	prefix=%{buildroot}%{_prefix}

# icon
%__install -m 644 resources/SMSTools-icon.png \
	%{buildroot}%{_datadir}/pixmaps/%{name}-smstools.png
cd ..

###################
# install Annotator
###################
cd Annotator-%{Annotator_version}
export QTDIR=%{Qt4dir}
scons -j$numprocs \
	install \
	prefix=%{buildroot}%{_prefix}

# icon
%__install -m 644 src/images/annotator-logo.png \
	%{buildroot}%{_datadir}/pixmaps/%{name}-annotator.png
cd ..

##########################
# adjust path for examples
##########################
for i in annotator networkeditor smstools; do
	%{buildroot}%{_datadir}/%{name}/sconstools/changeExampleDataPath.py \
		%{buildroot}%{_datadir}/$1 \
		%{_datadir}/$1
done

##############
# menu-entries
##############
#for i in Annotator NetworkEditor Prototyper SMSTools;do
#	%__sed -i -e 's|/usr/share/pixmaps/||g' \
#		%{buildroot}%{_datadir}/applications/$i.desktop
#	%__sed -i -e 's|.xpm|.png|g' \
#		%{buildroot}%{_datadir}/applications/$i.desktop
#	%__sed -i -e 's|Application;||g' \
#		%{buildroot}%{_datadir}/applications/$i.desktop
#	%__sed -i -e 's|Audio;||g' \
#		%{buildroot}%{_datadir}/applications/$i.desktop
#done

#%suse_update_desktop_file NetworkEditor AudioVideo Music
#%suse_update_desktop_file Prototyper AudioVideo Music
#%suse_update_desktop_file SMSTools AudioVideo Music
#%suse_update_desktop_file Annotator AudioVideo Music

%clean
[ -d %{buildroot} -a "%{buildroot}" != "" ] && %__rm -rf %{buildroot}

%files
%defattr(-,root,root)
%doc CHANGES INSTALL
%{_libdir}/libclam_*.so.*

%files devel
%defattr(-,root,root)
%doc CHANGES INSTALL
%dir %{_includedir}/CLAM
%{_includedir}/CLAM/*
%{_libdir}/libclam_*.so
%{_libdir}/pkgconfig/*
%{_datadir}/%{name}/sconstools/*.py*

%files doc
%defattr(-,root,root)
%doc CHANGES INSTALL
%doc doxygen/*

%files networkeditor
%defattr(-,root,root)
%doc NetworkEditor-%{NetworkEditor_version}/CHANGES
%doc NetworkEditor-%{NetworkEditor_version}/COPYING
%doc NetworkEditor-%{NetworkEditor_version}/INSTALL
%doc NetworkEditor-%{NetworkEditor_version}/README
%{_bindir}/NetworkEditor
%{_bindir}/OfflinePlayer
%{_bindir}/Prototyper
%{_libdir}/qt4/plugins/designer/*
%{_mandir}/man1/NetworkEditor*
%{_mandir}/man1/Prototyper*
%dir %{_datadir}/networkeditor
%{_datadir}/networkeditor/NetworkEditor-icon.svg
%dir %{_datadir}/networkeditor/i18n
%{_datadir}/networkeditor/i18n/*.qm
%{_datadir}/mime/packages/clam-network.xml
%{_datadir}/mimelnk/application/vnd.clam.network.desktop
%{_datadir}/applications/NetworkEditor.desktop
%{_datadir}/applications/Prototyper.desktop
%{_datadir}/pixmaps/clam-networkeditor.png

%files networkeditor-examples
%defattr(-,root,root)
%dir %{_datadir}/networkeditor
%dir %{_datadir}/networkeditor/example-data
%{_datadir}/networkeditor/example-data/*

%files smstools
%defattr(-,root,root)
%doc SMSTools-%{SMSTools_version}/CHANGES
%doc SMSTools-%{SMSTools_version}/COPYING
%doc SMSTools-%{SMSTools_version}/INSTALL
%{_bindir}/SMSTools
%{_bindir}/SMSBatch
%{_bindir}/SMSConsole
%{_mandir}/man1/SMS*
%{_datadir}/applications/SMSTools.desktop
%{_datadir}/pixmaps/clam-smstools.png

%files smstools-examples
%defattr(-,root,root)
%dir %{_datadir}/smstools
%dir %{_datadir}/smstools/example-data
%{_datadir}/smstools/example-data/*

%files annotator
%defattr(-,root,root)
%doc Annotator-%{Annotator_version}/CHANGES
%doc Annotator-%{Annotator_version}/COPYING
%doc Annotator-%{Annotator_version}/INSTALL
%doc Annotator-%{Annotator_version}/README
%{_bindir}/Annotator
%{_bindir}/BocaClient
%{_bindir}/ChordExtractor
%{_bindir}/ClamExtractorExample
%dir %{_datadir}/annotator
%dir %{_datadir}/annotator/i18n
%{_datadir}/annotator/i18n/*.qm
%{_mandir}/man1/Annotator*
%{_mandir}/man1/ClamExtractorExample*
%{_datadir}/applications/Annotator.desktop
%{_datadir}/pixmaps/clam-annotator.png

%files annotator-examples
%defattr(-,root,root)
%dir %{_datadir}/annotator
%dir %{_datadir}/annotator/example-data
%{_datadir}/annotator/example-data/*

%changelog
* Wed Feb 14 2007 Toni Graffy <toni@links2linux.de> - 0.98.0-0.pm.1
- update to 0.98
* Wed Jan 31 2007 Toni Graffy <toni@links2linux.de> - 0.97.0-0.pm.1
- initial build 0.97.0
