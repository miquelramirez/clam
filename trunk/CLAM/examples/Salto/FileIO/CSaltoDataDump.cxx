

#include "CSaltoDataDump.hxx"

namespace CLAM
{

void CSaltoDataDump::Open(char* fileName)
{
    mFout.open("test.txt",std::ios::out);
    
    if (!mFout.is_open())
      throw Err("CSaltoDataDump::Open File could not be opened");

}
void CSaltoDataDump::Close()
{
   mFout.close();
}

void CSaltoDataDump::WriteTextFile()
{
  
  for (int i=0;i<100;i++)
    mFout<<"This is line #"<<i<<std::endl;
    
   mFout.close(); 
  
}

}