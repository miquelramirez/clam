#include "CSaltoFileIO.hxx"
#include "CSaltoDefines.hxx"
#include "SpectralPeak.hxx"

namespace CLAM
{

CSaltoFileIO::CSaltoFileIO()
{

}
CSaltoFileIO::~CSaltoFileIO()
{


}

//----------------------------------------------------------------------------//
void CSaltoFileIO::ReadSDIFFile(const char* fileName,Segment *pSpecSeg,bool loadResidual)
{
  /* temporal SDIF Converter which reads in one pSpecSeg */
	SDIFInConfig cfg;
	cfg.SetMaxNumPeaks(100);
	cfg.SetFileName(fileName);
	cfg.SetEnableResidual(true);
	SDIFIn SDIFReader(cfg);
	
	//mSegment.AddAll();
	//mSegment.UpdateData();
	SDIFReader.Output.Attach(*pSpecSeg);
	
	SDIFReader.Start();
	
	while(SDIFReader.Do()) {}

	SDIFReader.Stop();

/*  SDIFIn SDIFIn(pSpecSeg,MAX_SINES);

  mpFile = new SDIF::File(fileName,eInput);
  mpFile->Open();
  SDIFConverter.SetEnableResidual(loadResidual);
  mpFile->Read(SDIFConverter);
  mpFile->Close();

  delete mpFile;*/
}
//----------------------------------------------------------------------------//

void CSaltoFileIO::ReadSaltoDataFile(const char* fileName,Array<CSaltoSegData> &saltoData)
{
  DataFileIO  *mpFileIO = NULL;
  mpFileIO = new DataFileIO(fileName,eInput);
  CLAM_ASSERT(mpFileIO, "CSaltoFileIO::ReadSaltoDataFile cant construct FileIO");
    
  mpFileIO->Open();
  mpFileIO->Read(saltoData);
  mpFileIO->Close();
  
  delete mpFileIO;  
}
//----------------------------------------------------------------------------//
void CSaltoFileIO::WriteSaltoDataFile(const char* fileName,Array<CSaltoSegData> &saltoData)
{
  DataFileIO  *mpFileIO = NULL;
  
  mpFileIO = new DataFileIO(fileName,eOutput);
  CLAM_ASSERT (mpFileIO,"CSaltoFileIO::WriteSaltoDataFile cant construct FileIO");
  mpFileIO->Open();
  
  mpFileIO->Write(saltoData);

  mpFileIO->Close();
  
  delete mpFileIO;  
}
//----------------------------------------------------------------------------//

void CSaltoFileIO::ReadStatTmplDataFile(const char* fileName,Array<CSaltoStatTmplData> &statData)
{
  DataFileIO  *mpFileIO = NULL;
  mpFileIO = new DataFileIO(fileName,eInput);
  CLAM_ASSERT (mpFileIO,"CSaltoFileIO::ReadSaltoDataFile cant construct FileIO");
    
  mpFileIO->Open();
  mpFileIO->Read(statData);
  TUInt32 nElems = statData.Size()*sizeof(CSaltoStatTmplData)/sizeof(TUInt32);
  mByteConvertor.FixByteOrder(
    (TByte*)statData.GetPtr(),nElems,sizeof(TUInt32));
  mpFileIO->Close();
  
  delete mpFileIO;  
}
//----------------------------------------------------------------------------//
void CSaltoFileIO::WriteStatTmplDataFile(const char* fileName,Array<CSaltoStatTmplData> &statData)
{
  DataFileIO  *mpFileIO = NULL;
  mpFileIO = new DataFileIO(fileName,eOutput);
  CLAM_ASSERT(mpFileIO, "CSaltoFileIO::WriteStatTmplDataFile cant construct FileIO");
  mpFileIO->Open();
  mpFileIO->Write(statData);
  mpFileIO->Close();
  
  delete mpFileIO;  
}
//----------------------------------------------------------------------------//
void CSaltoFileIO::WriteStatTmplDataFileToText(const char* fileName,Array<CSaltoStatTmplData> &statData)
{
  int i,j,nPeaks,nTempl;
  mFout.open(fileName,std::ios::out);  
  CLAM_ASSERT(mFout.is_open(), "CSaltoFileIO::WriteStatTmplDataFileToText could not be opened");
  
  nTempl=statData.Size();
  mFout<<nTempl<<std::endl;
  for (i=0;i<nTempl;i++)
  {
    nPeaks = statData[i].GetNumValidPeaks();
    mFout<<nPeaks<<std::endl;
    mFout<<statData[i].GetFundFreq()<<std::endl;
    for (j=0;j<nPeaks;j++)
    {
      SpectralPeak tempPeak = statData[i].GetPeak(j);
      mFout<<tempPeak.GetFreq()<<std::endl;
      mFout<<tempPeak.GetMag()<<std::endl;
      mFout<<tempPeak.GetPhase()<<std::endl;
    }
  }  
  mFout.close(); 
}
//----------------------------------------------------------------------------//
void CSaltoFileIO::ReadStatTmplDataFileFromText(const char* fileName,Array<CSaltoStatTmplData> &statData)
{
  int i,j,nPeaks,nTempl;
  double dVal;
	
	printf("GODVER %s\n\n",fileName);
	
  mFout.open(fileName,std::ios::in);  
  CLAM_ASSERT(mFout.is_open(), "CSaltoFileIO::ReadStatTmplDataFileFromText File could not be opened");
  
  SpectralPeak tempPeak;
  tempPeak.AddAll();
  tempPeak.UpdateData();
  tempPeak.SetBinPos(-1);
  tempPeak.SetBinWidth(-1);
  mFout>>nTempl;
  statData.SetSize(nTempl);
  for (i=0;i<nTempl;i++)
  {
    mFout>>nPeaks;
    statData[i].SetNumValidPeaks(nPeaks);    
    mFout>>dVal;
    statData[i].SetFundFreq(dVal);
    for (j=0;j<nPeaks;j++)
    {
      mFout>>dVal;
      tempPeak.SetFreq(dVal);
      mFout>>dVal;
      tempPeak.SetMag(dVal);
      mFout>>dVal;
      tempPeak.SetPhase(dVal);
      statData[i].SetPeak(j,tempPeak);
    }
  }    
   mFout.close(); 
}
//----------------------------------------------------------------------------//
void CSaltoFileIO::WriteSaltoDataFileToText(const char* fileName,Array<CSaltoSegData> &saltoData)
{
  int i,nSegments;
  mFout.open(fileName,std::ios::out);  
  CLAM_ASSERT(mFout.is_open(), "CSaltoFileIO::WriteSaltoDataFileToText could not be opened");

  nSegments = saltoData.Size();
  mFout<<nSegments<<std::endl;  
  for (i=0;i<nSegments;i++)
  {
      mFout<<saltoData[i].GetSegLength()<<std::endl;      
      mFout<<saltoData[i].GetPlayBackStart()<<std::endl;
      mFout<<saltoData[i].GetIndividualVolume()<<std::endl;
      mFout<<saltoData[i].GetLoopStart()<<std::endl;
      mFout<<saltoData[i].GetLoopEnd()<<std::endl;
      mFout<<saltoData[i].GetInterpolStart()<<std::endl;
      mFout<<saltoData[i].GetInterpolEnd()<<std::endl;       
      mFout<<saltoData[i].GetResFadeStart()<<std::endl;
      mFout<<saltoData[i].GetResFadeEnd()<<std::endl;
      mFout<<saltoData[i].GetAttackResVolume()<<std::endl;
      mFout<<saltoData[i].GetStatResVolume()<<std::endl;
  }  
  mFout.close(); 
}

//----------------------------------------------------------------------------//

void CSaltoFileIO::ReadSaltoDataFileFromText(const char* fileName,Array<CSaltoSegData> &saltoData)
{
  int i,iVal,nSegments;
  double dVal;
  mFout.open(fileName,std::ios::in);  
  CLAM_ASSERT(mFout.is_open(), "CSaltoFileIO::ReadStatTmplDataFileFromText File could not be opened");

  mFout>>nSegments;

	saltoData.Resize(nSegments);
  saltoData.SetSize(nSegments); // add security here ?

  for (i=0;i<nSegments;i++)
  {
      mFout>>iVal;
      saltoData[i].SetSegLength(iVal);
      mFout>>iVal;
      saltoData[i].SetPlayBackStart(iVal);
      mFout>>iVal;
      saltoData[i].SetIndividualVolume(iVal);
      mFout>>iVal;
      saltoData[i].SetLoopStart(iVal);
      mFout>>iVal;
      saltoData[i].SetLoopEnd(iVal);
      mFout>>iVal;
      saltoData[i].SetInterpolStart(iVal);
      mFout>>iVal;
      saltoData[i].SetInterpolEnd(iVal);
      mFout>>iVal;
      saltoData[i].SetResFadeStart(iVal);
      mFout>>iVal;
      saltoData[i].SetResFadeEnd(iVal);
      mFout>>dVal;
      saltoData[i].SetAttackResVolume(dVal);
      mFout>>dVal;
      saltoData[i].SetStatResVolume(dVal);
  }
  mFout.close();
}
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//

} // end of namespace CLAM
