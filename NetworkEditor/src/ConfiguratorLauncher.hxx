/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef ConfiguratorLauncher_hxx
#define ConfiguratorLauncher_hxx

#include "Qt4Configurator.hxx"
#include <CLAM/Processing.hxx>
#include <QtCore/QString>
#include <CLAM/Factory.hxx>

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



class ConfiguratorLauncherFactory : public CLAM::Factory<ConfiguratorLauncher>
{
public:
	static ConfiguratorLauncherFactory& GetInstance();
};

// Convenient macros (Not to use widely)
#define STANDARD_PROCESSING_CONFIG_REGISTER(configName) \
   	static CLAM::FactoryRegistrator<ConfiguratorLauncherFactory, TypedConfiguratorLauncher< CLAM::configName > > \
		 reg##configName(#configName)
#define SPECIAL_PROCESSING_CONFIG_REGISTER(configName, configurator) \
   	static CLAM::FactoryRegistrator<ConfiguratorLauncherFactory, WidgetTypedConfiguratorLauncher<configurator> > \
		 regt##configName( #configName)



#endif//ConfiguratorLauncher_hxx

