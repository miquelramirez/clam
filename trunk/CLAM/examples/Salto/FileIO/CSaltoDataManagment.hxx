#ifndef _CSaltoDataManagment_
#define _CSaltoDataManagment_

#ifndef SALTO_CONSOLE
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Output.H>
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <string.h>
#endif

//#include "CSaltoParameter.hxx"
#include "Parameters.hxx"
#include "CSaltoSynthFrame.hxx"
#include "CSaltoTimbreVektor.hxx"
#include "CSaltoSegData.hxx"
#include "CSaltoStatTmplData.hxx"
#include "CSaltoDefines.hxx"
#include "Array.hxx"
#include "Frame.hxx"

namespace CLAM
{
class CSaltoFileIO;
class Segment;

class SaltoDBErr : public Err
{
public:
	SaltoDBErr( const char* msg = 0 );
	
};

class CSaltoDataManagment
{
public:

	static CSaltoDataManagment* GetSaltoDBHandle();

	static bool InitSaltoDB( Parameters* parm );

	static bool ReleaseSaltoDB();

	~CSaltoDataManagment();
	//----------------------------------------------------------------------------//
private:
	CSaltoDataManagment(Parameters* pParams);

	static CSaltoDataManagment* pDB;

public:
	
	/* data load/save */
	void      LoadSDIFDatabase();
	void      StoreSaltoDatabase();
	void      LoadSaltoDatabase();
	void      LoadStatTmplDatabase();
	void      StoreStatTmplDatabase();
	void      LoadSDIFToEditPosition(const char* pFileName);
	void      LoadSDIFResiduals();
	
	/* frame Handling */
	void			GetNextSynthFrame(CSaltoSynthFrame *pSynthFrame,TIndex currSegment);
	
	Frame* GetSpectralFrame(CSaltoTimbreVektor &timbreVek,TIndex pos);
	
	Frame* GetSpectralFrame(TIndex timbreIndex,TIndex pos);
	
	TIndex		GetFramesInSeg(TIndex segPosition);
	TIndex		GetFramesInSeg(CSaltoTimbreVektor &timbreVek);
	
	void      SetFrameCounter(CSaltoTimbreVektor &timbreVek,TIndex frame);
	void      SetFrameCounter(TIndex segPosition,TIndex frame);
	
	Frame* GetEmptySpectralFrame(){return &mEmptySpectralFrame;}
	
	/* Stat SDIF Edit handling */
	Frame*  GetEditSpectralFrame  (TIndex pos);
	TIndex       GetFramesInEditSegment();
	
	void     AddStatTmpl   (CSaltoStatTmplData& d,TIndex pos);
	
  /* Stationary Residual Handling */
	Frame*     GetStatResidualSpectralFrame(TIndex pos);
	TIndex          GetFramesInStatResidualSegment();
	
	/* load Process Diplay */
	void                   ShowLoadProcess(const char* text,float percentage);
	
	void CorrectSpectralSegmentsPitch();
	void CorrectSpectralSegmentsPitchPreservingEvolution();
	void DoPitchMod(Frame *pSpectralFrame,double pitchFactor);
	void DoPitchMod2(Frame *pSpectralFrame,double pitchFactor);
	
	
	//----------------------------------------------------------------------------//
private:
	CSaltoFileIO   *mpFileIO;
	
	Array<Segment*>	 mSpectralSeg;
	Segment*	 mpSpectralSegEdit;
	Segment*   mpSpectralSegResidual;
	
	Frame     mEmptySpectralFrame;
	TIndex		*mFrameCounter;
	
	Array<CSaltoStatTmplData>* mpStatTemplDataArray;
	TIndex                        mnStatTemplArrays;
	
	//	CSaltoParameter               *mpParams;
	Parameters               *mpParams;	
	
	// Dislay
#ifndef SALTO_CONSOLE

	Fl_Window *mpLoadDisplayWindow;
	Fl_Output *mpTextBox,*mpCurrentTaskDisplay;
#endif
};

}
#endif

