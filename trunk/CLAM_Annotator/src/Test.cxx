#include "Schema.hxx"
#include <CLAM/DescriptionScheme.hxx>
#include <CLAM/Pool.hxx>
#include "SongFiles.hxx"
#include "Project.hxx"
#include "FrameDivision.hxx"

#include <CLAM/XMLStorage.hxx>
#include <CLAM/AudioFile.hxx>
#include <CLAM/Array.hxx>

//For Descriptor Computation
#include <CLAM/Segment.hxx>
#include <CLAM/SegmentDescriptors.hxx>
#include <CLAM/DescriptorComputation.hxx>
#include <CLAM/AudioFile.hxx>
#include <CLAM/MultiChannelAudioFileReader.hxx>
#include <CLAM/MultiChannelAudioFileReaderConfig.hxx>
#include <CLAM/FFT.hxx>
#include <CLAM/Spectrum.hxx>
#include <CLAM/SpectrumConfig.hxx>
#include <CLAM/Segmentator.hxx>

#include <time.h>

void BuildAndDumpTestSchema(const std::string & schemaLocation);
void PopulatePool(const std::string& song, CLAM::DescriptionDataPool& pool);
void GenerateRandomDescriptorValues(CLAM::TData* values, int size);
unsigned GenerateRandomSegmentationMarks(CLAM::DataArray & segmentation, CLAM::TData songDurationSeconds, CLAM::TData minSegmentDuration, CLAM::TData maxSegmentDuration);
unsigned GenerateNonOverlappingSegments(CLAM::DataArray & segmentation, CLAM::TData songDurationInSeconds, CLAM::TData maxGap, CLAM::TData maxSegmentDuration);
unsigned GenerateOverlappingSegments(CLAM::DataArray & segmentation, CLAM::TData songDurationInSeconds, CLAM::TData maxGap, CLAM::TData maxSegmentDuration);
void OpenSoundFile(const std::string& filename, CLAM::Audio& audio, CLAM::Text & artist, CLAM::Text & title);
void FFTAnalysis(const CLAM::Audio& audio, CLAM::Segment& s);
void ComputeSegment(const CLAM::Audio& audio,CLAM::Segment& segment, 
		    CLAM::SegmentDescriptors& segmentD);
void SegmentD2Pool(const CLAM::SegmentDescriptors& segmentD, CLAM::DescriptionDataPool& pool);
void ComputeSegmentationMarks(CLAM::Segment& segment,CLAM::SegmentDescriptors& segmentD);
void Segment2Marks(const CLAM::Segment& segment, CLAM::DataArray & marks);
CLAM::TData GetDurationInSeconds(const std::string& fileName);

int main(int argc, char ** argv)
{
	std::string projectDir = "../Samples/";
	std::string schemaLocation = "Schema.sc";
	std::string projectLocation = projectDir+"Project.pro";
	const char * songFileNames[] =
	{
		"SongsTest/LisaRein-SomethingBetter.mp3",
		"SongsTest/LisaRein-spunkyfunk.mp3",
		0
	};
	bool generateJustPools = (argc>1);

	if (!generateJustPools)
		BuildAndDumpTestSchema(projectDir+schemaLocation);

	//Create and store Project
	CLAM_Annotator::Project myProject;
	if (generateJustPools)
		for (unsigned i = 1; i<argc; i++)
			myProject.AppendSong(argv[i]);
	else
		for (const char ** filename = songFileNames; *filename; filename++)
			myProject.AppendSong(*filename);

	myProject.LoadScheme(schemaLocation, projectDir);
	myProject.SetExtractor("ClamExtractorExample");

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
		std::string songFile = currentSong->GetSoundFile();
		if (!generateJustPools) songFile = projectDir + songFile;
		std::cout<<"Computing Descriptors for file "<< songFile <<" Please wait..."<<std::endl;
		PopulatePool(songFile, pool);
		//Dump Descriptors Pool
		std::string poolFile;
		if (currentSong->HasPoolFile()) poolFile = currentSong->GetPoolFile();
		else poolFile = songFile+".pool";
		CLAM::XMLStorage::Dump(pool, "DescriptorsPool", poolFile);

		//Now we load the Pool and validate it with the schema
		CLAM::DescriptionDataPool toValidadDescription(myProject.GetDescriptionScheme());
		CLAM::XMLStorage::Restore(toValidadDescription, poolFile);

		std::cout << "Validating data..." << std::endl;

		std::ostringstream os;
		if (!myProject.ValidateDataPool(toValidadDescription, os))
		{
			std::cerr<<"Descriptor Pool Did Not Validate With Schema!"<<std::endl;
			std::cerr<< os.str() << std::endl;
		}
	}

	return 0;

}

