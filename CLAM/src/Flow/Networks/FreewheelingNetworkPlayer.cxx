#include "FreewheelingNetworkPlayer.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"

namespace CLAM
{

FreewheelingNetworkPlayer::FreewheelingNetworkPlayer()
{
}
bool FreewheelingNetworkPlayer::IsWorking() const
{
	return true;
}
std::string FreewheelingNetworkPlayer::NonWorkingReason() const
{
	return "Still not implemented";
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
	AudioSource& source = *GetAudioSources().front();
	AudioSink& sink = *GetAudioSinks().front();

	// configure file reader and writer
	int frameSize = 512;

	MonoAudioFileReader fileReader;
	fileReader.GetOutPort("Samples Read").SetSize( frameSize );
	fileReader.GetOutPort("Samples Read").SetHop( frameSize );
	MonoAudioFileReaderConfig readercfg;
	readercfg.SetSourceFile(mInFilename);
	fileReader.Configure( readercfg );

	MonoAudioFileWriterConfig writercfg;
	writercfg.SetTargetFile(mOutFilename);
	writercfg.SetSampleRate(fileReader.GetAudioFile().GetHeader().GetSampleRate());
	MonoAudioFileWriter fileWriter;
	fileWriter.Configure( writercfg );
	
	// start processings. do now worry about network processings since network is calling us.
	fileReader.Start();
	fileWriter.Start();
	
	// start freewheeling!
	Audio incomingAudio;
	Audio outgoingAudio;
	//incomingAudio.SetSampleRate( file.GetHeader().GetSampleRate() );
	incomingAudio.SetSize( frameSize );
	outgoingAudio.SetSize( frameSize );
	
	source.SetExternalBuffer( &(incomingAudio.GetBuffer()[0]) ,frameSize);
	sink.SetExternalBuffer( &(outgoingAudio.GetBuffer()[0]) ,frameSize); 
	while ( fileReader.Do(incomingAudio) )
	{
		GetNetwork().Do();
		fileWriter.Do( outgoingAudio );
	}
	
	// close files (stop processings)
	fileReader.Stop();
	fileWriter.Stop();
	
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

void FreewheelingNetworkPlayer::InputFile( const std::string& filename )
{
	mInFilename = filename;
}
void FreewheelingNetworkPlayer::OutputFile( const std::string& filename )
{
	mOutFilename = filename;
}

} //namespace

