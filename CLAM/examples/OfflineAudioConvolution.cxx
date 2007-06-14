
#include <iostream>
#include <CLAM/Audio.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
int main()
{
	std::string impulseResponseFile = "~/acustica-bm/Carnel_3P.wav";
	CLAM::MonoAudioFileReaderConfig config;
	config.SetSourceFile(impulseResponseFile);

	CLAM::MonoAudioFileReader reader(config);
	reader.Start();
	std::cout << "samples of file: "<< impulseResponseFile << " "
		<< reader.GetAudioFile().GetHeader().GetSamples() << std::endl;

	CLAM::Audio impulseResponse;
	CLAM::Audio input;
	CLAM::Audio reverbered;
	impulseResponse.SetSize(1000);
	input.SetSize(400000);
	reverbered.SetSize( impulseResponse.GetSize()+input.GetSize() );
	std::cout << "hello" << std::endl;
	return 0;
}
