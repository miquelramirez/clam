#include "Schema.hxx"
#include "DescriptionScheme.hxx"
#include "Pool.hxx"
#include "SongFiles.hxx"
#include "Project.hxx"

#include "XMLStorage.hxx"
#include "AudioFile.hxx"
#include "IndexArray.hxx"

//For Descriptor Computation
#include "Segment.hxx"
#include "SegmentDescriptors.hxx"
#include "DescriptorComputation.hxx"
#include "AudioFile.hxx"
#include "MultiChannelAudioFileReader.hxx"
#include "MultiChannelAudioFileReaderConfig.hxx"
#include "FFT.hxx"
#include "Spectrum.hxx"
#include "SpectrumConfig.hxx"
#include "Segmentator.hxx"

#include <time.h>

void BuildAndDumpSchema(const char * schemaLocation);
void CreateLLSchema(CLAM_Annotator::LLDSchema& llschema);
void CreateHLSchema(CLAM_Annotator::HLDSchema& hlschema);
void PopulatePool(const CLAM_Annotator::Schema& schema, const CLAM_Annotator::Song song,
		CLAM::DescriptionDataPool& pool);
void GenerateRandomDescriptorValues(CLAM::TData* values, int size);
void GenerateRandomSegmentationMarks(CLAM::IndexArray* segmentation,int nSamples, int frameSize);
void OpenSoundFile(const std::string& filename, CLAM::Audio& audio, CLAM::Text & artist, CLAM::Text & title);
void FFTAnalysis(const CLAM::Audio& audio, CLAM::Segment& s);
void ComputeSegment(const CLAM::Audio& audio,CLAM::Segment& segment, 
		    CLAM::SegmentDescriptors& segmentD);
void SegmentD2Pool(const CLAM::SegmentDescriptors& segmentD, CLAM::DescriptionDataPool& pool);
void ComputeSegmentationMarks(CLAM::Segment& segment,CLAM::SegmentDescriptors& segmentD);
void Segment2Marks(const CLAM::Segment& segment, CLAM::IndexArray* marks);
int GetnSamples(const std::string& fileName);

int main()
{
	const char * schemaLocation = "../Samples/Schema.sc";
	const char * projectLocation = "../Samples/Project.pro";
	const char* songFileNames[] =
	{
		"../../CLAM-TestData/trumpet.mp3",
		"../../CLAM-TestData/Elvis.ogg",
		"../../CLAM-TestData/trumpet.wav",
		"../../CLAM-TestData/Elvis.wav",
		"../Samples/SongsTest/02.mp3",
		"../Samples/SongsTest/03.mp3",
		0
	};

	BuildAndDumpSchema(schemaLocation);

	//Create and store Project
	CLAM_Annotator::Project myProject;
	for (const char ** filename = songFileNames; *filename; filename++)
		myProject.AppendSong(*filename);

	myProject.LoadScheme(schemaLocation);

	CLAM::XMLStorage::Dump(myProject,"Project",projectLocation);


	//Now we create a Pool for every sound file we have
	CLAM::DescriptionDataPool pool(myProject.GetDescriptionScheme());

	std::vector<CLAM_Annotator::Song>::iterator currentSong;
	srand(time(NULL));

	for (
		currentSong = myProject.GetSongs().begin();
		currentSong != myProject.GetSongs().end();
		currentSong++)
	{
		std::cout<<"Computing Descriptors for file "<<(*currentSong).GetSoundFile()
		     <<" Please wait..."<<std::endl;
		PopulatePool(myProject.GetAnnotatorSchema(), *currentSong, pool);
		//Dump Descriptors Pool
		std::string poolFile;
		if((*currentSong).HasPoolFile()) poolFile = (*currentSong).GetPoolFile();
		else poolFile = (*currentSong).GetSoundFile()+".pool";
		CLAM::XMLStorage::Dump(pool, "DescriptorsPool", poolFile);

		//Now we load the Pool and validate it with the schema
		CLAM::DescriptionDataPool loadedDescriptorPool(myProject.GetDescriptionScheme());
		CLAM::XMLStorage::Restore(loadedDescriptorPool,poolFile);

		if(myProject.GetAnnotatorSchema().GetHLDSchema().Validate(loadedDescriptorPool))
			std::cout<<"Descriptor Pool Validated With Schema"<<std::endl;
		else
			std::cout<<"Descriptor Pool Did Not Validate With Schema"<<std::endl;
	}

	return 0;

}

