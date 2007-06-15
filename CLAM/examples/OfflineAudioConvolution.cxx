#include <iostream>
#include <CLAM/Audio.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
int main()
{
	std::string impulseResponseFile = "/home/parumi/acustica-bm/Carnel_3P.wav";
	CLAM::MonoAudioFileReaderConfig config;
	config.SetSourceFile(impulseResponseFile);

	CLAM::MonoAudioFileReader reader(config);
	if (!reader.IsConfigured()) 
	{
		std::cout << reader.GetConfigErrorMessage() << std::endl;
		return -1;
	}
	reader.Start();

	long int nSamples = reader.GetHeader().GetSamples();
	std::cout << "samples of file: "<< impulseResponseFile << " " << nSamples << std::endl;

	CLAM::Audio impulseResponse;
	CLAM::Audio input;
	CLAM::Audio reverbered;
	impulseResponse.SetSize(1000);
	input.SetSize(400000);
	reverbered.SetSize( impulseResponse.GetSize()+input.GetSize() );
	std::cout << "hello" << std::endl;
	return 0;
}
