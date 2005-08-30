#include "Schema.hxx"
#include "LLDSchema.hxx"
#include "HLDSchema.hxx"
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
void CreatePoolScheme(const CLAM_Annotator::Schema& schema, CLAM::DescriptionScheme& poolScheme);
template <class T> CLAM_Annotator::Descriptor<T> MakeDescriptor(T value,const std::string& name);

void PopulatePool(const CLAM_Annotator::Schema& schema, const CLAM_Annotator::Song song,
		CLAM::DescriptionDataPool& pool);
void GenerateRandomDescriptorValues(CLAM::TData* values, int size);
void GenerateRandomSegmentationMarks(CLAM::IndexArray* segmentation,int nSamples, int frameSize);
void OpenSoundFile(const std::string& filename, CLAM::Audio& audio);
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
		"../Samples/SongsTest/02.mp3",
		"../Samples/SongsTest/03.mp3",
		0
	};

	BuildAndDumpSchema(schemaLocation);

	//Create and store Project
	CLAM_Annotator::Project myProject;
	myProject.SetSchema(schemaLocation);
	for (const char ** filename = songFileNames; *filename; filename++)
		myProject.AppendSong(*filename);

	CLAM::XMLStorage::Dump(myProject,"Project",projectLocation);

	//Load Schema
	CLAM_Annotator::Schema loadedSchema;
	CLAM::XMLStorage::Restore(loadedSchema, schemaLocation);

	//Create Descriptors Pool Scheme and add attributes following loaded schema
	CLAM::DescriptionScheme scheme;
	CreatePoolScheme(loadedSchema, scheme);

	//Now we create a Pool for every sound file we have
	CLAM::DescriptionDataPool pool(scheme);

	std::vector<CLAM_Annotator::Song>::iterator currentSong;
	srand(time(NULL));

	for (
		currentSong = myProject.GetSongs().begin();
		currentSong != myProject.GetSongs().end();
		currentSong++)
	{
		std::cout<<"Computing Descriptors for file "<<(*currentSong).GetSoundFile()
		     <<" Please wait..."<<std::endl;
		PopulatePool(loadedSchema, *currentSong, pool);
		//Dump Descriptors Pool
		std::string poolFile;
		if((*currentSong).HasPoolFile()) poolFile = (*currentSong).GetPoolFile();
		else poolFile = (*currentSong).GetSoundFile()+".pool";
		CLAM::XMLStorage::Dump(pool, "DescriptorsPool", poolFile);

		//Now we load the Pool and validate it with the schema
		CLAM::DescriptionDataPool loadedDescriptorPool(scheme);
		CLAM::XMLStorage::Restore(loadedDescriptorPool,poolFile);

		if(loadedSchema.GetHLDSchema().Validate(loadedDescriptorPool))
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
	llschema.GetLLDNames().push_back("Mean");
	llschema.GetLLDNames().push_back("GeometricMean");
	llschema.GetLLDNames().push_back("Energy");
	llschema.GetLLDNames().push_back("Centroid");
	llschema.GetLLDNames().push_back("Moment2");
	llschema.GetLLDNames().push_back("Moment3");
	llschema.GetLLDNames().push_back("Moment4");
	llschema.GetLLDNames().push_back("Moment5");
	llschema.GetLLDNames().push_back("Moment6");
	llschema.GetLLDNames().push_back("Flatness");
	llschema.GetLLDNames().push_back("MagnitudeKurtosis");
	llschema.GetLLDNames().push_back("MaxMagFreq"); 
	llschema.GetLLDNames().push_back("LowFreqEnergyRelation"); 
	llschema.GetLLDNames().push_back("Spread");
	llschema.GetLLDNames().push_back("MagnitudeSkewness");
	llschema.GetLLDNames().push_back("Rolloff"); 
	llschema.GetLLDNames().push_back("Slope"); 
	llschema.GetLLDNames().push_back("HighFrequencyContent");
}