void BuildAndDumpSchema(const char * schemaLocation)
{
	CLAM_Annotator::Schema testSchema;
	CreateLLSchema(testSchema.GetLLDSchema());
	CreateHLSchema(testSchema.GetHLDSchema());

	CLAM::XMLStorage::Dump(testSchema, "Schema",schemaLocation);
}

void CreateLLSchema(CLAM_Annotator::LLDSchema& llschema)
{
	const char * lowLevelDescriptorsNames[] =
	{
		"Mean",
		"GeometricMean",
		"Energy",
		"Centroid",
		"Moment2",
		"Moment3",
		"Moment4",
		"Moment5",
		"Moment6",
		"Flatness",
		"MagnitudeKurtosis",
		"MaxMagFreq",
		"LowFreqEnergyRelation",
		"Spread",
		"MagnitudeSkewness",
		"Rolloff",
		"Slope",
		"HighFrequencyContent",
		0
	};
	for (const char ** name = lowLevelDescriptorsNames; *name; name++)
		llschema.GetLLDNames().push_back(*name);
}

void AddRestrictedStringHLAttribute(
		CLAM_Annotator::HLDSchema& hlschema,
		const std::string & attribute,
		const char ** availableValues)
{
	CLAM_Annotator::HLDSchemaElement testHLDesc;
	testHLDesc.SetName(attribute);
	testHLDesc.SetType("RestrictedString");
	testHLDesc.AddRestrictionValues();
	testHLDesc.UpdateData();
	for (const char ** value = availableValues; *value; value++)
		testHLDesc.GetRestrictionValues().push_back(*value);
	hlschema.GetHLDs().push_back(testHLDesc);
}
void AddRangedIntHLAttribute(
		CLAM_Annotator::HLDSchema& hlschema,
		const std::string & attribute,
		int min, int max)
{
	CLAM_Annotator::HLDSchemaElement testHLDesc;
	testHLDesc.SetName(attribute);
	testHLDesc.SetType("Int");
	testHLDesc.AddiRange();
	testHLDesc.UpdateData();
	CLAM_Annotator::Range<int> range;
	range.SetMin(min);
	range.SetMax(max);
	testHLDesc.SetiRange(range);
	hlschema.GetHLDs().push_back(testHLDesc);
}

void AddRangedRealHLAttribute(
		CLAM_Annotator::HLDSchema& hlschema,
		const std::string & attribute,
		double min, double max)
{
	CLAM_Annotator::HLDSchemaElement testHLDesc;
	testHLDesc.SetName(attribute);
	testHLDesc.SetType("Float");
	testHLDesc.AddfRange();
	testHLDesc.UpdateData();
	CLAM_Annotator::Range<float> range;
	range.SetMin(min);
	range.SetMax(max);
	testHLDesc.SetfRange(range);
	hlschema.GetHLDs().push_back(testHLDesc);
}
void AddStringHLAttribute(
		CLAM_Annotator::HLDSchema& hlschema,
		const std::string & attribute)
{
	CLAM_Annotator::HLDSchemaElement testHLDesc;
	testHLDesc.SetName(attribute);
	testHLDesc.SetType("String");
	hlschema.GetHLDs().push_back(testHLDesc);
}

void AddSegmentationHLAttribute(CLAM_Annotator::HLDSchema& hlschema,
		const std::string & attribute, const std::string & childScope)
{
	CLAM_Annotator::HLDSchemaElement testHLDesc;
	testHLDesc.AddChildScope();
	testHLDesc.UpdateData();
	testHLDesc.SetName(attribute);
	testHLDesc.SetType("Segmentation");
	testHLDesc.SetChildScope(childScope);
	hlschema.GetHLDs().push_back(testHLDesc);
}

