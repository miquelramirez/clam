CONFIG      += designer plugin debug_and_release
TEMPLATE    = lib
DESTDIR     = plugins/designer
QT += opengl

CONFIG(debug, debug|release) {
    unix: TARGET = $$join(TARGET,,,_debug)
    else: TARGET = $$join(TARGET,,d)
}

HEADERS     = worldtimeclock.h \
              worldtimeclockplugin.h \
			  Plot.hxx \
			  PlotPlugin.hxx  \
			  CLAMWidgets.hxx

SOURCES     = worldtimeclock.cpp \
              worldtimeclockplugin.cpp \
			  Plot.cxx \
			  PlotPlugin.cxx  \
			  CLAMWidgets.cxx

CPPPATH=../ 
DEFINES=VERSION='"QMAKE"'

# install
target.path = $$[QT_INSTALL_PLUGINS]/designer
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/designer/clamwidgets
INSTALLS += target sources
