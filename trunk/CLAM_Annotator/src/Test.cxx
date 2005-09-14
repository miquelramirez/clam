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

void BuildAndDumpTestSchema(const char * schemaLocation);
void PopulatePool(const std::string& song, CLAM::DescriptionDataPool& pool);
void GenerateRandomDescriptorValues(CLAM::TData* values, int size);
void GenerateRandomSegmentationMarks(CLAM::IndexArray* segmentation,int nSamples, int frameSize);
void OpenSoundFile(const std::string& filename, CLAM::Audio& audio, CLAM::Text & artist, CLAM::Text & title);
void FFTAnalysis(const CLAM::Audio& audio, CLAM::Segment& s);
void ComputeSegment(const CLAM::Audio& audio,CLAM::Segment& segment, 
		    CLAM::SegmentDescriptors& segmentD);
void SegmentD2Pool(const CLAM::SegmentDescriptors& segmentD, CLAM::DescriptionDataPool& pool);
void ComputeSegmentationMarks(CLAM::Segment& segment,CLAM::SegmentDescriptors& segmentD);
void Segment2Marks(const CLAM::Segment& segment, CLAM::IndexArray & marks);
int GetnSamples(const std::string& fileName);

int main(int argc, char ** argv)
{
	const char * schemaLocation = "../Samples/Schema.sc";
	const char * projectLocation = "../Samples/Project.pro";
	const char * songFileNames[] =
	{
		"../../CLAM-TestData/trumpet.mp3",
		"../../CLAM-TestData/Elvis.ogg",
		"../../CLAM-TestData/trumpet.wav",
		"../../CLAM-TestData/Elvis.wav",
		"../Samples/SongsTest/02.mp3",
		"../Samples/SongsTest/03.mp3",
		"../Samples/SongsTest/Franz Ferdinand - Franz Ferdinand - 02 - Tell Her Tonight.ogg",
		"../Samples/SongsTest/Coldplay - Parachutes - 01 - Don't Panic.mp3",
		"../Samples/SongsTest/06 - Up In Arms.mp3",
		0
	};
	bool generateJustPools = (argc>1);

	if (!generateJustPools)
		BuildAndDumpTestSchema(schemaLocation);

	//Create and store Project
	CLAM_Annotator::Project myProject;
	if (generateJustPools)
		for (unsigned i = 1; i<argc; i++)
			myProject.AppendSong(argv[i]);
	else
		for (const char ** filename = songFileNames; *filename; filename++)
			myProject.AppendSong(*filename);

	myProject.LoadScheme(schemaLocation);

	if (!generateJustPools)
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
		std::cout<<"Computing Descriptors for file "<< currentSong->GetSoundFile()
		     <<" Please wait..."<<std::endl;
		PopulatePool(currentSong->GetSoundFile(), pool);
		//Dump Descriptors Pool
		std::string poolFile;
		if (currentSong->HasPoolFile()) poolFile = currentSong->GetPoolFile();
		else poolFile = currentSong->GetSoundFile()+".pool";
		CLAM::XMLStorage::Dump(pool, "DescriptorsPool", poolFile);

		//Now we load the Pool and validate it with the schema
		CLAM::DescriptionDataPool toValidadDescription(myProject.GetDescriptionScheme());
		CLAM::XMLStorage::Restore(toValidadDescription, poolFile);

		if (myProject.ValidateDataPool(toValidadDescription))
			std::cout<<"Descriptor Pool Validated With Schema"<<std::endl;
		else
			std::cout<<"Descriptor Pool Did Not Validate With Schema"<<std::endl;
	}

	return 0;

}

