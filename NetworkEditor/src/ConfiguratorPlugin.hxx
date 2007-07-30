#ifndef ConfiguratorPlugin_hxx
#define ConfiguratorPlugin_hxx
#include <list>
#include <CLAM/DynamicType.hxx>
#include <QtGui/QLabel>

/**
 * Abstract class for plugins to add new editable types to configuration dialogs
 */

class ConfiguratorPlugin
{
	public:
		typedef std::list<ConfiguratorPlugin *> Map;
		ConfiguratorPlugin()
		{
			GetList().push_back(this);
		}
		virtual ~ConfiguratorPlugin() {}
		static Map & GetList()
		{
			static Map map;
			return map;
		}
		static ConfiguratorPlugin & GetPlugin(const CLAM::DynamicType & object, unsigned attribute);
		virtual bool accepts(const CLAM::DynamicType & object, unsigned attribute) = 0;
		virtual QWidget * editorWidget(const CLAM::DynamicType & object, unsigned attribute)
		{
			return new QLabel("Plugin must define editorWidget");
		}
		virtual void takeFromWidget(const CLAM::DynamicType & object, unsigned attribute, QWidget * editorWidget)
		{
		}
};

#endif//ConfiguratorPlugin_hxx

