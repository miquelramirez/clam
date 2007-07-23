#ifndef ProcessingDataPlugin_hxx
#define ProcessingDataPlugin_hxx

#include <string>
#include <map>
#include <typeinfo>
#include <iostream>

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
public:
	ProcessingDataPlugin(const std::type_info & type, const std::string & color)
		: _type(type)
		, _color(color)
	{
		std::cout << "Adding TypePlugin " << type.name() << " with color " << color << std::endl;
		getTypeMap().insert(std::make_pair(type.name(), this));
	}
	const std::string & color() const { return _color; }
private:
	static TypeMap & getTypeMap();
public:
	static ProcessingDataPlugin * lookUp(const Type & type)
	{
		TypeMap::iterator it = getTypeMap().find(type.name());
		if (it==getTypeMap().end()) return 0;
		return it->second;
	}
public:
	template <typename DataType>
	class Registrator
	{
		ProcessingDataPlugin * _plugin;
	public:
		Registrator(const std::string & color)
			: _plugin( new ProcessingDataPlugin(typeid(DataType), color))
		{
		}
	};
};

}


#endif//ProcessingDataPlugin_hxx

