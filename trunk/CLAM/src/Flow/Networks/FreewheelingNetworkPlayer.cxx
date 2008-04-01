#include "FreewheelingNetworkPlayer.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include <fstream>

namespace CLAM
{

bool FreewheelingNetworkPlayer::IsWorking() const
{
	return _filenames.size()!=GetAudioSinks().size()+GetAudioSources().size();
}
std::string FreewheelingNetworkPlayer::NonWorkingReason() const
{
	std::stringstream ss;
	ss << GetAudioSources().size() << " inputs and " 
		<< GetAudioSinks().size() << " outputs needed but just " 
		<< _filenames.size() << " files provided" << std::ends;
	return ss.str();
}
void FreewheelingNetworkPlayer::Start()
{
	if ( !IsStopped() ) return;
	SetStopped(false);
	CollectSourcesAndSinks();

	const int frameSize = 512;
	const int sampleRate = 48000; //TODO deduce from inputs.

	std::vector<Audio> audioBuffers(_filenames.size());
	std::vector<MonoAudioFileReader*> readers;
	MonoAudioFileReaderConfig readercfg;
	unsigned fileIndex=0;
	for (unsigned i=0; i<GetAudioSources().size(); i++)
	{
		CLAM_ASSERT(fileIndex<_filenames.size(),
			"Not all the network inputs could be fullfiled. Have you checked the IsWorking() method?");
		std::ifstream checkfile(_filenames[fileIndex].c_str());
		CLAM_ASSERT(checkfile.is_open(), std::string(std::string("Could not open one of the input files: ")+_filenames[fileIndex]).c_str());
		AudioSource& source = *GetAudioSources()[fileIndex];
		readercfg.SetSourceFile(_filenames[fileIndex]);
		readercfg.SetLoop(_enableLoopInputWavs);
		MonoAudioFileReader * fileReader = new MonoAudioFileReader(readercfg);
		fileReader->GetOutPort("Samples Read").SetSize( frameSize );
		fileReader->GetOutPort("Samples Read").SetHop( frameSize );
		readers.push_back(fileReader);
		audioBuffers[fileIndex].SetSize( frameSize );
		source.SetExternalBuffer( &(audioBuffers[fileIndex].GetBuffer()[0]) ,frameSize);
		std::cout << " In: " << _filenames[fileIndex] << std::endl;
		fileIndex++;
	}
	std::vector<MonoAudioFileWriter*> writers;
	MonoAudioFileWriterConfig writercfg;
	for (unsigned i=0; i<GetAudioSinks().size(); i++)
	{
		if (fileIndex>=_filenames.size())
		{
			std::cerr <<
			"Not all the network outputs could be fullfiled.";
			break;
		}
		AudioSink& sink = *GetAudioSinks()[i];
		writercfg.SetTargetFile(_filenames[fileIndex]);
		writercfg.SetSampleRate(sampleRate);
		MonoAudioFileWriter * fileWriter = new MonoAudioFileWriter(writercfg);
		writers.push_back(fileWriter);
		audioBuffers[fileIndex].SetSize( frameSize );
		sink.SetExternalBuffer( &(audioBuffers[fileIndex].GetBuffer()[0]) ,frameSize);
		std::cout << " Out: " << _filenames[fileIndex] << std::endl;
		fileIndex++;
	}
	
	for (unsigned i = 0; i<readers.size(); i++)
		readers[i]->Start();
	for (unsigned i = 0; i<writers.size(); i++)
		writers[i]->Start();

	long iterationIndex = 0;
	bool timeLimitedMode = _resultWavsTime > 0.001;
	while ( true )
	{
		std::cout << "." << std::flush;
		unsigned audioIndex =0;
		bool someInputFinished=false;
		for (unsigned i = 0; i<readers.size(); i++)
			someInputFinished |= !readers[i]->Do(audioBuffers[audioIndex++]);
		if (someInputFinished and not _enableLoopInputWavs ) break;
		GetNetwork().Do();
		for (unsigned i = 0; i<writers.size(); i++)
			writers[i]->Do(audioBuffers[audioIndex++]);
		if (timeLimitedMode and float(iterationIndex*frameSize)/sampleRate > _resultWavsTime)
		{
			std::cout << "REACHED MAX TIME - finalizing"<< std::endl;
			break;
		}
		iterationIndex++;
	}
	for (unsigned i = 0; i<readers.size(); i++)
		readers[i]->Stop();
	for (unsigned i = 0; i<writers.size(); i++)
		writers[i]->Stop();
}
void FreewheelingNetworkPlayer::Stop()
{
	if ( IsStopped() ) return;
	SetStopped(true);
	//TODO close files
}

void FreewheelingNetworkPlayer::ProcessInputFile()
{
	GetNetwork().Do();
}

void FreewheelingNetworkPlayer::AddInputFile( const std::string& filename )
{
	_filenames.push_back(filename);
}
void FreewheelingNetworkPlayer::AddOutputFile( const std::string& filename )
{
	_filenames.push_back(filename);
}

} //namespace

