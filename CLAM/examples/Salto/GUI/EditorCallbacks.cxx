#include "CSaltoEditor.hxx"
// FLTK base services
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/fl_file_chooser.H>
// FLTK widgets
#include <FL/Fl_Button.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Adjuster.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Dial.H>
#include <FL/Fl_Value_Output.H>
#ifdef GetClassName
#undef GetClassName
#endif

#include "CSaltoSpectralDisplay.hxx"
#include "Parameters.hxx"
#include "MelodyTranslator.hxx"
#include "BaseAudioApplication.hxx"
#include "SaltoSynth.hxx"
#include "CSaltoDisplay.hxx"
#include "CSaltoSpectralDisplay.hxx"
#include "CSaltoSpectrumDisplay.hxx"
#include "CSaltoStatTmplData.hxx"

using CLAM::SaltoSynth;
using CLAM::MelodyTranslatorConfig;

namespace SALTO
{

 void CSaltoEditor::sStoreAll(Fl_Light_Button* pButton, CSaltoEditor* pThis)
{
//  pThis->mpSaltoApp->StoreAllParams();
}


 void CSaltoEditor::sStart(Fl_Light_Button* pButton, CSaltoEditor* pThis)
{
	//	pThis->mpParams->SetAudioStart(pButton->value());   // CHECKTHIS FOR iMPROVEMENT !!!
}


 void CSaltoEditor::sPlay(Fl_Button* pButton, CSaltoEditor* pThis)
{
	if(!pThis->mpParams->GetPlayNote()) // init once on play
	{
		pThis->mpParams->SetAttackTimbreLevel(105);
		pThis->mpParams->SetPitchModFactor(1.0);
		pThis->mpParams->SetPlayNoteOn(true);
	}
	else
		pThis->mpParams->SetPlayNoteOff( !bool(pButton->value()));

	pThis->mpParams->SetPlayNote(true);
}
	
 void CSaltoEditor::sExit(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton; // prevent unused variable warnings
	pThis->mpApp->Stop();
	pThis->mpParams->SetExit(true);
	pThis->mpSaltoApp->SetTerminationState( );
}

 void CSaltoEditor::sUseSines(Fl_Button* pButton, CSaltoEditor* pThis)
{
	pThis->mpParams->SetUseSines(bool(pButton->value()));
}

 void CSaltoEditor::sUseAttackResidual(Fl_Button* pButton, CSaltoEditor* pThis)
{
	pThis->mpParams->SetUseAttackResidual(bool(pButton->value()));
}

 void CSaltoEditor::sUseStatResidual(Fl_Button* pButton, CSaltoEditor* pThis)
{
	pThis->mpParams->SetUseStatResidual(bool(pButton->value()));
}

 void CSaltoEditor::sSetUsePhaseAlignment(Fl_Button* pButton, CSaltoEditor* pThis)
{
	pThis->mpParams->SetUsePhaseAlignment(bool(pButton->value()));
}

 void CSaltoEditor::sSetIPValue(Fl_Slider* pSlider, CSaltoEditor* pThis)
{
	pThis->mpParams->SetInterPolFactor(pSlider->value());
	pThis->mpParams->SetDisplayedValuesChanged(true); // display needs update 
}

 void CSaltoEditor::sShowIPEditor(Fl_Button* pSlider, CSaltoEditor* pThis)
{
	(void) pSlider;
	pThis->mpParams->SetSynthesisFlow(FLOW_INTERPOLATE);
	pThis->mpTmplChooser->hide();
	pThis->mpLoopControls->show();
	pThis->DisplayPeakArray(*pThis->mpParams->GetCurrentStatTemplatePtr());
}

