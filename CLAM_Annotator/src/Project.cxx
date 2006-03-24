
#include "Project.hxx"
#include <CLAM/Text.hxx>
#include <CLAM/XMLStorage.hxx>
#include <QtCore/QDir>

namespace CLAM_Annotator
{

void Project::SetProjectPath(const std::string & path)
{
	mFile = path;
	QString projectPath = QDir::cleanPath((path+"/../").c_str());
	mBasePath = projectPath.toStdString();
}

std::string Project::RelativeToAbsolute(const std::string & file) const
{
/*
	QDir projectPath(mBasePath.c_str());
	return projectPath.relativeFilePath(file.c_str()).toStdString();
*/
	QDir qdir = QString(file.c_str());
	if (qdir.isRelative())
		return QDir::cleanPath( QDir(mBasePath.c_str()).filePath(file.c_str()) ).toStdString();
	return file;
}
std::string Project::AbsoluteToRelative(const std::string & file) const
{
	QDir projectPath(mBasePath.c_str());
	return projectPath.relativeFilePath(file.c_str()).toStdString();

	QDir qdir = QString(file.c_str());
	if (qdir.isRelative()) return file;
	if (file.substr(0,mBasePath.length()+1)==(mBasePath+"/"))
		return file.substr(mBasePath.length()+1);
	return file;
}

void Project::CreatePoolScheme()
{
	mDescriptionScheme = CLAM::DescriptionScheme(); // reset

	const std::list<CLAM_Annotator::SchemaAttribute>& hlds = mSchema.GetAllAttributes();
	std::list<CLAM_Annotator::SchemaAttribute>::const_iterator it2;
	for(it2 = hlds.begin(); it2 != hlds.end(); it2++)
	{
		it2->AddTo(mDescriptionScheme);
	}
}

bool Project::LoadScheme(const std::string & schemeFileName, const std::string & basePath)
{
	CLAM_Annotator::Schema tempSchema;
	CLAM::XMLStorage::Restore(tempSchema,basePath+schemeFileName); // May throw an exception

	// Successfull file, just change it
	SetSchema(std::string(schemeFileName)); // temp created to support self assigns
	GetAnnotatorSchema() = tempSchema;
	GetDescriptionScheme() = CLAM::DescriptionScheme();
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

std::list<std::string> Project::GetNamesByScopeAndType(const std::string & scope, const std::string & type)
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

