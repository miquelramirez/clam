#ifdef _MSVC_VER
	#include <CLAM/InitAudioIO.hxx>
	#include <CLAM/InitProcessing.hxx>
#endif

#include "PrototypeLoader.hxx"
#include <QtGui/QApplication>
#include <iostream>

int usage(const std::string & program)
{
		std::cout << " Usage: " << program
			<< " <networkfile> [ <uifile> ] [-o] [-b <backend> [-b <backend> ...]]\n"
			<< "Options:\n"
			<< " -o             Non interactive mode\n"
			<< " -b <backend>   Try backend (portaudio, alsa, jack).\n"
			<< std::endl;
		return -1;
}

int main( int argc, char *argv[] )
{

#ifdef _MSVC_VER
	CLAM::ProcessingModule::init();
	CLAM::AudioIOModule::init();
#endif

	QApplication app( argc, argv );

	std::string networkFile;
	std::string uiFile;
	std::list<std::string> backends;
	bool isInteractive = true;

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
			if (arg=="-o")
			{
				isInteractive=false;
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
	QString undottedName = networkFile.c_str();
	undottedName= undottedName.mid(undottedName.lastIndexOf("/")+1, -1);
	undottedName.truncate(undottedName.indexOf("."));

	CLAM::PrototypeLoader prototype;

	if (! prototype.ChooseBackend( backends, undottedName.toStdString() ) ) return -1;
	if (! prototype.LoadNetwork( networkFile ) ) return -1;
	if (!isInteractive) 
	{
		prototype.Start();
		while (true) ; // Until Ctrl-C
	}
	if (! prototype.LoadInterface( uiFile.c_str() ) ) return -1;
	prototype.Show();
	prototype.ConnectWithNetwork();

	prototype.Start();
	int result = app.exec();
	prototype.Stop();

	return result;
}

