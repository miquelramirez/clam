/*
 * AudioFileIO.cxx
 * Implementation for AudioFileIO class. 
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include "AudioFileIO.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"

using namespace CLAM;

AudioFileIO::AudioFileIO(){}
AudioFileIO::~AudioFileIO(){}

int AudioFileIO::Load(const char* fileName,Audio& out)
{
	AudioFile file;
	file.OpenExisting(fileName);

	if((!file.IsReadable()) | (file.GetHeader().GetChannels() > 1))
		return -1; // no es legible o no es mono

	out.SetSize(file.GetHeader().GetSamples());

	MonoAudioFileReaderConfig cfg;
	cfg.SetSourceFile(file);

	MonoAudioFileReader infile;
	infile.Configure(cfg);

	infile.Start();
	infile.Do(out);
	infile.Stop();

	return 0;
}

int AudioFileIO::Save(const char* fileName,Audio& in)
{
	AudioFile file;
	AudioFileHeader header;
	header.SetValues(in.GetSampleRate(),1,EAudioFileFormat::eWAV);
	
	file.CreateNew(fileName, header);

	if(!file.IsWritable()) return -1;

	MonoAudioFileWriterConfig cfg;
	cfg.AddTargetFile();
	cfg.UpdateData();
	cfg.SetTargetFile(file);

	MonoAudioFileWriter outfile;
	outfile.Configure(cfg);
	
	outfile.Start();
	outfile.Do(in);
	outfile.Stop(); 

	return 0;
}

// END

