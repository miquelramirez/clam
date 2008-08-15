#ifndef LibloSource_hxx
#define LibloSource_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/OutControlArray.hxx>
#include <string>
#include <cmath>
#include <algorithm>

#include "lo/lo.h"
#ifdef GetClassName // windows.h introduces that
#undef GetClassName
#endif

namespace CLAM
{

class LibloSourceConfig : public CLAM::ProcessingConfig
{
	DYNAMIC_TYPE_USING_INTERFACE( LibloSourceConfig, 3, ProcessingConfig );
	DYN_ATTRIBUTE( 0, public, std::string, OscPath);
	DYN_ATTRIBUTE( 1, public, std::string, ServerPort);
	DYN_ATTRIBUTE( 2, public, CLAM::TData, NumberOfArguments);
	//TODO number of arguments/ports
protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
		SetNumberOfArguments(3);
		SetOscPath("/clam/target");
		SetServerPort("");
	};
};

class LibloSource : public CLAM::Processing
{
	typedef LibloSourceConfig Config;

public:
	LibloSource(const Config& config = Config()) 
		: _serverThread(0)
	{
		_serverThreadIsRunning=false;
		Configure( config );
	}
	
	~LibloSource()
	{
		RemoveOldControls();
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

        void RemoveOldControls()
        {
		_outControls.Clear();
		GetOutControls().Clear();
	}

	bool ConcreteConfigure(const CLAM::ProcessingConfig & config)
	{
		RemoveOldControls();
		CopyAsConcreteConfig(_config, config);
//		_config.AddAll();
//		_config.UpdateData();
//set outputs:
		int nOutputs = int(_config.GetNumberOfArguments());
		if (nOutputs < 1)
		{
			_config.SetNumberOfArguments(1);
			nOutputs = 1;
		}
		// multi-port names share user-configured identifier
		std::string path=_config.GetOscPath();
		std::replace(path.begin(),path.end(),'/','_');
		_outControls.Resize(nOutputs,path, this);

		if (_serverThreadIsRunning)
		{	
//			return true;
			std::cout <<"LibloSource: Restarting the server..."<<std::endl;
			lo_server_thread_free(_serverThread);
			_serverThreadIsRunning=false;
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
		_serverThread = lo_server_thread_new(port, error);
		/* add method that will match any path and args */
		lo_server_thread_add_method(_serverThread, NULL, NULL, generic_handler, this);

		std::string typespecMask="";
		for (int i=0;i<nOutputs;i++)
		{
			typespecMask+="f";
		}

		lo_server_thread_add_method(_serverThread, _config.GetOscPath().c_str(), typespecMask.c_str(), controls_handler, this);

		/* add method that will match the path /quit with no args */
		lo_server_thread_add_method(_serverThread, "/quit", "", quit_handler, this);

		lo_server_thread_start(_serverThread);

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
	
	lo_server_thread _serverThread;
	Config _config;
	OutControlArray _outControls;
};

} //namespace

#endif
