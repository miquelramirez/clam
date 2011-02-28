#include <CLAM/Network.hxx>
#include <CLAM/CLAMVersion.hxx>
#include <boost/python.hpp>
#include <iostream>
#include <string>


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

	void setupClamNetwork(CLAM::Network & network)
	{
		py::object clamProxyModule = py::import("Clam_NetworkProxy");
		py::object proxy = py::object(py::ptr(&network));
		setupNetwork(proxy);
	}

	void setupDummyNetwork()
	{
		py::object proxy = py::import("Dummy_NetworkProxy").attr("Dummy_NetworkProxy")();
		setupNetwork(proxy);
	}
private:
	void setupNetwork(py::object & proxy)
	{
		py::object networkClass = py::import("Network").attr("Network");
		_main_ns["net"] = networkClass(proxy);
	}
public:
	void run()
	{
		std::string shellBanner = "Interactive python console for CLAM "+std::string(CLAM::GetFullVersion());
		std::string shellHeader = "Start by typing 'net.' and pressing the tab key";
		std::string shellFarewell = "Bye";

		// This needs to be defined for ipython
		py::exec("sys.argv=[]\n", _main_ns, _main_ns);

		py::object shellmodule = py::import("IPython.Shell");
		py::list shellOptions;
		shellOptions.append("-prompt_in1");
		shellOptions.append("ipyclam > ");
		py::object shell = shellmodule.attr("IPShellEmbed")
			(shellOptions, shellBanner, shellFarewell);
		shell(shellHeader, _main_ns, _main_ns);
	}
};


int main( int argc, char ** argv )
{
	try
	{
		Py_Initialize();

		CLAM::Network network;
		IpyclamShell shell;
		if (argc>1 and std::string(argv[1])=="--dummy")
			shell.setupDummyNetwork();
		else
			shell.setupClamNetwork(network);

		shell.run();
	}
	catch (py::error_already_set & e)
	{
		std::cerr << "Run time Python error!" << std::endl;
		PyErr_Print();
	}
}



