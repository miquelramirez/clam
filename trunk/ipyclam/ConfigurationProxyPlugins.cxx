#include "ConfigurationProxyPlugins.hxx"

class NullConfigurationProxyPlugin : public ConfigurationProxyPlugin
{
public:
	virtual bool accepts(const ConfigurationProxy & object, unsigned attribute)
	{
		return true;
	}
};

static ConfigurationProxyPlugin & ConfigurationProxyPlugin::GetPlugin(const ConfigurationProxy & object, unsigned attribute)
{
	static NullConfigurationProxyPlugin nullPlugin;
	for (Map::iterator it = GetList().begin(); it!=GetList().end(); it++)
	{
		if ((*it)->accepts(object,attribute)) return **it;
	}
	return nullPlugin;
}

class StringConfigurationProxyPlugin : public ConfigurationProxyPlugin
{
public:
	virtual bool accepts(const ConfigurationProxy & object, unsigned attribute)
	{
		return object.attributeType(attribute) == typeid(std::string);
	}
};
static StringConfigurationProxyPlugin stringRegistrator;
