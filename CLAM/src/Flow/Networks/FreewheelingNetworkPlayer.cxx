#include "FreewheelingNetworkPlayer.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include <fstream>

namespace CLAM
{

FreewheelingNetworkPlayer::FreewheelingNetworkPlayer()
{
}
bool FreewheelingNetworkPlayer::IsWorking() const
{
	return mFilenames.size()!=GetAudioSinks().size()+GetAudioSources().size();
}
std::string FreewheelingNetworkPlayer::NonWorkingReason() const
{
	std::stringstream ss;
	ss << GetAudioSources().size() << " inputs and " 
		<< GetAudioSinks().size() << " outputs needed but just " 
		<< mFilenames.size() << " files provided" << std::ends;
	return ss.str();
}
void FreewheelingNetworkPlayer::Start()
{
	if ( !IsStopped() ) return;
	SetStopped(false);
	CollectSourcesAndSinks();
	if (GetAudioSources().size()>1)
	{
		std::cerr << "Warning: FreewheelingNetworkPlayer is going to use only one AudioSource. "
		<< "Network have " << GetAudioSources().size() << std::endl;
	}
	if (GetAudioSinks().size()>1)
	{
		std::cerr << "Warning: FreewheelingNetworkPlayer is going to use only one AudioSink. "
		<< "Network have " << GetAudioSinks().size() << std::endl;
	}
	const int frameSize = 512;
	std::vector<Audio> audioBuffers(mFilenames.size());
	std::vector<MonoAudioFileReader*> readers;
	MonoAudioFileReaderConfig readercfg;
	unsigned fileIndex=0;
	for (unsigned i=0; i<GetAudioSources().size(); i++)
	{
		CLAM_ASSERT(fileIndex<mFilenames.size(),
			"Not all the network inputs could be fullfiled. Have you checked the IsWorking() method?");
		std::ifstream checkfile(mFilenames[fileIndex].c_str());
		CLAM_ASSERT(checkfile.is_open(), std::string(std::string("Could not open one of the input files: ")+mFilenames[fileIndex]).c_str());
		AudioSource& source = *GetAudioSources()[fileIndex];
		readercfg.SetSourceFile(mFilenames[fileIndex]);
		MonoAudioFileReader * fileReader = new MonoAudioFileReader(readercfg);
		fileReader->GetOutPort("Samples Read").SetSize( frameSize );
		fileReader->GetOutPort("Samples Read").SetHop( frameSize );
		readers.push_back(fileReader);
		audioBuffers[fileIndex].SetSize( frameSize );
		source.SetExternalBuffer( &(audioBuffers[fileIndex].GetBuffer()[0]) ,frameSize);
		std::cout << " In: " << mFilenames[fileIndex] << std::endl;
		fileIndex++;
	}
	std::vector<MonoAudioFileWriter*> writers;
	MonoAudioFileWriterConfig writercfg;
	for (unsigned i=0; i<GetAudioSinks().size(); i++)
	{
		if (fileIndex>=mFilenames.size())
		{
			std::cerr <<
			"Not all the network outputs could be fullfiled.";
			break;
		}
		AudioSink& sink = *GetAudioSinks()[i];
		writercfg.SetTargetFile(mFilenames[fileIndex]);
		writercfg.SetSampleRate(48000); //TODO deduce from inputs.
		MonoAudioFileWriter * fileWriter = new MonoAudioFileWriter(writercfg);
		writers.push_back(fileWriter);
		audioBuffers[fileIndex].SetSize( frameSize );
		sink.SetExternalBuffer( &(audioBuffers[fileIndex].GetBuffer()[0]) ,frameSize);
		std::cout << " Out: " << mFilenames[fileIndex] << std::endl;
		fileIndex++;
	}
	
	for (unsigned i = 0; i<readers.size(); i++)
		readers[i]->Start();
	for (unsigned i = 0; i<writers.size(); i++)
		writers[i]->Start();
	while ( true )
	{
		std::cout << "." << std::flush;
		unsigned audioIndex =0;
		bool end=false;
		for (unsigned i = 0; i<readers.size(); i++)
			end |= !readers[i]->Do(audioBuffers[audioIndex++]);
		if (end) break;
		GetNetwork().Do();
		for (unsigned i = 0; i<writers.size(); i++)
			writers[i]->Do(audioBuffers[audioIndex++]);
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
	mFilenames.push_back(filename);
}
void FreewheelingNetworkPlayer::AddOutputFile( const std::string& filename )
{
	mFilenames.push_back(filename);
}

} //namespace

