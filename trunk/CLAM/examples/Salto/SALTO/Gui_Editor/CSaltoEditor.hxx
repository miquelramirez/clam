#ifndef _CSaltoEditor_
#define _CSaltoEditor_

#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Counter.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Adjuster.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Dial.H>
#include <FL/fl_file_chooser.H>
#include <pthread.h>
#include "CSaltoDisplay.hxx"
#include "CSaltoSpectralDisplay.hxx"
#include "CSaltoSpectrumDisplay.hxx"
#include "Parameters.hxx"
//#include "CSaltoMIDIInOut.hxx"
#include "CSaltoSegData.hxx"
#include "BaseAudioApplication.hxx"
#include "MelodyTranslator.hxx"
#include "FL/fl_file_chooser.H"

extern pthread_mutex_t FLTKmutex;
extern pthread_mutex_t DrawingMutex;

extern pthread_cond_t condPA;
extern pthread_mutex_t mutPA;

namespace CLAM
{

class SaltoSynth;

class CSaltoEditor
{
public:

CSaltoEditor(   Parameters *pParams,
				SaltoSynth *pSaltoApp,
				MelodyTranslator *melody, 
				BaseAudioApplication* pApp);//,CSaltoMIDIInOut *pMIDI);

~CSaltoEditor();

// public methods      


void 		DisplayBufferOfDouble(TData *array,TSize numElements);
void 		DisplayTmplArrayOfDouble(const DataArray &displayArray);

//void    DisplayPeakArray(const SpectralPeakArray* peakArray);
//void		DisplayResidual(const Spectrum* spec);
void    DisplayPeakArray(const SpectralPeakArray& peakArray);
void		DisplayAttackResidual(const Spectrum& spec);
void		DisplayStationaryResidual(const Spectrum& spec);
void    DisplaySynthesizedSpectrum(const Spectrum& spec);
void    FillDisplayPeakArray(const SpectralPeakArray& peakArray );
void		FillDisplayAttackResidual(const Spectrum& spec);
void		FillDisplayStationaryResidual(const Spectrum& spec);
void    FillDisplaySynthesizedSpectrum(const Spectrum& spec);
void    DisplayPeakArray();
void		DisplayAttackResidual();
void		DisplayStationaryResidual();
void    DisplaySynthesizedSpectrum();

void		SetValOut1(TData val);

void    LoadLoopValues(TIndex pos);
void    StoreLoopValues(TIndex pos);
void    LoadLoopValuesStat(TIndex pos);
void    StoreLoopValuesStat(TIndex pos);
void    LoadSDIFToEdit();
//void    StoreStationaryTmpl();
void    AddStationaryTmplToDataBase(TIndex pos);

void    SetDisplayParameters( TData sampRate, TSize spectralFrameSize );

// callbacks
//----------------------------------------------------------------------------//
// runtime flow control parameters 
//----------------------------------------------------------------------------//
static void sPlay(Fl_Button* pButton, CSaltoEditor* pThis)
{
  if(!pThis->mpParams->GetPlay()) // init once on play
  {
    pThis->mpParams->SetAttackTimbreLevel(105);
    pThis->mpParams->SetPitchModFactor(1.0);  
  }
  pThis->mpParams->SetPlay(pButton->value());
}

static void sExit(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton; // prevent unused variable warnings
	pThis->mpApp->Stop();
	pThis->mpParams->SetExit(true);
	pThis->mpSaltoApp->SetTerminationState( );
}
static void sStart(Fl_Light_Button* pButton, CSaltoEditor* pThis)
{
//	pThis->mpParams->SetAudioStart(pButton->value());   // CHECKTHIS FOR iMPROVEMENT !!!
}

static void sStoreAll(Fl_Light_Button* pButton, CSaltoEditor* pThis)
{
//  pThis->mpSaltoApp->StoreAllParams();
}


static void sUseSines(Fl_Button* pButton, CSaltoEditor* pThis)
{
	pThis->mpParams->SetUseSines(pButton->value());
}

static void sUseAttackResidual(Fl_Button* pButton, CSaltoEditor* pThis)
{
	pThis->mpParams->SetUseAttackResidual(pButton->value());
}

static void sUseStatResidual(Fl_Button* pButton, CSaltoEditor* pThis)
{
	pThis->mpParams->SetUseStatResidual(pButton->value());
}

static void sSetUsePhaseAlignment(Fl_Button* pButton, CSaltoEditor* pThis)
{
	pThis->mpParams->SetUsePhaseAlignment(pButton->value());
}

static void sSetIPValue(Fl_Slider* pSlider, CSaltoEditor* pThis)
{
	pThis->mpParams->SetInterPolFactor(pSlider->value());
  pThis->mpParams->SetDisplayedValuesChanged(true); // display needs update 
}
//----------------------------------------------------------------------------//

static void sShowIPEditor(Fl_Button* pSlider, CSaltoEditor* pThis)
{
  (void) pSlider;
	pThis->mpParams->SetSynthesisFlow(FLOW_INTERPOLATE);
	pThis->mpTmplChooser->hide();
	pThis->mpLoopControls->show();
	pThis->DisplayPeakArray(*pThis->mpParams->GetCurrentStatTemplatePtr());
}

static void sShowTemplateEditor(Fl_Button* pSlider, CSaltoEditor* pThis)
{
  (void) pSlider;
	pThis->mpParams->SetSynthesisFlow(FLOW_SDIF_EDIT);
	pThis->mpLoopControls->hide();
	pThis->mpTmplChooser->show();
	pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr());
}

static void sShowPlayEditor(Fl_Button* pSlider, CSaltoEditor* pThis)
{
	(void) pSlider;// prevent unused variable warning	  
  pThis->mpParams->SetSynthesisFlow(FLOW_GENERATE_SINE);
  pThis->mpTmplChooser->hide();
	pThis->mpLoopControls->hide();
  // do nothing else right now
}

//----------------------------------------------------------------------------//
// salto control parameter editing ATTACK
//----------------------------------------------------------------------------//
static void sSelectPitch(Fl_Counter* pCounter, CSaltoEditor* pThis)
{	
  pThis->mpParams->SetAttackTimbrePitch(pThis->mpPitch->value());  
  pThis->mpPitchDisplay->value(  
    pThis->mpParams->GetPitchAttribute(pCounter->value()) );
  
  TIndex pos = pThis->mpParams->GetAttackTimbre().GetPosition();   // abs. seg pos.
  pThis->mpOutput1->value(  pThis->mpParams->GetFileNameOfSegmentPos(pos) );
  pThis->LoadLoopValues(pos); // load values directly
  pThis->mpParams->SetDisplayedValuesChanged(true);
}

static void sSelectAttack(Fl_Counter* pCounter, CSaltoEditor* pThis)
{	
  pThis->mpParams->SetAttackTimbreAttack(pThis->mpCounterData->value());
 
  pThis->mpAttackDisplay->value(
    pThis->mpParams->GetAttackAttribute(pCounter->value()));

  TIndex pos = pThis->mpParams->GetAttackTimbre().GetPosition();  // depends on timbre vek.
  pThis->mpOutput1->value(  pThis->mpParams->GetFileNameOfSegmentPos(pos) );
  pThis->LoadLoopValues(pos); // load values directly
  pThis->mpParams->SetDisplayedValuesChanged(true);
}

/* chooser buttons for stat templates */
static void sStatTmplChoose0(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning	
  pThis->mpParams->SetCurrentStatTemplate(0);
  pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq0());
  pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr0());
}

