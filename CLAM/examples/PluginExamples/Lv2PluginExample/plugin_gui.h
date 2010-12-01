#ifndef PluginGui_h
#define PluginGui_h

#include <lv2.h>
#include "lv2_ui.h"
#include <gtk/gtk.h>
#include <QtGui/QX11EmbedWidget>
#include <QtUiTools/QUiLoader>

class PluginGui : public QX11EmbedWidget
{
	Q_OBJECT

public:
	unsigned _id;
	QWidget   *	qtWidget;
	GtkWidget *	widgetSocket;

	LV2UI_Write_Function 	write_function;
	LV2UI_Controller 	controller;
	const char * 		bundlePath;
	
	PluginGui()
	{}
};

#endif
