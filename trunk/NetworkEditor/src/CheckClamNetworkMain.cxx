#include "CheckClamNetwork.hxx"
#include "NetworkEditorVersion.hxx"

#ifdef USE_LADSPA
#	include <CLAM/RunTimeLadspaLibraryLoader.hxx>
#	include <CLAM/RunTimeFaustLibraryLoader.hxx>
#endif

int usage(const std::string & program)
{
	std::cout
		<< "Usage: CheckClamNetwork"
		<< " <networkfile> \n"
		<<
			"Checks that a CLAM network XML file can be safely loaded.\n"
			"Reports problems with XML syntax, unavalaible processing types,\n"
			"wrong configurations, miss-connections or problems on start\n"
			"or stopping the network. No processing step is done.\n"
			"Returns non-zero on error.\n"
		<< "\nReport bugs to http://clam-project.org\n"
		<< std::endl;
	return -1;
}

int version()
{
	std::cout
		<< "CheckClamNetwork " << NetworkEditor::GetFullVersion() << "\n"
		<< "Copyright (C) 2009, Universitat Pompeu Fabra.\n"
		<< "This is free software: you are free to change and redistribute it.\n"
		<< "There is NO WARRANTY, to the extent permitted by law.\n"
		<< "Licence GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl2.html>\n"
		<< "\n"
		<< "Written by Angelo Scorza and Pau Arumí.\n"
		<< std::endl;
	return 0;
}

int main( int argc, char *argv[] )
{
    if (argc < 2)
		return usage(argv[0]);

	for (int i=0; i<argc; i++)
	{
		if (argv[i] == std::string("--help")) return usage(argv[0]);
		if (argv[i] == std::string("-h")) return usage(argv[0]);
		if (argv[i] == std::string("--version")) return version();
	}

#ifdef USE_LADSPA
	RunTimeLadspaLibraryLoader ladspaLoader;
	ladspaLoader.Load();
	RunTimeFaustLibraryLoader faustLoader;
	faustLoader.Load();
#endif

	//TODO tabs
	//TODO one argument and return 0 or -1

	for(int i=1;i<argc;i++)
	{
		std::string networkFile = argv[i];
		if(CheckClamNetwork::isCorrectNetwork(networkFile))
		{
		std::cout << "\033[32mCorrect.\033[0m" << std::endl;
		continue;
		}

		std::cout << "\033[31mFailed.\033[0m" << std::endl;
		return -1;
	}
	return 0;
}
