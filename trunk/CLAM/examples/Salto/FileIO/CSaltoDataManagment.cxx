#ifndef SALTO_CONSOLE
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#endif
#include "CSaltoDataManagment.hxx"
#include "CSaltoFileIO.hxx"
#include "Segment.hxx"
#include "Err.hxx"
#include <iostream>

namespace CLAM
{

SaltoDBErr::SaltoDBErr( const char* msg )
	: Err( msg )
{
}

CSaltoDataManagment* CSaltoDataManagment::pDB = NULL;

CSaltoDataManagment* CSaltoDataManagment::GetSaltoDBHandle() 
{
	if ( pDB!=NULL )
		return pDB;
	else
		throw SaltoDBErr( "SALTO Database has not been yet initialized" );
}

bool CSaltoDataManagment::InitSaltoDB( Parameters* parm )
{
	if ( pDB )
		throw SaltoDBErr("SALTO Database already init, release first" );
	else
		pDB = new CSaltoDataManagment( parm );
	
	return true;
}

bool CSaltoDataManagment::ReleaseSaltoDB()
{
	if (!pDB)
		throw SaltoDBErr( "Cannot release the DB because it is not loaded yet" );
	else
		delete pDB;

	return true;
}

//CSaltoDataManagment::CSaltoDataManagment(CSaltoParameter* pParams)
CSaltoDataManagment::CSaltoDataManagment(Parameters* pParams)
: mpFileIO(NULL),
  mpSpectralSegResidual(NULL),
  mpSpectralSegEdit(NULL),
//XA  mStatTemplDataArray(MAX_STAT_TEMPLATES),
  mnStatTemplArrays(0),
  mpParams(pParams),
  mSpectralSeg(MAX_SPECTRAL_SEGMENTS)
{

	mFrameCounter = new TIndex[MAX_SPECTRAL_SEGMENTS];

/*  mpFileIO = NULL;
  mpSpectralSegResidual = NULL;
  mpSpectralSegEdit = NULL;
  mStatTemplDataArray.SetSize(MAX_STAT_TEMPLATES);
//  mStatTemplDataArray = MAX_STAT_TEMPLATES;
  mnStatTemplArrays = 0;
  mpParams = pParams;
*/
  int i;

  mpStatTemplDataArray=new Array<CSaltoStatTmplData>(MAX_STAT_TEMPLATES);
  
  // Load Process Display ..
  mSpectralSeg.SetSize(MAX_SPECTRAL_SEGMENTS);
#ifndef SALTO_CONSOLE

  mpLoadDisplayWindow = new Fl_Window(240,90);
  
  mpLoadDisplayWindow->color(FL_GRAY);
  mpTextBox = new Fl_Output (10,10,220,40);
  mpTextBox->color(FL_WHITE);
  mpTextBox->textfont(0);
  mpTextBox->textsize(20);
  mpTextBox->box(FL_THIN_UP_BOX);
  mpTextBox->value("SALTO Loading Data");

  mpCurrentTaskDisplay = new Fl_Output(10,60,220,20);
  mpCurrentTaskDisplay->color(FL_GRAY);
  mpCurrentTaskDisplay->textfont(0);
  mpCurrentTaskDisplay->textsize(10);
  mpCurrentTaskDisplay->box(FL_THIN_UP_BOX);
  
  mpLoadDisplayWindow->end();  
  mpLoadDisplayWindow->show();

  Fl::check();
#endif	
	// File-handling and Conversion
	mpFileIO = new CSaltoFileIO();
	if(mpFileIO==NULL)
		throw Err("OOM in CSaltoDataManagment::CSaltoDataManagment");
	
	// here is all our Spectral data stored
	for(i=0;i<MAX_SPECTRAL_SEGMENTS;i++)
	{
		mSpectralSeg[i] = NULL;
//		SpectralSegmentConfig SegmentCfg;
//		mSpectralSeg[i] = new SpectralSegment(SegmentCfg); //BUGSEGMENT!
		mSpectralSeg[i] = new Segment(); //BUGSEGMENT!
		(mSpectralSeg[i])->SetHoldsData(true);

		if(mSpectralSeg[i]==NULL)
			throw Err("OOM inCSaltoDataManagment::CSaltoDataManagment ->mSpectralSeg");
	}
	
	// init counters
	for(i=0;i<MAX_SPECTRAL_SEGMENTS;i++)
		mFrameCounter[i]=0;
	
	// load all salto data
	LoadSaltoDatabase();
	LoadStatTmplDatabase();
  LoadSDIFDatabase();
#ifdef CORRECT_PITCH
//    CorrectSpectralSegmentsPitch(); //all the segment will have the same pitch (very stable)
    CorrectSpectralSegmentsPitchPreservingEvolution(); //to preserve pitch evolution along the segment
#endif //CORRECT_PITCH
	LoadSDIFResiduals();
			
	/* load after salto database because
	*   num frames in seg is set here
	*/
	mpStatTemplDataArray->SetSize(MAX_STAT_TEMPLATES);

	
	
	// Default
	// this points to a temporaryly loaded SDIF File (used for StatTmpl-Editing)
	char filename[255];
	strcpy(filename,mpParams->GetFilePathOfSegmentPos(0));
	strcat(filename,mpParams->GetFileNameOfSegmentPos(0));

	LoadSDIFToEditPosition(filename);

#ifndef SALTO_CONSOLE
	
	delete mpLoadDisplayWindow;
	mpLoadDisplayWindow=NULL;
#endif 

	delete mpStatTemplDataArray;
	
		
}

//----------------------------------------------------------------------------//

CSaltoDataManagment::~CSaltoDataManagment()
{
	
	//delete mpStatTemplDataArray;
	
	if (mpFileIO!=NULL)
		delete mpFileIO;
	
	delete mFrameCounter;
	
	for(int i=0;i<MAX_SPECTRAL_SEGMENTS;i++)
	{
		if (mSpectralSeg[i]!=NULL)
			delete mSpectralSeg[i];
	}
}

//----------------------------------------------------------------------------//
void CSaltoDataManagment::LoadSDIFDatabase()
{
	bool readResidual = true;
	
	char filename[255];
	 	
 	for(int i=0;i<MAX_SPECTRAL_SEGMENTS;i++)
	{
	  if (mpParams->GetFileNameOfSegmentPos(i)!=NULL)
	  {
  	  // display process
  	  ShowLoadProcess(mpParams->GetFileNameOfSegmentPos(i),(float) i/SPECTRAL_SEGMENTS_IN_USE);
  	  
  	  strcpy(filename,mpParams->GetFilePathOfSegmentPos(i));
      strcat(filename,mpParams->GetFileNameOfSegmentPos(i));

  	  mpFileIO->ReadSDIFFile(filename,mSpectralSeg[i],readResidual);
   	  mpParams->SetSegDataNumFrames(i,mSpectralSeg[i]->GetnFrames());
    }
  }
}

//----------------------------------------------------------------------------//

void CSaltoDataManagment::LoadSDIFToEditPosition(const char* pFileName)
{  
  /* if we already allocated memory free it first...*/
  if(mpSpectralSegEdit!=NULL) 
    delete mpSpectralSegEdit;
	
	mpSpectralSegEdit = NULL;
//	SpectralSegmentConfig SegmentCfg;
//	mpSpectralSegEdit	= new SpectralSegment(SegmentCfg); //BUGSEGMENT!
	mpSpectralSegEdit	= new Segment(); //BUGSEGMENT!
	mpSpectralSegEdit->SetHoldsData(true);
	
	if(mpSpectralSegEdit ==NULL)
			throw Err("OOM inCSaltoDataManagment::LoadSDIFToEditPosition");

	bool readResidual = false;

  mpFileIO->ReadSDIFFile(pFileName,mpSpectralSegEdit,readResidual);
}

//----------------------------------------------------------------------------//
void CSaltoDataManagment::LoadSDIFResiduals()
{
 ShowLoadProcess(SALTO_TEST_RESIDUAL_NAME, 0);
 
 /* if we already allocated memory free it first...*/
  if(mpSpectralSegResidual!=NULL) 
    delete mpSpectralSegResidual;
	
	mpSpectralSegResidual	= NULL;
//	SpectralSegmentConfig SegmentCfg;
//	mpSpectralSegResidual	= new SpectralSegment(SegmentCfg); //BUGSEGMENT!
	mpSpectralSegResidual	= new Segment(); //BUGSEGMENT!
	mpSpectralSegResidual->SetHoldsData(true);
	
	if(mpSpectralSegResidual==NULL)
			throw Err("OOM inCSaltoDataManagment::LoadSDIFResiduals");

  // !! we dont need the peaks here, add the funtionality to sdif read
	bool readResidual = true;
  mpFileIO->ReadSDIFFile(SALTO_TEST_RESIDUAL_NAME,mpSpectralSegResidual,readResidual);

}
//----------------------------------------------------------------------------//
void
CSaltoDataManagment::StoreSaltoDatabase()
{	
  Array<CSaltoSegData> &segDataArray = mpParams->GetSegDataReference();
  segDataArray.SetSize(MAX_SPECTRAL_SEGMENTS); // otherwise size == 0 first time
  //mpFileIO->WriteSaltoDataFile(SALTO_DATA_FILE_NAME,segDataArray);
  mpFileIO->WriteSaltoDataFileToText(SALTO_DATA_TXT_FNAME,segDataArray);
}

//----------------------------------------------------------------------------//

void CSaltoDataManagment::LoadSaltoDatabase()
{
  // display process
	ShowLoadProcess(SALTO_DATA_FILE_NAME,0);
	  
  Array<CSaltoSegData> &segDataArray = mpParams->GetSegDataReference();
  mpFileIO->ReadSaltoDataFileFromText(SALTO_DATA_TXT_FNAME,segDataArray);    
  // mpFileIO->ReadSaltoDataFile(SALTO_DATA_FILE_NAME,segDataArray);
}

//----------------------------------------------------------------------------//

void CSaltoDataManagment::LoadStatTmplDatabase()
{
  // display process
	ShowLoadProcess(SALTO_STAT_TMPL_FILE_NAME,0);
  
  mpFileIO->ReadStatTmplDataFileFromText(SALTO_STAT_TMPL_TXT_FNAME,*mpStatTemplDataArray);
  //mpFileIO->ReadStatTmplDataFile(SALTO_STAT_TMPL_FILE_NAME,mStatTemplDataArray);  


  // Attention ! this is not very efficient but on loading time it doesnt hurt.
  // kludge for fixing the problem to cope with database of differing size
  // this should change anyway with the new  template arrays
  // further more its not really secure what happens if i change the 
  // stat templ fromat... ´¿
  // see also comments in Parameters.hxx
  if (mpStatTemplDataArray->Size()!=0)
  {
    mpParams->SetStatPeakArray0((*mpStatTemplDataArray)[0].GetStatPeakArray());
    mpParams->SetStatPeakArray1((*mpStatTemplDataArray)[1].GetStatPeakArray());
    mpParams->SetStatPeakArray2((*mpStatTemplDataArray)[2].GetStatPeakArray());
    mpParams->SetStatPeakArray3((*mpStatTemplDataArray)[3].GetStatPeakArray());
    mpParams->SetStatPeakArray4((*mpStatTemplDataArray)[4].GetStatPeakArray());
    mpParams->SetStatPeakArray5((*mpStatTemplDataArray)[5].GetStatPeakArray());
    mpParams->SetStatPeakArray6((*mpStatTemplDataArray)[6].GetStatPeakArray());
    mpParams->SetStatPeakArray7((*mpStatTemplDataArray)[7].GetStatPeakArray());
    
    mpParams->SetStatFundFreq0((*mpStatTemplDataArray)[0].GetFundFreq());
    mpParams->SetStatFundFreq1((*mpStatTemplDataArray)[1].GetFundFreq());
    mpParams->SetStatFundFreq2((*mpStatTemplDataArray)[2].GetFundFreq());
    mpParams->SetStatFundFreq3((*mpStatTemplDataArray)[3].GetFundFreq());
    mpParams->SetStatFundFreq4((*mpStatTemplDataArray)[4].GetFundFreq());
    mpParams->SetStatFundFreq5((*mpStatTemplDataArray)[5].GetFundFreq());
    mpParams->SetStatFundFreq6((*mpStatTemplDataArray)[6].GetFundFreq());
    mpParams->SetStatFundFreq7((*mpStatTemplDataArray)[7].GetFundFreq());
  }
}

//----------------------------------------------------------------------------//

void CSaltoDataManagment::StoreStatTmplDatabase()
{
  // mpFileIO->WriteStatTmplDataFile(SALTO_STAT_TMPL_FILE_NAME,mStatTemplDataArray);
  mpFileIO->WriteStatTmplDataFileToText(SALTO_STAT_TMPL_TXT_FNAME,*mpStatTemplDataArray);
}

//----------------------------------------------------------------------------//
Frame* CSaltoDataManagment::GetEditSpectralFrame(TIndex pos)
{
	Frame* pSpectralFrame = NULL;

  pSpectralFrame = &mpSpectralSegEdit->GetFrame(pos);
		
	return pSpectralFrame;
}
//----------------------------------------------------------------------------//
TIndex CSaltoDataManagment::GetFramesInEditSegment()
{
	return mpSpectralSegEdit->GetnFrames();
}

//----------------------------------------------------------------------------//
Frame* CSaltoDataManagment::GetStatResidualSpectralFrame(TIndex pos)
{
  Frame* pSpectralFrame = NULL;
  pSpectralFrame = &mpSpectralSegResidual->GetFrame(pos);
	
	return pSpectralFrame;
}
//----------------------------------------------------------------------------//

TIndex CSaltoDataManagment::GetFramesInStatResidualSegment()
{
   return mpSpectralSegResidual->GetnFrames();
}

//----------------------------------------------------------------------------//
void CSaltoDataManagment::AddStatTmpl(CSaltoStatTmplData& d,TIndex pos)
{
  if (pos>=MAX_STAT_TEMPLATES)
    return;
  
  (*mpStatTemplDataArray)[pos]=d;
}
//#pragma mark-
//----------------------------------------------------------------------------//
// depending on the timbre vektor, the nearest spectral frame will be returned
// (instead of reading out from the database, 
//  here could also some modelling happen...)
Frame* CSaltoDataManagment::GetSpectralFrame(CSaltoTimbreVektor &timbreVek,TIndex pos)
{
	// match closest entry :
	// for now : for every timbreVek there exists a sample
	
	TSize timbreIndex = timbreVek.GetPosition();

  // SECURITY CHECK FOR NOW TEST	
  // the pos is controlled by a higher instance
  if (timbreIndex>=SPECTRAL_SEGMENTS_IN_USE)
    timbreIndex = 0;

  // readout adequate spectralFrame
	return  &mSpectralSeg[timbreIndex]->GetFrame(pos);
}

//----------------------------------------------------------------------------//
Frame* CSaltoDataManagment::GetSpectralFrame(TIndex timbreIndex,TIndex pos)
{
	Frame* pSpectralFrame = NULL;
  pSpectralFrame = &mSpectralSeg[timbreIndex]->GetFrame(pos);
		
	return pSpectralFrame;
}

//----------------------------------------------------------------------------//

TIndex CSaltoDataManagment::GetFramesInSeg(CSaltoTimbreVektor &timbreVek)
{
	return	mSpectralSeg[timbreVek.GetPosition()]->GetnFrames();
}

//----------------------------------------------------------------------------//

TIndex CSaltoDataManagment::GetFramesInSeg(TIndex segPosition)
{
	return	mSpectralSeg[segPosition]->GetnFrames();
}

//----------------------------------------------------------------------------//

void CSaltoDataManagment::SetFrameCounter(CSaltoTimbreVektor &timbreVek,TIndex frame)
{
   TIndex timbreIndex = timbreVek.GetPosition();
   if (timbreIndex >= 0 && timbreIndex<SPECTRAL_SEGMENTS_IN_USE)
    mFrameCounter[timbreIndex] = frame;
   else
    throw Err("CSaltoDataManagment::SetFrameCounter out of bounds");
}	

//----------------------------------------------------------------------------//

void CSaltoDataManagment::SetFrameCounter(TIndex segPosition,TIndex frame)
{
   if (segPosition >= 0 && segPosition<SPECTRAL_SEGMENTS_IN_USE)
    mFrameCounter[segPosition] = frame;
   else
    throw Err("CSaltoDataManagment::SetFrameCounter out of bounds");
}	

//----------------------------------------------------------------------------//
//#pragma mark-
//----------------------------------------------------------------------------//
// this is only for test purposes, it allows to read out 
// frames of several segments
// the pSynthFrame spectral buffer gets cleared every time
void CSaltoDataManagment::GetNextSynthFrame(CSaltoSynthFrame *pSynthFrame,TIndex currSegment)
{
	long numFrames = mSpectralSeg[currSegment]->GetnFrames();
		
	Frame* pCurrSpecFrame = NULL;
	SpectralPeakArray* pCurrPeakArray = NULL;
	
	pCurrSpecFrame = &mSpectralSeg[currSegment]->GetFrame(mFrameCounter[currSegment]);
	
//	pSynthFrame->SetPeakArray(*(pCurrSpecFrame->GetSpectralPeakArray()));			  // This has to be optimized mMemory is allocated //
	pSynthFrame->SetPeakArray(pCurrSpecFrame->GetSpectralPeakArray());			  // This has to be optimized mMemory is allocated //	
	pSynthFrame->SetFundFreq( (pCurrSpecFrame->GetFundamental()).GetFreq() ); // ..is double
	pSynthFrame->SetResidualPtr(&(pCurrSpecFrame->GetResidualSpec()));								// ..is pointer ! //BUGFRAME!
	
	// clear spectrum buffer in synth-frame
	pSynthFrame->ClearSpectrum();
	
	mFrameCounter[currSegment]++;
	if(mFrameCounter[currSegment] >= mSpectralSeg[currSegment]->GetnFrames())
    mFrameCounter[currSegment]=0;
}
//----------------------------------------------------------------------------//
void CSaltoDataManagment::ShowLoadProcess(const char* text,float percentage)
{
#ifndef SALTO_CONSOLE
	if (mpCurrentTaskDisplay!=NULL)
		{
			mpCurrentTaskDisplay->value(text);
			
			Fl::check();
		}
#endif
	std::cout << "Loading " << text << std::endl;
	std::cout << percentage*100 << "% of Load done" << std::endl;
}

//correct pitch so all the frames in an spectral segment have the same base pitch
void CSaltoDataManagment::CorrectSpectralSegmentsPitch()
{
  for (int i=0;i<SPECTRAL_SEGMENTS_IN_USE;i++)
  {
    double PitchReference = LOWEST_PITCH *pow(2.0,(i%25)/12.0);

    for (int k=0;k<mSpectralSeg[i]->GetnFrames();k++)
    {
      Frame* pSpectralFrame = &mSpectralSeg[i]->GetFrame(k);
      double pitchCorrectionFactor = PitchReference/pSpectralFrame->GetFundamental().GetFreq();
      DoPitchMod2(pSpectralFrame,pitchCorrectionFactor);
    }
  }
}

//correct pitch using the ReferencePitch/AverageOfSpectralSeg as pitch correction factor, and maintain evolution in pitch along the stationary part
void CSaltoDataManagment::CorrectSpectralSegmentsPitchPreservingEvolution()
{
  int t = 0;
  int validFreqFrames = 0;
  double averageFreq = 0;
	int k;
	
  for (int i=0;i<SPECTRAL_SEGMENTS_IN_USE;i++)
  {
    double PitchReference = LOWEST_PITCH *pow(2.0,(i%25)/12.0);
    validFreqFrames = 0;
    averageFreq = 0;
    for (k=0;k<mSpectralSeg[i]->GetnFrames();k++)
    {
      Frame* pSpectralFrame = &mSpectralSeg[i]->GetFrame(k);

      if (pSpectralFrame->GetFundamental().GetFreq() > (PitchReference-130.0))
      {
        averageFreq += pSpectralFrame->GetFundamental().GetFreq();
        validFreqFrames++;
      }
    }

    averageFreq = averageFreq / validFreqFrames;
    double pitchCorrectionFactor = PitchReference/averageFreq;

    for (k=0;k<mSpectralSeg[i]->GetnFrames();k++)
    {
      Frame spectralFrame = mSpectralSeg[i]->GetFrame(k);

      DoPitchMod2(&spectralFrame,pitchCorrectionFactor);

    }
  }
}

void CSaltoDataManagment::DoPitchMod(Frame *pSpectralFrame,double pitchFactor)
{
  SpectralPeakArray* pBasePeakArray = &pSpectralFrame->GetSpectralPeakArray();
  TSize nPeaks = pBasePeakArray->GetnPeaks();
		//  SpectralPeak tempPeak;
  int i;

  for (i=0;i<nPeaks;i++)
  {
		//    tempPeak = pBasePeakArray->GetSpectralPeak(i);
		//    tempPeak.SetFreq(tempPeak.GetFreq()*pitchFactor);   // OPTIMIZE !!
		//    pBasePeakArray->SetSpectralPeak(i,tempPeak);
		
		pBasePeakArray->GetFreqBuffer()[i] *= pitchFactor;
  }

/*Fundamental TMPfund; //BUGFRAME!    //optimize!
	FundamentalConfig FundCfg;
	TMPfund.Configure(FundCfg);
	TMPfund.AddElem(pSpectralFrame->GetFundamental().GetFreq()*pitchFactor);
  pSpectralFrame->SetFundamental(TMPfund);*/

		//	pSpectralFrame->GetFundamental().SetFreq(0,pSpectralFrame->GetFundamental().GetFreq()*pitchFactor);	//we change freq 0 because we've only 1 candidate
	pSpectralFrame->SetFundamentalFreq(0,pSpectralFrame->GetFundamental().GetFreq()*pitchFactor);	//we change freq 0 because we've only 1 candidate


}

void CSaltoDataManagment::DoPitchMod2( Frame* pSpectralFrame, double pitchFactor )
{
	SpectralPeakArray* pBasePeakArray = &pSpectralFrame->GetSpectralPeakArray();
	TSize nPeaks = pBasePeakArray->GetnPeaks();

	if ( nPeaks > 0 )
	{

		TData* pBaseFreq = pBasePeakArray->GetFreqBuffer().GetPtr();

		for (int i=0;i<nPeaks;i++)
		{
			(*pBaseFreq++) *= pitchFactor;		
		}
		
	}

	pSpectralFrame->SetFundamentalFreq(0,pSpectralFrame->GetFundamental().GetFreq()*pitchFactor);	//we change freq 0 because we've only 1 candidate

}


} // end of namespace CLAM
