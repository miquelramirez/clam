#include <iostream>
#include <cmath>
#include <CLAM/Audio.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/MonoAudioFileWriter.hxx>
int error(const std::string & msg)
{
	std::cerr << msg << std::endl;
	return -1;
}

int main()
{
	const std::string impulseResponseFile = "/home/parumi/acustica-bm/Carnel_3P.wav";
	const std::string inputFile = "/home/parumi/acustica-bm/InLanguageAnecoic-cut.wav";
	const std::string reverberedFile = "out.wav"; // the output file

	CLAM::MonoAudioFileReaderConfig readerConfig;
	readerConfig.SetSourceFile(impulseResponseFile);

	CLAM::MonoAudioFileReader reader(readerConfig);
	if (!reader.IsConfigured()) return error(reader.GetConfigErrorMessage());
	reader.Start();
	long impulseSamples = reader.GetHeader().GetSamples();
	CLAM::Audio impulseResponse;
	impulseResponse.SetSize(impulseSamples);
	reader.Do( impulseResponse );
	reader.Stop();

	readerConfig.SetSourceFile(inputFile);
	reader.Configure(readerConfig);
	if (!reader.IsConfigured()) return error(reader.GetConfigErrorMessage());
	reader.Start();
	long inputSamples = reader.GetHeader().GetSamples();
	CLAM::Audio input;
	input.SetSize(inputSamples);
	reader.Do( input );
	reader.Stop();



//	long int inputSamples = reader.GetHeader().GetSamples();

	long reverberedSize = impulseResponse.GetSize()+input.GetSize();
	CLAM::Audio reverbered;
	reverbered.SetSize( reverberedSize );
	
	CLAM::TData * inputArray = &input.GetBuffer()[0];
	CLAM::TData * impulseArray = &impulseResponse.GetBuffer()[0];
	CLAM::TData * reverberedArray = &reverbered.GetBuffer()[0];
	unsigned frameSamples=0;
	for (long i=0; i<inputSamples; i++)
	{
		frameSamples++;
		frameSamples%=1000;
		if (!frameSamples)
			std::cout << "." << std::flush;
		for (long j=0; j<impulseSamples; j++)
			reverberedArray[i+j] += inputArray[i]*impulseArray[j];
	}
	std::cout << std::endl << "Done." << std::endl;
	// limiter:
	for (long i=0; i<reverberedSize; i++)
	{
		if (reverberedArray[i]>1.0) reverberedArray[i]=1.0;
		if (reverberedArray[i]<-1.0) reverberedArray[i]=-1.0;
	}
/*
	for (long i=0; i<impulseSamples; i++)
		std::cout << impulseResponse.GetBuffer()[i] << " ";
	for (long i=0; i<inputSamples; i++)
		std::cout << input.GetBuffer()[i] << " ";
	for (long i=0; i<reverberedSize; i++)
		std::cout << reverbered.GetBuffer()[i] << " ";
*/
	CLAM::MonoAudioFileWriterConfig writerConfig;
	writerConfig.SetTargetFile( reverberedFile );
	CLAM::MonoAudioFileWriter writer(writerConfig);
	if (!writer.IsConfigured()) return error(writer.GetConfigErrorMessage());
	writer.Start();
	writer.Do( reverbered );
	writer.Stop();


	return 0;
}