CLAM::TData randomNumber(CLAM::TData minimum, CLAM::TData maximum)
{
	return minimum + rand()*(maximum-minimum)/RAND_MAX;
}
const char * pitchValues[] =
{
	"C", "C#", "D", "D#",
	"E", "F", "F#", "G",
	"G#", "A", "A#", "B",
	0
};
const char * chordModeValues[] =
{
	"Major",
	"Minor",
	"Diminished",
	"Augmented",
	0
};

void BuildAndDumpTestSchema(const std::string & schemaLocation)
{
	CLAM_Annotator::Schema schema;
	schema.SetUri("descriptionScheme:www.iua.upf.edu:clam:dummyTest-0.90");
	schema.AddString("Song","Artist");
	schema.AddString("Song","Title");
	const char * genreValues[] =
	{
		"Dance",
		"Classic",
		"Jazz",
		"Rhythm&Blues",
		"Folk",
		0
	};
	schema.AddEnumerated("Song","Genre", genreValues);
	schema.AddRangedReal("Song","Danceability", 0., 10.);
	schema.AddEnumerated("Song","Key", pitchValues);
	const char * modeValues[] =
	{
		"Minor",
		"Major",
		0
	};
	schema.AddEnumerated("Song","Mode", modeValues);
	schema.AddRangedReal("Song","DynamicComplexity", 0., 10.);
	schema.AddRangedInt("Song","BPM", 0, 240);

	schema.AddSegmentation("Song","RandomSegments", CLAM_Annotator::SegmentationPolicy::eUnsized, "");

	const char * onsetKindValues[] =
	{
		"PitchChange",
		"EnergyChange",
		0
	};
	schema.AddSegmentation("Song","Onsets", CLAM_Annotator::SegmentationPolicy::eUnsized, "Onset");
	schema.AddRangedReal("Onset","Relevance", 0., 10.);
	schema.AddEnumerated("Onset","DetectedChange", onsetKindValues);

	schema.AddSegmentation("Song", "Notes", CLAM_Annotator::SegmentationPolicy::eOverlapping, "Note");
	schema.AddEnumerated("Note", "Pitch", pitchValues);
	schema.AddRangedInt("Note", "Octave", 1, 12);
	schema.AddString("Note", "Instrument");

	schema.AddSegmentation("Song", "Chords", CLAM_Annotator::SegmentationPolicy::eContinuous, "Chord");
	schema.AddEnumerated("Chord", "Root", pitchValues);
	schema.AddEnumerated("Chord", "Mode", chordModeValues);

	const char * partDescriptionValues[] = {
		"Versus",
		"Chorus",
		"Solo",
		"Accapella",
		0
	};
	const char * partGroupIds[] = {"A","B","C","D","E","F","G","H","I","J",0};
	schema.AddSegmentation("Song", "Structure", CLAM_Annotator::SegmentationPolicy::eDiscontinuous, "StructuralPart");
	schema.AddEnumerated("StructuralPart", "Description", partDescriptionValues);
	schema.AddEnumerated("StructuralPart", "SimilarityGroup", partGroupIds);

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
	schema.AddFrameDivision("Song", "Frames", "Frame");
	for (const char ** name = lowLevelDescriptorsNames; *name; name++)
		schema.AddFrameFloatAttribute(*name);

	struct 
	{
		const char * scope;
		const char * name;
		const char * documentation;
	} docs[] =
	{
		{"Song", "Danceability",
			"<p>The <em>Danceability</em> is a 0 to 10 score that depends on how much defined is the rhythm</p>\n"
			"<p>Data for this data is not the one in Simac but randomly generated.</p>\n"
		},
		{"Frame", "Mean",
			"<p>The spectral power mean value.\n"
			"The unit of this measure can be dB or none, depending on the scale set for the measured Spectrum object.</p>\n"
			"<p>\\f[ Mean(X) = \\frac{\\sum x_i }{Size(X)} \\f]</p>\n"
			"<p>Being X the spectrum magnitude array.</p>\n"
		},
		{0,0,0}
	};
	for (unsigned i=0; docs[i].name; i++)
	{
		schema.AttributeDocumentation(docs[i].scope, docs[i].name, docs[i].documentation);
	}

	CLAM::XMLStorage::Dump(schema, "DescriptionScheme", schemaLocation);
	CLAM::XMLStorage::Dump(schema, "DescriptionScheme", std::cout);
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

	CLAM::TData sampleRate = audio.GetSampleRate();
	CLAM::TData firstCenter = segment.GetFrame(0).GetCenterTime()/1000;
	CLAM::TData secondCenter = segment.GetFrame(1).GetCenterTime()/1000;
	CLAM_Annotator::FrameDivision & frames = pool.GetWritePool<CLAM_Annotator::FrameDivision>("Song","Frames")[0];
	frames.SetFirstCenter(firstCenter*sampleRate);
	frames.SetInterCenterGap((secondCenter-firstCenter)*sampleRate);

	SegmentD2Pool(segmentD,pool);

	// Write Song level descriptors
	pool.GetWritePool<CLAM::Text>("Song","Artist")[0] = artist;
	pool.GetWritePool<CLAM::Text>("Song","Title")[0] = title;
	pool.GetWritePool<CLAM_Annotator::Enumerated>("Song","Genre")[0] = "Folk";
	pool.GetWritePool<CLAM::TData>("Song","Danceability")[0] = 7.2;
	pool.GetWritePool<CLAM_Annotator::Enumerated>("Song","Key")[0] = "C";
	pool.GetWritePool<CLAM_Annotator::Enumerated>("Song","Mode")[0] = "Minor";
	pool.GetWritePool<CLAM::TData>("Song","DynamicComplexity")[0] = 8.1;
	pool.GetWritePool<int>("Song","BPM")[0] = 100;

	// Onset Segmentation
	CLAM::DataArray & segmentation = 
		pool.GetWritePool<CLAM::DataArray>("Song","Onsets")[0];
	ComputeSegmentationMarks(segment, segmentD);
	Segment2Marks(segment,segmentation);
	
	unsigned nOnsets = segmentation.Size()+1;
	pool.SetNumberOfContexts("Onset",nOnsets);
	CLAM::TData * onsetForces = pool.GetWritePool<CLAM::TData>("Onset","Relevance");
	CLAM_Annotator::Enumerated * onsetChange = pool.GetWritePool<CLAM_Annotator::Enumerated>("Onset","DetectedChange");
	for (unsigned i = 0; i<nOnsets; i++)
	{
		onsetForces[i] = randomNumber(0,10);
		onsetChange[i] = randomNumber(0,2)>1? "PitchChange" : "EnergyChange";
	}

	// Random Segmentation
	CLAM::DataArray* randomSegmentation = 
		pool.GetWritePool<CLAM::DataArray>("Song","RandomSegments");
	GenerateRandomSegmentationMarks(randomSegmentation[0], GetDurationInSeconds(song), .3, 4.);

	// Note Segmentation
	CLAM::DataArray* noteSegmentation = 
		pool.GetWritePool<CLAM::DataArray>("Song","Notes");
	unsigned nNotes = GenerateOverlappingSegments(noteSegmentation[0], GetDurationInSeconds(song), 8., 8.);
	pool.SetNumberOfContexts("Note",nNotes);
	CLAM_Annotator::Enumerated * notePitch = pool.GetWritePool<CLAM_Annotator::Enumerated>("Note","Pitch");
	int * noteOctave = pool.GetWritePool<int>("Note","Octave");
	for (unsigned i = 0; i<nNotes; i++)
	{
		noteOctave[i] = int(randomNumber(4,7));
		notePitch[i] = pitchValues[int(randomNumber(0,11.99))];
	}
	// Chord Segmentation
	CLAM::DataArray* chordSegmentation = 
		pool.GetWritePool<CLAM::DataArray>("Song","Chords");
	unsigned nChords = GenerateRandomSegmentationMarks(chordSegmentation[0], GetDurationInSeconds(song), .5, 8. )+1;
	pool.SetNumberOfContexts("Chord",nChords);
	CLAM_Annotator::Enumerated * chordRoot = pool.GetWritePool<CLAM_Annotator::Enumerated>("Chord","Root");
	CLAM_Annotator::Enumerated * chordMode = pool.GetWritePool<CLAM_Annotator::Enumerated>("Chord","Mode");
	for (unsigned i = 0; i<nChords; i++)
	{
		chordRoot[i] = pitchValues[int(randomNumber(0,11.99))];
		chordMode[i] = chordModeValues[int(randomNumber(0,3.999))];
	}
	// Structural Segmentation
	CLAM::DataArray* structuralSegmentation = 
		pool.GetWritePool<CLAM::DataArray>("Song","Structure");
	unsigned nParts = GenerateNonOverlappingSegments(structuralSegmentation[0], GetDurationInSeconds(song), 4., 30.);
	pool.SetNumberOfContexts("StructuralPart",nParts);
	CLAM_Annotator::Enumerated * partDescription =
		pool.GetWritePool<CLAM_Annotator::Enumerated>("StructuralPart","Description");
	CLAM_Annotator::Enumerated * partGroup =
		pool.GetWritePool<CLAM_Annotator::Enumerated>("StructuralPart","SimilarityGroup");
	const char * partDescriptionValues[] = {
		"Versus",
		"Chorus",
		"Solo",
		"Accapella",
		0
	};
	const char * partGroupIds[] = {
		"A","B","C","D","E",
		"F","G","H","I","J",0};
	for (unsigned i = 0; i<nParts; i++)
	{
		partDescription[i] = partDescriptionValues[int(randomNumber(0,3.5))];
		partGroup[i] = partGroupIds[int(randomNumber(0,9.99))];
	}

}

