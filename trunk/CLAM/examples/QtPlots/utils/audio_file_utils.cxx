#include "MonoAudioFileReader.hxx"
#include "MultiChannelAudioFileReader.hxx"
#include "audio_file_utils.hxx"

using namespace CLAM;

namespace qtvm_examples_utils
{
    int load_audio(const char* fileName,Audio& out)
    {
		AudioFileSource file;
		file.OpenExisting(fileName);
		if((!file.IsReadable()) | (file.GetHeader().GetChannels() > 1)) return 1; 
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

    int load_audio_st(const char* fileName,std::vector<Audio>& outputs)
    {
		AudioFileSource file;
		file.OpenExisting(fileName);

		if((!file.IsReadable()) | (file.GetHeader().GetChannels() != 2)) return 1;

		TSize readSize = TSize(TData(file.GetHeader().GetLength()/1000.0)*file.GetHeader().GetSampleRate());
		if(file.GetKind() == EAudioFileKind::ePCM)
		{
			readSize*=2;
		}

		outputs.resize(file.GetHeader().GetChannels());

		for(unsigned i = 0; i < outputs.size(); i++)
		{
			outputs[i].SetSize(readSize);
		}

		MultiChannelAudioFileReaderConfig cfg;
		cfg.SetSourceFile(file);

		MultiChannelAudioFileReader reader;
		reader.Configure(cfg);

		reader.Start();
		reader.Do(outputs); 
		reader.Stop();
	
		return 0;
    }
}

// END

