
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
	return (_outFileNames.size() != GetSize<Network::AudioSources>(GetAudioSources()))
		&& (_inFileNames.size() != GetSize<Network::AudioSinks>(GetAudioSinks()));
}

std::string OfflineNetworkPlayer::NonWorkingReason()
{
	std::stringstream ss;
	ss << GetSize<Network::AudioSources>(GetAudioSources()) << " inputs and " 
	   << GetSize<Network::AudioSinks>(GetAudioSinks()) << " outputs needed but just " 
	   << _inFileNames.size() << " input files provided" 
	   << _outFileNames.size() << " output files provided" 
	   << std::ends;
	return ss.str();
}

std::string OfflineNetworkPlayer::listOfSourcesSinksAndFiles(SndFileHandles const& infiles, 
															 SndFileHandles const& outfiles)
{
	std::ostringstream result;
	
	Network & net = GetNetwork();
	
	const Network::AudioSources & sources = GetAudioSources();
	unsigned inFileIndex = 0;
	unsigned inChannel = 0;

	for (Network::AudioSources::const_iterator it = sources.begin(); it != sources.end(); ++it)
	{
		std::string processingName = net.GetNetworkId(*it);

		const AudioSource::Ports & ports = (*it)->GetPorts();
		for(unsigned port = 0; port < ports.size(); ++port)
		{
			std::stringstream portName;
			if (ports.size() == 1)
				 portName << processingName;
			else
				 portName << processingName << "_" << ports[port].mPort->GetName();

			inChannel++;
			result << " * source:\t" << portName.str() << "\t";		
			result << "In:\t" << _inFileNames[inFileIndex] << "\tchannel " << inChannel << "\n";

			//We have read all the channels of infiles[inFileIndex]
			if((unsigned)infiles[inFileIndex]->channels() == inChannel)
			{
				inFileIndex++;
				inChannel = 0;	
			}
		}
	}

	const Network::AudioSinks & sinks = GetAudioSinks();
	unsigned outFileIndex = 0;
	unsigned outChannel = 0;

	for (Network::AudioSinks::const_iterator it = sinks.begin(); it != sinks.end(); ++it)
	{
		std::string processingName = net.GetNetworkId( *it );

		const AudioSink::Ports & ports = (*it)->GetPorts();
		for(unsigned port = 0; port < ports.size(); ++port)
		{
			//Register port on the JACK server
			std::stringstream portName;
			if (ports.size() == 1)
				 portName << processingName;
			else
				 portName << processingName << "_" << ports[port].mPort->GetName();

			outChannel++;
			result << " * sink:\t" << portName.str() << "\t";
			result << "Out:\t" << _outFileNames[outFileIndex] << "\tchannel " << outChannel << "\n";

			//We have read all the channels of outfiles[outFileIndex]
			if((unsigned)outfiles[outFileIndex]->channels() == outChannel)
			{
				outFileIndex++;
				outChannel = 0;
			}
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

		CLAM_ASSERT(checkfile.is_open(), std::string(std::string("Could not open one of the input files: ")+_inFileNames[fileIndex]).c_str());
		SndfileHandle* infile = new SndfileHandle(_inFileNames[fileIndex].c_str(), SFM_READ );

		CLAM_ASSERT(*infile, "Can not create the infile ");		
		inputChannelsCount += infile->channels();
		
		if(fileIndex == 0)
			sampleRate = infile->samplerate();

		if(infile->samplerate() != sampleRate)
		{
			std::cout << "All the input audio files have to have the same sample rate" << std::endl;
			exit(-1);	
		}

		infiles.push_back(infile);
	}

	// Check that the number of input channels matches the network	
	unsigned nInChannels = GetSize<Network::AudioSources>(_audioSources);
	if( inputChannelsCount != nInChannels)
	{
	 	std::cout <<"The number of input channels is different than the number of sources in the provided network." << std::endl
			<<"There are "<<nInChannels<<" sources and "
			<<inputChannelsCount<<" input channels summing all channels in "<<infiles.size()<<" input files"<<std::endl;
		exit(-1);
	}

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
		SndfileHandle* outfile = new SndfileHandle(_outFileNames[fileIndex].c_str(), SFM_WRITE,_format,_outChannelsFiles[fileIndex],sampleRate);
		CLAM_ASSERT(*outfile, "Can not create the outfile ");		
		outputChannelsCount +=_outChannelsFiles[fileIndex];
		outfiles.push_back(outfile);
	}

	// Check that the number of output channels matches the network	
	unsigned nOutChannels = GetSize<Network::AudioSinks>(_audioSinks);
	if( outputChannelsCount != nOutChannels)
	{
	 	std::cout <<"The number of output channels is different than the number of sinks in the provided network." << std::endl
			  <<"There are "<<nOutChannels<<" sinks and "
			   <<outputChannelsCount<<" output channels summing all channels in "<<outfiles.size()<<" output files"<<std::endl;
		exit(-1);
	}

	std::cout << "Sources and Sinks list:" <<std::endl;
	std::cout << listOfSourcesSinksAndFiles(infiles,outfiles)<<std::endl;

	// Prepare the sources
	std::vector<DataArray> inbuffers(inputChannelsCount);
	unsigned sourceIndex=0;
	unsigned fileIndex = 0;
	while(sourceIndex<_audioSources.size())
	{	
		if(fileIndex>=infiles.size())
		{
			std::cout 
				<< "The number of sources is greater than the intput files. " 
				<< "There are " << GetSize<Network::AudioSources>(GetAudioSources()) <<" sources and "
				<< infiles.size() << "input files" 
				<< std::endl;
			exit(-1);
		}

		for(int channel = 0; channel < infiles[fileIndex]->channels();channel++)
		{
			inbuffers[sourceIndex].Resize( frameSize );
			inbuffers[sourceIndex].SetSize( frameSize );

			unsigned port_size = _audioSources[sourceIndex]->GetPorts().size();
			for(unsigned port = 0; port < port_size; ++port)
				_audioSources[sourceIndex]->SetExternalBuffer( &(inbuffers[sourceIndex][0]),frameSize, port);
				
			//std::cout << " In: " << _inFileNames[fileIndex] << " channel "<< channel<< std::endl;
			sourceIndex++;
		}
		fileIndex++;
	}

	//Prepare the sinks
	std::vector<DataArray> outbuffers(outputChannelsCount);
	fileIndex = 0;
	unsigned sinkIndex = 0;
	while(sinkIndex < _audioSinks.size())
	{
		if(fileIndex >= outfiles.size())
		{
			std::cout << "The number of sinks is greater than the output files. " 
			          << "There are " << GetSize<Network::AudioSinks>(GetAudioSinks()) << " sinks and " 
					  << outfiles.size() <<"output files" << std::endl;
			exit(-1);
		}

		for(int channel = 0;channel < outfiles[fileIndex]->channels(); channel++)
		{
			unsigned sinkSize = GetSize<Network::AudioSinks>(GetAudioSinks());
			if(fileIndex+channel >= sinkSize)
			{
			 	std::cout << "The number of output channels is greater than the sinks. "
						  << "There are " << sinkSize << " sinks and " 
						  << fileIndex+channel << " ouput channels" << std::endl;
				exit(-1);
			}
			outbuffers[sinkIndex].Resize( frameSize );
			outbuffers[sinkIndex].SetSize( frameSize );

			unsigned port_size = _audioSinks[sinkIndex]->GetPorts().size();
			for(unsigned port = 0; port < port_size; ++port)
				_audioSinks[sinkIndex]->SetExternalBuffer(&(outbuffers[sinkIndex][0]), frameSize, port);

			//std::cout << " Out: " << _outFileNames[fileIndex] << " channel "<< channel<< std::endl;
			sinkIndex++;
		}		
		fileIndex++;
	}

	long iterationIndex = 0;
	bool timeLimitedMode = _resultWavsTime > 0.001;
	fileIndex = 0;
	while(true)
	{
		std::cout << "." << std::flush;
		unsigned inAudioIndex =0;
		bool someInputFinished=false;
		for(SndFileHandles::iterator it = infiles.begin(); it != infiles.end(); ++it)
		{
			unsigned int nChannels = (*it)->channels();
			CLAM_ASSERT((*it)->channels(), "The audio had no channels");
			int bufferReaderSize = nChannels*frameSize;
			float * bufferReader = new float[bufferReaderSize];
			int readSize = (*it)->read(bufferReader,bufferReaderSize);

			//We have read the last part (not complete) of the buffer file. Fill the buffer with zeros.
			if(readSize<bufferReaderSize)
			{
				for(int i = readSize; i < bufferReaderSize; i++)
				{	
					bufferReader[i] = 0;
				}			
				if(_enableLoopInputWavs)
					(*it)->seek(0,SEEK_SET);
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
			int bufferWriterSize = (*it)->channels()*frameSize;
			float*	bufferWriter = new float[bufferWriterSize];
	
			//Save the sources' buffers into the bufferWriter.
			for(int frameIndex = 0; frameIndex < frameSize; frameIndex ++)
			{	
				for(int channel = 0; channel < (*it)->channels(); channel++)
				{	
					bufferWriter[(frameIndex*(*it)->channels()) + channel] = 
						outbuffers[outAudioIndex + channel][frameIndex];
				}
			}	
			int writeSize = (*it)->write(bufferWriter, bufferWriterSize);
			CLAM_ASSERT(writeSize == bufferWriterSize,"The outfile has not been written correctly");
			outAudioIndex += (*it)->channels();
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

