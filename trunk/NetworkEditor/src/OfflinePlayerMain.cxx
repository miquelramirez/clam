#include <CLAM/FreewheelingNetworkPlayer.hxx>
#include <CLAM/XMLStorage.hxx>

#ifdef WIN32
#include <CLAM/InitAudioIO.hxx>
#include <CLAM/InitProcessing.hxx>
#endif

int usage(const std::string & program)
{
		std::cout << " Usage: " << program
			<< " <networkfile> <infile.wav> <outfile.wav>\n"
			<< std::endl;
		return -1;
}

int main( int argc, char *argv[] )
{

#ifdef WIN32
	CLAM::ProcessingModule::init();
	CLAM::AudioIOModule::init();
#endif
	if (argc != 4)
		return usage(argv[0]);

	std::string networkFile = argv[1];
	std::string inputFile = argv[2];
	std::string outputFile = argv[3];
	std::cout << "\nUsing the following files. Network: "<< networkFile << " Input: "<< inputFile
		<< " Output: " << outputFile << std::endl;

	CLAM::Network net;
 	CLAM::XMLStorage::Restore(net, networkFile);
	CLAM::FreewheelingNetworkPlayer * player =  new CLAM::FreewheelingNetworkPlayer;
	net.SetPlayer( player ); // network owns the player memory
	player->InputFile(inputFile);
	player->OutputFile(outputFile);
	net.Start();
	net.Stop();
	std::cout << "\nDone!" << std::endl;

	return 0;
}

