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

void GenerateRandomDescriptorValues(CLAM::TData* values, int size);
void GenerateRandomSegmentationMarks(CLAM::IndexArray* segmentation,int nSamples, int frameSize);
void FillSongNames(CLAM_Annotator::SongFiles& songFiles);
int GetnSamples(const std::string& fileName);


template <class T>
CLAM_Annotator::Descriptor<T> MakeDescriptor(T value,const std::string& name);

int main()
{
  //Create and store Project
  CLAM_Annotator::Project myProject;
  myProject.SetSongs("Songs.sl");
  myProject.SetSchema("Schema.sc");
  CLAM::XMLStorage::Dump(myProject,"Project","Project.pro");

  //Create and store SongList
  CLAM_Annotator::SongFiles songFiles;
  FillSongNames(songFiles);

  CLAM::XMLStorage::Dump(songFiles,"SongFiles","Songs.sl");
  

  //Create and store custom LLDSchema (basically a list of strings)
  CLAM_Annotator::LLDSchema testLLDSchema;
  testLLDSchema.GetLLDNames().push_back("Pitch");
  testLLDSchema.GetLLDNames().push_back("SpectralCentroid");
  testLLDSchema.GetLLDNames().push_back("SpectralSpread");
  testLLDSchema.GetLLDNames().push_back("SpectralDeviation");
  testLLDSchema.GetLLDNames().push_back("SpectralTilt");
  testLLDSchema.GetLLDNames().push_back("TemporalCentroid");
  testLLDSchema.GetLLDNames().push_back("LogAttackTime");
  testLLDSchema.GetLLDNames().push_back("Harmonicity");

  CLAM::XMLStorage::Dump(testLLDSchema, "LLDSchema", "LLDSchema.llsc");
  
   //Create and store custom HLDSchema
  CLAM_Annotator::HLDSchema testHLDSchema;
  
  {
    CLAM_Annotator::HLDSchemaElement testHLDesc;
    testHLDesc.SetName("Artist");
    testHLDesc.SetType("String");
    testHLDSchema.GetHLDs().push_back(testHLDesc);
  }
  {
    CLAM_Annotator::HLDSchemaElement testHLDesc;
    testHLDesc.SetName("Title");
    testHLDesc.SetType("String");
    testHLDSchema.GetHLDs().push_back(testHLDesc);
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
    testHLDSchema.GetHLDs().push_back(testHLDesc);
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
    testHLDSchema.GetHLDs().push_back(testHLDesc);
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
    testHLDSchema.GetHLDs().push_back(testHLDesc);
  }
  {
    CLAM_Annotator::HLDSchemaElement testHLDesc;
    testHLDesc.SetName("Mode");
    testHLDesc.SetType("RestrictedString");
    testHLDesc.AddRestrictionValues();
    testHLDesc.UpdateData();
    testHLDesc.GetRestrictionValues().push_back("Minor");
    testHLDesc.GetRestrictionValues().push_back("Major");
    testHLDSchema.GetHLDs().push_back(testHLDesc);
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
    testHLDSchema.GetHLDs().push_back(testHLDesc);
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
    testHLDSchema.GetHLDs().push_back(testHLDesc);
  }
  CLAM::XMLStorage::Dump(testHLDSchema, "HLDSchema","HLDSchema.hlsc");

  //Create and dump complete schema by adding LL and HL
  CLAM_Annotator::Schema testSchema;
  testSchema.SetLLDSchema(testLLDSchema);
  testSchema.SetHLDSchema(testHLDSchema);
  CLAM::XMLStorage::Dump(testSchema, "Schema","Schema.sc");

   //Load Schema
  CLAM_Annotator::Schema loadedSchema;
  CLAM::XMLStorage::Restore(loadedSchema, "Schema.sc");

  //Create Descriptors Pool Scheme and add attributes following loaded schema
  CLAM::DescriptionScheme scheme;
  
  //First we start with HLD
  std::list<CLAM_Annotator::HLDSchemaElement>& hlds = loadedSchema.GetHLDSchema().GetHLDs();
  std::list<CLAM_Annotator::HLDSchemaElement>::iterator it2;
  for(it2 = hlds.begin(); it2 != hlds.end(); it2++)
  {
    if((*it2).GetType()=="Float")
    {
      scheme.AddAttribute <float>("Song",(*it2).GetName());
    }
    else if((*it2).GetType()=="Int")
    {
      scheme.AddAttribute <int>("Song",(*it2).GetName());
    }
    else if((*it2).GetType()=="RestrictedString")
    {
      scheme.AddAttribute <CLAM_Annotator::RestrictedString>("Song",(*it2).GetName());
    }
    else
    {
      scheme.AddAttribute <std::string>("Song",(*it2).GetName());
    }
  }
  //And now we go into LLD
  std::list<std::string>::iterator it;
  std::list<std::string>& descriptorsNames = loadedSchema.GetLLDSchema().GetLLDNames();
  for(it = descriptorsNames.begin(); it != descriptorsNames.end(); it++)
  {
    scheme.AddAttribute <CLAM::TData>("Frame", (*it));
  }
  //finally we add segmentation marks
  scheme.AddAttribute<CLAM::IndexArray>("Song","Segments");

  //Now we create a Pool for every sound file we have
  std::vector<CLAM_Annotator::Song>::iterator currentSong;
  srand(time(NULL));
  int frameSize = 1024; //for example
  for (currentSong = songFiles.GetFileNames().begin(); currentSong != songFiles.GetFileNames().end(); currentSong++)
    {

      int nSamples = GetnSamples((*currentSong).GetSoundFile());
      int nFrames = nSamples/frameSize;
      //Create Descriptors Pool
      CLAM::DescriptionDataPool pool(scheme);
        
      pool.SetNumberOfContexts("Frame",nFrames);
      pool.SetNumberOfContexts("Song",1);
      
      int nDescriptors = descriptorsNames.size();
      int i;
  
      //Write HLD values
      for(it2 = hlds.begin(); it2 != hlds.end(); it2++)
	{
	  if((*it2).GetName()=="Artist")
	    {
	      std::string* value = pool.GetWritePool<std::string>("Song",(*it2).GetName());
	      *value = "Ruibal";
	    }
	  else if((*it2).GetName()=="Title")
	    {
	      std::string* value = pool.GetWritePool<std::string>("Song",(*it2).GetName());
	      *value = "Pension_Triana";
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
  
      //Generate random LLDs values
      //srand(time(NULL));
      for (i = 0,it = descriptorsNames.begin(); i < nDescriptors; i++,it++)
	{
	  CLAM::TData* values = pool.GetWritePool<CLAM::TData>("Frame",(*it));
	  GenerateRandomDescriptorValues(values,nFrames);
	}
      //Create segmentation marks
      CLAM::IndexArray* segmentation = 
	pool.GetWritePool<CLAM::IndexArray>("Song","Segments");
      GenerateRandomSegmentationMarks(segmentation, nSamples, frameSize);
      //Dump Descriptors Pool
      std::string poolFile;
      if((*currentSong).HasPoolFile()) poolFile = (*currentSong).GetPoolFile();
      else poolFile = (*currentSong).GetSoundFile()+".pool";
      CLAM::XMLStorage::Dump(pool, "DescriptorsPool", poolFile);

      //Now we are going to load the Pool and validate it with the schema
      CLAM::DescriptionDataPool loadedDescriptorPool(scheme);
      CLAM::XMLStorage::Restore(loadedDescriptorPool,poolFile);
      
      if(loadedSchema.GetHLDSchema().Validate(loadedDescriptorPool))
	std::cout<<"Descriptor Pool Validated With Schema"<<std::endl;
      else
	std::cout<<"Descriptor Pool Did Not Validate With Schema"<<std::endl;
    }
  return 0;

}

void GenerateRandomDescriptorValues(CLAM::TData* values, int size)
{
  int i;
  int randomInt=(float (rand())/float(RAND_MAX))*100;
  int randomIncr;
  for (i=0; i<size; i++)
  {
     randomIncr = (float (rand())/float(RAND_MAX))*20-10;
     randomInt += randomIncr;
     if(randomInt>100) randomInt = 80;
     if(randomInt<0) randomInt=20;
     
     values[i] = randomInt;
  }


}

void FillSongNames(CLAM_Annotator::SongFiles& songFiles)
{
  CLAM_Annotator::Song song;
  song.SetSoundFile("SongsTest/07276EC58D0000000000E8_02.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/07276EC58D0000000000E8_03.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/07276EC58D0000000000E8_04.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/07276EC58D0000000000E8_05.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/07276EC58D0000000000E8_06.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/07276EC58D0000000000E8_07.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/07FAB6C09A0000000000C7_01.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/07FAB6C09A0000000000C7_07.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/081290D29F00000000009A_03.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/081290D29F00000000009A_07.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/08785F4E190000000000A7_05.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/08785F4E190000000000A7_06.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/08B64B245C000000000072_01.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/08B64B245C000000000072_02.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/08B64B245C000000000072_03.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/08B64B245C000000000072_04.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/08B64B245C000000000072_05.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/08B64B245C000000000072_06.mp3");
  songFiles.GetFileNames().push_back(song);
  song.SetSoundFile("SongsTest/08B64B245C000000000072_07.mp3");
  songFiles.GetFileNames().push_back(song);
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
