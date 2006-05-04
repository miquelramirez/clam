#ifndef InstantViewPlugin_hxx
#define InstantViewPlugin_hxx
#include <string>
#include <list>
#include <map>
#include <QtCore/QString>

namespace CLAM_Annotator { class Project; }
namespace CLAM_Annotator { class InstantView; }
namespace CLAM { namespace VM { class InstantView; } }
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
	virtual CLAM::VM::InstantView * createView(QWidget * parent, const CLAM_Annotator::Project & project, CLAM_Annotator::InstantView & config)=0;
public:
	static InstantViewPlugin * getPlugin(const std::string & id);
	static std::list<std::string> availablePlugins();
};





#endif//InstantViewPlugin_hxx
