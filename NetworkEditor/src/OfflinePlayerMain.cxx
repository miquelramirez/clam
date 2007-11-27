#include <CLAM/FreewheelingNetworkPlayer.hxx>
#include <CLAM/XMLStorage.hxx>
#include <fstream>
#ifdef _MSVC_VER
#include <CLAM/InitAudioIO.hxx>
#include <CLAM/InitProcessing.hxx>
#endif

int usage(const std::string & program)
{
		std::cout << " Usage: " << program
			<< " <networkfile> [<infile.wav> [<infile.wav> ...] ] [<outfile.wav> [<outfile.wav> ...]]\n"
			<< std::endl;
		return -1;
}

int main( int argc, char *argv[] )
{

#ifdef _MSVC_VER
	CLAM::ProcessingModule::init();
	CLAM::AudioIOModule::init();
#endif
	if (argc < 2)
		return usage(argv[0]);

	std::string networkFile = argv[1];
	CLAM::Network net;
 	CLAM::XMLStorage::Restore(net, networkFile);
	CLAM::FreewheelingNetworkPlayer * player =  new CLAM::FreewheelingNetworkPlayer;
	net.SetPlayer( player ); // network owns the player memory
	std::cout << "Sources and Sinks list:\n" << player->SourcesAndSinksToString() << std::endl;
	if (argc==2)
		return 0;
	for (int i=2; i<argc; i++)
	{
		player->AddInputFile(argv[i]);
	}
	net.Start();
	net.Stop();
	std::cout << "\nDone!" << std::endl;

	return 0;
}

