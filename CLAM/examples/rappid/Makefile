PROGRAMS=Rondebosse CMLRappid TestSoftSwitch TestAudioMixer TestAudioCompressor TestDynamicMixer TestRappidIO
TOP=../..
INCLUDE_DIRS=/usr/include/kde ./
include $(TOP)/unix_tools/make.rules

#### The following is very KDE specific, and only needed for Rondebosse

mainwindow.h: mainwindow.ui
	uic -o mainwindow.h mainwindow.ui

../../examples/rappid/mainwindow.moc.cpp: mainwindow.h
	moc mainwindow.h -o mainwindow.moc.cpp
  
../../examples/rappid/mainwindow.cpp: mainwindow.h mainwindow.ui
	uic -i mainwindow.h mainwindow.ui -o mainwindow.cpp 

../../examples/rappid/rondebosse.moc.cpp: mainwindow.h rondebosse.h
	moc rondebosse.h -o rondebosse.moc.cpp

rondebosse_meta_unload.cpp: rondebosse.moc.cpp mainwindow.moc.cpp
	echo 'static const char * _metalist_rondebosse[] = {' > rondebosse_meta_unload.cpp
	cat  rondebosse.moc.cpp mainwindow.moc.cpp | grep 'char.*className' | sed -e 's/.*[^A-Za-z0-9_:]\([A-Za-z0-9_:]*\)::className.*$$/\"\1\",/' | sort | uniq >> rondebosse_meta_unload.cpp
	echo '0};' >> rondebosse_meta_unload.cpp
	echo '#include <kunload.h>' >> rondebosse_meta_unload.cpp
	echo '_UNLOAD(rondebosse)' >> rondebosse_meta_unload.cpp

../../dep/examples/rappid/rondebosse.moc.d: rondebosse.moc.cpp
../../dep/examples/rappid/mainwindow.d: mainwindow.cpp
../../dep/examples/rappid/mainwindow.moc.d: mainwindow.moc.cpp

