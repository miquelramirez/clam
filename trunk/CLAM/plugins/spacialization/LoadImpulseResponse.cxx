#include "LoadImpulseResponse.hxx"
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/AudioWindowing.hxx>
#include "MyFFT.hxx"

namespace CLAM
{

bool computeResponseSpectrums(const std::string & wavfile, std::vector<ComplexSpectrum> & responseSpectrums, unsigned framesize, std::string & errorMsg)
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
//	const unsigned nSamples = reader.GetHeader().GetSamples();
//	std::cout << "ComputeResponseSpectrums: NSamples: " << nSamples << std::endl;

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
bool ImpulseResponseDatabase::loadImpulseResponseDatabase( 
		const std::string & filePrefix,
		unsigned frameSize,
		std::string & errorMsg )
{
	NXEmitter=8;
	NYEmitter=10;
	NZEmitter=1;
	NXReceiver=8;
	NYReceiver=10;
	NZReceiver=1;
	long unsigned totalFiles = NXEmitter*NYEmitter*NZEmitter*NXReceiver*NYReceiver*NZReceiver;
	_storage.resize( totalFiles);
	int percentDone=-1;
	std::cout << "Loading impulse response files: " << std::flush;
	unsigned i=0;
	for (unsigned xEmitter=0; xEmitter<NXEmitter; xEmitter++)
	for (unsigned yEmitter=0; yEmitter<NYEmitter; yEmitter++)
	for (unsigned zEmitter=0; zEmitter<NZEmitter; zEmitter++)
	for (unsigned xReceiver=0; xReceiver<NXReceiver; xReceiver++)
	for (unsigned yReceiver=0; yReceiver<NYReceiver; yReceiver++)
	for (unsigned zReceiver=0; zReceiver<NZReceiver; zReceiver++)
	{
		std::ostringstream os;
		os << filePrefix 
			<< "_emissor_"<< xEmitter << "-" << yEmitter << "-" << zEmitter
			<< "_receptor_" << xReceiver << "-" << yReceiver << "-" << zReceiver
			<< ".wav" << std::flush;
		if (!computeResponseSpectrums( os.str(), _storage[i], frameSize, errorMsg )) return false;
		i++;
		if (int(i/float(totalFiles)*10)>percentDone)
		{
			percentDone++;
			std::cout << percentDone*10 <<"% "<< std::flush;
		}
	}
	std::cout << std::endl;
	return true;

}

ImpulseResponse &  ImpulseResponseDatabase::get(int x1, int y1, int z1, int x2, int y2, int z2 )
{
	
	long unsigned index =
		z2 + NZReceiver * (
		y2 + NYReceiver * (
		x2 + NXReceiver * (
		z1 + NZEmitter * (
		y1 + NYEmitter * (
		x1
		)))));
//		std::cout << " x1 "<<x1 <<" y1 "<<y1<<" z1 "<<z1<<std::endl <<" x2 "<<x2<<" y2 "<<y2<<" z2 "<<z2 << std::endl<< "index "<<index <<std::endl;
	return _storage[index];
}

} //namespace CLAM