void CreateHLSchema(CLAM_Annotator::HLDSchema& hlschema)
{
	AddStringHLAttribute(hlschema, "Artist");
	AddStringHLAttribute(hlschema, "Title");
	const char * genreValues[] =
	{
		"Dance",
		"Classic",
		"Jazz",
		"Rhythm&Blues",
		"Folk",
		0
	};
	AddRestrictedStringHLAttribute(hlschema, "Genre", genreValues);
	AddRangedRealHLAttribute(hlschema, "Danceability", 0., 10.);
	const char * keyValues[] =
	{
		"A", "A#", "B", "C", "C#",
		"D", "D#", "E", "F", "F#",
		"G", "G#", 0
	};
	AddRestrictedStringHLAttribute(hlschema, "Key", keyValues);
	const char * modeValues[] =
	{
		"Minor",
		"Major",
		0
	};
	AddRestrictedStringHLAttribute(hlschema, "Mode", modeValues);
	AddRangedRealHLAttribute(hlschema, "DynamicComplexity", 0., 10.);
	AddRangedIntHLAttribute(hlschema, "BPM", 0, 240);
	AddSegmentationHLAttribute(hlschema, "Onsets", "Onset");
	AddSegmentationHLAttribute(hlschema, "RandomSegments", "");
}


void PopulatePool(const CLAM_Annotator::Schema& schema, const CLAM_Annotator::Song song, 
		CLAM::DescriptionDataPool& pool)
{
  
	//Create Descriptors Pool
	pool.SetNumberOfContexts("Song",1);

	//Generate LLDs values
	CLAM::Audio audio;
	CLAM::Segment segment;
	CLAM::SegmentDescriptors segmentD;
	CLAM::Text artist="unknown";
	CLAM::Text title="unknown";
	OpenSoundFile(song.GetSoundFile(),audio, artist, title);
	ComputeSegment(audio,segment,segmentD);
	SegmentD2Pool(segmentD,pool);

	//Create segmentation marks
	CLAM::IndexArray* segmentation = 
		pool.GetWritePool<CLAM::IndexArray>("Song","Onsets");
	ComputeSegmentationMarks(segment, segmentD);
	Segment2Marks(segment,segmentation);
	
	CLAM::IndexArray* randomSegmentation = 
		pool.GetWritePool<CLAM::IndexArray>("Song","RandomSegments");
	GenerateRandomSegmentationMarks(randomSegmentation, GetnSamples(song.GetSoundFile()), 1024);

	//Write HLD values
	std::list<CLAM_Annotator::HLDSchemaElement>& hlds = schema.GetHLDSchema().GetHLDs();
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it2 = hlds.begin();
	for(int i=0; it2 != hlds.end(); it2++,i++)
	{
		if((*it2).GetName()=="Artist")
		{
			CLAM::Text* value = pool.GetWritePool<CLAM::Text>("Song",(*it2).GetName());
			*value = artist;
		}
		else if((*it2).GetName()=="Title")
		{
			CLAM::Text* value = pool.GetWritePool<CLAM::Text>("Song",(*it2).GetName());
			*value = title;
		}
		else if((*it2).GetName()=="Genre")
		{
			CLAM_Annotator::RestrictedString* value = 
			pool.GetWritePool<CLAM_Annotator::RestrictedString>("Song",(*it2).GetName());
			*value = "Folk";
		}
		else if((*it2).GetName()=="Danceability")
		{
			float* value = pool.GetWritePool<float>("Song",(*it2).GetName());
			*value = 7.2;
		}
		else if((*it2).GetName()=="Key")
		{
			CLAM_Annotator::RestrictedString* value = 
			pool.GetWritePool<CLAM_Annotator::RestrictedString>("Song",(*it2).GetName());
			*value = "C";
		}
		else if((*it2).GetName()=="Mode")
		{
			CLAM_Annotator::RestrictedString* value = 
			pool.GetWritePool<CLAM_Annotator::RestrictedString>("Song",(*it2).GetName());
			*value = "Minor";
		}
		else if((*it2).GetName()=="DynamicComplexity")
		{
			float* value = pool.GetWritePool<float>("Song",(*it2).GetName());
			*value = 8.1;
		}
		else if((*it2).GetName()=="BPM")
		{
			int* value = pool.GetWritePool<int>("Song",(*it2).GetName());
			*value = 100;
		}
	}

}

