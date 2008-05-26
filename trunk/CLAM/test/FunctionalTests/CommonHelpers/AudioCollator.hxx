#ifndef AudioCollator_hxx
#define AudioCollator_hxx

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include <CLAM/Audio.hxx>
#include <CLAM/MonoAudioFileReader.hxx>
#include <CLAM/MonoAudioFileWriter.hxx>

#include <CLAM/AudioFile.hxx>
#include <CLAM/CLAM_Math.hxx>

#include <string>
#include <fstream> // used for open(..) : we want to check if some file exists

namespace CLAMTest
{
	/// checks that a given filename exists
	inline bool helperFileExist(const std::string& filename)
	{
		std::fstream file;
		file.open( filename.c_str() );
		return file.is_open();
	}
	
	/// Loads audio from file using processing AudioFileIn
	inline void helperLoadAudioFromFile(const std::string filename, CLAM::Audio& audioToWrite )
	{
		CLAM_ASSERT( helperFileExist(filename), "helperLoadAudioFromFile(..) received a non existing file" );
		CLAM::MonoAudioFileReaderConfig conf;
		conf.SetSourceFile(filename);

		CLAM::MonoAudioFileReader in(conf);
		in.Start();
		
		audioToWrite.SetSize( in.GetHeader().GetSamples() );
		in.Do( audioToWrite );
		in.Stop();
	}
	
	/// Saves audio to file using processig AudioFileOut
	inline void helperSaveAudioToFile( CLAM::Audio& audio, const std::string filename )
	{
		CLAM::MonoAudioFileWriterConfig cfg;
		cfg.SetTargetFile(filename);
		cfg.SetSampleRate(audio.GetSampleRate());
		CLAM::MonoAudioFileWriter writer(cfg);
		writer.Start();
		writer.Do(audio);
		writer.Stop();
	}

	
	/// this helper func. should result in the future in a processing or assert. 
	inline bool helperAudiosAreEqual(
			CLAM::Audio& first, 
			CLAM::Audio& second, 
			std::string& whyDifferents, 
			double delta = 0.001)
	{
		if (first.GetSize() != second.GetSize() )
		{	
			std::stringstream formatter;
			formatter << "different audio sizes: first " << 
				first.GetSize() << " second " << second.GetSize() << std::endl;
			whyDifferents += formatter.str();
			return false;
		}
		if (first.GetSampleRate() != second.GetSampleRate() )
		{
			std::stringstream formatter;
			formatter << "compared audios have different sample-rates: first "<<
				first.GetSampleRate() << " second " << second.GetSampleRate() << std::endl;
			whyDifferents += formatter.str();
		}
		CLAM::DataArray& samplesFirst = first.GetBuffer();
		CLAM::DataArray& samplesSecond = second.GetBuffer();
		for (int i=0; i<samplesFirst.Size(); i++)
		{
			if( fabs(samplesFirst[i] - samplesSecond[i]) > delta )
			{
				std::stringstream formatter;
				formatter << "found a different sampler in position " << 
					i << " first value is "<< samplesFirst[i] <<
					" second value is " << samplesSecond[i] << 
					" with delta = " << delta;
				whyDifferents += formatter.str();
				return false;
			}
		}
		return true;
	}
	/// shorthand for loading an audio and then call
	/// helperAudiosAreEqual
	/// in case of not-equals, saves the result audio file
	inline bool helperAudioIsEqualToAudioFile( 
		CLAM::Audio& audio, 
		const std::string& audioFile, 
		std::string& whyDifferents,
		double delta=0.0001 )
	{
		std::string resultAudioFile = audioFile+"_result.wav";
		whyDifferents = "comparing with file: "+ audioFile +
			"\nThe calculated audio has been saved in:  " + resultAudioFile +"\n";
		if( !helperFileExist(audioFile) )
		{
			helperSaveAudioToFile(audio, resultAudioFile);
			return false;
		}
		CLAM::Audio loadedAudio;
		helperLoadAudioFromFile( audioFile, loadedAudio);
		
		bool result = helperAudiosAreEqual(audio, loadedAudio, whyDifferents, delta);
		if (!result)
		{
			helperSaveAudioToFile(audio, resultAudioFile);
		}
		return result;
	}


	/// shorthand for loading two audios and then call
	/// helperAudiosAreEqual
	/// in any case it does NOT save any file (no need to)
	inline bool helperCompareTwoAudioFiles( 
		const std::string& audioFile1,
		const std::string& audioFile2, 
		std::string& whyDifferents,
		double delta=0.0001 )
	{
		if( !helperFileExist(audioFile1) )
		{
			whyDifferents += "Error in helperCompareTwoAudioFiles: file "+audioFile1+" doesn't exist";
			return false;
		}
		if (!helperFileExist(audioFile2) )
		{
			whyDifferents += "Error in helperCompareTwoAudioFiles: file "+audioFile2+" doesn't exist";
			return false;
		}

		whyDifferents += "comparing file1: " + audioFile1 + 
			" with file2: "+audioFile2 + "\n";
		CLAM::Audio loadedAudio1, loadedAudio2;
		helperLoadAudioFromFile( audioFile1, loadedAudio1);
		helperLoadAudioFromFile( audioFile2, loadedAudio2);
		
		bool result = helperAudiosAreEqual(loadedAudio1, loadedAudio2, whyDifferents, delta);
		return result;
	}
	
} // namespace CLAMTest

#endif

