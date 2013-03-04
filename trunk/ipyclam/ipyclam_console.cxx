//#include <CLAM/Network.hxx>
//#include <CLAM/CLAMVersion.hxx>
//#include <CLAM/JACKNetworkPlayer.hxx>
#include <boost/python.hpp>
#include <iostream>
#include <string>


bool hasattr(boost::python::object obj, std::string const &attrName)
{
	return PyObject_HasAttrString(obj.ptr(), attrName.c_str());
}

namespace py = boost::python;

class IpyclamShell
{
	py::object _main;
	py::object _main_ns;
public:
	IpyclamShell()
		: _main(py::import("__main__"))
		, _main_ns(_main.attr("__dict__"))
	{
		exec("import sys; sys.path.append('.')" , _main_ns, _main_ns);
	}
	void setupClamEngine()
	{
		py::object proxy = py::import("ipyclam").attr("Clam_Engine")();
		setupNetwork(proxy);
	}
/*
	void setupClamEngine(CLAM::Network & network)
	{
		py::object clamProxyModule = py::import("ipyclam");
		py::object proxy = py::object(py::ptr(&network));
		setupNetwork(proxy);
	}
*/
	void setupJackEngine()
	{
		py::object proxy = py::import("ipyclam").attr("Jack_Engine")();
		setupNetwork(proxy);
	}
	void setupDummyEngine()
	{
		py::object proxy = py::import("ipyclam").attr("Dummy_Engine")();
		setupNetwork(proxy);
	}
	void setupEngine(const std::string & engine)
	{
		std::cout << "Engine: " << engine << std::endl;
		py::object proxy = py::import("ipyclam").attr(engine.c_str())();
		setupNetwork(proxy);
	}
private:
	void setupNetwork(py::object & proxy)
	{
		py::object networkClass = py::import("ipyclam").attr("Network");
		_main_ns["net"] = networkClass(proxy);
	}
public:
	void run()
	{
		std::string shellBanner =
			"Interactive Python console for CLAM ";// + std::string(CLAM::GetFullVersion()) + "\n";
		std::string shellHeader = "Start by typing 'net.' and pressing the tab key";
		std::string shellFarewell = "Bye";

		// This needs to be defined for ipython
		py::exec("sys.argv=['ipyclam']\n", _main_ns, _main_ns);

		py::object configmodule = py::import("IPython.config.loader");
		py::object configClass = configmodule.attr("Config");
		py::object config = configClass();
		config.attr("PromptManager").attr("in_template") = "ipyclam[\\#] > ";

		py::object shellmodule = py::import("IPython.frontend.terminal.embed");
		py::dict shellConstructorArgs;
		shellConstructorArgs["config"]=config;
		shellConstructorArgs["exit_msg"] = shellFarewell;
		shellConstructorArgs["banner1"] = shellBanner;

		py::object shell = shellmodule.attr("InteractiveShellEmbed")
			(*py::tuple(), **shellConstructorArgs);
		py::dict shellCallArgs;
		shell(shellHeader, _main_ns, py::object(), 0, py::object(), _main_ns );
	}
};


int main( int argc, char ** argv )
{
	try
	{
		Py_Initialize();

		IpyclamShell shell;
		if (argc>1)
		{
			std::string option = argv[1];
			if (option=="--dummy")
				shell.setupEngine("Dummy_Engine");
			else if (option=="--jack")
				shell.setupEngine("Jack_Engine");
		}
		else 
			shell.setupEngine("Clam_Engine");

		shell.run();
	}
	catch (py::error_already_set & e)
	{
		std::cerr << "Run time Python error!" << std::endl;
		PyErr_Print();
	}
}