void GenerateRandomDescriptorValues(CLAM::TData* values, int size)
{
	int randomInt=(float (rand())/float(RAND_MAX))*100;
	int randomIncr;
	for (int i=0; i<size; i++)
	{
		randomIncr = (float (rand())/float(RAND_MAX))*20-10;
		randomInt += randomIncr;
		if(randomInt>100) randomInt = 80;
		if(randomInt<0) randomInt=20;

		values[i] = randomInt;
	}


}

int GetnSamples(const std::string& fileName)
{
	CLAM::AudioFile file;
	file.OpenExisting(fileName);
	CLAM::TData duration = file.GetHeader().GetLength();
	CLAM::TData sampleRate = file.GetHeader().GetSampleRate();
	return (int)(duration*sampleRate/1000.);
}

void GenerateRandomSegmentationMarks(CLAM::IndexArray* segmentation,int nSamples, 
				     int frameSize)
{
	int index = 0, randomIncr;
	while(index<nSamples)
	{
		  //random number between 10 and 30 frames
		randomIncr = ((float (rand())/float(RAND_MAX))*200+100)*frameSize;
		index += randomIncr;
		(*segmentation).AddElem(index);
	}

}

void SegmentD2Pool(const CLAM::SegmentDescriptors& segmentD, CLAM::DescriptionDataPool& pool)
{
	int nFrames = segmentD.GetFramesD().Size();
	pool.SetNumberOfContexts("Frame",nFrames);

	CLAM::Array<CLAM::FrameDescriptors>& frameD = segmentD.GetFramesD();
	CLAM::TData* values;

	values = pool.GetWritePool<CLAM::TData>("Frame","Mean");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetMean();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","GeometricMean");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetGeometricMean();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","Energy");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetEnergy();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","Centroid");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetCentroid();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","Moment2");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetMoment2();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","Moment3");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetMoment3();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","Moment4");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetMoment4();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","Moment5");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetMoment5();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","Moment6");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetMoment6();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","Flatness");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetFlatness();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","MagnitudeKurtosis");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetMagnitudeKurtosis();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","MaxMagFreq");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetMaxMagFreq();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","LowFreqEnergyRelation");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetLowFreqEnergyRelation();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","Spread");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetSpread();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","MagnitudeSkewness");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetMagnitudeSkewness();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","Rolloff");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetRolloff();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","Slope");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetSlope();
	}

	values = pool.GetWritePool<CLAM::TData>("Frame","HighFrequencyContent");
	for(int i=0; i<nFrames; i++)
	{
		values[i]=frameD[i].GetSpectrumD().GetHighFrequencyContent();
	}
}

void ComputeSegment(const CLAM::Audio& audio,CLAM::Segment& segment, 
		     CLAM::SegmentDescriptors& segmentDescriptors)
{
	FFTAnalysis(audio, segment);

	segmentDescriptors.AddAll();
	segmentDescriptors.UpdateData();

	CLAM::SpectralDescriptors specProto;
	specProto.AddAll();
	specProto.UpdateData();

	CLAM::FrameDescriptors frameProto;
	frameProto.AddSpectrumD();
	frameProto.UpdateData();

	frameProto.GetSpectrumD().SetPrototype(specProto);
		
	segmentDescriptors.SetFramePrototype(frameProto,segment.GetnFrames());
	segmentDescriptors.SetpSegment(&segment);
		
	CLAM::DescriptorComputation processing;
	processing.Do(segmentDescriptors);
}

