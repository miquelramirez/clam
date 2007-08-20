#include "SDIFDatabase.hxx"
#include "SDIFInConfig.hxx"
#include <algorithm>
#include "SimpleTransformer.hxx"
#include "DataUtil.hxx"
#include <limits.h>

namespace CLAM
{

SDIFDatabase::SDIFDatabase() :
	mMetadataReader(), mSimpleTransformer(), mThreadPool()
{
	sourceMetadata.AddAll();
	sourceMetadata.UpdateData();
	targetMetadata.AddAll();
	targetMetadata.UpdateData();
	lowerBoundMetadata.AddAll();
	lowerBoundMetadata.UpdateData();
	upperBoundMetadata.AddAll();
	upperBoundMetadata.UpdateData();
}

SDIFDatabase::~SDIFDatabase()
{
	mThreadPool.EmptyPool();

	if (isAnyProgramLoaded())
		UnloadProgram();
}

bool SDIFDatabase::LoadProgram(std::string& argDirectoryPath, bool isResidualEnabled)
{
	if (isAnyProgramLoaded() && !isThisProgramLoaded(argDirectoryPath))
		UnloadProgram();

	currentProgramsDirectoryPath = argDirectoryPath;

	try
	{
		// load metadata from directory
		mMetadataReader.ReadMetadata( argDirectoryPath, metadataVector );
	}
	// if directory specified by this path does not exist, a BadFilenameException
	// is thrown
	catch (CLAM::BadFilenameException& exc)
	{
		delete &exc;
		return false;
	}

	// if there is no metadata found in this directory, return false
	if (metadataVector.size() == 0)
		return false;

	// load SDIF files given in xml metadata files
	for (int counter = 0; counter < metadataVector.size(); counter++)
	{
		CLAM::SampleMetadata& sampleMetadata = metadataVector[counter];
		//std::cout << "Loading sample: " << sampleMetadata.GetFilename() << std::endl;
		
		// construct and set the absolute path to the sample referenced in the metadata
		std::string absolutePath = argDirectoryPath + std::string("/") + sampleMetadata.GetFilename();
		CLAM::SDIFInConfig theSDIFInConfig;
		theSDIFInConfig.SetFileName( absolutePath );
		
		// convert all the loop points from milliseconds to frames and find the
		// smallest end loop point. we'll preload the sample up until this point
		int minimumEndFrame = INT_MAX;
		int maximumEndFrame = -1;
		if (sampleMetadata.HasListOfLoops())
		{
			std::vector<SimpleLoopMetadata>& theLoopList = sampleMetadata.GetListOfLoops();
			ConvertMillisecondsToFrames(theLoopList);
			minimumEndFrame = FindMinimumEndLoopPoint(theLoopList);
			maximumEndFrame = FindMaximumEndLoopPoint(theLoopList);
		}
		
		if ( minimumEndFrame != INT_MAX )
		{
			theSDIFInConfig.AddNumberOfFramesToPreload();
			theSDIFInConfig.UpdateData();
			theSDIFInConfig.SetNumberOfFramesToPreload( minimumEndFrame );
		}
		if ( maximumEndFrame != -1 )
		{
			theSDIFInConfig.AddNumberOfFramesToLoad();
			theSDIFInConfig.UpdateData();
			theSDIFInConfig.SetNumberOfFramesToLoad( maximumEndFrame );
		}
		
		theSDIFInConfig.SetEnableResidual( isResidualEnabled );
		
		CLAM::LoopingSDIFFileReader* aLoopingSDIFFileReader = new CLAM::LoopingSDIFFileReader( theSDIFInConfig );

		//sdifReaderVector.push_back( aBufferedSDIFFileReader );
		// if the metadata has a loop definition, set the loop points on the BufferedSDIFFileReader
		if (sampleMetadata.HasListOfLoops())
		{
			aLoopingSDIFFileReader->SetListOfLoops( sampleMetadata.GetListOfLoops() );
			//std::cout << "SDIFDatabase: found loop points (" << simpleLoop.GetStartFrame() << ", " << simpleLoop.GetEndFrame() << ") for file <" << sampleMetadata.GetFilename() << ">" << std::endl;
		}

		// let's store the SDIFFileReader in a map where it's tied to its filename
		// so that we can easily find it later.
		sdifReaderMap[ sampleMetadata.GetFilename() ] = aLoopingSDIFFileReader;
	}

	// debug information for the metadata sorting
	//std::cout << "Metadata vector ordering." << std::endl;
	//for (int counter = 0; counter < metadataVector.size(); counter++)
	//	std::cout << metadataVector[counter].GetPitch() << std::endl;

	// sort the metadata for faster searching
	SortMetadata();

	// debug information for the metadata sorting
	//std::cout << "Metadata vector ordering." << std::endl;
	//for (int counter = 0; counter < metadataVector.size(); counter++)
	//	std::cout << metadataVector[counter].GetPitch() << std::endl;

	return true;
}

void SDIFDatabase::ConvertMillisecondsToFrames(std::vector<CLAM::SimpleLoopMetadata>& theLoopList)
{
	std::vector<SimpleLoopMetadata>::iterator theIterator;
	for( theIterator = theLoopList.begin(); theIterator != theLoopList.end(); theIterator++)
	{
		SimpleLoopMetadata& simpleLoop = *theIterator;
		int endFrame;
		if ( (!simpleLoop.HasUnits()) 
				|| (simpleLoop.GetUnits() == ELoopPointUnits::eMilliseconds))
		{
			simpleLoop.SetStart( ConvertMillisecondsToFrames( simpleLoop.GetStart() ) );
			simpleLoop.SetEnd( ConvertMillisecondsToFrames( simpleLoop.GetEnd() ) );
			simpleLoop.SetUnits( ELoopPointUnits::eFrames );
		}
	}
}

int SDIFDatabase::FindMinimumEndLoopPoint(std::vector<CLAM::SimpleLoopMetadata>& theLoopList)
{
	int minimumEndFrame = INT_MAX;
	std::vector<SimpleLoopMetadata>::iterator theIterator;
	for( theIterator = theLoopList.begin(); theIterator != theLoopList.end(); theIterator++)
	{
		SimpleLoopMetadata& simpleLoop = *theIterator;
		if (minimumEndFrame > simpleLoop.GetEnd() )
			minimumEndFrame = simpleLoop.GetEnd();
	}
	
	return minimumEndFrame;
}

int SDIFDatabase::FindMaximumEndLoopPoint(std::vector<CLAM::SimpleLoopMetadata>& theLoopList)
{
	int maximumEndFrame = -1;
	std::vector<SimpleLoopMetadata>::iterator theIterator;
	for( theIterator = theLoopList.begin(); theIterator != theLoopList.end(); theIterator++)
	{
		SimpleLoopMetadata& simpleLoop = *theIterator;
		if (maximumEndFrame < simpleLoop.GetEnd() )
			maximumEndFrame = simpleLoop.GetEnd();
	}
	
	return maximumEndFrame;
}

bool SDIFDatabase::isAnyProgramLoaded()
{
	return (metadataVector.empty() && sdifReaderMap.empty());
}

bool SDIFDatabase::isThisProgramLoaded(std::string argProgramsDirectoryPath)
{
	return (currentProgramsDirectoryPath.compare( argProgramsDirectoryPath ) == 0);
}

void SDIFDatabase::UnloadProgram()
{
	// delete all of the LoopingSDIFFileReader objects we created with new
	std::map<std::string, CLAM::LoopingSDIFFileReader*>::iterator iter;
	for( iter = sdifReaderMap.begin(); iter != sdifReaderMap.end(); iter++ ) {
		CLAM::LoopingSDIFFileReader* aLoopingSDIFFileReader = iter->second;
		delete aLoopingSDIFFileReader;
	}

	// empty the vector and map
	sdifReaderMap.clear();
	metadataVector.clear();
}

int SDIFDatabase::ConvertMillisecondsToFrames(double milliseconds)
{
	// TODO Get hopSize and samplingRate from config file
	//int hopSize = (!HasAnalysisHopSize()) ? 256 : GetAnalysisHopSize(); 
	//int samplingRate = (!HasSamplingRate()) ? 44100 : GetSamplingRate(); 
	
	int hopSize = 256;
	int samplingRate = 44100;
	
	return (int) milliseconds / 1000. * samplingRate / hopSize;
}

int SDIFDatabase::ConvertFramesToMilliseconds(int frames)
{
	// TODO Get hopSize and samplingRate from config file
	//int hopSize = (!HasAnalysisHopSize()) ? 256 : GetAnalysisHopSize(); 
	//int samplingRate = (!HasSamplingRate()) ? 44100 : GetSamplingRate(); 

	int hopSize = 256;
	int samplingRate = 44100;

	// multiply by 1.0 to make sure that frames is converted to a double
	// before the division operation
	return (int) frames * 1.0 * hopSize / samplingRate * 1000.;
}

VoiceId SDIFDatabase::CreateVoice()
{
	time_t currentTime = time(NULL);

	voiceStageMap[currentTime] = Attack;

	return currentTime;
}

void SDIFDatabase::DeleteVoice(VoiceId id)
{
	std::map<VoiceId, VoiceStage>::iterator iter = voiceStageMap.find(id);
	if (iter == voiceStageMap.end())
	{
		// TODO. we didn't find a voice with this id. what should we do now?
		return;
	}
	else
	{
		voiceStageMap.erase( iter );
		
		// let's reset the reader that was being used by the last voice
		CLAM::LoopingSDIFFileReader* readerForVoice = GetLoopingSDIFFileReaderForVoiceId(id);
		if (readerForVoice != NULL)
		{	
			// TODO figure out why this is causing artifacts
			readerForVoice->Reset();
		}
	}
}

Frame* SDIFDatabase::SelectFrameForVoice(VoiceId id, float argPitch, float argAmplitude)
{
	// it's more elegant to pack the given values into a SampleMetadata object
	// singleton and give that to the find object
	sourceMetadata.SetPitch(argPitch);
	sourceMetadata.SetAmplitude(argAmplitude);

	// if this is an existing voice, let's feed it frames from the same SDIFFileReader
	// that we've already been using to avoid artifacts.
	// TODO add morphing feature to move between two different voices.
	CLAM::LoopingSDIFFileReader* aLoopingSDIFFileReader = GetLoopingSDIFFileReaderForVoiceId(id);

	// if the aLoopingSDIFFileReader == NULL, then this is a new voice, and we
	// need to find the best fitting SDIFFile for it
	if (aLoopingSDIFFileReader == NULL)
	{
//		std::cout << "SDIFDatabase: BufferReader is null for id " << id << std::endl;
		// find the closest sample value for our given pitch, amplitude, etc
		// targetMetadata tells us what the pitch and amplitude of the returned sample
		// actually is
		FindClosestSample(sourceMetadata, targetMetadata);
	
		//std::cout << "SDIFDatabase::SelectFrameForVoice. Using file " << targetMetadata.GetFilename() << " for pitch " << argPitch << " and amplitude " << argAmplitude << std::endl;
	
		// we find the BufferedSDIFReader for the closest sample
		aLoopingSDIFFileReader = sdifReaderMap[ targetMetadata.GetFilename() ];
	}

	CLAM_DEBUG_ASSERT( aLoopingSDIFFileReader != NULL, "SDIFDatabase::SelectFrameForVoiceParameters - LoopingSDIFFileReader is null" );

	// we read the next frame of data from this BufferedSDIFReader
	Frame* frameFromBuffer = aLoopingSDIFFileReader->ReadFrame();

	// if the BufferedSDIFReader is not loading more frames in the background already
	// give it a frame with which it can do so
	if ( !aLoopingSDIFFileReader->IsThreaded() )
	{
		aLoopingSDIFFileReader->LoadFramesIntoBufferOnThread( mThreadPool.GetThreadFromPool() );
	}

	// TODO how do you want to handle NULL frames?
	//CLAM_DEBUG_ASSERT( aFrame != NULL, "SDIFDatabase::SelectFrameForVoiceParameters - LoopingSDIFFileReader is null" );
	if (frameFromBuffer == NULL)
	{
		CLAM::SDIFInConfig theConfig = aLoopingSDIFFileReader->GetConfig();
		std::cout << "SDIFDatabase::SelectFrameForVoice; SDIFFile " << theConfig.GetFileName() << " has no more frames." << std::endl;
	}

	// we will later modify the frame, but we don't want the frame object
	// being cached by the BufferedSDIFFIleReader to be modified
	// so let's copy it.
	//DataUtil::CopyFrameMembers(*frameFromBuffer, *singletonFrame);
	
	// transform the frequency and amplitude of the frame we have to fit that of the
	// given value for pitch and amplitude
	mSimpleTransformer.TransformFrequency( frameFromBuffer, targetMetadata.GetPitch(), argPitch );
	// TODO get the real amplitude from the SampleMetadata 
	float sourceAmplitude = 0.75;
	mSimpleTransformer.TransformAmplitude( frameFromBuffer, sourceAmplitude, argAmplitude);

	// save this BufferedSDIFReader object. we'll need it later so that we can
	// reset it if we are no longer using it
	previousSDIFReaderMap[id] = aLoopingSDIFFileReader;
	
	return frameFromBuffer;
}

CLAM::LoopingSDIFFileReader* SDIFDatabase::GetLoopingSDIFFileReaderForVoiceId(VoiceId id)
{
	std::map<VoiceId, CLAM::LoopingSDIFFileReader*>::iterator iter = previousSDIFReaderMap.find(id);
	if( iter == previousSDIFReaderMap.end() ) 
	{
		return NULL;
	}
	else
	{
		CLAM::LoopingSDIFFileReader* previousReader = iter->second;
		
		return previousReader;
	}
}

// TODO: add an argument so that this method can be given the index of the dynamic
// type to be used for the sort
void SDIFDatabase::SortMetadata()
{
	// TODO replace this ugly code by making the sort parametrizable
	// by an algorithm object
	for (int counter = 0; counter < metadataVector.size(); counter++)
	{
		metadataVector[counter].SetSortIndex( SAMPLE_METADATA_PITCH_INDEX );
	}

	sort(metadataVector.begin(), metadataVector.end());
}


void SDIFDatabase::FindClosestSample(SampleMetadata& argSourceMetadata, SampleMetadata& argTargetMetadata)
{
	FindBounds(argSourceMetadata, lowerBoundMetadata, upperBoundMetadata);

	float pitchDifferenceLowerBound = argSourceMetadata.GetPitch() - lowerBoundMetadata.GetPitch();
	float pitchDifferenceUpperBound = argSourceMetadata.GetPitch() - lowerBoundMetadata.GetPitch();

	if (pitchDifferenceLowerBound < pitchDifferenceUpperBound)
	{
		argTargetMetadata = lowerBoundMetadata;
	}
	else
	{
		argTargetMetadata = upperBoundMetadata;
	}
}

void SDIFDatabase::FindBounds(SampleMetadata& argSourceMetadata, SampleMetadata& argLowerBoundMetadata, SampleMetadata& argUpperBoundMetadata)
{
	argSourceMetadata.SetSortIndex( SAMPLE_METADATA_PITCH_INDEX );

	//std::vector<SampleMetadata>::iterator findIterator = find(metadataVector.begin(), metadataVector.end(), sourceMetadata);
	//std::vector<SampleMetadata>::iterator lowerBoundIterator = lower_bound(metadataVector.begin(), metadataVector.end(), sourceMetadata);
	std::vector<SampleMetadata>::iterator upperBoundIterator = upper_bound(metadataVector.begin(), metadataVector.end(), argSourceMetadata);

	// the given target pitch is higher than any of our sampled pitches.
	// set both the lower and upper bound to our highest pitched sample
	if (upperBoundIterator == metadataVector.end())
	{
		argUpperBoundMetadata = *(--upperBoundIterator);
		argLowerBoundMetadata = argUpperBoundMetadata;
	}
	// the given target pitch is lower than any of our sampled pitches.
	// set both the lower and upper bound to our lowest pitched sample
	else if (upperBoundIterator == metadataVector.begin())
	{
		argUpperBoundMetadata = *(upperBoundIterator);
		argLowerBoundMetadata = argUpperBoundMetadata;
	}
	// the given target pitch falls within our range of sampled pitches
	// it either equals one pitch exactly or falls between two samples pitches.
	// if it equals the pitch of one sample exactly, this perfect match will
	// be assigned to the lower bound varible.
	else
	{
		argUpperBoundMetadata = *upperBoundIterator;
		argLowerBoundMetadata = *(--upperBoundIterator);
	}

	//std::cout << "lower_bound at position " << argLowerBoundMetadata.GetPitch() << std::endl;
	//std::cout << "upper_bound at position " << argUpperBoundMetadata.GetPitch() << std::endl;
}

} // end namespace CLAM



