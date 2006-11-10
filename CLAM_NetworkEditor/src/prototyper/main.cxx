#ifdef WIN32
	#include <CLAM/InitAudioIO.hxx>
	#include <CLAM/InitProcessing.hxx>
#endif

#include "PrototypeLoader.hxx"
#include <QtGui/QApplication>
#include <iostream>

int usage(const std::string & program)
{
		std::cout << " Usage: " << program
			<< " <networkfile> [ <uifile> ] [-b <ALSA|JACK> ]"
			<< std::endl;
		return -1;
}

int main( int argc, char *argv[] )
{

#ifdef WIN32
	CLAM::ProcessingModule::init();
	CLAM::AudioIOModule::init();
#endif

	QApplication app( argc, argv );

	std::string networkFile;
	std::string uiFile;
	std::list<std::string> backends;

	enum { none, backend } optionArgument = none;
	int argument=0;
	for (int i=1; i<argc; i++)
	{
		std::string arg = argv[i];
		if (optionArgument!=none)
		{
			if (optionArgument==backend)
			   	backends.push_back(arg);
			optionArgument=none;
			continue;
		}
		if (arg[0]=='-')
		{
			if (arg=="-b")
			{
				optionArgument=backend;
				continue;
			}
			std::cerr << "Invalid option '" << arg << "'." << std::endl;
			return usage(argv[0]);
		}
		switch (argument++)
		{
			case 0:
				networkFile = arg;
			break;
			case 1:
				uiFile = arg;
			break;
			default:
				std::cerr << "Too much arguments." << std::endl;
				return usage(argv[0]);
		}
	}

	CLAM::PrototypeLoader prototype;

	if (not prototype.LoadNetwork( networkFile ) ) return -1;
	if (not prototype.ChooseBackend( backends ) ) return -1;
	if (not prototype.LoadInterface( uiFile.c_str() ) ) return -1;
	prototype.ConnectWithNetwork();

	prototype.Show();
	prototype.Start();
	int result = app.exec();
	prototype.Stop();

	return result;
}

