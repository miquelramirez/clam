#include "plugin_gui.h"
#include <QtGui/QVBoxLayout>
#include <QtGui/QApplication>
#include <QtGui/QIcon>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtUiTools/QUiLoader>
#include <string>
#include <iostream>
#include <QtCore/QVariant>

void PluginGui::setValueControlAtWidget(unsigned id, double value)
{
	for(unsigned i=0;i<containers.size();i++)
	{
		containers[i]->setValueAtWidget(id,value);
	}
}

void PluginGui::setValueControlAtPlugin(unsigned id, double value)
{
	CLAM_ASSERT(id<containers.size(), "Warning id isn't valid.");

	float fvalue = (float)value;
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

	std::vector< std::pair<int, Lv2GuiContainer*> > aux; // Ports in widget maybe unordered

	for (QList<QWidget*>::Iterator it=widgets.begin();
                        it!=widgets.end();
                        it++)
	{
		QWidget * aWidget = *it;
		QVariant v = aWidget->property("clamLv2Port");
		if (not v.isValid()) continue;

		int nport = v.toInt();
		Lv2GuiContainer * temp=new Lv2DoubleSpinBox(aux.size(),(QDoubleSpinBox*)aWidget,pgui);
		aux.push_back( std::make_pair(nport,temp) );
	}
	
	int size = aux.size();
	pgui->containers.resize(size);
	for(int i=0;i<size;i++)
	{
		CLAM_ASSERT(aux[i].first<size, "Port number isn't valid!");
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

class Lv2UiLibrary
{
	std::vector<LV2UI_Descriptor * > _descriptors;
public:
	Lv2UiLibrary()
	{
	}
	~Lv2UiLibrary()
	{
        for (unsigned i=0; i<_descriptors.size(); i++)
			CleanUpDescriptor(_descriptors[i]);

	}
	void AddPluginType(LV2UI_Descriptor * descriptor, const std::string & uiFile)
	{
		_descriptors.push_back(descriptor);
		addUri(descriptor->URI,uiFile);
	}

	LV2UI_Descriptor * pluginAt(unsigned i)
	{
		if (i<_descriptors.size()) return _descriptors[i];
		return 0;
	}
	static std::string getUiFile(const std::string & uri)
	{
		return uri2ui()[uri];
	}
	static void addUri(const std::string & uri, const std::string & uiFile)
	{
		uri2ui()[uri] = uiFile;
	}
private:
	static std::map<std::string,std::string> & uri2ui()
	{
		static std::map<std::string,std::string> map;
		return map;
	}
	void CleanUpDescriptor(LV2UI_Descriptor *& descriptor)
	{
		if (not descriptor) return;
		delete descriptor;
		descriptor = 0;
	}
};

static LV2UI_Handle instantiate_callback(
	const LV2UI_Descriptor* descriptor,
	const char * plugin_uri,
	const char * bundle_path,
	LV2UI_Write_Function write_function,
	LV2UI_Controller controller,
	LV2UI_Widget * widget,
	const LV2_Feature* const* features)
{
	std::cout << "Controller: " << (void*)controller << std::endl;

	PluginGui * pluginGui = new PluginGui();

	pluginGui->write_function = write_function;
	pluginGui->controller     = controller;
	pluginGui->bundlePath     = bundle_path;
	
	/*************** Qt */

	pluginGui->widgetSocket = gtk_socket_new();

	std::string fullPluginPath = bundle_path + Lv2UiLibrary::getUiFile(descriptor->URI);
	std::cout << "fullPluginPath: " << fullPluginPath << std::endl;
	std::cout << "uri: " << plugin_uri << std::endl;
	pluginGui->qtWidget = loadUi(QString(fullPluginPath.c_str()));

	bindWidgets(pluginGui->qtWidget, pluginGui);

	gtk_widget_set_size_request(
		pluginGui->widgetSocket,
		pluginGui->qtWidget->width(),
		pluginGui->qtWidget->height());

	g_signal_connect(G_OBJECT(pluginGui->widgetSocket), "realize",
		G_CALLBACK(on_socket_realized), gpointer(pluginGui) );

	*widget = (LV2UI_Widget) pluginGui->widgetSocket;

	/****************/

	return pluginGui;
}

static void cleanup_callback(LV2UI_Handle ui)
{
}

static void port_event_callback(
				LV2UI_Handle ui,
				uint32_t port,
				uint32_t buffer_size,
				uint32_t format,
				const void*  buffer)
{
	PluginGui * pluginGui = (PluginGui*) ui;
	if(format==0) 
	{
		float value=* (float *) buffer;
		pluginGui->setValueControlAtWidget(port, (double)value);
	}
}

class Lv2UiDescriptor
{
public:
	Lv2UiDescriptor(Lv2UiLibrary & library, const char * uri, const char * uifile)
	{
		LV2UI_Descriptor * descriptor = new LV2UI_Descriptor;
		descriptor->URI = uri;
		descriptor->instantiate = instantiate_callback;
		descriptor->cleanup = cleanup_callback;
		descriptor->port_event = port_event_callback;
		descriptor->extension_data = NULL;
		library.AddPluginType(descriptor,uifile);
	}
};

const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{	
	static QApplication * app = 0;
	static char * argv[] = {0};
	static int argc = 0;

	if (not QApplication::instance())
		app = new QApplication(argc, argv);

	static Lv2UiLibrary library;
	static Lv2UiDescriptor p1(library, "http://clam-project.org/examples/lv2/stereocable/gui", "stereocable.ui");
	static Lv2UiDescriptor p2(library, "http://clam-project.org/examples/lv2/othercable/gui", "othercable.ui");

	return library.pluginAt(index);
}



