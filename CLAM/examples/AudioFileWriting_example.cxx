// Welcome to the "AudioFileWriting" example. In this example we will show you
// how to use CLAM::MultiChannelAudioFileWriter, a Processing that allows you
// to write simultaneously several channels at once from the same file.

// Before attempting to follow this example you should take a look first on the
// "FileInfo" example ( examples/AudioFileWriting_example.cxx ) and the
// "AudioFileReading" example ( examples/AudioFileReading_example.cxx ).

// We need to include the following files to have access to classes
// AudioFile, MultiChannelAudioFileWriter and MultiChannelAudioFileWriterConfig
#include "AudioFile.hxx"
#include "MultiChannelAudioFileWriter.hxx"

// We also need the declaration of the CLAM::Audio object
#include "Audio.hxx"

// For accessing <cmath> header in a cross-platform way
#include "CLAM_Math.hxx"
#include "DataTypes.hxx"

#include <vector>

#include <iostream>

int main( int argc, char** argv )
{
	// As we did in the FileInfo example we will take the output file name from
	// the command line.

	if ( argc == 1 ) // No input file
	{
		std::cerr << "No input file" << std::endl;
		std::cerr << "Usage: AudioFileWriting <output file name>" << std::endl;

		exit( - 1 );
	}
	else if ( argc > 2 ) // Too many parameters
	{
		std::cerr << "Too many parameters" << std::endl;
		std::cerr << "Usage: AudioFileWriting <output file name>" << std::endl;
		
		exit( -1 );
	}
	
	// As we can be quite sure that we have one parameter at argv[1] we can
	// set the Audio file location:
	CLAM::AudioFile file;
	file.SetLocation( argv[1] );

	// When we write on to an audio file it is our responsability to define
	// its header: the output format, the number of channels in the file, etc.
	// So let's first create a CLAM::AudioFileHeader object:
	CLAM::AudioFileHeader header;

	// Some usual thing to do, is to deduce the file format from the file extension.
	// You can obtain the file format from a given filename with the static method:
	// CLAM::EAudioFileFormat::FileFormatFromFilename() which takes as a parameter a std::string and returns
	// a CLAM::AudioFileFormat object.

	CLAM::EAudioFileFormat outputFormat = 
		CLAM::EAudioFileFormat::FormatFromFilename( file.GetLocation() );

	std::cout << "Desired output format: " << outputFormat << std::endl;

	// Besides the output format, we also must to decide the output sample rate and
	// number of channels
	const CLAM::TData sampleRate = 44100.; // Hz
	const CLAM::TSize nChannels = 2;

	// Now, to set the header values we can use the method AudioFileHeader::SetValues,
	// whose parameters in order are: the file sample rate, the number of channels and
	// the target format

	header.SetValues( sampleRate, nChannels, outputFormat );

	// And now we set the file header to the one we have just defines
	file.SetHeader( header );

	// Finally, we must check if the header settings are compatible with the
	// output format we specified:

	if ( !file.IsWritable() )
	{
		std::cerr << "The desired format " << outputFormat;
		std::cerr << "does not support current header parameters" << std::endl;
		exit(-1);
	}

	// Much like in the "AudioFileReading" example, now we must create a 
	// MultiChannelAudioFileWriterConfig object, for configuring the 'writer' processing
	CLAM::MultiChannelAudioFileWriterConfig configObject;

	// We add the 'Target File' attribute to the config
	configObject.AddTargetFile();
	configObject.UpdateData();
	
	// And we set the 'Target File' attribute to the AudioFile object
	// we have been working on
	configObject.SetTargetFile( file );

	// Now we can instantiate the MultiChannelAudioFileWriter and configure it
	CLAM::MultiChannelAudioFileWriter writer;
	writer.Configure( configObject );

	// Finally, we must set the data we want to write on to the file. We will just implement
	// a simple sinusoidal synthesis, where in each channel there is a sinusoid with the same
	// frequency but different phase

	const CLAM::TData sineFreq = 440.; // Hz
	// initial phases
	CLAM::TData phi[] = { -M_PI / 2.0, M_PI/2.0 };

	// Delta phase: this trick allows us to compute incrementally current sine
	// phase
	CLAM::TData dphi[] = { ( 2.0 * M_PI * sineFreq ) / sampleRate,  ( 2.0 * M_PI * sineFreq ) / sampleRate};

	// Now we must decide our 'write size', i.e. the number of samples we will write
	// at once at the same channel
	const CLAM::TSize writeSize = 2048; // 0.0464 s

	// And now we set our 'input buffers', where we will be generating the sines
	
	std::vector< CLAM::Audio > inputs;
	inputs.resize( nChannels );         // As many inputs as channels

	for ( int i = 0; i < inputs.size(); i++ )
	{
		inputs[i].SetSize( writeSize );
	}

	// We attach both Audio objects to our writer input ports

	for ( int i = 0; i < inputs.size(); i++ )
		writer.GetInPorts().GetByNumber( i ).Attach( inputs[i] );

	// Now we want to synthesize approx. 1s

	writer.Start();
	
	for ( int i = 0; i < 25; i++ )
	{
		for ( int j = 0; j < inputs.size(); j++ )
			for ( int k = 0; k < inputs[j].GetSize(); k++ )
			{
				inputs[j].GetBuffer()[k] = 0.5 * cos( phi[j] );
				phi[j] += dphi[j];
			}

		// Once both input buffers have been generated we call
		// the writer Do()
		writer.Do();
	}
	
	writer.Stop();

	return 0;
}
