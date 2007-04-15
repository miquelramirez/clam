/*
 * AudioFileIO.cxx
 * Implementation for AudioFileIO class. 
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include "AudioFileIO.hxx"
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/MonoAudioFileWriter.hxx>

using namespace CLAM;

namespace V2M
{

AudioFileIO::AudioFileIO(){}
AudioFileIO::~AudioFileIO(){}

int AudioFileIO::Load(const char* fileName,Audio& out)
{
    MonoAudioFileReaderConfig cfg;
    cfg.SetSourceFile(fileName);

    MonoAudioFileReader infile;
    if (!infile.Configure(cfg))
        return -1;

    out.SetSize(infile.GetAudioFile().GetHeader().GetSamples());

    infile.Start();
    infile.Do(out);
    infile.Stop();

    return 0;
}

int AudioFileIO::Save(const char* fileName,Audio& in)
{
    MonoAudioFileWriterConfig cfg;
    cfg.SetTargetFile(fileName);
	cfg.SetSampleRate(in.GetSampleRate());
    MonoAudioFileWriter outfile;
    if (!outfile.Configure(cfg)) return -1;
	
    outfile.Start();
    outfile.Do(in);
    outfile.Stop(); 

    return 0;
}

} // namespace V2M

// END

