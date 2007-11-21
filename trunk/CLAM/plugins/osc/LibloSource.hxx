#ifndef LibloSource_hxx
#define LibloSource_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControl.hxx>
#include <string>
#include <cmath>
#include "lo/lo.h"


namespace CLAM
{

class LibloSourceConfig : public CLAM::ProcessingConfig
{ 
    DYNAMIC_TYPE_USING_INTERFACE( LibloSourceConfig, 1, ProcessingConfig );
    DYN_ATTRIBUTE( 0, public, std::string, OscPath);
protected:
    void DefaultInit()
    {
          AddAll();
          UpdateData();
          SetOscPath("/clam/target");
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
		_dummy=0;
		Configure( config );
	}
	const CLAM::ProcessingConfig & GetConfig() const
	{
		return _config;
	}
	bool Do()
	{
//		_out.SendControl(std::sin(_dummy));
		_dummy += 0.001;
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

		std::cout << "Starting the server" << std::endl;
		_serverThreadIsRunning=true;
		/* start a new server on port 7770 */
		lo_server_thread st = lo_server_thread_new("7770", error);
		/* add method that will match any path and args */
		lo_server_thread_add_method(st, NULL, NULL, generic_handler, this);

		/* add method that will match the path /foo/bar, with two numbers, coerced
		* to float and int */
		lo_server_thread_add_method(st, "/foo/bar", "fi", foo_handler, this);

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

	static int foo_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data);

	static int controls_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data);

	static int quit_handler(const char *path, const char *types, lo_arg **argv, int argc,
			 void *data, void *user_data);

	float _dummy;
	bool _serverThreadIsRunning;

	Config _config;
	OutControl _out1;
	OutControl _out2;
	OutControl _out3;
};

} //namespace

#endif