static void sStatTmplChoose1(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning	
  pThis->mpParams->SetCurrentStatTemplate(1);
  pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq1());
  pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr1());
}

static void sStatTmplChoose2(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning	
  pThis->mpParams->SetCurrentStatTemplate(2);
  pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq2());  
  pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr2());
}

static void sStatTmplChoose3(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning	
  pThis->mpParams->SetCurrentStatTemplate(3);
  pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq3());
  pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr3());
}

static void sStatTmplChoose4(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning	
  pThis->mpParams->SetCurrentStatTemplate(4);
  pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq4());
  pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr4());
}

static void sStatTmplChoose5(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning	 
  pThis->mpParams->SetCurrentStatTemplate(5);
  pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq5());
  pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr5());
}

static void sStatTmplChoose6(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning	
  pThis->mpParams->SetCurrentStatTemplate(6);
  pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq6());
  pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr6());
}

static void sStatTmplChoose7(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning	  
  pThis->mpParams->SetCurrentStatTemplate(7);
  pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq7());
  pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr7());
}

// load / store loop params
static void sLoadLoopParams(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;// prevent unused variable warning	  
  TIndex pos = pThis->mpParams->GetAttackTimbre().GetPosition();
  
  pThis->LoadLoopValues(pos);
}