void GenerateRandomDescriptorValues(CLAM::TData* values, int size)
{
	int randomInt=randomNumber(0,100);
	int randomIncr;
	for (int i=0; i<size; i++)
	{
		randomIncr = randomNumber(-10,10);
		randomInt += randomIncr;
		if(randomInt>100) randomInt = 80;
		if(randomInt<0) randomInt=20;

		values[i] = randomInt;
	}
}

CLAM::TData GetDurationInSeconds(const std::string& fileName)
{
	CLAM::AudioFile file;
	file.OpenExisting(fileName);
	return file.GetHeader().GetLength()/1000;
}

unsigned GenerateRandomSegmentationMarks(CLAM::DataArray & segmentation,
		CLAM::TData songDurationInSeconds, CLAM::TData minSegmentDuration, CLAM::TData maxSegmentDuration)
{
	unsigned nSegments = 0; 
	CLAM::TData positionInSeconds = 0;
	while(positionInSeconds<songDurationInSeconds)
	{
		CLAM::TData randomIncr = randomNumber(minSegmentDuration,maxSegmentDuration);
		positionInSeconds += randomIncr;
		if (positionInSeconds>=songDurationInSeconds) break;
		segmentation.AddElem(positionInSeconds);
		nSegments++;
	}
	return nSegments;

}

