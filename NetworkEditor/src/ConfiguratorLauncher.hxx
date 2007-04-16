#ifndef ConfiguratorLauncher_hxx
#define ConfiguratorLauncher_hxx

#include "Qt4Configurator.hxx"
#include <CLAM/Processing.hxx>
#include <QtCore/QString>

class ConfiguratorLauncher : public CLAM::Qt4Configurator // Inherits just to access protected
{
public:
	virtual ~ConfiguratorLauncher() {}
	virtual bool Launch(CLAM::Processing & processing, const QString & name)=0;
	virtual const CLAM::ProcessingConfig& GetConfiguration() const=0;
};

template <typename Config>
class TypedConfiguratorLauncher : public ConfiguratorLauncher
{
	Config _config;
public:
	bool Launch(CLAM::Processing & processing, const QString & name)
	{
		const CLAM::ProcessingConfig & originalConfig = processing.GetConfig();
		_config = dynamic_cast<const Config&> (originalConfig);

		CLAM::Qt4Configurator configurator;
		configurator.setWindowTitle(name);
		configurator.SetConfig(_config);
		return configurator.exec();
	}
	const CLAM::ProcessingConfig& GetConfiguration() const
	{
		return _config;
	}
	
};

template <typename Widget>
class WidgetTypedConfiguratorLauncher : public ConfiguratorLauncher
{
	typedef typename Widget::Config Config;
	Config _config;
public:
	bool Launch(CLAM::Processing & processing, const QString & name)
	{
		const CLAM::ProcessingConfig & originalConfig = processing.GetConfig();
		_config = dynamic_cast<const Config&> (originalConfig);

		Widget configurator;
		configurator.setWindowTitle(name);
		configurator.SetConfig(_config);
		return configurator.exec();
	}
	const CLAM::ProcessingConfig& GetConfiguration() const
	{
		return _config;
	}
	
};




#endif//ConfiguratorLauncher_hxx

