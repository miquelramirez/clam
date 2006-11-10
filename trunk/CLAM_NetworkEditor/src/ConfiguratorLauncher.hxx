#ifndef ConfiguratorLauncher_hxx
#define ConfiguratorLauncher_hxx

#include "Qt4Configurator.hxx"
#include <CLAM/Processing.hxx>
#include <QtCore/QString>

class ConfiguratorLauncher : public CLAM::Qt4Configurator
{
public:
	virtual ~ConfiguratorLauncher() {}
	virtual bool Launch(CLAM::Processing & processing, const QString & name) =0;
};

template <typename Config>
class TypedConfiguratorLauncher : public ConfiguratorLauncher
{
public:
	bool Launch(CLAM::Processing & processing, const QString & name)
	{
		const CLAM::ProcessingConfig & originalConfig = processing.GetConfig();
		Config config = dynamic_cast<const Config&> (originalConfig);

		CLAM::Qt4Configurator configurator;
		configurator.setWindowTitle(name);
		configurator.SetConfig(config);
		if (not configurator.exec()) return false;
		processing.Configure(config);
		return true;
	}
};




#endif//ConfiguratorLauncher_hxx
