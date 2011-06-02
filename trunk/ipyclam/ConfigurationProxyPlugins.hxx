#ifndef ConfigurationProxyPlugins_hxx
#define ConfigurationProxyPlugins_hxx

#include "ConfigurationProxy.hxx"

class ConfigurationProxyPlugin
{
public:
	typedef std::list<ConfigurationProxyPlugin *> Map;
	ConfigurationProxyPlugin()
	{
		GetList().push_back(this);
	}
	virtual ~ConfigurationProxyPlugin() {}
	static Map & GetList()
	{
		static Map map;
		return map;
	}
	static ConfigurationProxyPlugin & GetPlugin(const ConfigurationProxy & object, unsigned attribute);
	virtual bool accepts(const ConfigurationProxy & object, unsigned attribute) = 0;
};

#endif
