#include "MonoOfflineNetworkPlayer.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include <fstream>

namespace CLAM
{

bool MonoOfflineNetworkPlayer::IsWorking()
{
	CacheSourcesAndSinks();
	return _filenames.size()!=GetNSinks()+GetNSources();
}

std::string MonoOfflineNetworkPlayer::NonWorkingReason()
{
	std::stringstream ss;
	ss << GetNSources()    << " inputs and "
		<< GetNSinks() << " outputs needed but just "
		<< _filenames.size() << " files provided" << std::ends;
	return ss.str();
}

void MonoOfflineNetworkPlayer::Start()
{
	if ( IsPlaying() )
		return;
	BePlaying();

	CacheSourcesAndSinks();

	const int frameSize = 512;
	const int sampleRate = 48000; //TODO deduce from inputs.

	std::vector<Audio> audioBuffers(_filenames.size());
	std::vector<MonoAudioFileReader*> readers;
	MonoAudioFileReaderConfig readercfg;
	unsigned fileIndex=0;
	for (unsigned i=0; i<GetNSources(); i++)
	{
		CLAM_ASSERT(fileIndex<_filenames.size(),
			"Not all the network inputs could be fullfiled. Have you checked the IsWorking() method?");
		{
			std::ifstream checkfile(_filenames[fileIndex].c_str());
			CLAM_ASSERT(checkfile.is_open(), std::string(std::string("Could not open one of the input files: ")+_filenames[fileIndex]).c_str());
		}
		readercfg.SetSourceFile(_filenames[fileIndex]);
		readercfg.SetLoop(_enableLoopInputWavs);
		MonoAudioFileReader * fileReader = new MonoAudioFileReader(readercfg);
		fileReader->GetOutPort("Samples Read").SetSize( frameSize );
		fileReader->GetOutPort("Samples Read").SetHop( frameSize );
		readers.push_back(fileReader);
		audioBuffers[fileIndex].SetSize( frameSize );

		TData * buffer = &audioBuffers[fileIndex].GetBuffer()[0];
		SetSourceBuffer(i, buffer, frameSize);
		std::cout << " In " << SourceName(i) << ":" << _filenames[fileIndex] << std::endl;
		fileIndex++;
	}

	std::vector<MonoAudioFileWriter*> writers;
	MonoAudioFileWriterConfig writercfg;
	for (unsigned i=0; i<GetNSinks(); i++)
	{
		if (fileIndex>=_filenames.size())
		{
			std::cerr <<
			"Not all the network outputs could be fullfiled.";
			break;
		}

		writercfg.SetTargetFile(_filenames[fileIndex]);
		writercfg.SetSampleRate(sampleRate);
		MonoAudioFileWriter * fileWriter = new MonoAudioFileWriter(writercfg);
		writers.push_back(fileWriter);
		audioBuffers[fileIndex].SetSize( frameSize );

		TData * buffer = &audioBuffers[fileIndex].GetBuffer()[0];
		SetSinkBuffer(i, buffer, frameSize);
		std::cout << " Out: " << SinkName(i) << ":" << _filenames[fileIndex] << std::endl;
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

void MonoOfflineNetworkPlayer::Stop()
{
	if ( IsStopped() ) return;
	BeStopped();
	//TODO close files
}

void MonoOfflineNetworkPlayer::ProcessInputFile()
{
	GetNetwork().Do();
}

void MonoOfflineNetworkPlayer::AddInputFile( const std::string& filename )
{
	_filenames.push_back(filename);
}

void MonoOfflineNetworkPlayer::AddOutputFile( const std::string& filename )
{
	_filenames.push_back(filename);
}

} //namespace

