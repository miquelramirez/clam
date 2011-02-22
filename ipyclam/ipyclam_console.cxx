#include <CLAM/Network.hxx>
#include <CLAM/CLAMVersion.hxx>
#include <boost/python.hpp>
#include <iostream>
#include <string>

int main( int argc, char ** argv )
{
	try
	{
		Py_Initialize();
		using namespace boost::python;

		object main = import("__main__");
		object main_ns = main.attr("__dict__");
		exec("import sys; sys.path.append('.')" , main_ns, main_ns);
		object networkClass = import("Network").attr("Network");

		CLAM::Network network;
//		object proxy = ptr(&network);
		object proxy = import("Dummy_NetworkProxy").attr("Dummy_NetworkProxy")();

		main_ns["net"] = networkClass(proxy);


		std::string shellBanner = "Interactive python console for CLAM "+std::string(CLAM::GetFullVersion());
		std::string shellHeader = "Start by typing 'net.' and pressing the tab key";
		std::string shellFarewell = "Bye";

		// This needs to be defined for ipython
		exec("sys.argv=[]\n", main_ns, main_ns);

		object shellmodule = import("IPython.Shell");
		list shellOptions;
		shellOptions.append("-prompt_in1");
		shellOptions.append("ipyclam > ");
		object shell = shellmodule.attr("IPShellEmbed")
			(shellOptions, shellBanner, shellFarewell);
		shell(shellHeader, main_ns, main_ns);

	}
	catch (boost::python::error_already_set & e)
	{
		std::cerr << "Run time Python error!" << std::endl;
		PyErr_Print();
	}
}