void BuildAndDumpTestSchema(const char * schemaLocation)
{
	CLAM_Annotator::Schema schema;

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
		schema.AddFrameFloatAttribute(*name);

	CLAM_Annotator::HLDSchema& hlschema = schema.GetHLDSchema();
	hlschema.AddString("Song","Artist");
	hlschema.AddString("Song","Title");
	const char * genreValues[] =
	{
		"Dance",
		"Classic",
		"Jazz",
		"Rhythm&Blues",
		"Folk",
		0
	};
	hlschema.AddRestrictedString("Song","Genre", genreValues);
	hlschema.AddRangedReal("Song","Danceability", 0., 10.);
	const char * keyValues[] =
	{
		"A", "A#", "B", "C", "C#",
		"D", "D#", "E", "F", "F#",
		"G", "G#", 0
	};
	hlschema.AddRestrictedString("Song","Key", keyValues);
	const char * modeValues[] =
	{
		"Minor",
		"Major",
		0
	};
	hlschema.AddRestrictedString("Song","Mode", modeValues);
	hlschema.AddRangedReal("Song","DynamicComplexity", 0., 10.);
	hlschema.AddRangedInt("Song","BPM", 0, 240);
	hlschema.AddSegmentation("Song","RandomSegments", "");
	hlschema.AddSegmentation("Song","Onsets", "Onset");
	hlschema.AddRangedReal("Onset","Relevance", 0., 10.);
	const char * onsetKindValues[] =
	{
		"PitchChange",
		"EnergyChange",
		0
	};
	hlschema.AddRestrictedString("Onset","DetectedChange", onsetKindValues);
	hlschema.AddSegmentation("Song", "Notes", "Note");
	hlschema.AddRestrictedString("Note", "Pitch", keyValues);
	hlschema.AddRangedInt("Note", "Octave", 1, 12);
	hlschema.AddString("Note", "Instrument");

	CLAM::XMLStorage::Dump(schema, "Schema", schemaLocation);
	CLAM::XMLStorage::Dump(schema, "SChema", std::cout);
}


