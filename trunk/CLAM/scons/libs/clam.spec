Summary: CLAM - C++ Library for Audio and Music
Name: CLAM
Version: 0.91.0
Release: 3
License: GPL
Packager: CLAM Team <clam@iua.upf.edu>
URL: http://clam.iua.upf.edu
Group: System Environment/Libraries
Source0: http://clam.iua.upf.edu/download/src/CLAM-0.91.0.tar.gz
Source1: http://clam.iua.upf.edu/download/src/CLAM-Annotator-0.3.2.tar.gz
Source2: http://clam.iua.upf.edu/download/src/CLAM-NetworkEditor-0.4.1.tar.gz
Source3: http://clam.iua.upf.edu/download/src/CLAM-SMSTools-0.4.1.tar.gz
#Patch: eject-2.0.2-buildroot.patch
BuildRoot: /var/tmp/%{name}-buildroot
%description
CLAM is a framework to develop audio and music applications in C++.
It provides tools to perform advanced analysis, transformations and synthesis,
visualization for audio and music related objects, and other tools that are
useful to abstract target platform for most tasks in an audio applications
such as audio devices, file formats, threading...

%package devel
Summary: CLAM - C++ Library for Audio and Music - development files
Group: Development/Libraries
%description devel
CLAM is a framework to develop audio and music applications in C++.
It provides tools to perform advanced analysis, transformations and synthesis,
visualization for audio and music related objects, and other tools that are
useful to abstract target platform for most tasks in an audio applications
such as audio devices, file formats, threading...

This package includes the development files

%package applications
Summary: CLAM Applications
Group: Applications/Multimedia
Version: 0.91.0
%description applications
CLAM is a framework to develop audio and music applications in C++.
It provides tools to perform advanced analysis, transformations and synthesis,
visualization for audio and music related objects, and other tools that are
useful to abstract target platform for most tasks in an audio applications
such as audio devices, file formats, threading...

This package includes the flag ship applications:
- SMSTools: A sound authoring tool using spectral transfomations.
- NetworkEditor: A CLAM network patching tool to build audio and music processing cores for applications.
- Prototyper: An aplication builder that joins CLAM networks with QtDesigner interfaces.
- Annotator: An Annotation tool to extract, and edit information from the music.

%prep
#%setup
#%patch -p1 -b .buildroot
export CLAM=CLAM-0.91.0
export ANNOTATOR=CLAM-Annotator-0.3.2
export SMSTOOLS=CLAM-SMSTools-0.4.1
export NETWORKEDITOR=CLAM-NetworkEditor-0.4.1

cd $RPM_BUILD_DIR
rm -rf CLAM*
#tar xvfz $RPM_SOURCE_DIR/$CLAM.tar.gz
#tar xvfz $RPM_SOURCE_DIR/$ANNOTATOR.tar.gz
#tar xvfz $RPM_SOURCE_DIR/$SMSTOOLS.tar.gz
#tar xvfz $RPM_SOURCE_DIR/$NETWORKEDITOR.tar.gz
cvs co -d$CLAM CLAM
cvs co -d$ANNOTATOR CLAM_Annotator
cvs co -d$SMSTOOLS CLAM_SMSTools
cvs co -d$NETWORKEDITOR CLAM_NetworkEditor

%build
echo Testing whether NETWORKEDITOR is $NETWORKEDITOR
export CLAM=CLAM-0.91.0
export ANNOTATOR=CLAM-Annotator-0.3.2
export SMSTOOLS=CLAM-SMSTools-0.4.1
export NETWORKEDITOR=CLAM-NetworkEditor-0.4.1
export INSTALLDIR=$RPM_BUILD_ROOT/usr
export SCONSTOOLSDIR=$RPM_BUILD_DIR/$CLAM/scons/sconstools/
mkdir -p $INSTALLDIR
export QTDIR=/usr/lib/qt-3.3
cd $RPM_BUILD_DIR/$CLAM/scons/libs
scons configure xmlbackend=xmlpp with_osc_support=no release=1
scons prefix=$INSTALLDIR 
scons install prefix=$INSTALLDIR 

cd $RPM_BUILD_DIR/$NETWORKEDITOR
scons install_prefix=$INSTALLDIR clam_prefix=$INSTALLDIR clam_sconstools=$SCONSTOOLSDIR release=1

cd $RPM_BUILD_DIR/$SMSTOOLS
scons install_prefix=$INSTALLDIR clam_prefix=$INSTALLDIR clam_sconstools=$SCONSTOOLSDIR release=1

export QTDIR=/usr/lib/qt4

cd $RPM_BUILD_DIR/$ANNOTATOR/vmqt
scons install_prefix=$INSTALLDIR clam_prefix=$INSTALLDIR clam_sconstools=$SCONSTOOLSDIR release=1

cd $RPM_BUILD_DIR/$ANNOTATOR
scons install_prefix=$INSTALLDIR clam_prefix=$INSTALLDIR clam_sconstools=$SCONSTOOLSDIR clam_vmqt4_path=vmqt release=1

%install
export CLAM=CLAM-0.91.0
export ANNOTATOR=CLAM-Annotator-0.3.2
export SMSTOOLS=CLAM-SMSTools-0.4.1
export NETWORKEDITOR=CLAM-NetworkEditor-0.4.1
export INSTALLDIR=$RPM_BUILD_ROOT/usr
export SCONSTOOLSDIR=$RPM_BUILD_DIR/$CLAM/scons/sconstools/
export QTDIR=/usr/lib/qt-3.3
rm -rf $RPM_BUILD_ROOT
mkdir -p $INSTALLDIR
cd $RPM_BUILD_DIR/$CLAM/scons/libs
scons install
cd $RPM_BUILD_DIR/$SMSTOOLS
scons install
cd $RPM_BUILD_DIR/$NETWORKEDITOR
scons install
export QTDIR=/usr/lib/qt4
cd $RPM_BUILD_DIR/$ANNOTATOR
scons install
# TODO: install it properly on the scons
mkdir -p $RPM_BUILD_ROOT/usr/lib/qt-3.3/plugins/
mv $RPM_BUILD_ROOT/usr/bin/designer $RPM_BUILD_ROOT/usr/lib/qt-3.3/plugins/
rm -f $RPM_BUILD_ROOT/usr/lib/.sconsign
$SCONSTOOLSDIR/changeExampleDataPath.py $RPM_BUILD_ROOT/usr/share/smstools /usr/share/smstools
$SCONSTOOLSDIR/changeExampleDataPath.py $RPM_BUILD_ROOT/usr/share/networkeditor /usr/share/networkeditor

%clean
#rm -rf $RPM_BUILD_ROOT


%files
#%defattr(-,root,root)
#%doc README COPYING ChangeLog.txt
/usr/lib/*.so*
/usr/lib/pkgconfig

%files devel
/usr/include

%files applications
/usr/bin
/usr/share
/usr/lib/qt-3.3/plugins/designer/libCLAMWidgets.so

%changelog
* Mon May 21 2006 CLAM Team <clam@iua.upf.edu> 
- First try

