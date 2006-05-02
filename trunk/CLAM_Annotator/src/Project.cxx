
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
	mDescriptionScheme = CLAM::DescriptionScheme(); // reset

	const std::list<CLAM_Annotator::SchemaAttribute>& hlds = mSchema.GetAllAttributes();
	std::list<CLAM_Annotator::SchemaAttribute>::const_iterator it2;
	for(it2 = hlds.begin(); it2 != hlds.end(); it2++)
	{
		it2->AddTo(mDescriptionScheme);
	}
}

bool Project::LoadScheme()
{
	CLAM_Annotator::Schema tempSchema;
	CLAM::XMLStorage::Restore(tempSchema,RelativeToAbsolute(GetSchema())); // May throw an exception
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

