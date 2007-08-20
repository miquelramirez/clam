#ifndef _SampleManager_
#define _SampleManager_

#include "SDIFFileReader.hxx"
#include "SampleMetadata.hxx"
#include "DirectoryBasedMetadataReader.hxx"
#include "LoopingSDIFFileReader.hxx"
#include "SimpleTransformer.hxx"
#include "ThreadPool.hxx"
#include "ContinuousExcitationConstants.hxx"
#include <string>
#include <vector>
#include <map>
#include <list>
#include <time.h>

namespace CLAM
{
enum VoiceStage { Attack, Sustain, Release };

/**
* This class can be used to manage a database of SDIF files and their associated
* metadata. Given a directory path, this class loads all of the XML metadata
* files describing a database and then loads all of the SDIF files described
* in the metadata files. Then, it can be queried with control information such
* as the desired pitch and amplitude and it returns a frame of data drawn from
* the SDIF database that best meets the given specifications.
*
* \author greg kellum [gkellum@iua.upf.edu] 7/15/07
* \since  CLAM v1.1.
*/
class SDIFDatabase
{
public:
	SDIFDatabase();
	
	~SDIFDatabase();

	/**
	* Give this method a directory path and it will load the SDIF database
	* contained in that directory.
	*
	* @returns true if the directoryPath exist and metadata information was
	*          found in this directory
	*/
	bool LoadProgram(std::string& directoryPath, bool isResidualEnabled = true);

	/**
	* Call this method to unload a program and reclaim the associated memory.
	* If LoadProgram is called a second time, UnloadProgram will automatically
	* be called to clean up the last loaded program.
	*/
	void UnloadProgram();

	/**
	* The SDIF database needs to know from which stage of a sample a particular
	* frame should be drawn. If frames should be drawn first from the attack
	* stage, then create a new voice, and for this voice frames will be drawn
	* sequentially from the attack stage until the steady state portion of the
	* signal is reached.
	*/
	VoiceId CreateVoice();

	/**
	* Call this when a particular voice has ended.
	*/
	void DeleteVoice(VoiceId id);

	/**
	* Use this method to query an SDIFDatabase to select a particular frame
	* with a given set of control information. If no exact match is found,
	* the closest frame is retrieved and its content is transformed to
	* match the pitch and amplitude desired.
	*
	* @argument id - Use CreateVoice() to retrieve a voice id
	* @argument pitch - the pitch of the target frame
	* @argument amplitude - the amplitude of the target frame
	*/
	Frame* SelectFrameForVoice(VoiceId id, float pitch, float amplitude);

protected:
	/**
	* This returns true if a program, any program, is already loaded.
	*/
	bool isAnyProgramLoaded();
	/**
	* This returns true if the given program is already loaded.
	*/
	bool isThisProgramLoaded(std::string programsDirectoryPath);
	/**
	* This sorts the metadata run faster.
	*/
	void SortMetadata();
	/**
	* This finds the frame that is the closest match to the given target metadata.
	*/
	void FindClosestSample(SampleMetadata& inSourceMetadata, SampleMetadata& outTargetMetadata);
	/**
	* This finds the frames that surround metadata that doesn't match any given frame exactly
	*/
	void FindBounds(SampleMetadata& inSourceMetadata, SampleMetadata& outLowerBoundMetadata, SampleMetadata& outUpperBoundMetadata);

	CLAM::LoopingSDIFFileReader* GetLoopingSDIFFileReaderForVoiceId(VoiceId id);
	
	int ConvertMillisecondsToFrames(double milliseconds);

	int ConvertFramesToMilliseconds(int frames);

	void ConvertMillisecondsToFrames(std::vector<CLAM::SimpleLoopMetadata>& theLoopList);
	int FindMinimumEndLoopPoint(std::vector<CLAM::SimpleLoopMetadata>& theLoopList);
	int FindMaximumEndLoopPoint(std::vector<CLAM::SimpleLoopMetadata>& theLoopList);

private:

	std::string currentProgramsDirectoryPath;
	DirectoryBasedMetadataReader mMetadataReader;
	SampleMetadata sourceMetadata;
	SampleMetadata targetMetadata;
	SampleMetadata lowerBoundMetadata;
	SampleMetadata upperBoundMetadata;
	std::vector<CLAM::SampleMetadata> metadataVector;
	std::map<std::string, CLAM::LoopingSDIFFileReader*> sdifReaderMap;
	std::map<VoiceId, VoiceStage> voiceStageMap;
	// this is a map containing that sample that was played for a VoiceId
	std::map<VoiceId, CLAM::LoopingSDIFFileReader*> previousSDIFReaderMap;
	SimpleTransformer mSimpleTransformer;
	ThreadPool mThreadPool;
};

} // end namespace CLAM

#endif
