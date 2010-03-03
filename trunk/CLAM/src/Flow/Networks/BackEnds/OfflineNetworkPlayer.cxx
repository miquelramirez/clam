
#include "OfflineNetworkPlayer.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include "AudioSink.hxx"
#include "AudioSource.hxx"

#include <iostream>
#include <sstream>

namespace CLAM
{

bool OfflineNetworkPlayer::IsWorking()
{
	CacheSourcesAndSinks();
	return (_inFileNames.size() != GetNSources())
		&& (_outFileNames.size() != GetNSinks());
}

std::string OfflineNetworkPlayer::NonWorkingReason()
{
	std::stringstream ss;
	ss << GetNSources()  << " inputs and "
	   << GetNSinks()    << " outputs needed but just "
	   << _inFileNames.size()  << " input files provided"
	   << _outFileNames.size() << " output files provided"
	   << std::ends;
	return ss.str();
}

std::string OfflineNetworkPlayer::listOfSourcesSinksAndFiles(const SndFileHandles & infiles, const SndFileHandles & outfiles)
{
	std::ostringstream result;

	unsigned inFileIndex = 0;
	unsigned inChannel = 0;
	for (unsigned i=0; i<GetNSources(); i++)
	{
		++inChannel;
		result << " * source:\t" << SourceName(i) << "\t";
		result << "In:\t" << _inFileNames[inFileIndex] << "\tchannel " << inChannel << "\n";

		//We have read all the channels of infiles[inFileIndex]
		if((unsigned)infiles[inFileIndex]->channels() == inChannel)
		{
			inFileIndex++;
			inChannel = 0;
		}
	}

	unsigned outFileIndex = 0;
	unsigned outChannel = 0;
	for (unsigned i=0; i<GetNSinks(); i++)
	{
		outChannel++;
		result << " * sink:\t" << SinkName(i) << "\t";
		result << "Out:\t" << _outFileNames[outFileIndex] << "\tchannel " << outChannel << "\n";

		//We have read all the channels of outfiles[outFileIndex]
		if((unsigned)outfiles[outFileIndex]->channels() == outChannel)
		{
			outFileIndex++;
			outChannel = 0;
		}
	}

	return result.str();
}

void OfflineNetworkPlayer::Start()
{
	if ( IsPlaying() )
		return;

	BePlaying();

	CacheSourcesAndSinks();

	const int frameSize = 512;

	//Open the files, get the total number of channels and the sample rate
	int sampleRate = 0;
	unsigned inputChannelsCount = 0;
	SndFileHandles infiles;

	for(unsigned fileIndex = 0; fileIndex < _inFileNames.size(); fileIndex++)
	{
		CLAM_ASSERT(fileIndex < _inFileNames.size(),
			"Not all the network inputs could be fullfiled. Have you checked the IsWorking() method?");
		std::ifstream checkfile(_inFileNames[fileIndex].c_str());

		CLAM_ASSERT(checkfile.is_open(),std::string("Could not open one of the input files: "+_inFileNames[fileIndex]).c_str());
		SndfileHandle* infile = new SndfileHandle(_inFileNames[fileIndex].c_str(), SFM_READ );

		CLAM_ASSERT(*infile, "Can not create the infile ");
		inputChannelsCount += infile->channels();

		if(fileIndex == 0)
			sampleRate = infile->samplerate();

		CLAM_ASSERT(infile->samplerate() == sampleRate,
			"All the input audio files have to have the same sample rate");

		infiles.push_back(infile);
	}

	// Check that the number of input channels matches the number of ports in the network
	CLAM_ASSERT(inputChannelsCount == GetNSources(),
	 	"The number of input channels is different than the number of sourceports in the provided network.");

	//Open the files and get the total number of channels
	unsigned outputChannelsCount=0;
	SndFileHandles outfiles;
	for(unsigned fileIndex=0;fileIndex<_outFileNames.size();fileIndex++)
	{
		if (fileIndex>=_outFileNames.size())
		{
			std::cerr << "Not all the network outputs could be fullfiled.";
			break;
		}

		SndfileHandle* outfile = new SndfileHandle(_outFileNames[fileIndex].c_str(), SFM_WRITE,
												   _format,_outChannelsFiles[fileIndex],sampleRate);
		CLAM_ASSERT(*outfile, "Can not create the outfile ");
		outputChannelsCount +=_outChannelsFiles[fileIndex];
		outfiles.push_back(outfile);
	}

	// Check that the number of output channels matches the of ports in the network
	CLAM_ASSERT(outputChannelsCount == GetNSinks(),
	 	"The number of output channels is different than the number of sinkports in the provided network.");

	std::cout << "Sources and Sinks list:" <<std::endl;
	std::cout << listOfSourcesSinksAndFiles(infiles,outfiles)<<std::endl;

	// Prepare the sources
	//CLAM_ASSERT(_audioSources.size() == infiles.size(),
	//		"The number of sources is greater than the intput files.");

	std::vector<DataArray> inbuffers(inputChannelsCount);
	for(unsigned i = 0; i < GetNSources(); ++i)
	{
		inbuffers[i].Resize( frameSize );
		inbuffers[i].SetSize( frameSize );
		const TData * data = &inbuffers[i][0];
		SetSourceBuffer(i, data, frameSize);
	}

	//Prepare the sinks
	//CLAM_ASSERT(_audioSinks.size() == outfiles.size(),
	//		"The number of sinks is greater than the output files ");

	std::vector<DataArray> outbuffers(outputChannelsCount);
	for( unsigned i = 0; i < GetNSinks(); ++i)
	{
		outbuffers[i].Resize( frameSize );
		outbuffers[i].SetSize( frameSize );

		TData * data = &outbuffers[i][0];
		SetSinkBuffer(i, data, frameSize);
	}


	long iterationIndex = 0;
	bool timeLimitedMode = _resultWavsTime > 0.001;
	int fileIndex = 0;
	while(true)
	{
		std::cout << "." << std::flush;
		unsigned inAudioIndex =0;
		bool someInputFinished=false;
		for(SndFileHandles::iterator it = infiles.begin(); it != infiles.end(); ++it)
		{
			SndfileHandle * sndfileHandle = (*it);
			unsigned int nChannels = sndfileHandle->channels();
			CLAM_ASSERT(nChannels, "The audio had no channels");
			int bufferReaderSize = nChannels*frameSize;
			float * bufferReader = new float[bufferReaderSize];
			int readSize = sndfileHandle->read(bufferReader,bufferReaderSize);

			//We have read the last part (not complete) of the buffer file. Fill the buffer with zeros.
			if(readSize<bufferReaderSize)
			{
				for(int i = readSize; i < bufferReaderSize; i++)
				{
					bufferReader[i] = 0;
				}
				if(_enableLoopInputWavs)
					sndfileHandle->seek(0,SEEK_SET);
				someInputFinished = true;
			}
			//Save the bufferReader into the sources' buffers.
			for(int frameIndex=0; frameIndex <frameSize; frameIndex ++)
			{
				for(unsigned channel=0; channel < nChannels; channel++)
				{
					inbuffers[inAudioIndex+channel][frameIndex] = bufferReader[(frameIndex*nChannels)+channel];
				}
			}
			inAudioIndex += nChannels;
			fileIndex ++;
			delete[] bufferReader;
		}

		GetNetwork().Do();

		unsigned outAudioIndex = 0;
		for(SndFileHandles::iterator it = outfiles.begin(); it != outfiles.end(); ++it)
		{
			SndfileHandle * sndfileHandle = (*it);
			unsigned int nChannels = sndfileHandle->channels();
			int bufferWriterSize = nChannels*frameSize;
			float*	bufferWriter = new float[bufferWriterSize];

			//Save the sources' buffers into the bufferWriter.
			for(int frameIndex = 0; frameIndex < frameSize; frameIndex ++)
			{
				for(unsigned channel = 0; channel < nChannels; channel++)
				{
					bufferWriter[(frameIndex*nChannels) + channel] =
						outbuffers[outAudioIndex + channel][frameIndex];
				}
			}
			int writeSize = sndfileHandle->write(bufferWriter, bufferWriterSize);
			CLAM_ASSERT(writeSize == bufferWriterSize,"The outfile has not been written correctly");
			outAudioIndex += nChannels;
			delete[] bufferWriter;
		}

		if (timeLimitedMode and float(iterationIndex * frameSize) / sampleRate > _resultWavsTime)
		{
			std::cout << "REACHED MAX TIME - finalizing"<< std::endl;
			break;
		}
		iterationIndex++;

		if (someInputFinished and not _enableLoopInputWavs )
			break;
	}

	//Deleting the sndfiles
	for(SndFileHandles::iterator it = infiles.begin(); it != infiles.end(); ++it)
		delete *it;

	for(SndFileHandles::iterator it = outfiles.begin(); it != outfiles.end(); ++it)
		delete *it;
}

void OfflineNetworkPlayer::Stop()
{
	if ( IsStopped() )
		return;
	BeStopped();

	//TODO close files
}

void OfflineNetworkPlayer::ProcessInputFile()
{
	GetNetwork().Do();
}

void OfflineNetworkPlayer::AddInputFile( const std::string& filename )
{
	_inFileNames.push_back(filename);
}

void OfflineNetworkPlayer::AddOutputFile( const std::string& filename )
{
	_outFileNames.push_back(filename);
}

void OfflineNetworkPlayer::AddNumChannels(int channel)
{
	_outChannelsFiles.push_back(channel);
}

void OfflineNetworkPlayer::SetFormat(int format)
{
	_format = format;
}

} //namespace

