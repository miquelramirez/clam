#ifndef ConfiguratorLauncher_hxx
#define ConfiguratorLauncher_hxx

#include "Qt4Configurator.hxx"
#include <CLAM/Processing.hxx>
#include <QtCore/QString>

class ConfiguratorLauncher : public CLAM::Qt4Configurator // Inherits just to access protected
{
public:
	virtual ~ConfiguratorLauncher() {}
	virtual bool Launch(CLAM::Processing & processing, const QString & name) =0;
	virtual void Apply(CLAM::Processing & processing)=0;
};

template <typename Config>
class TypedConfiguratorLauncher : public ConfiguratorLauncher
{
	Config config;
public:
	bool Launch(CLAM::Processing & processing, const QString & name)
	{
		const CLAM::ProcessingConfig & originalConfig = processing.GetConfig();
		config = dynamic_cast<const Config&> (originalConfig);

		CLAM::Qt4Configurator configurator;
		configurator.setWindowTitle(name);
		configurator.SetConfig(config);
		return configurator.exec();
	}
	void Apply(CLAM::Processing & processing)
	{
		processing.Configure(config);
	}
	
};

template <typename Widget>
class WidgetTypedConfiguratorLauncher : public ConfiguratorLauncher
{
	typedef typename Widget::Config Config;
	Config config;
public:
	bool Launch(CLAM::Processing & processing, const QString & name)
	{
		const CLAM::ProcessingConfig & originalConfig = processing.GetConfig();
		config = dynamic_cast<const Config&> (originalConfig);

		Widget configurator;
		configurator.setWindowTitle(name);
		configurator.SetConfig(config);
		return configurator.exec();
	}
	void Apply(CLAM::Processing & processing)
	{
		processing.Configure(config);
	}
	
};




#endif//ConfiguratorLauncher_hxx
