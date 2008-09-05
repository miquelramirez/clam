#include <CLAM/OfflineNetworkPlayer.hxx>
#include <CLAM/XMLStorage.hxx>
#include <fstream>
#include <sstream>
#include <sndfile.hh>

#ifdef USE_LADSPA
#	include <CLAM/RunTimeLadspaLibraryLoader.hxx>
#	include <CLAM/RunTimeFaustLibraryLoader.hxx> 
#endif

int usage(const std::string & program)
{
		std::cout << " Usage: " << program
			<< " <networkfile> [<infile.wav> [<infile.wav> ...] ] -o [<outfile.wav> [<outfile.wav> ...]]\n"
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
	CLAM::OfflineNetworkPlayer * player =  new CLAM::OfflineNetworkPlayer;
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
	CLAM_ASSERT(std::string(argv[argIndex]) != "-o","There are not intput files.");

	while(std::string(argv[argIndex]) != "-o")
	{
		player->AddInputFile(argv[argIndex]);
		argIndex++;
		CLAM_ASSERT(argIndex<argc,"There are not output files. The output files are separated by -o");
	}
	argIndex ++;

	if(std::string(argv[argIndex]) == "-f")
	{	
		if(argc <= ++argIndex)
		{
			std::cout << "Error: needed a format argument. The arguments posibles are 16, 24 signed data or 32, 64 float data. Example -c 16" << std::endl;
			return -1;
		}
		std::istringstream stream(argv[argIndex++]);
		int format;
		stream >> format;		
		switch(format)
		{
			case(16):	player->SetFormat(SF_FORMAT_WAV | SF_FORMAT_PCM_16);
					break;
			case(24):	player->SetFormat(SF_FORMAT_WAV | SF_FORMAT_PCM_24);
					break;
			case(32):	player->SetFormat(SF_FORMAT_WAV | SF_FORMAT_FLOAT);
					break;
			case(64):	player->SetFormat(SF_FORMAT_WAV | SF_FORMAT_DOUBLE);
					break;
			default: 	std::cout << " Error, the format type must be16, 24 signed data or 32, 64 float data"<<std::endl;
					return -1;
		}
	}
	else
	{	player->SetFormat(SF_FORMAT_WAV | SF_FORMAT_FLOAT);
	}

	int channel;
	for (; argIndex<argc; argIndex++)
	{		
		if(std::string(argv[argIndex]) == "-c")
		{
			if(argc <= ++argIndex)
			{
				std::cout << "Error: needed a channel argument and name outfile. Example -c 2 stereo_out.wav" << std::endl;
				return -1;
			}
			std::istringstream stream(argv[argIndex++]);
			stream >> channel;
		}
		else
		{
			channel = 1;
		}

		player->AddNumChannels(channel);			
		player->AddOutputFile(argv[argIndex]);
	}

	CLAM_ASSERT(argIndex == argc,"There are so many arguments");


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

