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
	std::cout 
		<< "Schema: " << schemaFileName.toStdString() << std::endl
		<< "Config: " << configFileName.toStdString() << std::endl;
	QProcess process;
	process.start(GetExtractor().c_str(),
		QStringList()
			<< "-c" << configFileName
			<< "-s" << schemaFileName
			);
	std::cout << "Launched extractor to get the schema..." << std::endl;
	process.waitForFinished(-1); // TODO: This stalls
	std::cout << "Output:\n" << process.readAllStandardOutput().constData() << std::endl;
	std::cerr << "Error:\n" << process.readAllStandardError().constData() << std::endl;
	return process.exitCode()==0;
}

const Schema & Extractor::schema() const
{
	return _schema;
}
void Extractor::loadSchemaFile(const std::string & schemaFile)
{
	CLAM_Annotator::Schema tempSchema;
	CLAM::XMLStorage::Restore(tempSchema,schemaFile); // Throws exception if failed
	_schema = tempSchema;
}

bool Extractor::generateSchema(QWidget * window)
{
	bool debugging = true;
	QTemporaryFile configFile;
	configFile.setAutoRemove(not debugging);
	QTemporaryFile schemaFile;
	schemaFile.setAutoRemove(not debugging);
	if (GetExtractor()!="")
	{
		configFile.open();
		configFile.write(GetConfiguration().c_str());
		std::cout << "Config content:" << std::endl << GetConfiguration() << std::endl;
		QString configFileName = configFile.fileName();
		configFile.close();
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
		loadSchemaFile(schemaFileName.toStdString());
		return true;
	}
	if (HasSchema() && GetSchema()!="")
	{
		loadSchemaFile(GetSchema());
		return true;
	}
	return false;
}
bool Extractor::isRunnable(QWidget * window)
{
	QProcess process;
	process.start(GetExtractor().c_str(),
		QStringList()
			);
	if (not process.waitForStarted(-1)) return false;
	if (not process.waitForFinished(2000)) return false;
	std::cout << "Output:\n" << process.readAllStandardOutput().constData() << std::endl;
	std::cerr << "Error:\n" << process.readAllStandardError().constData() << std::endl;

	return true;
}

bool Extractor::computeDescriptors(QWidget * window, const QString & wavefile)
{
	QProcess process;
	QTemporaryFile * configFile = new QTemporaryFile(&process);
	configFile->open();
	QString configFileName = configFile->fileName();
	configFile->write(GetConfiguration().c_str());
	configFile->close();
	process.start(GetExtractor().c_str(),
		QStringList()
			<< "-c" << configFileName
			<< "-f" << GetPoolSuffix().c_str()
			<< wavefile
			);
	std::cout << "Launched extractor to get the descriptors..." << std::endl;
	process.waitForFinished(-1); // TODO: This stalls
	std::cout << "Output:\n" << process.readAllStandardOutput().constData() << std::endl;
	std::cerr << "Error:\n" << process.readAllStandardError().constData() << std::endl;
	return process.exitCode()==0;
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

void Project::dumpExtractorInfo(CLAM_Annotator::Extractor & extractor)
{
	extractor.AddAll();
	extractor.UpdateData();
	if (HasExtractor())
		extractor.SetExtractor(GetExtractor());
	if (HasSchema())
		extractor.SetSchema(RelativeToAbsolute(GetSchema()));
	if (HasConfiguration())
		extractor.SetConfiguration(GetConfiguration());
	if (HasPoolSuffix())
		extractor.SetPoolSuffix(GetPoolSuffix());
}

bool Project::LoadSchema()
{
	CLAM_Annotator::Extractor extractor;
	dumpExtractorInfo(extractor);
	if (not extractor.generateSchema(0)) return false;
	GetAnnotatorSchema() = extractor.schema();
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

