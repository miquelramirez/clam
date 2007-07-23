#include "SDIFDatabase.hxx"
#include "SDIFInConfig.hxx"
#include <algorithm>
#include "SimpleTransformer.hxx"

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

bool SDIFDatabase::LoadProgram(std::string& argDirectoryPath)
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
		std::string absolutePath = argDirectoryPath + std::string("/") + sampleMetadata.GetFilename();
		CLAM::SDIFInConfig theSDIFInConfig;
		theSDIFInConfig.SetFileName( absolutePath );
		CLAM::BufferedSDIFFileReader* aBufferedSDIFFileReader = new CLAM::BufferedSDIFFileReader( theSDIFInConfig );

		//sdifReaderVector.push_back( aBufferedSDIFFileReader );
		// if the metadata has a loop definition, set the loop points on the BufferedSDIFFileReader
		if (sampleMetadata.HasSimpleLoop())
		{
			SimpleLoopMetadata& simpleLoop = sampleMetadata.GetSimpleLoop();
			aBufferedSDIFFileReader->SetLoopPoints(simpleLoop.GetStartFrame(), simpleLoop.GetEndFrame());
			//std::cout << "SDIFDatabase: found loop points (" << simpleLoop.GetStartFrame() << ", " << simpleLoop.GetEndFrame() << ") for file <" << sampleMetadata.GetFilename() << ">" << std::endl;
		}

		// let's store the SDIFFileReader in a map where it's tied to its filename
		// so that we can easily find it later.
		sdifReaderMap[ sampleMetadata.GetFilename() ] = aBufferedSDIFFileReader;
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
	// delete all of the BufferedSDIFFileReader objects we created with new
	std::map<std::string, CLAM::BufferedSDIFFileReader*>::iterator iter;
	for( iter = sdifReaderMap.begin(); iter != sdifReaderMap.end(); iter++ ) {
		CLAM::BufferedSDIFFileReader* aBufferedSDIFFileReader = iter->second;
		delete aBufferedSDIFFileReader;
	}

	// empty the vector and map
	sdifReaderMap.clear();
	metadataVector.clear();
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
	}
}

Frame* SDIFDatabase::SelectFrameForVoice(VoiceId id, float argPitch, float argAmplitude)
{
	sourceMetadata.SetPitch(argPitch);
	sourceMetadata.SetAmplitude(argAmplitude);

	FindClosestSample(sourceMetadata, targetMetadata);

	CLAM::BufferedSDIFFileReader* aBufferedSDIFFileReader = sdifReaderMap[ targetMetadata.GetFilename() ];

	CLAM_DEBUG_ASSERT( aBufferedSDIFFileReader != NULL, "SDIFDatabase::SelectFrameForVoiceParameters - BufferedSDIFFileReader is null" );

	Frame* aFrame = aBufferedSDIFFileReader->ReadFrame();

	if ( !aBufferedSDIFFileReader->IsThreaded() )
	{
		aBufferedSDIFFileReader->LoadFramesIntoBufferOnThread( mThreadPool.GetThreadFromPool() );
	}

	// TODO how do you want to handle NULL frames?
	//CLAM_DEBUG_ASSERT( aFrame != NULL, "SDIFDatabase::SelectFrameForVoiceParameters - BufferedSDIFFileReader is null" );
	if (aFrame == NULL)
	{
		CLAM::SDIFInConfig theConfig = aBufferedSDIFFileReader->GetConfig();
		std::cout << "SDIFDatabase::SelectFrameForVoice; SDIFFile " << theConfig.GetFileName() << " has no more frames." << std::endl;
	}

	mSimpleTransformer.TransformFrequency( aFrame, targetMetadata.GetPitch(), argPitch );
	float sourceAmplitude = 0.75;
	mSimpleTransformer.TransformAmplitude( aFrame, sourceAmplitude, argAmplitude);

	return aFrame;
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
	argSourceMetadata.SetPitch(440);

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



