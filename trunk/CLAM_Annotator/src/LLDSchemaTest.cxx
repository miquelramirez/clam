#include "LLDSchema.hxx"
#include "DescriptionScheme.hxx"
#include "Pool.hxx"


#include "XMLStorage.hxx"

int main()
{
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
  int nFrames = 25;
  pool.SetNumberOfContexts("Frame",nFrames);
  /*BTW, What happens if the Number of Contexts is modified after values have
    been written? Is it a destructive process?*/

  int nDescriptors = descriptorsNames.size();
  int i,n;
  
  /*Instantiate values and set them to zero (scope definition does not call
  constructors?)*/
  for (i = 0,it = descriptorsNames.begin(); i < nDescriptors; i++,it++)
  {
    CLAM::TData* values = pool.GetWritePool<CLAM::TData>("Frame",(*it));
    for (n = 0; n<nFrames; n++)
    {
      values[n]=0;
    }
  }
  //Dump Descriptors Pool
  CLAM::XMLStorage::Dump(pool, "MyDescriptorsPool", "DescriptorsPool.xml");

  return 0;

}