void OpenSoundFile(const std::string& filename, CLAM::Audio& audio, CLAM::Text & artist, CLAM::Text & title)
{
	const CLAM::TSize readSize = 1024;
	CLAM::AudioFile file;
	file.OpenExisting(filename);
	const CLAM::AudioTextDescriptors & textDescriptors = file.GetTextDescriptors();
	if (textDescriptors.HasArtist()) artist = textDescriptors.GetArtist();
	if (textDescriptors.HasTitle()) title = textDescriptors.GetTitle();
	int nChannels = file.GetHeader().GetChannels();
	std::vector<CLAM::Audio> audioFrameVector(nChannels);
	for (int i=0;i<nChannels;i++)
		audioFrameVector[i].SetSize(readSize);
	CLAM::MultiChannelAudioFileReaderConfig cfg;
	cfg.SetSourceFile( file );
	CLAM::MultiChannelAudioFileReader reader(cfg);
	reader.Start();
	int beginSample=0;
	int nSamples = file.GetHeader().GetSamples(); 
	audio.SetSize(nSamples);
	while(reader.Do(audioFrameVector))
	{
		audio.SetAudioChunk(beginSample,audioFrameVector[0]);
		beginSample+=readSize;
		if(beginSample+readSize>nSamples) break;
	}
	reader.Stop();
}

void FFTAnalysis(const CLAM::Audio& audio, CLAM::Segment& s)
{
	int frameSize = 1023;
	CLAM::FFTConfig cfg;
	cfg.SetAudioSize(frameSize);
	CLAM::FFT fft(cfg);
	fft.Start();
	CLAM::SpectrumConfig spcfg;
	spcfg.SetSize(512);
	int audioSize = audio.GetSize();
	int samplingRate = audio.GetSampleRate();
	int duration = 1000*frameSize/samplingRate;
	for (int i=0; i< audioSize; i+=frameSize)
	{
		CLAM::Audio audioFrame;
		audio.GetAudioChunk(i, i+frameSize, audioFrame);

		CLAM::Spectrum spec(spcfg);
		fft.Do(audioFrame, spec);

		CLAM::Frame tmpFrame;
		tmpFrame.AddSpectrum();
		tmpFrame.UpdateData();
		tmpFrame.SetSpectrum(spec);
		tmpFrame.SetDuration(duration);
		tmpFrame.SetCenterTime(1000*(i+frameSize*0.5)/samplingRate);

		s.AddFrame(tmpFrame);
	}
}

void ComputeSegmentationMarks(CLAM::Segment& segment,CLAM::SegmentDescriptors& segmentD)
{
	CLAM::TData ePercentil, eThr, minLength;
	bool useDefault=true;
	if(useDefault) {
		ePercentil = 400;
		eThr = CLAM::TData(2.00);//0.0032;
		minLength = 2;
	}
	CLAM::SegmentatorConfig sgConfig;
	CLAM::TDescriptorsParams tmpParams;
	tmpParams.id=CLAM::SpectralEnergyId;
	tmpParams.percentil=ePercentil;
	tmpParams.threshold=eThr;
	sgConfig.AddDescParams(tmpParams);
	sgConfig.SetMinSegmentLength(int(minLength));
	CLAM::Segmentator mySegmentator(sgConfig);
	mySegmentator.Start();

	//Segmentate
	mySegmentator.Do(segment,segmentD);
}

void Segment2Marks(const CLAM::Segment& segment, CLAM::IndexArray* marks)
{
	CLAM::List<CLAM::Segment>& children = segment.GetChildren();
	children.DoFirst();
	int samplingRate = segment.GetSamplingRate();
	int nSegments = children.Size();
	int segmentDuration = segment.GetEndTime();
	for (int i=0; i<nSegments; i++)
	{
		int currentTime = children[i].GetEndTime();
		if(currentTime>0&&currentTime<segmentDuration)
		{
			marks->AddElem(children[i].GetEndTime()/1000.*samplingRate);
		}
	}

}


