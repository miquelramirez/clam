TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -l GLU

INCLUDEPATH	+= ../../common/src/ ./

HEADERS	+= ../../common/src/ListViewItem.h \
	../../common/src/TableItem.hxx \
	../../common/src/Table.hxx \
	../../common/src/ComboTableItem.h \
	AnnotatorBrowserGL/AnnotatorBrowserGL.hxx

SOURCES	+= main.cxx \
	../../common/src/ListViewItem.cxx \
	../../common/src/TableItem.cxx \
	../../common/src/Table.cxx \
	../../common/src/ComboTableItem.cxx \
	AnnotatorBrowserGL/AnnotatorBrowserGL.cxx

FORMS	= AnnotatorBase.ui \
	AboutBase.ui \
	ConfigurationDialogBase.ui \
	AnalyzeWindowBase.ui \
	AnnotatorEnterBase.ui \
	NewProjectDialogBase.ui \
	NewProjectNameBase.ui \
	OpenProjectDialogBase.ui \
	SaveProjectAsDialogBase.ui

IMAGES	= ../images/simac.png \
	../images/graphmini.png \
	../images/graph2.png \
	../images/graph5.png \
	../images/resume.png \
	../images/pause.png \
	../images/stock_autopilot.png \
	../images/stock_new-document-32.png \
	../images/stock_open.png \
	../images/stock_save_as.png \
	../images/stock_copy.png \
	../images/stock_cut.png \
	../images/stock_paste.png \
	../images/stock_undo.png \
	../images/Annotator.png \
	images/stock_exit.png \
	images/stock_data-first-16.png \
	images/stock_data-last-16.png \
	images/stock_data-pause-16.png \
	images/stock_data-play-16.png

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
