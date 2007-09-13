#include "LoadImpulseResponse.hxx"
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/AudioWindowing.hxx>
#include "MyFFT.hxx"

namespace CLAM
{

bool ComputeResponseSpectrums(const std::string & wavfile, std::vector<ComplexSpectrum> & responseSpectrums, unsigned framesize, std::string & errorMsg)
{
	MonoAudioFileReaderConfig readerConfig;
	readerConfig.SetSourceFile(wavfile);
	MonoAudioFileReader reader(readerConfig);
	if (!reader.IsConfigured())
	{
		errorMsg += "Configuring the inner AudioFileReader:\n";
		errorMsg += reader.GetConfigErrorMessage();
		return false;
	}
	const unsigned nSamples = reader.GetHeader().GetSamples();
	//std::cout << "ComputeResponseSpectrums: NSamples: " << nSamples << std::endl;

	AudioWindowingConfig windowerConfig;
	windowerConfig.SetSamplingRate(44100); // TODO: Take it from the file
	windowerConfig.SetHopSize(framesize);
	windowerConfig.SetWindowSize(framesize+1);
	windowerConfig.SetFFTSize(framesize*2);
	windowerConfig.SetDoHalfWindowShift(false);
	windowerConfig.SetWindowType(EWindowType::eNone);
	AudioWindowing windower(windowerConfig);
	if (!windower.IsConfigured())
	{
		errorMsg += "Configuring the inner AudioWindowing:\n";
		errorMsg += windower.GetConfigErrorMessage();
		return false;
	}
	FFTConfig fftConfig; 
	fftConfig.SetAudioSize(framesize*2);
	MyFFT fft(fftConfig);

	ConnectPorts(reader,0,windower,0);
	ConnectPorts(windower,0,fft,0);
	InPort<ComplexSpectrum> fetcher;
	fft.GetOutPorts().GetByNumber(0).ConnectToIn(fetcher);

	responseSpectrums.clear();
	reader.Start();
	windower.Start();
	fft.Start();
	
	for (bool samplesAvailable=true; samplesAvailable; )
	{
		samplesAvailable = reader.Do();
		if (!windower.CanConsumeAndProduce()) continue;
		windower.Do();
		fft.Do();
		responseSpectrums.push_back(fetcher.GetData());
//			fetcher.GetData().dump(std::cout);
		fetcher.Consume();
	}

	reader.Stop();
	windower.Stop();
	fft.Stop();
	return true;
}

} //namespace CLAM

