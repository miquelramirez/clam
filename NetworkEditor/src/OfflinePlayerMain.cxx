#include <CLAM/OfflineNetworkPlayer.hxx>
#include <CLAM/XMLStorage.hxx>
#include <fstream>
#include <sstream>
#include <sndfile.hh>
#include "NetworkEditorVersion.hxx"

#ifdef USE_LADSPA
#	include <CLAM/RunTimeLadspaLibraryLoader.hxx>
#	include <CLAM/RunTimeFaustLibraryLoader.hxx> 
#endif

int usage(const std::string & program)
{
	// If you change that remember to update the manpage with help2man
	// $ help2man -N ./OfflinePlayer -n "Plays CLAM networks off-line"
	std::cout
		<< "Usage: OfflinePlayer"
		<< " <networkfile> OPTIONS... [<inwave> ...] -o [[-c CHANNELS] <outwave> ...]\n"
		<< "Executes a CLAM network off-line, by mapping audio sink\n"
		<< "and audio sources to wave files.\n"
		<< "  -t SECONDS   Loops and/or cuts inputs to reach SECONDS seconds long.\n"
		<< "  -f BITS      Use PCM format of BITS depth for all output waves (16,24,32,64).\n"
		<< "  -o           Indicates that the next parameters are outputs instead inputs.\n"
		<< "  -c CHANNELS  Groups the next CHANNELS sinks in a the next file.\n"
		<< "When no wave file is provided, inputs and outputs available in the network are shown.\n"
		<< "\nReport bugs to http://clam-project.org\n"
		<< std::endl;
	return -1;
}

int version()
{
	// If you change that remember to update the manpage with help2man
	// $ help2man -N ./OfflinePlayer -n "Plays CLAM networks off-line"
	std::cout 
		<< "OfflinePlayer " << NetworkEditor::GetFullVersion() << "\n"
		<< "Copyright (C) 2009, Universitat Pompeu Fabra.\n"
		<< "This is free software: you are free to change and redistribute it.\n"
		<< "There is NO WARRANTY, to the extent permitted by law.\n"
		<< "Licence GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl2.html>\n"
		<< "\n"
		<< "Written by Ferran Orriols and Pau Arumí.\n"
		<< std::endl;
	return 0;
}

bool isWavFile(std::string filename)
{
	if (filename.size()<=5)
		return false;
	std::string originalExtension = "wav";
	int posDot = filename.size()-(originalExtension.size()+1);

	std::string fileExtension = filename.substr(posDot+1);

	if(filename.at(posDot)!='.')
		return false;
	
	if(fileExtension.compare(originalExtension)!=0)
		return false;

	return true;
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

	std::string networkFile = argv[1];
	CLAM::Network net;
	CLAM::OfflineNetworkPlayer * player =  new CLAM::OfflineNetworkPlayer;
	net.SetPlayer( player ); // network owns the player memory	
 	CLAM::XMLStorage::Restore(net, networkFile);

	if (argc==2)
	{
		std::cout << "Sources and Sinks list:\n" << player->SourcesAndSinksToString() << std::endl;
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

	if(std::string(argv[argIndex]) == "-o")
	{
		std::cout << "There are not intput files." << std::endl;
		return -1;
	}


	while(std::string(argv[argIndex]) != "-o")
	{
		if(!isWavFile(std::string(argv[argIndex])))
		{	std::cout << "Found bad input file. The input files "<<std::string(argv[argIndex])<<" have to finish with .wav" << std::endl;
			return -1;			
		}
		player->AddInputFile(argv[argIndex]);
		argIndex++;
		if(argc<=argIndex)
		{	std::cout << "There are not output files. The output files are separated by -o" << std::endl;
			return -1;
		}		
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
		if(!isWavFile(std::string(argv[argIndex])))
		{	std::cout << "The output files "<<std::string(argv[argIndex])<<" have to finish with .wav" << std::endl;
			return -1;			
		}
		player->AddNumChannels(channel);			
		player->AddOutputFile(argv[argIndex]);
	}

	if(argIndex != argc)
	{	std::cout << "There are so many arguments" << std::endl;
		return -1;
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

