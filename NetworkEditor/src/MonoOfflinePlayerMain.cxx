#include <CLAM/MonoOfflineNetworkPlayer.hxx>
#include <CLAM/XMLStorage.hxx>
#include <fstream>
#include <sstream>

#ifdef USE_LADSPA
#	include <CLAM/RunTimeLadspaLibraryLoader.hxx>
#	include <CLAM/RunTimeFaustLibraryLoader.hxx> 
#endif

int usage(const std::string & program)
{
		std::cout << " Usage: " << program
			<< " <networkfile> [-t <seconds to process>] [<infile.wav> [<infile.wav> ...] ] [<outfile.wav> [<outfile.wav> ...]]\n"
			<< std::endl;
		return -1;
}

int main( int argc, char *argv[] )
{

#ifdef USE_LADSPA
	RunTimeLadspaLibraryLoader ladspaLoader;
	ladspaLoader.Load();
	RunTimeFaustLibraryLoader faustLoader;
	faustLoader.Load();
#endif
	if (argc < 2)
		return usage(argv[0]);

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