unsigned GenerateNonOverlappingSegments(CLAM::DataArray & segmentation,
		CLAM::TData songDurationInSeconds, CLAM::TData maxGap, CLAM::TData maxSegmentDuration)
{
	unsigned nSegments = 0; 
	unsigned lastOffset = 0;
	while(lastOffset<songDurationInSeconds)
	{
		//random number between 10 and 30 frames
		CLAM::TData randomOnset = randomNumber(lastOffset,lastOffset+maxGap);
		CLAM::TData randomOffset = randomNumber(randomOnset,randomOnset+maxSegmentDuration);
		if (randomOffset>songDurationInSeconds) break;
		segmentation.AddElem(randomOnset);
		segmentation.AddElem(randomOffset);
		lastOffset = randomOffset;
		nSegments++;
	}
	return nSegments;
}

unsigned GenerateOverlappingSegments(CLAM::DataArray & segmentation,
		CLAM::TData songDurationInSeconds, CLAM::TData maxGap, CLAM::TData maxSize)
{
	return GenerateNonOverlappingSegments(segmentation, songDurationInSeconds, maxGap, maxSize);
	unsigned nSegments = 0; 
	CLAM::TData lastOnset = 0;
	while(lastOnset<songDurationInSeconds)
	{
		//random number between 10 and 30 frames
		CLAM::TData randomOnset = randomNumber(lastOnset,lastOnset+maxGap);
		CLAM::TData randomOffset = randomNumber(randomOnset,randomOnset+maxSize);
		if (randomOffset>songDurationInSeconds) break;
		segmentation.AddElem(randomOnset);
		segmentation.AddElem(randomOffset);
		lastOnset = randomOnset;
		nSegments++;
	}
	return nSegments;
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

void Segment2Marks(const CLAM::Segment& segment, CLAM::DataArray & marks)
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
			marks.AddElem(children[i].GetEndTime()/1000.); // insert seconds
		}
	}

}


