#include "InstantViewPlugin.hxx"
#include "Project.hxx"
#include "PcpTorus.hxx"
#include "KeySpace.hxx"

#include <string>
#include <map>
#include <QtGui/QInputDialog>

static InstantViewPlugin::PluginMap plugins;

class Initializer
{
public:
	Initializer::Initializer();
};

static Initializer initializer;

InstantViewPlugin::~InstantViewPlugin() {}

std::list<std::string> InstantViewPlugin::availablePlugins()
{
	std::list<std::string> list;
	for (PluginMap::iterator it = plugins.begin(); it!=plugins.end(); it++)
		list.push_back(it->first);
	return list;
}

InstantViewPlugin * InstantViewPlugin::getPlugin(const std::string & type)
{
	try { return plugins[type]; }
	catch (...) { return 0; }
}

/// Concrete plugins


class PcpTorusPlugin : public InstantViewPlugin
{
	virtual const char * id() const { return "PcpTorus"; }
	virtual QString name() const { return QObject::tr("Pitch Torus"); }
	CLAM::VM::PcpTorus * createView(QWidget * parent, const CLAM_Annotator::Project & project, CLAM_Annotator::InstantView & config)
	{
		CLAM::VM::PcpTorus * view =  new CLAM::VM::PcpTorus(parent);
		view->setSource(project, config.GetAttributeScope(), config.GetAttributeName());
		return view;
	}
	virtual bool configureDialog(const CLAM_Annotator::Project & project, CLAM_Annotator::InstantView & config)
	{
		QStringList attributeList;
		const std::list<std::string>& divisionNames = project.GetNamesByScopeAndType("Song", "FrameDivision");
		std::list<std::string>::const_iterator divisionName;
		for(divisionName = divisionNames.begin();divisionName != divisionNames.end(); divisionName++)
		{
			const std::string & frameDivisionChildScope = project.GetAttributeScheme("Song", *divisionName).GetChildScope();
			const std::list<std::string>& descriptorsNames = project.GetNamesByScopeAndType(frameDivisionChildScope, "FloatArray");
			std::list<std::string>::const_iterator it;
			for(it = descriptorsNames.begin();it != descriptorsNames.end(); it++)
			{
				const CLAM_Annotator::SchemaAttribute & attribute = project.GetAttributeScheme(frameDivisionChildScope,*it);
				if (!attribute.HasBinLabels()) continue;
				unsigned binSize = attribute.GetBinLabels().size();
				if (binSize!=12 && binSize!=24) continue; 
				attributeList << (frameDivisionChildScope+"::"+*it).c_str();
			}
		}
		if (attributeList.size()==0) return false;
		bool ok=false;
		QString chosen = QInputDialog::getItem(0, QObject::tr("Choose the attribute to take the pitches from"),
				QObject::tr("PCP attribute:"), attributeList, 0, false, &ok);
		if (!ok) return false;
		QStringList splitted = chosen.split("::");
		config.SetAttributeScope(splitted[0].toStdString());
		config.SetAttributeName(splitted[1].toStdString());
		return true;
	}
};

class KeySpacePlugin : public InstantViewPlugin
{
	virtual const char * id() const { return "KeySpace"; }
	virtual QString name() const { return QObject::tr("Key Space"); }
	CLAM::VM::PcpTorus * createView(QWidget * parent, const CLAM_Annotator::Project & project, CLAM_Annotator::InstantView & config)
	{
		CLAM::VM::KeySpace * view = new CLAM::VM::KeySpace(parent);
		view->setSource(project, config.GetAttributeScope(), config.GetAttributeName());
		return view;
	}
	virtual bool configureDialog(const CLAM_Annotator::Project & project, CLAM_Annotator::InstantView & config)
	{
		QStringList attributeList;
		const std::list<std::string>& divisionNames = project.GetNamesByScopeAndType("Song", "FrameDivision");
		std::list<std::string>::const_iterator divisionName;
		for(divisionName = divisionNames.begin();divisionName != divisionNames.end(); divisionName++)
		{
			const std::string & frameDivisionChildScope = project.GetAttributeScheme("Song", *divisionName).GetChildScope();
			const std::list<std::string>& descriptorsNames = project.GetNamesByScopeAndType(frameDivisionChildScope, "FloatArray");
			std::list<std::string>::const_iterator it;
			for(it = descriptorsNames.begin();it != descriptorsNames.end(); it++)
			{
				const CLAM_Annotator::SchemaAttribute & attribute = project.GetAttributeScheme(frameDivisionChildScope,*it);
				if (!attribute.HasBinLabels()) continue;
				unsigned binSize = attribute.GetBinLabels().size();
				if (binSize!=24) continue; 
				attributeList << (frameDivisionChildScope+"::"+*it).c_str();
			}
		}
		if (attributeList.size()==0) return false;
		bool ok=false;
		QString chosen = QInputDialog::getItem(0, QObject::tr("Choose the attribute to take the pitches from"),
				QObject::tr("PCP attribute:"), attributeList, 0, false, &ok);
		if (!ok) return false;
		QStringList splitted = chosen.split("::");
		config.SetAttributeScope(splitted[0].toStdString());
		config.SetAttributeName(splitted[1].toStdString());
		return true;
	}
};

Initializer::Initializer()
{
	plugins["KeySpace"] = new KeySpacePlugin;
	plugins["PcpTorus"] = new PcpTorusPlugin;
}



