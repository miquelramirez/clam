
#ifndef _CSaltoDataDump_
#define _CSaltoDataDump_

#include "Err.hxx"

#include <iostream>
#include <fstream>
#include <string>

namespace CLAM
{

class CSaltoDataDump
{

CSaltoDataDump(){};
~CSaltoDataDump(){};

public:
void  Open(char* fileName);
void  Close();
void  WriteTextFile();

private:
  std::fstream mFout;



};

} // end of namespace CLAM
#endif