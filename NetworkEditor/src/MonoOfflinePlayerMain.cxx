#include <CLAM/MonoOfflineNetworkPlayer.hxx>
#include <CLAM/XMLStorage.hxx>
#include <fstream>
#include <sstream>
#include "NetworkEditorVersion.hxx"

#ifdef USE_LADSPA
#	include <CLAM/RunTimeLadspaLibraryLoader.hxx>
#	include <CLAM/RunTimeFaustLibraryLoader.hxx> 
#endif

int usage(const std::string & program)
{
	// If you change that remember to update the manpage with help2man
	// $ help2man -N ./MonoOfflinePlayer -n "Plays CLAM networks off-line"
	std::cout
		<< "Usage: MonoOfflinePlayer <networkfile> [-t SECONDS] [<infile.wav> ...] [<outfile.wav> ...]\n"
		<< "Executes a CLAM network off-line, by mapping audio sink\n"
		<< "and audio sources to wave files.\n"
		<< "Unlike OfflinePlayer, just mono files are generated on output, and,"
		<< "in the case of multichannel inputs, just the first channel is taken.\n"
		<< "Output wave file formats are chosen by extension (wav, aiff, ogg...)\n"
		<< "  -t SECONDS   Loops and/or cuts inputs to reach SECONDS seconds long.\n"
		<< "\nReport bugs to http://clam-project.org\n"
		<< std::endl;
	return -1;
}
int version()
{
	// If you change that remember to update the manpage with help2man
	// $ help2man -N ./MonoOfflinePlayer -n "Plays CLAM networks off-line"
	std::cout 
		<< "MonoOfflinePlayer " << NetworkEditor::GetFullVersion() << "\n"
		<< "Copyright (C) 2009, Universitat Pompeu Fabra.\n"
		<< "This is free software: you are free to change and redistribute it.\n"
		<< "There is NO WARRANTY, to the extent permitted by law.\n"
		<< "Licence GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl2.html>\n"
		<< "\n"
		<< "Written by Pau Arumí and David García.\n"
		<< std::endl;
	return 0;
}

int main( int argc, char *argv[] )
{

	if (argc < 2) return usage(argv[0]);
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
	std::string networkFile = argv[1];
	CLAM::Network net;
 	CLAM::XMLStorage::Restore(net, networkFile);
	CLAM::MonoOfflineNetworkPlayer * player =  new CLAM::MonoOfflineNetworkPlayer;
	net.SetPlayer( player ); // network owns the player memory	
	std::cout << "Sources and Sinks list:\n" << player->SourcesAndSinksToString() << std::endl;
	if (argc==2)
	{
		std::cout << "Inspecting Network." << std::endl;
		return 0;
	}
	int argIndex = 2;
	if (std::string(argv[argIndex])== "-t")
	{
		std::cout << "Resulting wavs time (in seconds) specified Mode" << std::endl;
		if (argc==3)
		{
			std::cout << "Error: needed a time argument. Example -t 10.5 (seconds)" << std::endl;
			return -1;
		}
		std::istringstream iss(argv[++argIndex]);
		double seconds;
		iss >> seconds;
		std::cout << "TIME (seconds) " << seconds << std::endl;
		player->EnableLoopInputWavs();
		player->SetResultWavsTime(seconds);
		argIndex++;
		
	}
	for (; argIndex<argc; argIndex++)
	{
		player->AddInputFile(argv[argIndex]);
	}
	if (not net.IsReady())
	{
		if (net.HasMisconfiguredProcessings())
			std::cerr << net.GetConfigurationErrors() << std::endl;
		else if (net.IsEmpty())
			std::cerr << "Empty network" << std::endl;
		else if (net.HasUnconnectedInPorts())
			std::cerr << "The network has unconnected inports" << std::endl;
		return -1;
	}
	net.Start();
	net.Stop();
	std::cout << "\nDone!" << std::endl;

	return 0;
}