static void sStoreLoopParams(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;// prevent unused variable warning		
	TIndex pos = pThis->mpParams->GetAttackTimbre().GetPosition();
  
	pThis->StoreLoopValues(pos);
}


//----------------------------------------------------------------------------//
// salto control parameter Stationary Template Editor
//----------------------------------------------------------------------------//
static void sLoadSDIFToEdit(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;// prevent unused variable warning	
	pThis->LoadSDIFToEdit();
}

static void sStoreStationaryTmpl(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	//pThis->StoreStationaryTmpl();
}

static void sStatFrameFader(Fl_Value_Slider* pSlider, CSaltoEditor* pThis)
{	
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pSlider->value());
//  pThis->mpParams->SetStatPeakArray2(currFrame->GetSpectralPeakArray()); //no pointer
  pThis->mpParams->SetStatPeakArray(currFrame->GetSpectralPeakArray()); //no pointer
  pThis->mpParams->SetStatFundFreq(currFrame->GetFundamental().GetFreq());
  pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr());
}

static void sPlayFrameOnly(Fl_Button* pButton, CSaltoEditor* pThis)
{
 //pThis->mpParams->SetPlayFrameOnly(pButton->value());

}

static void sAddStationaryTmpl(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	pThis->AddStationaryTmplToDataBase(0);
}


