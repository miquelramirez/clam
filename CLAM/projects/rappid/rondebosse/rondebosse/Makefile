all: mainwindow.cpp mainwindow.moc.cpp rondebosse.moc.cpp rondebosse_meta_unload.cpp

mainwindow.h: mainwindow.ui
	uic -o mainwindow.h mainwindow.ui

mainwindow.moc.cpp: mainwindow.h
	moc mainwindow.h -o mainwindow.moc.cpp
  
mainwindow.cpp: mainwindow.h mainwindow.ui
	uic -i mainwindow.h mainwindow.ui -o mainwindow.cpp 

rondebosse.moc.cpp: rondebosse.h
	moc rondebosse.h -o rondebosse.moc.cpp

rondebosse_meta_unload.cpp: rondebosse.moc.cpp mainwindow.moc.cpp
	echo 'static const char * _metalist_rondebosse[] = {' > rondebosse_meta_unload.cpp
	cat  rondebosse.moc.cpp mainwindow.moc.cpp | grep 'char.*className' | sed -e 's/.*[^A-Za-z0-9_:]\([A-Za-z0-9_:]*\)::className.*$$/\"\1\",/' | sort | uniq >> rondebosse_meta_unload.cpp
	echo '0};' >> rondebosse_meta_unload.cpp
	echo '#include <kunload.h>' >> rondebosse_meta_unload.cpp
	echo '_UNLOAD(rondebosse)' >> rondebosse_meta_unload.cpp

clean:
	rm -f mainwindow.h *.moc.cpp mainwindow.cpp 
