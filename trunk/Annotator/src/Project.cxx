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
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>

namespace CLAM_Annotator
{


bool Extractor::generateSchema(const QString & configFileName, const QString schemaFileName)
{
	if (!HasExtractor()) return false;
	if (GetExtractor()=="") return false;
	std::cout << schemaFileName.toStdString() << " " << configFileName.toStdString() << std::endl;
	QProcess process;
	process.start(GetExtractor().c_str(),
		QStringList()
			<< "-c" << configFileName
			<< "-s" << schemaFileName
			);
	std::cout << "Launched extractor to get the schema..." << std::endl;
	process.waitForFinished(-1); // TODO: This stalls
	std::cout << process.readAllStandardOutput().constData() << std::endl;
	std::cerr << process.readAllStandardError().constData() << std::endl;
	return process.exitCode()==0;
}

const Schema & Extractor::schema() const
{
	return _schema;
}

bool Extractor::generateSchema(QWidget * window)
{
	if (HasSchema() && GetSchema()!="")
	{
		CLAM_Annotator::Schema tempSchema;
		try
		{
			CLAM::XMLStorage::Restore(tempSchema,GetSchema());
		}
		catch (CLAM::XmlStorageErr & err)
		{
			QMessageBox::critical(window, QObject::tr("Regenerating Schema"),
				QObject::tr("<p><b>Error: Unable read schema file %1</b></p>\n%2")
					.arg(GetSchema().c_str())
					.arg(err.what())
				);
			return false;
		}
		_schema = tempSchema;
		return true;
	}
	if (!HasExtractor()) return false;
	if (GetExtractor()=="") return false;
	std::cout << "Generating Schema..." << std::endl;
	QTemporaryFile configFile;
	configFile.setAutoRemove(false);
	configFile.open();
	configFile.write(GetConfiguration().c_str());
	QString configFileName = configFile.fileName();
	configFile.close();
	QTemporaryFile schemaFile;
	schemaFile.setAutoRemove(false);
	schemaFile.open();
	QString schemaFileName = schemaFile.fileName();
	schemaFile.close();

	if (not generateSchema(configFileName, schemaFileName))
	{
		QMessageBox::critical(window, QObject::tr("Regenerating Schema"),
			QObject::tr(
				"<p><b>Error: Unable to launch the extractor to regenerate the schema</b></p>\n"
				"<p>Check that the project extractor is well configured and you have permissions to run it.</p>\n"
				"<p>The configured command was:</p>\n<tt>%1</tt>")
			.arg(GetExtractor().c_str())
			);
		return false;
	}
	CLAM_Annotator::Schema tempSchema;
	try
	{
		CLAM::XMLStorage::Restore(tempSchema,schemaFileName.toStdString());
	}
	catch (CLAM::XmlStorageErr & err)
	{
		QMessageBox::critical(window, QObject::tr("Regenerating Schema"),
			QObject::tr("<p><b>Error: Unable read generated schema %1</b></p>\n%2")
				.arg(schemaFileName)
				.arg(err.what())
			);
		return false;
	}
	_schema = tempSchema;
	return true;
}

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

void Project::InitConfiguration()
{
	QString filePath(File().c_str());
	QFile *file=new QFile(filePath.append(".conf"));
	if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	if(!file->resize(0))
		return; 
	QTextStream out(file);
	out << GetConfiguration().c_str();	
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

bool Project::LoadSchema()
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

