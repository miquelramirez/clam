#ifndef ProcessingDataPlugin_hxx
#define ProcessingDataPlugin_hxx

#include <string>
#include <cstdlib>
#include <map>
#include <list>
#include <typeinfo>
#include <iostream>

#ifdef __GNUC__
#include <cxxabi.h>
#endif//__GNUC__
namespace CLAM
{

class ProcessingDataPlugin
{
public:
	typedef std::string Key;
	typedef std::type_info Type;
	typedef std::map<Key,ProcessingDataPlugin *> TypeMap;
private:
	const Type & _type;
	std::string _color;
	std::string _displayName;
	std::string _name;
	static std::string demangle(const std::string & mangledName)
	{
		std::string result = mangledName;
		#ifdef __GNUC__
		int demangleError = 0;
		char * demangled = abi::__cxa_demangle(mangledName.c_str(),0,0,&demangleError);
		if (!demangleError && demangled) 
			result = demangled;
		if (demangled) free(demangled);
		#endif//__GNUC__
		return result;
	}
	ProcessingDataPlugin(const std::type_info & type, const std::string & color, const std::string & displayName)
		: _type(type)
		, _color(color)
	{
		_name = _displayName = type.name();
		_displayName = displayName.empty()?
			demangle(_name) : displayName;
//		std::cout << "Adding TypePlugin " << _name << " shown as " << _displayName << " with color " << color << std::endl;
		getTypeMap().insert(std::make_pair(_name, this));
	}
public:
	const std::string & color() const { return _color; }
	const std::string & name() const { return _name; }
	const std::string & displayName() const { return _displayName; }
private:
	static TypeMap & getTypeMap();
public:
	static std::list<std::string> types()
	{
		std::list<std::string> result;
		for (TypeMap::iterator it=getTypeMap().begin();
			it!=getTypeMap().end(); it++)
		{
			result.push_back(it->first);
		}
		return result;
	}
	static ProcessingDataPlugin * lookUp(const Type & type)
	{
		TypeMap::iterator it = getTypeMap().find(type.name());
		if (it==getTypeMap().end()) return 0;
		return it->second;
	}
	static std::string colorFor(const std::type_info & type)
	{
		CLAM::ProcessingDataPlugin * plugin = lookUp(type);
		if (plugin) return plugin->color();
		return "";
	}
	static std::string displayNameFor(const std::type_info & type)
	{
		CLAM::ProcessingDataPlugin * plugin = lookUp(type);
		if (plugin) return plugin->displayName();
		return demangle(type.name());
	}

public:
	template <typename DataType>
	class Registrator
	{
		ProcessingDataPlugin * _plugin;
	public:
		Registrator(const std::string & color, const std::string & displayName="")
			: _plugin( new ProcessingDataPlugin(typeid(DataType), color, displayName))
		{
		}
	};
};

}


#endif//ProcessingDataPlugin_hxx

