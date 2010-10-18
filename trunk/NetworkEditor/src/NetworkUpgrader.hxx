#ifndef NetworkUpgrader_hxx
#define NetworkUpgrader_hxx

#include <string>

class NetworkUpgrader
{
	class Impl;
	Impl * _impl;
public:
	NetworkUpgrader();
	~NetworkUpgrader();
//	NetworkUpgrader(std::string & migrationScript);
	const char * run(const std::string & networkContents);
	const char * errorMessage() const;
};

#endif//NetworkUpgrader_hxx
