#ifndef LibloSource_hxx
#define LibloSource_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <string>
#include <cmath>
#include "lo/lo.h"
#ifdef GetClassName // windows.h introduces that
#undef GetClassName
#endif


namespace CLAM
{

class LibloSourceConfig : public CLAM::ProcessingConfig
{ 
    DYNAMIC_TYPE_USING_INTERFACE( LibloSourceConfig, 2, ProcessingConfig );
    DYN_ATTRIBUTE( 0, public, std::string, OscPath);
    DYN_ATTRIBUTE( 1, public, std::string, ServerPort);
    //TODO number of arguments/ports
protected:
    void DefaultInit()
    {
          AddAll();
          UpdateData();
          SetOscPath("/clam/target");
          SetServerPort("");
    };
};

class LibloSource : public CLAM::Processing
{
	typedef LibloSourceConfig Config;

public:
	LibloSource(const Config& config = Config()) 
		: _out1("osc to control 1", this)
		, _out2("osc to control 2", this)
		, _out3("osc to control 3", this)
	{
		_serverThreadIsRunning=false;
		Configure( config );
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}
	bool Do()
	{
		return true;
	}
	const char* GetClassName() const
	{
		return "LibloSource";
	}
protected:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		CopyAsConcreteConfig(_config, config);

		if (_serverThreadIsRunning)
		{	
			return true;
		}

		/* start a new server on port 7770 */
		if (_config.GetServerPort()=="")
		{
			std::cout << "LibloSource::ConcreteConfigure server NOT started -- default config" << std::endl;
			return true;
		}

		std::cout << "Starting the server" << std::endl;
		_serverThreadIsRunning=true;
		const char * port = _config.GetServerPort().c_str();
		std::cout << "LibloSource::ConcreteConfigure: STARTING the server. port " << port << std::endl;
		lo_server_thread st = lo_server_thread_new(port, error);
		/* add method that will match any path and args */
		lo_server_thread_add_method(st, NULL, NULL, generic_handler, this);

		lo_server_thread_add_method(st, _config.GetOscPath().c_str(), "fff", controls_handler, this);

		/* add method that will match the path /quit with no args */
		lo_server_thread_add_method(st, "/quit", "", quit_handler, this);

		lo_server_thread_start(st);

		return true; // Configuration ok
	}
private:
	static void error(int num, const char *m, const char *path);

	static int generic_handler(const char *path, const char *types, lo_arg **argv,
			    int argc, void *data, void *user_data);

	static int controls_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data);

	static int quit_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data);

	bool _serverThreadIsRunning;

	Config _config;
	OutControl _out1;
	OutControl _out2;
	OutControl _out3;
};

} //namespace

#endif
