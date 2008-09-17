#include "OfflineNetworkPlayer.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include <fstream>
#include <sndfile.hh>  

namespace CLAM
{

bool OfflineNetworkPlayer::IsWorking() const
{
	return (_outFileNames.size()!=GetAudioSinks().size())&&(_inFileNames.size()!=GetAudioSources().size());
}
std::string OfflineNetworkPlayer::NonWorkingReason() const
{
	std::stringstream ss;
	ss << GetAudioSources().size() << " inputs and " 
		<< GetAudioSinks().size() << " outputs needed but just " 
		<< _inFileNames.size() << " input files provided"
		<< _outFileNames.size() << " output files provided" << std::ends;
	return ss.str();
}
void OfflineNetworkPlayer::Start()
{
	if ( IsPlaying() ) return;
	BePlaying();
	CollectSourcesAndSinks();

	const int frameSize = 512;
	//Open the files, get the total number of channels and the sample rate
	int sampleRate=0; 
	unsigned inputChannelsCount=0;
	std::vector<SndfileHandle*> infiles;

	for(unsigned fileIndex=0;fileIndex<_inFileNames.size();fileIndex++)
	{
		CLAM_ASSERT(fileIndex<_inFileNames.size(),
		"Not all the network inputs could be fullfiled. Have you checked the IsWorking() method?");
		std::ifstream checkfile(_inFileNames[fileIndex].c_str());
		CLAM_ASSERT(checkfile.is_open(), std::string(std::string("Could not open one of the input files: ")+_inFileNames[fileIndex]).c_str());
		SndfileHandle* infile = new SndfileHandle(_inFileNames[fileIndex].c_str(), SFM_READ );
		CLAM_ASSERT(*infile, "Can not create the infile ");		
		 inputChannelsCount += infile->channels();
		if(fileIndex==0)
			sampleRate = infile->samplerate();
		if(infile->samplerate() != sampleRate)
		{
			std::cout <<"All the input audio files have to have the same sample rate"<<std::endl;
			exit(-1);	
		}
		infiles.push_back(infile);
	}

	// Check that the number of input channels matches the network	
	if( inputChannelsCount != GetAudioSources().size())
	{
	 	std::cout <<"The number of input channels is different than the number of sources." << std::endl
			<<"There are "<<GetAudioSources().size()<<" sources and "
			<<inputChannelsCount<<" input channels"<<std::endl;
		exit(-1);			
	}

	//Open the files and get the total number of channels
	unsigned outputChannelsCount=0;
	std::vector<SndfileHandle*> outfiles;
	for(unsigned fileIndex=0;fileIndex<_outFileNames.size();fileIndex++)
	{
		if (fileIndex>=_outFileNames.size())
		{
			std::cerr <<
			"Not all the network outputs could be fullfiled.";
			break;
		}
		SndfileHandle* outfile = new SndfileHandle(_outFileNames[fileIndex].c_str(), SFM_WRITE,_format,_outChannelsFiles[fileIndex],sampleRate);
		CLAM_ASSERT(*outfile, "Can not create the outfile ");		
		outputChannelsCount +=_outChannelsFiles[fileIndex];
		outfiles.push_back(outfile);
	}
	// Check that the number of output channels matches the network	
	if( outputChannelsCount != GetAudioSinks().size())
	{
	 	std::cout <<"The number of output channels is different than the number of sinks." << std::endl
			  <<"There are "<<GetAudioSinks().size()<<" sinks and "
			   <<outputChannelsCount<<" output channels"<<std::endl;
		exit(-1);			
	}

	// Prepare the sources
	std::vector<DataArray> inbuffers( inputChannelsCount);
	unsigned sourceIndex=0;
	unsigned fileIndex = 0;
	while(sourceIndex<GetAudioSources().size())
	{	
		for(int channel=0;channel<infiles[fileIndex]->channels();channel++)
		{
			inbuffers[sourceIndex].Resize( frameSize );
			inbuffers[sourceIndex].SetSize( frameSize );
			AudioSource& source = *GetAudioSources()[sourceIndex];				
			source.SetExternalBuffer( &(inbuffers[sourceIndex][0]),frameSize);
			std::cout << " In: " << _inFileNames[fileIndex] << " channel "<< channel+1<< std::endl;
			sourceIndex++;
		}
		fileIndex++;
	}

	//Prepare the sinks
	std::vector<DataArray> outbuffers(outputChannelsCount);
	fileIndex =0;
	unsigned sinkIndex = 0;
	while(sinkIndex<GetAudioSinks().size())
	{	if(fileIndex>=outfiles.size())
		{
			std::cout << "The number of sinks is greater than the output files. " << "There are "<<GetAudioSinks().size()<<" sinks "<<" and "<<outfiles.size()<<"output files"<<std::endl;
			exit(-1);
		}
		for(int channel=0;channel<outfiles[fileIndex]->channels();channel++)
		{	if(fileIndex+channel>=GetAudioSinks().size())
			{
			 	std::cout <<"The number of output channels is greater than the sinks. "<<"There are "<<GetAudioSinks().size()<<" sinks and "<<fileIndex+channel<<" ouput channels"<<std::endl;
				exit(-1);
			}
			outbuffers[sinkIndex].Resize( frameSize );
			outbuffers[sinkIndex].SetSize( frameSize );
			AudioSink& sink = *GetAudioSinks()[sinkIndex];
			sink.SetExternalBuffer( &(outbuffers[sinkIndex][0]) ,frameSize);
			std::cout << " Out: " << _outFileNames[fileIndex] << " channel "<< channel+1<< std::endl;
			sinkIndex++;
		}		
		fileIndex++;
	}

	long iterationIndex = 0;
	bool timeLimitedMode = _resultWavsTime > 0.001;
	float*	bufferReader;
	float*	bufferWriter;
	fileIndex = 0;
	while(true)
	{
		std::cout << "." << std::flush;
		unsigned inAudioIndex =0;
		bool someInputFinished=false;
		for(std::vector<SndfileHandle*>::iterator it=infiles.begin();it!=infiles.end();it++)
		{	
			int bufferReaderSize = (*it)->channels()*frameSize;
			bufferReader = new float[bufferReaderSize];
			int readSize = (*it)->read(bufferReader,bufferReaderSize);

			//We have read the last part (not complete) of the buffer file. Fill the buffer with zeros.
			if(readSize<bufferReaderSize)
			{
				for(int i=readSize;i<bufferReaderSize;i++)
				{	
					bufferReader[i] = 0;
				}			
				if(_enableLoopInputWavs)
					(*it)->seek(0,SEEK_SET);						
				someInputFinished = true;
			}
			//Save the bufferReader into the sources' buffers.
			for(int frameIndex=0; frameIndex <frameSize; frameIndex ++)
			{	for(int channel=0; channel < (*it)->channels(); channel++)
				{	inbuffers[inAudioIndex+channel][frameIndex] = bufferReader[(frameIndex*(*it)->channels())+channel];
				}
			}
			inAudioIndex += (*it)->channels();
			fileIndex ++;
			delete(bufferReader);
		}		

		GetNetwork().Do();
		
		unsigned outAudioIndex = 0;
		for(std::vector<SndfileHandle*>::iterator it=outfiles.begin();it!=outfiles.end();it++)
		{	
			int bufferWriterSize = (*it)->channels()*frameSize;
			bufferWriter = new float[bufferWriterSize];
	
			//Save the sources' buffers into the bufferWriter.
			for(int frameIndex=0; frameIndex <frameSize; frameIndex ++)
			{	for(int channel=0; channel < (*it)->channels(); channel++)
				{	bufferWriter[(frameIndex*(*it)->channels())+channel] = outbuffers[outAudioIndex+channel][frameIndex];					
				}
			}	
			int writeSize = (*it)->write(bufferWriter,bufferWriterSize);
			CLAM_ASSERT(writeSize==bufferWriterSize,"The outfile has not been written correctly");
			outAudioIndex += (*it)->channels();
			delete bufferWriter;	
		}

		if (timeLimitedMode and float(iterationIndex*frameSize)/sampleRate > _resultWavsTime)
		{
			std::cout << "REACHED MAX TIME - finalizing"<< std::endl;
			break;
		}
		iterationIndex++;

		if (someInputFinished and not _enableLoopInputWavs ) break;
	}
	//Deleting the sndfiles
	for(std::vector<SndfileHandle*>::iterator it=infiles.begin();it!=infiles.end();it++) delete *it;
	for(std::vector<SndfileHandle*>::iterator it=outfiles.begin();it!=outfiles.end();it++) delete *it;
}
void OfflineNetworkPlayer::Stop()
{
	if ( IsStopped() ) return;
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