 void CSaltoEditor::sShowTemplateEditor(Fl_Button* pSlider, CSaltoEditor* pThis)
{
	(void) pSlider;
	pThis->mpParams->SetSynthesisFlow(FLOW_SDIF_EDIT);
	pThis->mpLoopControls->hide();
	pThis->mpTmplChooser->show();
	pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr());
}


 void CSaltoEditor::sShowPlayEditor(Fl_Button* pSlider, CSaltoEditor* pThis)
{
	(void) pSlider;// prevent unused variable warning
	pThis->mpParams->SetSynthesisFlow(FLOW_GENERATE_SINE);
	pThis->mpTmplChooser->hide();
	pThis->mpLoopControls->hide();
	// do nothing else right now
}

 void CSaltoEditor::sSelectPitch(Fl_Counter* pCounter, CSaltoEditor* pThis)
{	
	pThis->mpParams->SetAttackTimbrePitch(pThis->mpPitch->value());  
	// MRJ: I found this line "as is" in the code...
	//	pThis->mpPitchDisplay->value(  
	pThis->mpParams->GetPitchAttribute(pCounter->value() );

	TIndex pos = pThis->mpParams->GetAttackTimbre().GetPosition();   // abs. seg pos.
	pThis->mpOutput1->value(  pThis->mpParams->GetFileNameOfSegmentPos(pos) );
	pThis->LoadLoopValues(pos); // load values directly
	pThis->mpParams->SetDisplayedValuesChanged(true);
}

 void CSaltoEditor::sSelectAttack(Fl_Counter* pCounter, CSaltoEditor* pThis)
{	
	pThis->mpParams->SetAttackTimbreAttack(pThis->mpCounterData->value());

	// MRJ: I found this line "as is" in the code...
	// pThis->mpAttackDisplay->value(
	pThis->mpParams->GetAttackAttribute(pCounter->value());

	TIndex pos = pThis->mpParams->GetAttackTimbre().GetPosition();  // depends on timbre vek.
	pThis->mpOutput1->value(  pThis->mpParams->GetFileNameOfSegmentPos(pos) );
	pThis->LoadLoopValues(pos); // load values directly
	pThis->mpParams->SetDisplayedValuesChanged(true);
}

 void CSaltoEditor::sStatTmplChoose0(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning	
	pThis->mpParams->SetCurrentStatTemplate(0);
	pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq0());
	pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr0());
}

 void CSaltoEditor::sStatTmplChoose1(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning
	pThis->mpParams->SetCurrentStatTemplate(1);
	pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq1());
	pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr1());
}

 void CSaltoEditor::sStatTmplChoose2(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning
	pThis->mpParams->SetCurrentStatTemplate(2);
	pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq2());
	pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr2());
}

 void CSaltoEditor::sStatTmplChoose3(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning
	pThis->mpParams->SetCurrentStatTemplate(3);
	pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq3());
	pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr3());
}

 void CSaltoEditor::sStatTmplChoose4(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning
	pThis->mpParams->SetCurrentStatTemplate(4);
	pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq4());
	pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr4());
}

 void CSaltoEditor::sStatTmplChoose5(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning
	pThis->mpParams->SetCurrentStatTemplate(5);
	pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq5());
	pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr5());
}

 void CSaltoEditor::sStatTmplChoose6(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning
	pThis->mpParams->SetCurrentStatTemplate(6);
	pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq6());
	pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr6());
}

 void CSaltoEditor::sStatTmplChoose7(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;// prevent unused variable warning
	pThis->mpParams->SetCurrentStatTemplate(7);
	pThis->mpParams->SetStatFundFreq(pThis->mpParams->GetStatFundFreq7());
	pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr7());
}

 void CSaltoEditor::sLoadLoopParams(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;// prevent unused variable warning
	TIndex pos = pThis->mpParams->GetAttackTimbre().GetPosition();

	pThis->LoadLoopValues(pos);
}

 void CSaltoEditor::sStoreLoopParams(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;// prevent unused variable warning		
	TIndex pos = pThis->mpParams->GetAttackTimbre().GetPosition();

	pThis->StoreLoopValues(pos);
}

 void CSaltoEditor::sLoadSDIFToEdit(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;// prevent unused variable warning	
	pThis->LoadSDIFToEdit();
}

 void CSaltoEditor::sStoreStationaryTmpl(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	//pThis->StoreStationaryTmpl();
}

 void CSaltoEditor::sStatFrameFader(Fl_Value_Slider* pSlider, CSaltoEditor* pThis)
{	
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pSlider->value());
//	pThis->mpParams->SetStatPeakArray2(currFrame->GetSpectralPeakArray()); //no pointer
	pThis->mpParams->SetStatPeakArray(currFrame->GetSpectralPeakArray()); //no pointer
	pThis->mpParams->SetStatFundFreq(currFrame->GetFundamental().GetFreq());
	pThis->DisplayPeakArray(*pThis->mpParams->GetStatPeakArrayPtr());
}

 void CSaltoEditor::sAddStationaryTmpl(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	pThis->AddStationaryTmplToDataBase(0);
}

 void CSaltoEditor::sAddStationaryTmpl0(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
	pThis->mpParams->SetStatPeakArray0(currFrame->GetSpectralPeakArray());
	pThis->mpParams->SetStatFundFreq0(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(0);
}

 void CSaltoEditor::sAddStationaryTmpl1(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
	pThis->mpParams->SetStatPeakArray1(currFrame->GetSpectralPeakArray());
	pThis->mpParams->SetStatFundFreq1(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(1);
}

 void CSaltoEditor::sAddStationaryTmpl2(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
	pThis->mpParams->SetStatPeakArray2(currFrame->GetSpectralPeakArray());
	pThis->mpParams->SetStatFundFreq2(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(2);
}

 void CSaltoEditor::sAddStationaryTmpl3(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
	pThis->mpParams->SetStatPeakArray3(currFrame->GetSpectralPeakArray());
	pThis->mpParams->SetStatFundFreq3(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(3);
}

 void CSaltoEditor::sAddStationaryTmpl4(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
	pThis->mpParams->SetStatPeakArray4(currFrame->GetSpectralPeakArray());
	pThis->mpParams->SetStatFundFreq4(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(4);
}

 void CSaltoEditor::sAddStationaryTmpl5(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
	pThis->mpParams->SetStatPeakArray5(currFrame->GetSpectralPeakArray());
	pThis->mpParams->SetStatFundFreq5(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(5);
}

 void CSaltoEditor::sAddStationaryTmpl6(Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
	pThis->mpParams->SetStatPeakArray6(currFrame->GetSpectralPeakArray());
	pThis->mpParams->SetStatFundFreq6(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(6);
}

 void CSaltoEditor::sAddStationaryTmpl7(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;	
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
	pThis->mpParams->SetStatPeakArray7(currFrame->GetSpectralPeakArray());
	pThis->mpParams->SetStatFundFreq7(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(7);
}

 void CSaltoEditor::sdisplay (Fl_Button* pButton, CSaltoEditor* pThis)
{	
	(void) pButton;
	Frame* currFrame;
	currFrame = pThis->mpSaltoApp->GetStatTmplFrame((TIndex)pThis->mpFrameNum->value());
	pThis->mpParams->SetStatPeakArray3(currFrame->GetSpectralPeakArray());
	pThis->mpParams->SetStatFundFreq3(currFrame->GetFundamental().GetFreq());
	
	pThis->AddStationaryTmplToDataBase(3);
}

 void CSaltoEditor::sOldTimer(void* p)
{	
	CSaltoEditor* pThis = (CSaltoEditor*) p;
	pThis->DisplayTmplArrayOfDouble(pThis->mpSaltoApp->GetAudioBufferReference());
	if ((pThis->mpParams->GetSynthesisFlow()==FLOW_INTERPOLATE) && (pThis->mpParams->GetPlay())) // only in synthesis mode...
	{
		//This is done to avoid that DisplaypeakArray() in CSaltoEditor tryes to draw a peak array
		//that is not yet filled in CSaltoInterpolation::DoInterpolation(...)

//		SpectralPeakArray peak2 = *pThis->mpSaltoApp->GetCurrentPeakArrayPtr();
//		peak2.Debug();
		pThis->DisplayPeakArray(*pThis->mpSaltoApp->GetCurrentPeakArrayPtr()); //BUG displays the peaks
//		Spectrum spec2 = *pThis->mpSaltoApp->GetCurrentResidualPtr();
//		spec2.Debug();
 		pThis->DisplayAttackResidual(*pThis->mpSaltoApp->GetCurrentResidualPtr()); //BUG displays the peaks		
 		pThis->DisplayStationaryResidual(*pThis->mpSaltoApp->GetCurrentResidualPtr()); //BUG displays the peaks		
		pThis->DisplaySynthesizedSpectrum(*pThis->mpSaltoApp->GetSynthFramePtr()->GetSpectrumPtr()); //BUG displays the peaks		

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

 void CSaltoEditor::sTimer(void* p)
{
	CSaltoEditor* pThis = (CSaltoEditor*) p;

	pThis->DisplayTmplArrayOfDouble(pThis->mpSaltoApp->GetAudioBufferReference());
	if ((pThis->mpParams->GetSynthesisFlow()==FLOW_INTERPOLATE) && (pThis->mpParams->GetPlay())) // only in synthesis mode...
	{
		//This is done to avoid that DisplaypeakArray() in CSaltoEditor tryes to draw a peak array
		//that is not yet filled in CSaltoInterpolation::DoInterpolation(...)
//		pthread_mutex_lock(&mutPA);
//		pthread_cond_wait(&condPA, &mutPA);
//		pthread_mutex_lock(&FLTKmutex);

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
//		pThis->mpVal_Output1f->value(pThis->mpParams->GetCurrentAttack());
		pThis->mpVal_Output1g->value(pThis->mpParams->GetAttackTimbre().GetPosition());
		pThis->mpVal_Output1h->value(pThis->mpParams->GetTransitionMode());
		pThis->mpVal_Output1i->value(pThis->mpParams->GetTransitionFrequency());
		pThis->mpIPSlider->value(pThis->mpParams->GetInterPolFactor());
		pThis->mpCurrentTemplateDisplay->value(pThis->mpParams->GetStatTemplAttribute(pThis->mpParams->GetCurrentStatTemplate()));
		pThis->mpParams->SetDisplayedValuesChanged(false);
	}
	Fl::add_timeout(OSCILLOSCOPE_REFRESH_TIME,sTimer,pThis);
}

 void CSaltoEditor::sStatResGain(Fl_Value_Slider* pSlider, CSaltoEditor* pThis)
{	 
	TSize dB = pow(10,pSlider->value()/20.);
	pThis->mpParams->SetStatResGain(dB);
}

 void CSaltoEditor::sStatResonanceFreq(Fl_Value_Slider* pSlider, CSaltoEditor* pThis)
{	 
	pThis->mpParams->SetAttackTimbreTransFrames((int)pSlider->value());
	pThis->mpParams->SetDisplayedValuesChanged(true);
}

 void CSaltoEditor::sSetGain(Fl_Value_Slider* pSlider, CSaltoEditor* pThis)
{	 
//	TSize dB = powf(20.0f,float(pSlider->value())/20.0f);
//	pThis->mpParams->SetGain(dB);
}

 void CSaltoEditor::sSetTestSlider(Fl_Value_Slider* pSlider, CSaltoEditor* pThis)
{	 
	pThis->mpParams->SetRandomRange(pSlider->value());
}

 void CSaltoEditor::sSetTestButton(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
	(void) pButton;	
	pThis->mpParams->SetWriteToFile(bool(pButton->value()));
}

 void CSaltoEditor::sSetTestButton1(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
	(void) pButton;

	pThis->mpParams->SetUseMidiKeyboard(bool(pButton->value()));
}

 void CSaltoEditor::sSetTestButton2(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
	pThis->mpParams->SetUsePitchCorrection(bool(pButton->value()));
}

 void CSaltoEditor::sSetTestButton3(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
	pThis->mpParams->SetUseRandomDeviations(bool(pButton->value()));
}

 void CSaltoEditor::sSetTestButton4(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
	if(pButton->value())
		pThis->mpParams->SetTranspose(2);
	else
		pThis->mpParams->SetTranspose(0);
}

 void CSaltoEditor::sSetTestButton5(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
	pThis->mpParams->SetUseBreathController(bool(pButton->value()));
}

 void CSaltoEditor::sSetTestButton6(Fl_Button* pButton, CSaltoEditor* pThis)
{	 
	pThis->mpParams->SetUseRandomLoop(bool(pButton->value()));
}

 void CSaltoEditor::sLoadMelody(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton; // prevent unused variable warnings
	char* str = fl_file_chooser("Select analysis data file","*.xml","");
	
	if(str)
	{
		std::string melodyFileName(str);

		MelodyTranslatorConfig melodyCfg;
		melodyCfg.SetFileName( melodyFileName );

		if( pThis->mMelody->IsRunning() ) pThis->mMelody->Stop();

		pThis->mMelody->Configure( melodyCfg );
		pThis->mMelody->Start();

		pThis->mpSaltoApp->ResetEventSample();
		

		pThis->mpPlayMelodyButton->activate();
	}
}

/**  Plays a previously loaded Melody
 */
 void CSaltoEditor::sPlayMelody(Fl_Button* pButton, CSaltoEditor* pThis)
{
	(void) pButton;
	pThis->mpParams->SetUseMelody(true);
}


}
