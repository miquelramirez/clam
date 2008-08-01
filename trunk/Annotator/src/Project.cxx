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


#include "Project.hxx"
#include <CLAM/Text.hxx>
#include <CLAM/XMLStorage.hxx>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

namespace CLAM_Annotator
{

void Project::DumpSchema()
{
	CLAM::XMLStorage::Dump(GetAnnotatorSchema(), "DescriptionScheme", RelativeToAbsolute(GetSchema()));
}
void Project::DumpSchema(std::ostream & os)
{
	CLAM::XMLStorage::Dump(GetAnnotatorSchema(), "DescriptionScheme", os);
}
void Project::SetProjectPath(const std::string & path)
{
	mFile = path;
	mBasePath = QFileInfo(path.c_str()).path().toStdString();
	std::cout << "Project file: " << mFile << " at Base " << mBasePath << std::endl;
}

std::string Project::RelativeToAbsolute(const std::string & projectRelative) const
{
	QString file = projectRelative.c_str();
	QDir projectPath(mBasePath.c_str());
	QString absolute = projectPath.absoluteFilePath(file);
	return QDir::current().relativeFilePath(absolute).toStdString();
}
std::string Project::AbsoluteToRelative(const std::string & currentRelative) const
{
	QString file = currentRelative.c_str();
	QDir projectPath(mBasePath.c_str());
	QString absolute = QDir::current().absoluteFilePath(file);
	return projectPath.relativeFilePath(absolute).toStdString();
}

void Project::CreatePoolScheme()
{
	mDescriptionScheme = CLAM::DescriptionScheme(); // Reseting it
	mSchema.FillDescriptionScheme(mDescriptionScheme);
}

bool Project::LoadScheme(const std::string & GetSchemaRelative)
{
	CLAM_Annotator::Schema tempSchema;
	CLAM::XMLStorage::Restore(tempSchema,RelativeToAbsolute(GetSchemaRelative)); // May throw an exception
	GetAnnotatorSchema() = tempSchema;
	CreatePoolScheme();
	return true;
}

Project::ScopeSchema Project::GetScopeSchema(const std::string & scope) const
{
	Project::ScopeSchema scopeSchema;
	const std::list<CLAM_Annotator::SchemaAttribute>& hlds = mSchema.GetAllAttributes();
	std::list<CLAM_Annotator::SchemaAttribute>::const_iterator it;
	for(it = hlds.begin(); it != hlds.end(); it++)
	{
		if (it->GetScope()!=scope) continue;
		scopeSchema.push_back(*it);
	}
	return scopeSchema;
}

const CLAM_Annotator::SchemaAttribute & Project::GetParentAttribute(const std::string & scope) const
{
	const std::list<CLAM_Annotator::SchemaAttribute>& attribs = mSchema.GetAllAttributes();
	std::list<CLAM_Annotator::SchemaAttribute>::const_iterator it;
	for(it = attribs.begin(); it != attribs.end(); it++)
	{
		if (!it->HasChildScope()) continue;
		if (it->GetChildScope()!=scope) continue;
		return *it;
	}
	throw "No parent";
}

std::list<std::string> Project::GetNamesByScopeAndType(const std::string & scope, const std::string & type) const
{
	const std::list<CLAM_Annotator::SchemaAttribute>& hlds = mSchema.GetAllAttributes();
	std::list<CLAM_Annotator::SchemaAttribute>::const_iterator it;
	std::list<std::string> nameList;
	for(it = hlds.begin(); it != hlds.end(); it++)
	{
		if (it->GetScope()!=scope) continue;
		if (it->GetType()!=type) continue;
		nameList.push_back(it->GetName());
	}
	return nameList;
}

}

