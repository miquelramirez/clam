/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef InstantViewPlugin_hxx
#define InstantViewPlugin_hxx
#include <string>
#include <list>
#include <map>
#include <QtCore/QString>
#include "Tonnetz.hxx"
#include <CLAM/DataTypes.hxx>

namespace CLAM_Annotator { class Project; }
namespace CLAM_Annotator { class InstantView; }
namespace CLAM { class DescriptionDataPool; }

class QWidget;

class InstantViewPlugin
{
public:
	typedef std::map<std::string, InstantViewPlugin *> PluginMap;
			

public:
	virtual ~InstantViewPlugin();
	virtual const char * id() const = 0;
	virtual QString name() const = 0;
	virtual bool configureDialog(const CLAM_Annotator::Project & project, CLAM_Annotator::InstantView & config) = 0;
	virtual QWidget * createView(QWidget * parent, const CLAM_Annotator::Project & project, CLAM_Annotator::InstantView & config)=0;

	virtual void updateData(const CLAM::DescriptionDataPool & data, CLAM::TData samplingRate)=0;
	virtual void clearData()=0;
	virtual void setCurrentTime(double timeMiliseconds)=0;

public:
	static InstantViewPlugin * createPlugin(const std::string & id);
	static std::list<std::string> availablePlugins();
};





#endif//InstantViewPlugin_hxx

