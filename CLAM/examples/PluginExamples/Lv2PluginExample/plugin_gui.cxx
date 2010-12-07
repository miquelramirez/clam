#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <lv2.h>
#include "lv2_ui.h"
#include "plugin_gui.h"
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <QtGui/QX11EmbedWidget>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtUiTools/QUiLoader>
#include <iostream>
#include <iostream>
#include <string>
#include <sstream>
#include <QtCore/QVariant>

static LV2UI_Descriptor *gdescriptor[]={0,0};

void PluginGui::setValueControlAtWidget(unsigned id, double value)
{
	for(unsigned i=0;i<containers.size();i++)
	{
		containers[i]->setValueAtWidget(id,value);
	}
}

void PluginGui::setValueControlAtPlugin(unsigned id, double value)
{
	float fvalue = (float)value;

	if(id>=containers.size())
	{
		std::cerr << "Warning id isn't valid." << std::endl;
		return;
	}

	(*write_function)(controller, id, 4, 0, &fvalue);
}

/**
	This method embed qtwiget into the gtk socket
**/
static void on_socket_realized(GtkWidget *widget, gpointer   user_data)
{
	PluginGui * pluginGui = (PluginGui*) user_data;
	int id = gtk_socket_get_id (GTK_SOCKET(pluginGui->widgetSocket));
	pluginGui->setLayout(new QVBoxLayout);
	pluginGui->layout()->addWidget(pluginGui->qtWidget);
	pluginGui->embedInto(id);
	pluginGui->show();
}


/**
	This method find all QWidget in the userInterface and create the adapter
**/
void bindWidgets(QWidget * userInterface, PluginGui * pgui)
{
        QList<QWidget*> widgets = userInterface->findChildren<QWidget*>();

	int idx=0;
	Lv2GuiContainer * temp=0;

	std::vector< std::pair<int, Lv2GuiContainer*> > aux; // Ports in widget maybe unordered

	for (QList<QWidget*>::Iterator it=widgets.begin();
                        it!=widgets.end();
                        it++)
	{
                QWidget * aWidget = *it;
		QVariant v = aWidget->property("clamLv2Port");

		if(v.isValid())
		{
			int nport = v.toInt();
			std::cout << "No." << nport << " Name: " << aWidget->metaObject()->className() << std::endl;
			temp=new Lv2DoubleSpinBox(idx,(QDoubleSpinBox*)aWidget,pgui);
			aux.push_back( std::make_pair(nport,temp) );
			idx++;
		}
	}
	
	int size = aux.size();
	pgui->containers.resize(size);
	for(int i=0;i<size;i++)
	{
		if(aux[i].first>=size)
		{
			std::cerr 	<< "==== ERROR Port number isn't valid! It wants number port:" << aux[i].first 
					<< ", but It only has " << size << "ports.====" << std::endl; 
			exit(0);
		}
		pgui->containers[aux[i].first]=aux[i].second;
	}
}


/**
	This method load a Ui interface
**/
QWidget * loadUi(const QString & uiFilename)
{
	QFile file(uiFilename);
	file.open(QFile::ReadOnly);
	QUiLoader loader;
	loader.addPluginPath("/user/share/NetworkEditor/qtplugins"); //TODO Make that an option
	QDir dir(QApplication::applicationDirPath());
	loader.addPluginPath( QString(dir.absolutePath())+"/../plugins" ); //TODO do only for mac?

	QStringList paths = loader.pluginPaths();
	for (QStringList::iterator it = paths.begin(); it!=paths.end(); it++)
	{
	    std::cout << "Looking for plugins at path: " << it->toLocal8Bit().constData() << std::endl;
	}
	QWidget * userInterface = loader.load(&file, 0 );
	if (false and userInterface)
	{
	    if (userInterface->windowIcon().isNull())
	       userInterface->setWindowIcon(QIcon(":/icons/images/Prototyper-icon.svg"));
	}
	file.close();

	return userInterface;
}

template <unsigned num>
static LV2UI_Handle instantiateIToneGui(const struct _LV2UI_Descriptor* descriptor
				, const char* plugin_uri
				, const char* bundle_path
				, LV2UI_Write_Function write_function
				, LV2UI_Controller controller
				, LV2UI_Widget* widget
				, const LV2_Feature* const* features)
{
	std::cout << "Controller: " << (void*)controller << std::endl;

	PluginGui *pluginGui = new PluginGui();

	if(pluginGui==NULL) return NULL;

	printf("\nInitializing GUI!!!!!!!!!!!!!\n");

	pluginGui->write_function = write_function;
	pluginGui->controller     = controller;
	pluginGui->bundlePath     = bundle_path;
	pluginGui->_id 		  = num;
	
	/*************** Qt */

	pluginGui->widgetSocket = gtk_socket_new();
	
	pluginGui->qtWidget 	= loadUi(QString(bundle_path)+"example.ui");

	bindWidgets(pluginGui->qtWidget, pluginGui);

	gtk_widget_set_size_request(pluginGui->widgetSocket
					, pluginGui->qtWidget->width()
					, pluginGui->qtWidget->height());

	g_signal_connect(G_OBJECT(pluginGui->widgetSocket), "realize",
		G_CALLBACK(on_socket_realized), gpointer(pluginGui) );

	*widget = (LV2UI_Widget) pluginGui->widgetSocket;

	/****************/

	return pluginGui;
}

static void cleanupIToneGui(LV2UI_Handle ui)
{}

static void port_eventIToneGui(LV2UI_Handle ui
				, uint32_t port
				, uint32_t buffer_size
				, uint32_t format
				, const void*  buffer)
{
	PluginGui * pluginGui = (PluginGui*) ui;

	if(format==0) 
	{
		float value=* (float *) buffer;
		pluginGui->setValueControlAtWidget(port, (double)value);
	}
}

static void init()
{
	gdescriptor[0] 			= new LV2UI_Descriptor();
	gdescriptor[0]->URI 		= "http://clam-project.org/examples/lv2/othercable/gui";
	gdescriptor[0]->instantiate 	= instantiateIToneGui<0>;
	gdescriptor[0]->cleanup		= cleanupIToneGui;
	gdescriptor[0]->port_event	= port_eventIToneGui;
	gdescriptor[0]->extension_data 	= NULL;
	
	gdescriptor[1] 			= new LV2UI_Descriptor();
	gdescriptor[1]->URI 		= "http://clam-project.org/examples/lv2/stereocable/gui";
	gdescriptor[1]->instantiate 	= instantiateIToneGui<1>;
	gdescriptor[1]->cleanup		= cleanupIToneGui;
	gdescriptor[1]->port_event	= port_eventIToneGui;
	gdescriptor[1]->extension_data 	= NULL;
}

const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{	
	static QApplication * app = 0;
	static char * argv[] = {0};
	static int argc = 0;

	if (not QApplication::instance())
		app = new QApplication(argc, argv);

	if (!gdescriptor[0]) init();

	return ((index == 0 or index == 1)?gdescriptor[index]:NULL);
}
