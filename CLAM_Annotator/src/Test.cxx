#include "LLDSchema.hxx"
#include "DescriptionScheme.hxx"
#include "Pool.hxx"
#include "SongFiles.hxx"
#include "Project.hxx"

#include "XMLStorage.hxx"

void GenerateRandomDescriptorValues(CLAM::TData* values, int size);
void FillSongNames(SongFiles& songFiles);

int main()
{
  //Create and store Project
  Project myProject;
  myProject.SetSongs("Songs.xml");
  myProject.SetDescriptorPool("DescriptorsPool.xml");
  myProject.SetLLDSchema("LLDSchema.xml");
  CLAM::XMLStorage::Dump(myProject,"Project","Project.xml");

  //Create and store SongList
  SongFiles songFiles;
  FillSongNames(songFiles);

  CLAM::XMLStorage::Dump(songFiles,"SongFiles","Songs.xml");
  

  //Create and store custom LLDSchema (basically a list of strings)
  CLAM::LLDSchema testSchema;
  testSchema.GetLLDNames().push_back("Pitch");
  testSchema.GetLLDNames().push_back("SpectralCentroid");
  testSchema.GetLLDNames().push_back("SpectralSpread");
  testSchema.GetLLDNames().push_back("SpectralDeviation");
  testSchema.GetLLDNames().push_back("SpectralTilt");
  testSchema.GetLLDNames().push_back("TemporalCentroid");
  testSchema.GetLLDNames().push_back("LogAttackTime");
  testSchema.GetLLDNames().push_back("Harmonicity");

  CLAM::XMLStorage::Dump(testSchema, "LLDSchema", "LLDSchema.xml");
  
  //Load LLDschema
  CLAM::LLDSchema loadedSchema;
  CLAM::XMLStorage::Restore(loadedSchema, "LLDSchema.xml");


  //Create and store custom HLDSchema
  CLAM::HLDSchema hTestSchema;
  CLAM HLD testHDesc;
  testHDesc

  //Create Descriptors Pool Scheme and add attributes following loaded LLD schema
  CLAM::DescriptionScheme scheme;
  
  std::list<std::string>::iterator it;
  std::list<std::string>& descriptorsNames = loadedSchema.GetLLDNames();
  for(it = descriptorsNames.begin(); it != descriptorsNames.end(); it++)
  {
    scheme.AddAttribute <CLAM::TData>("Frame", (*it));
  }
 
  //Create Descriptors Pool
  CLAM::DescriptionDataPool pool(scheme);
  
  //Define Number of frames
  int nFrames = 100;
  pool.SetNumberOfContexts("Frame",nFrames);
  /*BTW, What happens if the Number of Contexts is modified after values have
    been written? Is it a destructive process?*/

  int nDescriptors = descriptorsNames.size();
  int i,n;
  
  /*Instantiate values and set them to zero (scope definition does not call
  constructors?)*/
  srand(time(NULL));
  for (i = 0,it = descriptorsNames.begin(); i < nDescriptors; i++,it++)
  {
    CLAM::TData* values = pool.GetWritePool<CLAM::TData>("Frame",(*it));
    GenerateRandomDescriptorValues(values,nFrames);
  }
  //Dump Descriptors Pool
  CLAM::XMLStorage::Dump(pool, "MyDescriptorsPool", "DescriptorsPool.xml");

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

void FillSongNames(SongFiles& songFiles)
{
  songFiles.GetFileNames().push_back("SongsTest/07276EC58D0000000000E8_02.mp3");
  songFiles.GetFileNames().push_back("SongsTest/07276EC58D0000000000E8_03.mp3");
  songFiles.GetFileNames().push_back("SongsTest/07276EC58D0000000000E8_04.mp3");
  songFiles.GetFileNames().push_back("SongsTest/07276EC58D0000000000E8_05.mp3");
  songFiles.GetFileNames().push_back("SongsTest/07276EC58D0000000000E8_06.mp3");
  songFiles.GetFileNames().push_back("SongsTest/07276EC58D0000000000E8_07.mp3");
  songFiles.GetFileNames().push_back("SongsTest/07FAB6C09A0000000000C7_01.mp3");
  songFiles.GetFileNames().push_back("SongsTest/07FAB6C09A0000000000C7_07.mp3");
  songFiles.GetFileNames().push_back("SongsTest/081290D29F00000000009A_03.mp3");
  songFiles.GetFileNames().push_back("SongsTest/081290D29F00000000009A_07.mp3");
  songFiles.GetFileNames().push_back("SongsTest/08785F4E190000000000A7_05.mp3");
  songFiles.GetFileNames().push_back("SongsTest/08785F4E190000000000A7_06.mp3");
  songFiles.GetFileNames().push_back("SongsTest/08B64B245C000000000072_01.mp3");
  songFiles.GetFileNames().push_back("SongsTest/08B64B245C000000000072_02.mp3");
  songFiles.GetFileNames().push_back("SongsTest/08B64B245C000000000072_03.mp3");
  songFiles.GetFileNames().push_back("SongsTest/08B64B245C000000000072_04.mp3");
  songFiles.GetFileNames().push_back("SongsTest/08B64B245C000000000072_05.mp3");
  songFiles.GetFileNames().push_back("SongsTest/08B64B245C000000000072_06.mp3");
  songFiles.GetFileNames().push_back("SongsTest/08B64B245C000000000072_07.mp3");
}