void PopulatePool(const std::string & song, 
		CLAM::DescriptionDataPool& pool)
{
  
	//Create Descriptors Pool
	pool.SetNumberOfContexts("Song",1);

	//Generate LLDs values
	CLAM::Audio audio;
	CLAM::Segment segment;
	CLAM::SegmentDescriptors segmentD;
	CLAM::Text artist="Unknown Artist";
	CLAM::Text title="Unknown Title";
	OpenSoundFile(song, audio, artist, title);
	ComputeSegment(audio,segment,segmentD);
	SegmentD2Pool(segmentD,pool);

	//Create segmentation marks
	CLAM::IndexArray & segmentation = 
		pool.GetWritePool<CLAM::IndexArray>("Song","Onsets")[0];
	ComputeSegmentationMarks(segment, segmentD);
	Segment2Marks(segment,segmentation);
	
	unsigned nOnsets = segmentation.Size();
	if (nOnsets==0) nOnsets=1; // KLUDGE!!
	pool.SetNumberOfContexts("Onset",nOnsets);
	float * onsetForces = pool.GetWritePool<float>("Onset","Relevance");
	CLAM_Annotator::RestrictedString * onsetChange = pool.GetWritePool<CLAM_Annotator::RestrictedString>("Onset","DetectedChange");
	for (unsigned i = 0; i<nOnsets; i++)
	{
		onsetForces[i] = float (rand())/float(RAND_MAX)*10;
		onsetChange[i] = (float (rand())/float(RAND_MAX)*2)>1.0 ? "PitchChange" : "EnergyChange";
	}

	CLAM::IndexArray* randomSegmentation = 
		pool.GetWritePool<CLAM::IndexArray>("Song","RandomSegments");
	GenerateRandomSegmentationMarks(randomSegmentation, GetnSamples(song), 1024);

	CLAM::IndexArray* noteSegmentation = 
		pool.GetWritePool<CLAM::IndexArray>("Song","Notes");
	GenerateRandomSegmentationMarks(noteSegmentation, GetnSamples(song), 1024);
	unsigned nNotes = noteSegmentation->Size();
	pool.SetNumberOfContexts("Note",nNotes);
	CLAM_Annotator::RestrictedString * notePitch = pool.GetWritePool<CLAM_Annotator::RestrictedString>("Note","Pitch");
	int * noteOctave = pool.GetWritePool<int>("Note","Octave");
	const char * pitchValues[] =
	{
		"A", "A#", "B", "C", "C#",
		"D", "D#", "E", "F", "F#",
		"G", "G#", 0
	};
	for (unsigned i = 0; i<nNotes; i++)
	{
		noteOctave[i] = std::max(std::min(int(float (rand())/float(RAND_MAX)*11),10), 0)+1;
		unsigned pitch = std::max(std::min(int(float (rand())/float(RAND_MAX)*12), 11), 0);
		notePitch[i] = pitchValues[pitch];
	}


	pool.GetWritePool<CLAM::Text>("Song","Artist")[0] = artist;
	pool.GetWritePool<CLAM::Text>("Song","Title")[0] = title;
	pool.GetWritePool<CLAM_Annotator::RestrictedString>("Song","Genre")[0] = "Folk";
	pool.GetWritePool<float>("Song","Danceability")[0] = 7.2;
	pool.GetWritePool<CLAM_Annotator::RestrictedString>("Song","Key")[0] = "C";
	pool.GetWritePool<CLAM_Annotator::RestrictedString>("Song","Mode")[0] = "Minor";
	pool.GetWritePool<float>("Song","DynamicComplexity")[0] = 8.1;
	pool.GetWritePool<int>("Song","BPM")[0] = 100;

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
	struct GetterMap 
	{
		const char * name;
		CLAM::TData & (CLAM::SpectralDescriptors::*getter)() const;
	} spectralDescriptors[] =
	{
		{"Mean", &CLAM::SpectralDescriptors::GetMean},
		{"GeometricMean",  &CLAM::SpectralDescriptors::GetGeometricMean},
		{"Energy",  &CLAM::SpectralDescriptors::GetEnergy},
		{"Centroid",  &CLAM::SpectralDescriptors::GetCentroid},
		{"Moment2",  &CLAM::SpectralDescriptors::GetMoment2},
		{"Moment3",  &CLAM::SpectralDescriptors::GetMoment3},
		{"Moment4",  &CLAM::SpectralDescriptors::GetMoment4},
		{"Moment5",  &CLAM::SpectralDescriptors::GetMoment5},
		{"Moment6",  &CLAM::SpectralDescriptors::GetMoment6},
		{"Flatness",  &CLAM::SpectralDescriptors::GetFlatness},
		{"MagnitudeKurtosis",  &CLAM::SpectralDescriptors::GetMagnitudeKurtosis},
		{"MaxMagFreq",  &CLAM::SpectralDescriptors::GetMaxMagFreq},
		{"LowFreqEnergyRelation",  &CLAM::SpectralDescriptors::GetLowFreqEnergyRelation},
		{"Spread",  &CLAM::SpectralDescriptors::GetSpread},
		{"MagnitudeSkewness",  &CLAM::SpectralDescriptors::GetMagnitudeSkewness},
		{"Rolloff",  &CLAM::SpectralDescriptors::GetRolloff},
		{"Slope",  &CLAM::SpectralDescriptors::GetSlope},
		{"HighFrequencyContent",  &CLAM::SpectralDescriptors::GetHighFrequencyContent},
		{0,0}
	};
	for (GetterMap * map = spectralDescriptors; map->name; map++)
	{
		CLAM::TData* values= pool.GetWritePool<CLAM::TData>("Frame",map->name);
		for(int i=0; i<nFrames; i++)
		{
			values[i]=(frameD[i].GetSpectrumD().*(map->getter))();
		}
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

void Segment2Marks(const CLAM::Segment& segment, CLAM::IndexArray & marks)
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
			marks.AddElem(children[i].GetEndTime()/1000.*samplingRate);
		}
	}

}


