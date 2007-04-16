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

#ifndef REGISTER_CONFIGURATOR_LAUNCHER_HXX
#define REGISTER_CONFIGURATOR_LAUNCHER_HXX

#include <CLAM/Factory.hxx>
#include "ConfiguratorLauncher.hxx"

typedef CLAM::Factory<ConfiguratorLauncher> ProcessingConfigPresentationFactory;

// Convenient macros (Not to use widely)
#define STANDARD_PROCESSING_CONFIG_REGISTER(configName) \
   	static ProcessingConfigPresentationFactory::Registrator<  \
		TypedConfiguratorLauncher< CLAM::configName > > \
		 reg##configName(#configName)
#define SPECIAL_PROCESSING_CONFIG_REGISTER(configName, configurator) \
	static ProcessingConfigPresentationFactory::Registrator< \
		WidgetTypedConfiguratorLauncher<configurator> > \
		 regt##configName( #configName)

#endif