static void sAddStationaryTmpl0(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
  pThis->mpParams->SetStatPeakArray0(currFrame->GetSpectralPeakArray());
  pThis->mpParams->SetStatFundFreq0(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(0);
}
static void sAddStationaryTmpl1(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
  pThis->mpParams->SetStatPeakArray1(currFrame->GetSpectralPeakArray());
  pThis->mpParams->SetStatFundFreq1(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(1);
}
static void sAddStationaryTmpl2(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
  pThis->mpParams->SetStatPeakArray2(currFrame->GetSpectralPeakArray());
  pThis->mpParams->SetStatFundFreq2(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(2);
}
static void sAddStationaryTmpl3(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
  pThis->mpParams->SetStatPeakArray3(currFrame->GetSpectralPeakArray());
  pThis->mpParams->SetStatFundFreq3(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(3);
}
static void sAddStationaryTmpl4(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
  pThis->mpParams->SetStatPeakArray4(currFrame->GetSpectralPeakArray());
  pThis->mpParams->SetStatFundFreq4(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(4);
}
static void sAddStationaryTmpl5(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
  pThis->mpParams->SetStatPeakArray5(currFrame->GetSpectralPeakArray());
  pThis->mpParams->SetStatFundFreq5(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(5);
}
static void sAddStationaryTmpl6(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
  pThis->mpParams->SetStatPeakArray6(currFrame->GetSpectralPeakArray());
  pThis->mpParams->SetStatFundFreq6(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(6);
}
static void sAddStationaryTmpl7(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;	
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
  pThis->mpParams->SetStatPeakArray7(currFrame->GetSpectralPeakArray());
  pThis->mpParams->SetStatFundFreq7(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(7);
}

//----------------------------------------------------------------------------//
// Timbre Vektor Display
//----------------------------------------------------------------------------//
static void sdisplay (Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
  pThis->mpParams->SetStatPeakArray3(currFrame->GetSpectralPeakArray());
  pThis->mpParams->SetStatFundFreq3(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(3);
}

//----------------------------------------------------------------------------//
// TIMER FOR SALTO DISPLAY
//----------------------------------------------------------------------------//

static void sOldTimer(void* p)
{	
  CSaltoEditor* pThis = (CSaltoEditor*) p;
  pThis->DisplayTmplArrayOfDouble(pThis->mpSaltoApp->GetAudioBufferReference());
  if ((pThis->mpParams->GetSynthesisFlow()==FLOW_INTERPOLATE) && (pThis->mpParams->GetPlay())) // only in synthesis mode...
	{
		//This is done to avoid that DisplaypeakArray() in CSaltoEditor tryes to draw a peak array
		//that is not yet filled in CSaltoInterpolation::DoInterpolation(...)
		pthread_mutex_lock(&mutPA);
    pthread_cond_wait(&condPA, &mutPA);

//		SpectralPeakArray peak2 = *pThis->mpSaltoApp->GetCurrentPeakArrayPtr();
//   	peak2.Debug();
	  pThis->DisplayPeakArray(*pThis->mpSaltoApp->GetCurrentPeakArrayPtr()); //BUG displays the peaks
//		Spectrum spec2 = *pThis->mpSaltoApp->GetCurrentResidualPtr();
//    spec2.Debug();
 		pThis->DisplayAttackResidual(*pThis->mpSaltoApp->GetCurrentResidualPtr()); //BUG displays the peaks		
 		pThis->DisplayStationaryResidual(*pThis->mpSaltoApp->GetCurrentResidualPtr()); //BUG displays the peaks		
		pThis->DisplaySynthesizedSpectrum(*pThis->mpSaltoApp->GetSynthFramePtr()->GetSpectrumPtr()); //BUG displays the peaks		

		pthread_mutex_unlock(&mutPA);
	}
  
  if (pThis->mpParams->GetDisplayedValuesChanged())
  {
    // test purpose // these diplays should be handeled by a message manager later !!
    pThis->mpVal_Output1a->value(pThis->mpParams->GetAttackTimbre().GetPitch());
    pThis->mpVal_Output1b->value(pThis->mpParams->GetAttackTimbre().GetAttack());
    pThis->mpVal_Output1c->value(pThis->mpParams->GetAttackTimbre().GetLevel());
    pThis->mpVal_Output1d->value(pThis->mpParams->GetInterPolFactor());
    pThis->mpVal_Output1e->value(pThis->mpParams->GetAttackTimbre().GetTransitionFrames());
  //  pThis->mpVal_Output1f->value(pThis->mpParams->GetCurrentAttack());
    pThis->mpVal_Output1g->value(pThis->mpParams->GetAttackTimbre().GetPosition());
    pThis->mpVal_Output1h->value(pThis->mpParams->GetTransitionMode());
    pThis->mpVal_Output1i->value(pThis->mpParams->GetTransitionFrequency());
    pThis->mpIPSlider->value(pThis->mpParams->GetInterPolFactor());
    pThis->mpCurrentTemplateDisplay->value(pThis->mpParams->GetStatTemplAttribute(pThis->mpParams->GetCurrentStatTemplate()));
    pThis->mpParams->SetDisplayedValuesChanged(false);
  }
  Fl::add_timeout(OSCILLOSCOPE_REFRESH_TIME,sTimer,pThis);
}


static void sTimer(void* p)
{
  CSaltoEditor* pThis = (CSaltoEditor*) p;



	pThis->DisplayTmplArrayOfDouble(pThis->mpSaltoApp->GetAudioBufferReference());
  if ((pThis->mpParams->GetSynthesisFlow()==FLOW_INTERPOLATE) && (pThis->mpParams->GetPlay())) // only in synthesis mode...
	{
		//This is done to avoid that DisplaypeakArray() in CSaltoEditor tryes to draw a peak array
		//that is not yet filled in CSaltoInterpolation::DoInterpolation(...)
//		pthread_mutex_lock(&mutPA);
//    pthread_cond_wait(&condPA, &mutPA);
//pthread_mutex_lock(&FLTKmutex);

//XA pthread_mutex_lock(&DrawingMutex);
//	if(!pThis->mpSaltoApp->GLOBAL_DRAWING)	
		pThis->DisplayPeakArray(); //BUG displays the peaks
// 	if(!pThis->mpSaltoApp->GLOBAL_DRAWING)	
		pThis->DisplayAttackResidual(); //BUG displays the peaks		
// 	if(!pThis->mpSaltoApp->GLOBAL_DRAWING)	
		pThis->DisplayStationaryResidual();
// 	if(!pThis->mpSaltoApp->GLOBAL_DRAWING)	
		pThis->DisplaySynthesizedSpectrum(); //BUG displays the peaks		
//XA pthread_mutex_unlock(&DrawingMutex);


//pthread_mutex_unlock(&FLTKmutex);

//		pthread_mutex_unlock(&mutPA);
	}

  if (pThis->mpParams->GetDisplayedValuesChanged())
  {
    // test purpose // these diplays should be handeled by a message manager later !!
    pThis->mpVal_Output1a->value(pThis->mpParams->GetAttackTimbre().GetPitch());
    pThis->mpVal_Output1b->value(pThis->mpParams->GetAttackTimbre().GetAttack());
    pThis->mpVal_Output1c->value(pThis->mpParams->GetAttackTimbre().GetLevel());
    pThis->mpVal_Output1d->value(pThis->mpParams->GetInterPolFactor());
    pThis->mpVal_Output1e->value(pThis->mpParams->GetAttackTimbre().GetTransitionFrames());
 //   pThis->mpVal_Output1f->value(pThis->mpParams->GetCurrentAttack());
    pThis->mpVal_Output1g->value(pThis->mpParams->GetAttackTimbre().GetPosition());
    pThis->mpVal_Output1h->value(pThis->mpParams->GetTransitionMode());
    pThis->mpVal_Output1i->value(pThis->mpParams->GetTransitionFrequency());
    pThis->mpIPSlider->value(pThis->mpParams->GetInterPolFactor());
    pThis->mpCurrentTemplateDisplay->value(pThis->mpParams->GetStatTemplAttribute(pThis->mpParams->GetCurrentStatTemplate()));
    pThis->mpParams->SetDisplayedValuesChanged(false);
  }
  Fl::add_timeout(OSCILLOSCOPE_REFRESH_TIME,sTimer,pThis);
}



//----------------------------------------------------------------------------//
// test display
//----------------------------------------------------------------------------//

static void sStatResGain(Fl_Value_Slider* pSlider, CSaltoEditor* pThis)
{	 
	TSize dB = pow(10,pSlider->value()/20.);
	pThis->mpParams->SetStatResGain(dB);
}

static void sStatResonanceFreq(Fl_Value_Slider* pSlider, CSaltoEditor* pThis)
{	 
	pThis->mpParams->SetAttackTimbreTransFrames((int)pSlider->value());
	pThis->mpParams->SetDisplayedValuesChanged(true);
}

static void sSetGain(Fl_Value_Slider* pSlider, CSaltoEditor* pThis)
{	 
//	TSize dB = pow(10,pSlider->value()/20.);
//	pThis->mpParams->SetGain(dB);
}

static void sSetTestSlider(Fl_Value_Slider* pSlider, CSaltoEditor* pThis)
{	 
	pThis->mpParams->SetRandomRange(pSlider->value());
}

static void sSetTestButton(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
	(void) pButton;	
	pThis->mpParams->SetWriteToFile(pButton->value());
//	pThis->mpMIDI->SimulateMidiProcess1();
}

static void sSetTestButton1(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
	(void) pButton;

	pThis->mpParams->SetUseMidiKeyboard(pButton->value());
//	pThis->mpMIDI->SimulateMidiProcess2();
}

static void sSetTestButton2(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
	pThis->mpParams->SetUsePitchCorrection(pButton->value());
}

static void sSetTestButton3(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
	pThis->mpParams->SetUseRandomDeviations(pButton->value());
}

static void sSetTestButton4(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
	if(pButton->value())
	  pThis->mpParams->SetTranspose(2);
	else
	  pThis->mpParams->SetTranspose(0);
}

static void sSetTestButton5(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
  pThis->mpParams->SetUseBreathController(pButton->value());
}

static void sSetTestButton6(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
  pThis->mpParams->SetUseRandomLoop(pButton->value());
}

static void sLoadMelody(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton; // prevent unused variable warnings
	char* str = fl_file_chooser("Select analysis data file","*.xml","");
	std::string melodyFileName(str);
	//pThis->mpSaltoApp->LoadMelody(melodyFileName);

	MelodyTranslatorConfig melodyCfg;
	melodyCfg.SetFileName( melodyFileName );

	if( pThis->mMelody->IsRunning() ) pThis->mMelody->Stop();

	pThis->mMelody->Configure( melodyCfg );
	pThis->mMelody->Start();

	pThis->mpSaltoApp->ResetEventSample();
	

	pThis->mpPlayMelodyButton->activate();
}

static void sPlayMelody(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;
	pThis->mpParams->SetUseMelody(true);
}


	Fl_Double_Window				*mpEditorWindow;

private:

	CSaltoDisplay						*mpWidgetWave;
	CSaltoSpectralDisplay   *mpWidgetPeaks;
	CSaltoSpectrumDisplay   *mpWidgetAttackResidual;
	CSaltoSpectrumDisplay   *mpWidgetStationaryResidual;
	CSaltoSpectrumDisplay   *mpWidgetSynthesizedSpectrum;

Fl_Value_Slider
  *mpIPSlider,
  *mpTVektor1A,  
  *mpTVektor1B,
  *mpTVektor1C,
  *mpTVektor2A,
  *mpTVektor2B,
  *mpTVektor2C,
  *mpTVektor2D,
  *mpTVektor2E,
  *mpTVektor2F,
  *mpTVektor2G;

Fl_Counter
  *mpPitch,
  *mpCounterData;  // seg data chooser
	
Fl_Group
	*mpSamples,
	*mpControls,
	*mpLoopControls,
	*mpTansportFunctions,
	*mpTmplChooser,
	*mpTVektorDisplay;
	
Fl_Button				
	*mpExitButton,
	*mpRadioButton0,
	*mpRadioButton1,
	*mpRadioButton2,
	*mpStartPlay,
	*mpStoreButton,
	*mpLoadMelodyButton,
	*mpPlayMelodyButton;
	
Fl_Light_Button
  	*mpAudioStartButton;

Fl_Output
  *mpPitchDisplay,
  *mpAttackDisplay,
  *mpCurrentTemplateDisplay,
	*mpOutput1;
	
	
Fl_Value_Output
	*mpVal_Output1a,
	*mpVal_Output1b,
	*mpVal_Output1c,
	*mpVal_Output1d,
  *mpVal_Output1e,
  *mpVal_Output1f,
  *mpVal_Output1g,
  *mpVal_Output1h,
  *mpVal_Output1i;
	
//  TemplChooser
Fl_Output *mpOutput3;
Fl_Value_Slider *mpFrameNum;

// pointers other classes
//CSaltoParameter	*mpParams;
Parameters *mpParams;
SaltoSynth *mpSaltoApp;
MelodyTranslator *mMelody;
BaseAudioApplication* mpApp;
//CSaltoMIDIInOut *mpMIDI;

TData mSamplingRate;
TSize mSpectralFrameSize;

};

} // end of namespace CLAM
#endif


/* alternative callback function!
static void sSetUsePhaseAlignment(Fl_Button* pButton)
{
	CSaltoEditor* pThis = (CSaltoEditor*) pButton->window()->user_data();
	pThis->mpParams->SetUsePhaseAlignment(pButton->value());
}
*/
