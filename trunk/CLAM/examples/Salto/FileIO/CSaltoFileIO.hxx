#ifndef _CSaltoFileIO_
#define _CSaltoFileIO_

#include <iostream>
#include <fstream>
#include <string>
#include "SDIFIn.hxx"
#include "DataFileIO.hxx"
#include "ByteOrderConvert.hxx"
#include "CSaltoSegData.hxx"
#include "CSaltoStatTmplData.hxx"
#include "Array.hxx"

namespace CLAM
{

class CSaltoFileIO
{
//----------------------------------------------------------------------------//
public:
CSaltoFileIO();
~CSaltoFileIO();
//----------------------------------------------------------------------------//

  // accesors

  // methods
  void ReadSDIFFile(const char* fileName,Segment* pSpecSeg,bool loadResidual);
  void ReadSaltoDataFile(const char* fileName,Array<CSaltoSegData> &SaltoData);
  void ReadSaltoDataFileFromText(const char* fileName,Array<CSaltoSegData> &SaltoData);
  void WriteSaltoDataFile(const char* fileName,Array<CSaltoSegData> &SaltoData);
  void WriteSaltoDataFileToText(const char* fileName,Array<CSaltoSegData> &SaltoData);
  void ReadStatTmplDataFile(const char* fileName,Array<CSaltoStatTmplData> &statData);
  void WriteStatTmplDataFile(const char* fileName,Array<CSaltoStatTmplData> &statData);
  void WriteStatTmplDataFileToText(const char* fileName,Array<CSaltoStatTmplData> &statData);
  void ReadStatTmplDataFileFromText(const char* fileName,Array<CSaltoStatTmplData> &statData);

//----------------------------------------------------------------------------//
private:

	ByteOrderConvert      mByteConvertor;
	std::fstream              mFout;

};

} // end of namespace CLAM
#endif