void CreateHLSchema(CLAM_Annotator::HLDSchema& hlschema)
{
	{
		CLAM_Annotator::HLDSchemaElement testHLDesc;
		testHLDesc.SetName("Artist");
		testHLDesc.SetType("String");
		hlschema.GetHLDs().push_back(testHLDesc);
	}
	{
		CLAM_Annotator::HLDSchemaElement testHLDesc;
		testHLDesc.SetName("Title");
		testHLDesc.SetType("String");
		hlschema.GetHLDs().push_back(testHLDesc);
	}
	{
		CLAM_Annotator::HLDSchemaElement testHLDesc;
		testHLDesc.SetName("Genre");
		testHLDesc.SetType("RestrictedString");
		testHLDesc.AddRestrictionValues();
		testHLDesc.UpdateData();
		testHLDesc.GetRestrictionValues().push_back("Dance");
		testHLDesc.GetRestrictionValues().push_back("Classic");
		testHLDesc.GetRestrictionValues().push_back("Jazz");
		testHLDesc.GetRestrictionValues().push_back("Rhythm&Blues");
		testHLDesc.GetRestrictionValues().push_back("Folk");
		hlschema.GetHLDs().push_back(testHLDesc);
	}
	{
		CLAM_Annotator::HLDSchemaElement testHLDesc;
		testHLDesc.SetName("Danceability");
		testHLDesc.SetType("Float");
		testHLDesc.AddfRange();
		testHLDesc.UpdateData();
		CLAM_Annotator::Range<float> range;
		range.SetMin(0.);
		range.SetMax(10.);
		testHLDesc.SetfRange(range);
		hlschema.GetHLDs().push_back(testHLDesc);
	}
	{
		CLAM_Annotator::HLDSchemaElement testHLDesc;
		testHLDesc.SetName("Key");
		testHLDesc.SetType("RestrictedString");
		testHLDesc.AddRestrictionValues();
		testHLDesc.UpdateData();
		testHLDesc.GetRestrictionValues().push_back("A");
		testHLDesc.GetRestrictionValues().push_back("A#");
		testHLDesc.GetRestrictionValues().push_back("B");
		testHLDesc.GetRestrictionValues().push_back("C");
		testHLDesc.GetRestrictionValues().push_back("C#");
		testHLDesc.GetRestrictionValues().push_back("D");
		testHLDesc.GetRestrictionValues().push_back("D#");
		testHLDesc.GetRestrictionValues().push_back("E");
		testHLDesc.GetRestrictionValues().push_back("F");
		testHLDesc.GetRestrictionValues().push_back("F#");
		testHLDesc.GetRestrictionValues().push_back("G");
		testHLDesc.GetRestrictionValues().push_back("G#");
		hlschema.GetHLDs().push_back(testHLDesc);
	}
	{
		CLAM_Annotator::HLDSchemaElement testHLDesc;
		testHLDesc.SetName("Mode");
		testHLDesc.SetType("RestrictedString");
		testHLDesc.AddRestrictionValues();
		testHLDesc.UpdateData();
		testHLDesc.GetRestrictionValues().push_back("Minor");
		testHLDesc.GetRestrictionValues().push_back("Major");
		hlschema.GetHLDs().push_back(testHLDesc);
	}
	{
		CLAM_Annotator::HLDSchemaElement testHLDesc;
		testHLDesc.SetName("DynamicComplexity");
		testHLDesc.SetType("Float");
		testHLDesc.AddfRange();
		testHLDesc.UpdateData();
		CLAM_Annotator::Range<float> range;
		range.SetMin(0.);
		range.SetMax(10.);
		testHLDesc.SetfRange(range);
		hlschema.GetHLDs().push_back(testHLDesc);
	}
	{
		CLAM_Annotator::HLDSchemaElement testHLDesc;
		testHLDesc.SetName("BPM");
		testHLDesc.SetType("Int");
		testHLDesc.AddiRange();
		testHLDesc.UpdateData();
		CLAM_Annotator::Range<int> range;
		range.SetMin(0);
		range.SetMax(240);
		testHLDesc.SetiRange(range);
		hlschema.GetHLDs().push_back(testHLDesc);
	}
}

void CreatePoolScheme(const CLAM_Annotator::Schema& schema, CLAM::DescriptionScheme& poolScheme)
{
	//First we start with HLD
	std::list<CLAM_Annotator::HLDSchemaElement>& hlds = schema.GetHLDSchema().GetHLDs();
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it2;
	for(it2 = hlds.begin(); it2 != hlds.end(); it2++)
	{
		if((*it2).GetType()=="Float")
		{
			poolScheme.AddAttribute <float>("Song",(*it2).GetName());
		}
		else if((*it2).GetType()=="Int")
		{
			poolScheme.AddAttribute <int>("Song",(*it2).GetName());
		}
		else if((*it2).GetType()=="RestrictedString")
		{
			poolScheme.AddAttribute <CLAM_Annotator::RestrictedString>("Song",(*it2).GetName());
		}
		else
		{
			poolScheme.AddAttribute <std::string>("Song",(*it2).GetName());
		}
	}
	//And now we go into LLD
	std::list<std::string>::iterator it;
	std::list<std::string>& descriptorsNames = schema.GetLLDSchema().GetLLDNames();
	for(it = descriptorsNames.begin(); it != descriptorsNames.end(); it++)
	{
		poolScheme.AddAttribute <CLAM::TData>("Frame", (*it));
	}
	//finally we add segmentation marks
	poolScheme.AddAttribute<CLAM::IndexArray>("Song","Segments");

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
	OpenSoundFile(song.GetSoundFile(),audio);
	ComputeSegment(audio,segment,segmentD);
	SegmentD2Pool(segmentD,pool);

	//Create segmentation marks
	CLAM::IndexArray* segmentation = 
	pool.GetWritePool<CLAM::IndexArray>("Song","Segments");
	//GenerateRandomSegmentationMarks(segmentation, GetnSamples(song.GetSoundFile()), 1024);
	ComputeSegmentationMarks(segment, segmentD);
	Segment2Marks(segment,segmentation);

	//Write HLD values
	std::list<CLAM_Annotator::HLDSchemaElement>& hlds = schema.GetHLDSchema().GetHLDs();
	std::list<CLAM_Annotator::HLDSchemaElement>::iterator it2 = hlds.begin();
	for(int i=0; it2 != hlds.end(); it2++,i++)
	{
		if((*it2).GetName()=="Artist")
		{
			std::string* value = pool.GetWritePool<std::string>("Song",(*it2).GetName());
			*value = "Triana";
		}
		else if((*it2).GetName()=="Title")
		{
			std::string* value = pool.GetWritePool<std::string>("Song",(*it2).GetName());
			if(i==0)
				*value = "Pájaro_de_Alas_Blancas";
			else
				*value = "En_una_Esquina_Cualquiera";
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

void OpenSoundFile(const std::string& filename, CLAM::Audio& audio)
{
	const CLAM::TSize readSize = 1024;
	CLAM::AudioFile file;
	file.OpenExisting(filename);
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


