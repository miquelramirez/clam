#include "SaltoSynth.hxx"
#include "CSaltoEditor.hxx"
#include "EditorCallbacks.hxx"
#include "CSaltoDisplay.hxx"
#include "CSaltoSpectralDisplay.hxx"
#include "CSaltoSpectrumDisplay.hxx"
#include "Parameters.hxx"
#include "CSaltoStatTmplData.hxx"
#include "BaseAudioApplication.hxx"
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/fl_file_chooser.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Adjuster.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Dial.H>

#include "CSaltoSegData.hxx"

using CLAM::CSaltoStatTmplData;
using CLAM::CSaltoSegData;

const int TP_FUNCTIONS_X = 200;
const int TP_FUNCTIONS_Y = 660;

const int S_CHOOSER_X = 310;
const int S_CHOOSER_Y = 565;

const int TOGGLE1_X  = 205;
const int TOGGLE1_Y = 565;

const int ALIGN_X_4 = 410;
const int ALIGN_Y_4 = 565;

const int IP_SLIDERS_X= 420;
const int IP_SLIDERS_Y= 250;

const int LOOP_CONTROL_X= 205;
const int LOOP_CONTROL_Y=  285;

const int LOOP_CONTROL_STAT_X = 540;
const int LOOP_CONTROL_STAT_Y = 285;

const int TEST_FADERS_X = 545;
const int TEST_FADERS_Y = 565;

namespace SALTO
{

class MyWindow:public Fl_Double_Window {
public:
/* this class is only a kludge to use the enter key as a shortcut
   for the play button; this ought to work with Fl_Widget::shortcut
   but doesn«t when the widget has a when() different then default.
*/
  Fl_Button* mpPlayButton;
  
  MyWindow(int W,int H,char* name):Fl_Double_Window(W,H,name)
  {
    
  }
  
  static void idle(void* ptr)
  {
    if (Fl::event_key(' ')==0)
    {
        Fl::remove_idle(idle,ptr);
        ((MyWindow*)ptr)->mpPlayButton->value(0); 
        ((MyWindow*)ptr)->mpPlayButton->do_callback();
    }
  }
  
  int handle(int event)
  {
    if (event==FL_KEYBOARD)
    {
      if (Fl::event_key()==' ')
      {
        mpPlayButton->value(1); 
        mpPlayButton->do_callback();
      }
      Fl::add_idle(idle,this);
    }
  
    return Fl_Double_Window::handle(event);
  }
};

//CSaltoEditor::CSaltoEditor(CSaltoParameter *pParams,CSaltoDSP *pSaltoApp,CSaltoMIDIInOut *pMIDI)
CSaltoEditor::CSaltoEditor(Parameters *pParams,SaltoSynth *pSaltoApp, MelodyTranslator *melody, BaseAudioApplication* pApp)//,CSaltoMIDIInOut *pMIDI)
{
  mpApp = pApp;
  mpParams = pParams;
  mpSaltoApp = pSaltoApp;
  mMelody = melody;
//  mpMIDI =pMIDI;
	
  mpEditorWindow = new MyWindow(1024,860,"SALTO");
  mpEditorWindow->color(FL_GRAY);
//----------------------------------------------------------------------------//
  // waveform dislay	
  mpWidgetWave = new CSaltoDisplay(19,10,512,120);
  // peaks display
  mpWidgetPeaks = new CSaltoSpectralDisplay(19,140,512,100);
	// residual display
  mpWidgetAttackResidual = new CSaltoSpectrumDisplay(565,10,450,90);
//  mpWidgetAttackResidual = new CSaltoSpectrumDisplay(5,695,520,90);
// test spectrum display
//  mpWidgetStationaryResidual = new CSaltoSpectrumDisplay(565,140,512,90);
//  mpWidgetStationaryResidual = new CSaltoSpectrumDisplay(5,790,520,90);
// test spectrum display
  mpWidgetSynthesizedSpectrum = new CSaltoSpectrumDisplay(565,140,450,90);
//  mpWidgetSynthesizedSpectrum = new CSaltoSpectrumDisplay(5,885,520,90);

//----------------------------------------------------------------------------//	  
  // transport functions
  mpTansportFunctions = new Fl_Group(TP_FUNCTIONS_X,TP_FUNCTIONS_Y,550,30);
  mpTansportFunctions->box(FL_THIN_UP_BOX);
  {
    mpAudioStartButton = new Fl_Light_Button(TP_FUNCTIONS_X+5,TP_FUNCTIONS_Y+5,100,20,"Audio Run");
    mpAudioStartButton->when(FL_WHEN_CHANGED);
    mpAudioStartButton->callback((Fl_Callback*) sStart,this);
 // 	mpAudioStartButton->value(mpParams->GetAudioStart());
#if defined linux || defined WIN32
    mpAudioStartButton->hide();
#endif //linux

    mpStoreButton = new Fl_Button(TP_FUNCTIONS_X+105,TP_FUNCTIONS_Y+5,100,20,"SAVE");
  	mpStoreButton->callback((Fl_Callback*) sStoreAll,this);
#ifdef DEMO_VERSION
   	mpStoreButton->deactivate();
#endif //DEMO_VERSION 		
  	  	
  	mpExitButton = new Fl_Button(TP_FUNCTIONS_X+205,TP_FUNCTIONS_Y+5,100,20,"EXIT");
    mpExitButton->shortcut(FL_Escape);
  	mpExitButton->callback((Fl_Callback*) sExit,this);

  	mpStartPlay	= new Fl_Button(TP_FUNCTIONS_X+445,TP_FUNCTIONS_Y+5,100,20,"Play");
  	mpStartPlay->when(FL_WHEN_CHANGED); //doesnt work with shortcut !  
  	mpStartPlay->callback((Fl_Callback*) sPlay,this);

    ((MyWindow*)mpEditorWindow)->mpPlayButton=mpStartPlay;

  }
  mpTansportFunctions->end();

  
  	//XA: Buttons to load and play melody
	mpLoadMelodyButton	= new Fl_Button(800,400,100,20,"LoadMelody");
	//mpLoadMelodyButton->when(FL_WHEN_CHANGED);
  	mpLoadMelodyButton->callback((Fl_Callback*) sLoadMelody,this);

	mpPlayMelodyButton	= new Fl_Button(800,450,100,20,"PlayMelody");
  	mpPlayMelodyButton->callback((Fl_Callback*) sPlayMelody,this);
	mpPlayMelodyButton->deactivate();

//----------------------------------------------------------------------------//	
	// Interpolation

  Fl_Output* pTBox = new Fl_Output (IP_SLIDERS_X-100,IP_SLIDERS_Y,90,20);
	pTBox->color(FL_GRAY);
	pTBox->textfont(0);
	pTBox->textsize(9);
	pTBox->box(FL_THIN_UP_BOX);
	pTBox->value("InterpolationFactor");

  mpIPSlider = new Fl_Value_Slider(IP_SLIDERS_X,IP_SLIDERS_Y,100,20);
//  mpIPSlider->type(FL_HORIZONTAL);
  mpIPSlider->type(5);
  mpIPSlider->range(0,1);
  mpIPSlider->value(0);
  mpIPSlider->callback((Fl_Callback*) sSetIPValue,this);

  char* name="CurrTempl";
  mpCurrentTemplateDisplay = new Fl_Output(IP_SLIDERS_X+120,IP_SLIDERS_Y,60,20);
  mpCurrentTemplateDisplay->textfont(0);
  mpCurrentTemplateDisplay->textsize(9);
  mpCurrentTemplateDisplay->value(name);
  mpCurrentTemplateDisplay->align(FL_ALIGN_BOTTOM);	
	
//----------------------------------------------------------------------------//	
	// -------------- Editor chooser ----------------------------// 
  mpSamples = new Fl_Group(S_CHOOSER_X,S_CHOOSER_Y,100,90);
  mpSamples->box(FL_THIN_UP_BOX);
  {
	mpRadioButton0 = new Fl_Button(S_CHOOSER_X+5,S_CHOOSER_Y+5, 20, 20, "IP-Edit");
    mpRadioButton0->type(FL_RADIO_BUTTON);
	mpRadioButton0->selection_color(1);
	mpRadioButton0->labelfont(FL_HELVETICA);
    mpRadioButton0->labelsize(12);
	mpRadioButton0->align(FL_ALIGN_RIGHT);
	mpRadioButton0->callback((Fl_Callback*) sShowIPEditor,this);
#ifdef DEMO_VERSION
  	mpRadioButton0->deactivate();
#endif //DEMO_VERSION
  }
  {
	mpRadioButton1 = new Fl_Button(S_CHOOSER_X+5,S_CHOOSER_Y+25, 20, 20, "TPL-Edit");
    mpRadioButton1->type(FL_RADIO_BUTTON);
    mpRadioButton1->selection_color(1);
    mpRadioButton1->labelfont(FL_HELVETICA);
    mpRadioButton1->labelsize(12);
	mpRadioButton1->align(FL_ALIGN_RIGHT);
    mpRadioButton1->callback((Fl_Callback*) sShowTemplateEditor,this);
#ifdef DEMO_VERSION
  	mpRadioButton1->deactivate();
#endif //DEMO_VERSION
  }
  {
    mpRadioButton2 = new Fl_Button(S_CHOOSER_X+5,S_CHOOSER_Y+45, 20, 20, "TestSweep");
    mpRadioButton2->type(FL_RADIO_BUTTON);
	mpRadioButton2->labelfont(FL_HELVETICA);
    mpRadioButton2->labelsize(12);
    mpRadioButton2->selection_color(1);
  	mpRadioButton2->align(FL_ALIGN_RIGHT);
  	mpRadioButton2->callback((Fl_Callback*) sShowPlayEditor,this);
#ifdef DEMO_VERSION
  	mpRadioButton2->deactivate();
#endif //DEMO_VERSION
  }
  mpSamples->end();
//----------------------------------------------------------------------------//	
  // -------------- toggle controls ----------------------------//
  mpControls	= new Fl_Group(TOGGLE1_X,TOGGLE1_Y,100,90);
  mpControls->box(FL_THIN_UP_BOX);

  Fl_Button* t0 = new Fl_Button(TOGGLE1_X+5,TOGGLE1_Y+5, 20, 20, "Sines");
    t0->type(FL_TOGGLE_BUTTON);
	t0->selection_color(1);
	t0->labelfont(FL_HELVETICA);
    t0->labelsize(12);
	t0->align(FL_ALIGN_RIGHT);
	t0->callback((Fl_Callback*) sUseSines,this);
	t0->value(mpParams->GetUseSines());

  Fl_Button* t1 = new Fl_Button(TOGGLE1_X+5,TOGGLE1_Y+25, 20, 20, "PhAlign");
    t1->type(FL_TOGGLE_BUTTON);
	t1->selection_color(1);
	t1->labelfont(FL_HELVETICA);
    t1->labelsize(12);
	t1->align(FL_ALIGN_RIGHT);
	t1->callback((Fl_Callback*) sSetUsePhaseAlignment,this);
	t1->value(mpParams->GetUsePhaseAlignment());
	  
  Fl_Button* t2 = new Fl_Button(TOGGLE1_X+5,TOGGLE1_Y+45, 20, 20, "A-Residual");
    t2->type(FL_TOGGLE_BUTTON);
	t2->labelfont(FL_HELVETICA);
    t2->labelsize(12);
	t2->selection_color(1);
	t2->align(FL_ALIGN_RIGHT);
	t2->callback((Fl_Callback*) sUseAttackResidual,this);		
	t2->value(mpParams->GetUseAttackResidual());

  Fl_Button* t3 = new Fl_Button(TOGGLE1_X+5,TOGGLE1_Y+65, 20, 20, "S-Residual");
    t3->type(FL_TOGGLE_BUTTON);
	t3->labelfont(FL_HELVETICA);
    t3->labelsize(12);
    t3->selection_color(1);
    t3->align(FL_ALIGN_RIGHT);
	t3->callback((Fl_Callback*) sUseStatResidual,this);		
			
  mpControls->end();

//----------------------------------------------------------------------------//	
  // -------------- LoopControls	Attack ----------------------------//
  mpLoopControls = new Fl_Group(LOOP_CONTROL_X,LOOP_CONTROL_Y,540,275);
  mpLoopControls->box(FL_THIN_UP_BOX);
  {
   	mpPitch = new Fl_Counter(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+5,60,20);
	  mpPitch->type(FL_HORIZONTAL);
	  mpPitch->textfont(0);
	  mpPitch->textsize(9);
	  mpPitch->step(1.0);
	  mpPitch->range(0,MAX_PITCH_VALUE-1);
	  mpPitch->callback((Fl_Callback*) sSelectPitch,this);

  	char*  name="Pitch";
	mpPitchDisplay = new Fl_Output(LOOP_CONTROL_X+70,LOOP_CONTROL_Y+5,60,20);
	  mpPitchDisplay->textfont(0);
	  mpPitchDisplay->textsize(9);
      mpPitchDisplay->value(name);
      mpPitchDisplay->align(FL_ALIGN_BOTTOM);	
  
  	mpCounterData = new Fl_Counter(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+25,60,20);
	  mpCounterData->type(FL_HORIZONTAL);
	  mpCounterData->step(1.0);
	  mpCounterData->textfont(0);
	  mpCounterData->textsize(9);
	  mpCounterData->range(0,MAX_ATTACKS_VALUE-1);
	  mpCounterData->callback((Fl_Callback*) sSelectAttack,this);

  	char*  name1="Attack";
	mpAttackDisplay = new Fl_Output(LOOP_CONTROL_X+70,LOOP_CONTROL_Y+25,60,20);
	  mpAttackDisplay->textfont(0);
	  mpAttackDisplay->textsize(9);
      mpAttackDisplay->value(name1);
      mpAttackDisplay->align(FL_ALIGN_BOTTOM);	

    const char*  name2=mpParams->GetFileNameOfSegmentPos(0);//default
	mpOutput1 = new Fl_Output(LOOP_CONTROL_X+150,LOOP_CONTROL_Y+5,110,20);
	  mpOutput1->textfont(0);
	  mpOutput1->textsize(9);
      mpOutput1->value(name2);
      mpOutput1->align(FL_ALIGN_BOTTOM);	
     
    mpTVektor1A = new Fl_Value_Slider(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+55,190,20);
      mpTVektor1A->type(FL_HORIZONTAL);
      mpTVektor1A->step(1.0);
  	  mpTVektor1A->range(0,100);
#ifdef DEMO_VERSION
  	  mpTVektor1A->deactivate();
#endif //DEMO_VERSION
  	
  	Fl_Output* pTextBox1A = new  Fl_Output(LOOP_CONTROL_X+200,LOOP_CONTROL_Y+55,60,20);
      pTextBox1A->color(FL_GRAY);
      pTextBox1A->textfont(0);
  	  pTextBox1A->textsize(9);
  	  pTextBox1A->box(FL_THIN_UP_BOX);
  	  pTextBox1A->value("StartFrame");
#ifdef DEMO_VERSION  	
  	  pTextBox1A->deactivate();
#endif //DEMO_VERSION  	

  	mpTVektor1B = new	Fl_Value_Slider(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+75,190,20);
  	  mpTVektor1B->type(FL_HORIZONTAL);
 	  mpTVektor1B->step(1.0);
 	  mpTVektor1B->range(0,100);
 	  mpTVektor1B->value(0);
#ifdef DEMO_VERSION
      mpTVektor1B->deactivate();
#endif //DEMO_VERSION

  	Fl_Output* pTextBox1B = new  Fl_Output(LOOP_CONTROL_X+200,LOOP_CONTROL_Y+75,60,20);
      pTextBox1B->color(FL_GRAY);
  	  pTextBox1B->textfont(0);
  	  pTextBox1B->textsize(9);
  	  pTextBox1B->box(FL_THIN_UP_BOX);
  	  pTextBox1B->value("Vol[dB]");
#ifdef DEMO_VERSION  	
  	  pTextBox1B->deactivate();
#endif //DEMO_VERSION  	  	

    mpTVektor1C = new Fl_Value_Slider(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+95,190,20);
 	  mpTVektor1C->type(FL_HORIZONTAL);
 	  mpTVektor1C->step(1.0);
 	  mpTVektor1C->range(-10,10);
 	  mpTVektor1C->value(0);
#ifdef DEMO_VERSION
      mpTVektor1C->deactivate();
#endif //DEMO_VERSION 		
 		
 	Fl_Output* pTextBox1C = new  Fl_Output(LOOP_CONTROL_X+200,LOOP_CONTROL_Y+95,60,20);
  	  pTextBox1C->color(FL_GRAY);
  	  pTextBox1C->textfont(0);
  	  pTextBox1C->textsize(9);
  	  pTextBox1C->box(FL_THIN_UP_BOX);
  	  pTextBox1C->value("LoopIn");
#ifdef DEMO_VERSION  	
  	  pTextBox1C->deactivate();
#endif //DEMO_VERSION  	  	

  	mpTVektor2A	= new	Fl_Value_Slider(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+115,190,20);
  	  mpTVektor2A->type(FL_HORIZONTAL);
 	  mpTVektor2A->step(1.0);
 	  mpTVektor2A->range(0,100);
 	  mpTVektor2A->value(0);
#ifdef DEMO_VERSION
      mpTVektor2A->deactivate();
#endif //DEMO_VERSION 		 		

    Fl_Output* pTextBox2A = new  Fl_Output(LOOP_CONTROL_X+200,LOOP_CONTROL_Y+115,60,20);
  	  pTextBox2A->color(FL_GRAY);
  	  pTextBox2A->textfont(0);
  	  pTextBox2A->textsize(9);
  	  pTextBox2A->box(FL_THIN_UP_BOX);
  	  pTextBox2A->value("LoopOut");
#ifdef DEMO_VERSION  	
  	  pTextBox2A->deactivate();
#endif //DEMO_VERSION  	  	

  	mpTVektor2B	= new	Fl_Value_Slider(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+135,190,20);
      mpTVektor2B->type(FL_HORIZONTAL);
 	  mpTVektor2B->step(1.0);
 	  mpTVektor2B->range(0,100);
 	  mpTVektor2B->value(0);
#ifdef DEMO_VERSION
      mpTVektor2B->deactivate();
#endif //DEMO_VERSION 		 		 		
	
    Fl_Output* pTextBox2B = new  Fl_Output(LOOP_CONTROL_X+200,LOOP_CONTROL_Y+135,60,20);
      pTextBox2B->color(FL_GRAY);
  	  pTextBox2B->textfont(0);
  	  pTextBox2B->textsize(9);
  	  pTextBox2B->box(FL_THIN_UP_BOX);
  	  pTextBox2B->value("IPStart");
#ifdef DEMO_VERSION  	
  	  pTextBox2B->deactivate();
#endif //DEMO_VERSION  	  	

  	mpTVektor2C = new Fl_Value_Slider(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+155,190,20);
  	  mpTVektor2C->type(FL_HORIZONTAL);
 	  mpTVektor2C->step(1.0);
 	  mpTVektor2C->range(0,100);
 	  mpTVektor2C->value(0);
#ifdef DEMO_VERSION
      mpTVektor2C->deactivate();
#endif //DEMO_VERSION 		 		 		 		

    Fl_Output* pTextBox2C = new  Fl_Output(LOOP_CONTROL_X+200,LOOP_CONTROL_Y+155,60,20);
  	  pTextBox2C->color(FL_GRAY);
  	  pTextBox2C->textfont(0);
  	  pTextBox2C->textsize(9);
  	  pTextBox2C->box(FL_THIN_UP_BOX);
  	  pTextBox2C->value("IPEnd");
#ifdef DEMO_VERSION  	
  	  pTextBox2C->deactivate();
#endif //DEMO_VERSION  	  	

    mpTVektor2D	= new	Fl_Value_Slider(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+175,190,20);
	  mpTVektor2D->type(FL_HORIZONTAL);
 	  mpTVektor2D->step(1.0);
 	  mpTVektor2D->range(0,100);
 	  mpTVektor2D->value(0);
#ifdef DEMO_VERSION
      mpTVektor2D->deactivate();
#endif //DEMO_VERSION 		 		 		 		 		

    Fl_Output* pTextBox2D = new  Fl_Output(LOOP_CONTROL_X+200,LOOP_CONTROL_Y+175,60,20);
  	  pTextBox2D->color(FL_GRAY);
  	  pTextBox2D->textfont(0);
  	  pTextBox2D->textsize(9);
  	  pTextBox2D->box(FL_THIN_UP_BOX);
  	  pTextBox2D->value("ResXF>");
#ifdef DEMO_VERSION  	
  	  pTextBox2D->deactivate();
#endif //DEMO_VERSION  	  	
  	
  	mpTVektor2E	= new	Fl_Value_Slider(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+195,190,20);
 	  mpTVektor2E->type(FL_HORIZONTAL);
 	  mpTVektor2E->step(1.0);
 	  mpTVektor2E->range(0,100);
 	  mpTVektor2E->value(0);
#ifdef DEMO_VERSION
      mpTVektor2E->deactivate();
#endif //DEMO_VERSION 		 		 		 		
 		 		
    Fl_Output* pTextBox2E = new  Fl_Output(LOOP_CONTROL_X+200,LOOP_CONTROL_Y+195,60,20);
   	  pTextBox2E->color(FL_GRAY);
  	  pTextBox2E->textfont(0);
  	  pTextBox2E->textsize(9);
  	  pTextBox2E->box(FL_THIN_UP_BOX);
  	  pTextBox2E->value("ResXF<");
#ifdef DEMO_VERSION  	
  	  pTextBox2E->deactivate();
#endif //DEMO_VERSION  	  	

    mpTVektor2F = new Fl_Value_Slider(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+215,190,20);
	  mpTVektor2F->type(FL_HORIZONTAL);
 	  mpTVektor2F->step(0.01);
 	  mpTVektor2F->range(0,10);
 	  mpTVektor2F->value(0);
#ifdef DEMO_VERSION
      mpTVektor2F->deactivate();
#endif //DEMO_VERSION 		 		 		 		
 		
    Fl_Output* pTextBox2F = new  Fl_Output(LOOP_CONTROL_X+200,LOOP_CONTROL_Y+215,60,20);
  	  pTextBox2F->color(FL_GRAY);
  	  pTextBox2F->textfont(0);
  	  pTextBox2F->textsize(9);
  	  pTextBox2F->box(FL_THIN_UP_BOX);
  	  pTextBox2F->value("A-ResVol");
#ifdef DEMO_VERSION  	
  	  pTextBox2F->deactivate();
#endif //DEMO_VERSION  	  	

    mpTVektor2G	= new	Fl_Value_Slider(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+235,190,20);
 	  mpTVektor2G->type(FL_HORIZONTAL);
 	  mpTVektor2G->step(0.01);
 	  mpTVektor2G->range(0,10);
 	  mpTVektor2G->value(0);
#ifdef DEMO_VERSION
      mpTVektor2G->deactivate();
#endif //DEMO_VERSION 		 		 		 		 		

    Fl_Output* pTextBox2G = new  Fl_Output(LOOP_CONTROL_X+200,LOOP_CONTROL_Y+235,60,20);
      pTextBox2G->color(FL_GRAY);
  	  pTextBox2G->textfont(0);
  	  pTextBox2G->textsize(9);
  	  pTextBox2G->box(FL_THIN_UP_BOX);
  	  pTextBox2G->value("S-ResVol");
#ifdef DEMO_VERSION  	
  	  pTextBox2G->deactivate();
#endif //DEMO_VERSION  	  	

  	// stat templ handling
  	Fl_Group* pStatTempControls = new Fl_Group(LOOP_CONTROL_X+280,LOOP_CONTROL_Y+55,80,200);
	  pStatTempControls->box(FL_THIN_UP_BOX);
      {
    	Fl_Output* pTextBox3A = new  Fl_Output(LOOP_CONTROL_X+290,LOOP_CONTROL_Y+60,60,20);
      	  pTextBox3A->color(FL_GRAY);
    	  pTextBox3A->textfont(0);
    	  pTextBox3A->textsize(9);
    	  pTextBox3A->box(FL_THIN_UP_BOX);
    	  pTextBox3A->value(" StatTemp");
    	
    	Fl_Button* o2 = new Fl_Button(LOOP_CONTROL_X+290,LOOP_CONTROL_Y+85,60,18,mpParams->GetStatTemplAttribute(0));
    	  o2->type(FL_RADIO_BUTTON);
    	  o2->labelfont(FL_HELVETICA);
    	  o2->labelsize(9);
          o2->callback((Fl_Callback*) sStatTmplChoose0,this);
#ifdef DEMO_VERSION  	
    	  o2->deactivate();
#endif //DEMO_VERSION  	  	    	

      Fl_Button* o3 = new Fl_Button(LOOP_CONTROL_X+290,LOOP_CONTROL_Y+105,60,18,mpParams->GetStatTemplAttribute(1));
        o3->type(FL_RADIO_BUTTON);
        o3->labelfont(FL_HELVETICA);
    	o3->labelsize(9);
        o3->callback((Fl_Callback*) sStatTmplChoose1,this);
#ifdef DEMO_VERSION  	
    	o3->deactivate();
#endif //DEMO_VERSION  	  	    	
     	
      Fl_Button* o4 = new Fl_Button(LOOP_CONTROL_X+290,LOOP_CONTROL_Y+125,60,18,mpParams->GetStatTemplAttribute(2));
        o4->type(FL_RADIO_BUTTON);
        o4->labelfont(FL_HELVETICA);
    	o4->labelsize(9);
        o4->callback((Fl_Callback*) sStatTmplChoose2,this);
#ifdef DEMO_VERSION  	
    	o4->deactivate();
#endif //DEMO_VERSION  	  	    	
     
      Fl_Button* o5 = new Fl_Button(LOOP_CONTROL_X+290,LOOP_CONTROL_Y+145,60,18,mpParams->GetStatTemplAttribute(3));
        o5->type(FL_RADIO_BUTTON);
        o5->labelfont(FL_HELVETICA);
    	o5->labelsize(9);
        o5->callback((Fl_Callback*) sStatTmplChoose3,this);
#ifdef DEMO_VERSION  	
    	o5->deactivate();
#endif //DEMO_VERSION  	  	    	

      Fl_Button* o6 = new Fl_Button(LOOP_CONTROL_X+290,LOOP_CONTROL_Y+165,60,18,mpParams->GetStatTemplAttribute(4));
    	o6->type(FL_RADIO_BUTTON);
    	o6->labelfont(FL_HELVETICA);
    	o6->labelsize(9);
        o6->callback((Fl_Callback*) sStatTmplChoose4,this);
#ifdef DEMO_VERSION  	
    	o6->deactivate();
#endif //DEMO_VERSION  	  	    	
     
      Fl_Button* o7 = new Fl_Button(LOOP_CONTROL_X+290,LOOP_CONTROL_Y+185,60,18,mpParams->GetStatTemplAttribute(5));
        o7->type(FL_RADIO_BUTTON);
        o7->labelfont(FL_HELVETICA);
    	o7->labelsize(9);
        o7->callback((Fl_Callback*) sStatTmplChoose5,this);
#ifdef DEMO_VERSION  	
    	o7->deactivate();
#endif //DEMO_VERSION  	  	    	
     	
      Fl_Button* o8 = new Fl_Button(LOOP_CONTROL_X+290,LOOP_CONTROL_Y+205,60,18,mpParams->GetStatTemplAttribute(6));
        o8->type(FL_RADIO_BUTTON);
        o8->labelfont(FL_HELVETICA);
    	o8->labelsize(9);
        o8->callback((Fl_Callback*) sStatTmplChoose6,this);
#ifdef DEMO_VERSION  	
    	o8->deactivate();
#endif //DEMO_VERSION  	  	    	
     
      Fl_Button* o9 = new Fl_Button(LOOP_CONTROL_X+290,LOOP_CONTROL_Y+225,60,18,mpParams->GetStatTemplAttribute(7));
        o9->type(FL_RADIO_BUTTON);
        o9->labelfont(FL_HELVETICA);
    	o9->labelsize(9);
        o9->callback((Fl_Callback*) sStatTmplChoose7,this);
#ifdef DEMO_VERSION  	
    	o9->deactivate();
#endif //DEMO_VERSION  	  	    	
    }
    pStatTempControls->end();
    
    //----------  TimbreVektorDisplay------------------------------//
    mpTVektorDisplay = new Fl_Group(LOOP_CONTROL_X+360,LOOP_CONTROL_Y+55,110,200);
  	mpTVektorDisplay->box(FL_THIN_UP_BOX);
    {
      Fl_Output* pTextBox3a1 = new  Fl_Output(LOOP_CONTROL_X+365,LOOP_CONTROL_Y+60,85,18);
        pTextBox3a1->color(FL_GRAY);
    	pTextBox3a1->textfont(0);
  	    pTextBox3a1->textsize(9);
  	    pTextBox3a1->box(FL_THIN_UP_BOX);
  	    pTextBox3a1->value("Timbre Vector");
     	
      mpVal_Output1a = new Fl_Value_Output(LOOP_CONTROL_X+365,LOOP_CONTROL_Y+85,40,18);
        mpVal_Output1a->textfont(0);
        mpVal_Output1a->textsize(9);
      
      Fl_Output* pTextBox3a = new  Fl_Output(LOOP_CONTROL_X+410,LOOP_CONTROL_Y+85,40,18);
  	    pTextBox3a->color(FL_GRAY);
  	    pTextBox3a->textfont(0);
  	    pTextBox3a->textsize(9);
  	    pTextBox3a->box(FL_THIN_UP_BOX);
  	    pTextBox3a->value("Pitch");
      
      mpVal_Output1b = new Fl_Value_Output(LOOP_CONTROL_X+365,LOOP_CONTROL_Y+105,40,18);
        mpVal_Output1b->textfont(0);
        mpVal_Output1b->textsize(9);
      
      Fl_Output* pTextBox3b = new  Fl_Output(LOOP_CONTROL_X+410,LOOP_CONTROL_Y+105,40,18);
  	    pTextBox3b->color(FL_GRAY);
  	    pTextBox3b->textfont(0);
  	    pTextBox3b->textsize(9);
  	    pTextBox3b->box(FL_THIN_UP_BOX);
  	    pTextBox3b->value("Attack");
    	
      mpVal_Output1c = new Fl_Value_Output(LOOP_CONTROL_X+365,LOOP_CONTROL_Y+125,40,18);
        mpVal_Output1c->textfont(0);
        mpVal_Output1c->textsize(9);

      Fl_Output* pTextBox3c = new  Fl_Output(LOOP_CONTROL_X+410,LOOP_CONTROL_Y+125,40,18);
  	    pTextBox3c->color(FL_GRAY);
  	    pTextBox3c->textfont(0);
  	    pTextBox3c->textsize(9);
  	    pTextBox3c->box(FL_THIN_UP_BOX);
  	    pTextBox3c->value("Level");
    	
      mpVal_Output1d = new Fl_Value_Output(LOOP_CONTROL_X+365,LOOP_CONTROL_Y+145,40,18);
        mpVal_Output1d->textfont(0);
        mpVal_Output1d->textsize(9);

      Fl_Output* pTextBox3d = new  Fl_Output(LOOP_CONTROL_X+410,LOOP_CONTROL_Y+145,40,18);
  	    pTextBox3d->color(FL_GRAY);
  	    pTextBox3d->textfont(0);
  	    pTextBox3d->textsize(9);
  	    pTextBox3d->box(FL_THIN_UP_BOX);
  	    pTextBox3d->value("IP_Factor");
    
      mpVal_Output1e = new Fl_Value_Output(LOOP_CONTROL_X+365,LOOP_CONTROL_Y+165,40,18);
        mpVal_Output1e->textfont(0);
        mpVal_Output1e->textsize(9);

      Fl_Output* pTextBox3e = new  Fl_Output(LOOP_CONTROL_X+410,LOOP_CONTROL_Y+165,40,18);
  	    pTextBox3e->color(FL_GRAY);
  	    pTextBox3e->textfont(0);
  	    pTextBox3e->textsize(9);
  	    pTextBox3e->box(FL_THIN_UP_BOX);
  	    pTextBox3e->value("T-Frames");
      
      mpVal_Output1f = new Fl_Value_Output(LOOP_CONTROL_X+365,LOOP_CONTROL_Y+185,40,18);
        mpVal_Output1f->textfont(0);
        mpVal_Output1f->textsize(9);

      Fl_Output* pTextBox3f = new  Fl_Output(LOOP_CONTROL_X+410,LOOP_CONTROL_Y+185,40,18);
  	    pTextBox3f->color(FL_GRAY);
  	    pTextBox3f->textfont(0);
  	    pTextBox3f->textsize(9);
  	    pTextBox3f->box(FL_THIN_UP_BOX);
  	    pTextBox3f->value("Sel-Attack");
      
      mpVal_Output1g = new Fl_Value_Output(LOOP_CONTROL_X+365,LOOP_CONTROL_Y+205,40,18);
        mpVal_Output1g->textfont(0);
        mpVal_Output1g->textsize(9);

      Fl_Output* pTextBox3g = new  Fl_Output(LOOP_CONTROL_X+410,LOOP_CONTROL_Y+205,40,18);
  	    pTextBox3g->color(FL_GRAY);
  	    pTextBox3g->textfont(0);
  	    pTextBox3g->textsize(9);
  	    pTextBox3g->box(FL_THIN_UP_BOX);
  	    pTextBox3g->value("Position");
      
      mpVal_Output1h = new Fl_Value_Output(LOOP_CONTROL_X+365,LOOP_CONTROL_Y+225,40,18);
        mpVal_Output1h->textfont(0);
        mpVal_Output1h->textsize(9);

      Fl_Output* pTextBox3h = new  Fl_Output(LOOP_CONTROL_X+410,LOOP_CONTROL_Y+225,40,18);
  	    pTextBox3h->color(FL_GRAY);
  	    pTextBox3h->textfont(0);
  	    pTextBox3h->textsize(9);
  	    pTextBox3h->box(FL_THIN_UP_BOX);
  	    pTextBox3h->value("Trans-Mode");
  	  
  	  mpVal_Output1i = new Fl_Value_Output(LOOP_CONTROL_X+365,LOOP_CONTROL_Y+245,40,18);
        mpVal_Output1i->textfont(0);
        mpVal_Output1i->textsize(9);
        mpVal_Output1i->hide();

      Fl_Output* pTextBox3i = new  Fl_Output(LOOP_CONTROL_X+410,LOOP_CONTROL_Y+245,40,18);
  	    pTextBox3i->color(FL_GRAY);
  	    pTextBox3i->textfont(0);
  	    pTextBox3i->textsize(9);
  	    pTextBox3i->box(FL_THIN_UP_BOX);
  	    pTextBox3i->value("T-Freq");
  	    pTextBox3i->hide();
    }	
    mpTVektorDisplay->end();
    
  	Fl_Button* o = new Fl_Button(LOOP_CONTROL_X+150,LOOP_CONTROL_Y+25,40,20,"Store");
  	  o->shortcut('s');
      o->callback((Fl_Callback*) sStoreLoopParams,this);
#ifdef DEMO_VERSION  	
  	  o->deactivate();
#endif //DEMO_VERSION  	  	
    
    Fl_Button* o1 = new Fl_Button(LOOP_CONTROL_X+190,LOOP_CONTROL_Y+25,40,20,"Load");
      o1->shortcut('l');
      o1->callback((Fl_Callback*) sLoadLoopParams,this);
#ifdef DEMO_VERSION  	
  	  o1->deactivate();
#endif //DEMO_VERSION  	  	  	
  }
  mpLoopControls->end();

//----------------------------------------------------------------------------//
  // TemplChooser
//----------------------------------------------------------------------------//  
  /*
  mpTmplChooser = new Fl_Group(LOOP_CONTROL_X,LOOP_CONTROL_Y,540,205);
  mpTmplChooser->box(FL_THIN_UP_BOX);
  {
    char*  name3="FileName";
	mpOutput3 = new Fl_Output(LOOP_CONTROL_X+150,LOOP_CONTROL_Y+15,110,20);
      mpOutput3->textfont(0);
	  mpOutput3->textsize(9);
      mpOutput3->value(name3);
      mpOutput3->align(FL_ALIGN_BOTTOM);	
     
    mpFrameNum	= new	Fl_Value_Slider(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+55,190,20);
      mpFrameNum->type(FL_HORIZONTAL);
  	  mpFrameNum->step(1.0);
  	  mpFrameNum->range(0,mpSaltoApp->GetnFramesInStatTmplSeg());
      mpFrameNum->callback((Fl_Callback*) sStatFrameFader,this);
    
  	Fl_Output* pTextBox3A = new  Fl_Output(LOOP_CONTROL_X+200,LOOP_CONTROL_Y+55,60,20);
      pTextBox3A->color(FL_GRAY);
      pTextBox3A->textfont(0);
  	  pTextBox3A->textsize(9);
  	  pTextBox3A->box(FL_THIN_UP_BOX);
  	  pTextBox3A->value("StatTemplate");
  	  	
    Fl_Button* o1 = new Fl_Button(LOOP_CONTROL_X+50,LOOP_CONTROL_Y+180,40,20,"Load");
      o1->callback((Fl_Callback*) sLoadSDIFToEdit,this);
	  
    Fl_Button* o = new Fl_Button(LOOP_CONTROL_X+5,LOOP_CONTROL_Y+180,40,20,"Store");
      o->callback((Fl_Callback*) sStoreStationaryTmpl,this);
	
	Fl_Button* o3 = new Fl_Button(LOOP_CONTROL_X+95,LOOP_CONTROL_Y+180,40,20,"Add");
      o3->callback((Fl_Callback*) sAddStationaryTmpl,this);

  	Fl_Button* o2 = new Fl_Button(LOOP_CONTROL_X+435,LOOP_CONTROL_Y+180,95,20,"LoopFrame");
      o2->type(FL_TOGGLE_BUTTON);
      o2->selection_color(1);
      o2->callback((Fl_Callback*) sPlayFrameOnly,this);
  
    Fl_Button* o4 = new Fl_Button(LOOP_CONTROL_X+300,LOOP_CONTROL_Y+55,40,20,mpParams->GetStatTemplAttribute(0));
	  o4->labelfont(FL_HELVETICA);
      o4->labelsize(9);
      o4->callback((Fl_Callback*) sAddStationaryTmpl0,this);

    Fl_Button* o5 = new Fl_Button(LOOP_CONTROL_X+350,LOOP_CONTROL_Y+55,40,20,mpParams->GetStatTemplAttribute(1));
      o5->labelfont(FL_HELVETICA);
      o5->labelsize(9);
      o5->callback((Fl_Callback*) sAddStationaryTmpl1,this);

    Fl_Button* o6 = new Fl_Button(LOOP_CONTROL_X+400,LOOP_CONTROL_Y+55,40,20,mpParams->GetStatTemplAttribute(2));
      o6->labelfont(FL_HELVETICA);
      o6->labelsize(9);
      o6->callback((Fl_Callback*) sAddStationaryTmpl2,this);

    Fl_Button* o7 = new Fl_Button(LOOP_CONTROL_X+450,LOOP_CONTROL_Y+55,40,20,mpParams->GetStatTemplAttribute(3));
      o7->labelfont(FL_HELVETICA);
      o7->labelsize(9);
      o7->callback((Fl_Callback*) sAddStationaryTmpl3,this);
   
    Fl_Button* o8 = new Fl_Button(LOOP_CONTROL_X+300,LOOP_CONTROL_Y+85,40,20,mpParams->GetStatTemplAttribute(4));
	  o8->labelfont(FL_HELVETICA);
      o8->labelsize(9);
      o8->callback((Fl_Callback*) sAddStationaryTmpl4,this);

    Fl_Button* o9 = new Fl_Button(LOOP_CONTROL_X+350,LOOP_CONTROL_Y+85,40,20,mpParams->GetStatTemplAttribute(5));
      o9->labelfont(FL_HELVETICA);
      o9->labelsize(9);
      o9->callback((Fl_Callback*) sAddStationaryTmpl5,this);

    Fl_Button* o10 = new Fl_Button(LOOP_CONTROL_X+400,LOOP_CONTROL_Y+85,40,20,mpParams->GetStatTemplAttribute(6));
      o10->labelfont(FL_HELVETICA);
      o10->labelsize(9);
      o10->callback((Fl_Callback*) sAddStationaryTmpl6,this);

    Fl_Button* o11 = new Fl_Button(LOOP_CONTROL_X+450,LOOP_CONTROL_Y+85,40,20,mpParams->GetStatTemplAttribute(7));
      o11->labelfont(FL_HELVETICA);
      o11->labelsize(9);
      o11->callback((Fl_Callback*) sAddStationaryTmpl7,this);
  
  }
  mpTmplChooser->end();
  mpTmplChooser->hide();
*/  	
  // OSCILLOSCOPE REFRESH TIMER
  Fl::add_timeout(OSCILLOSCOPE_REFRESH_TIME,sTimer,this);
	
  // test faders
//  Fl_Value_Slider* o = new Fl_Value_Slider(TEST_FADERS_X,TEST_FADERS_Y,190,20,"StatResVolume[dB]");
  Fl_Dial* o = new Fl_Dial(TEST_FADERS_X+20,TEST_FADERS_Y,30,30,"StatResVolume[dB]");
//    o->type(FL_HORIZONTAL);
    o->labelfont(FL_HELVETICA);
    o->labelsize(8);
    o->step(1.0);
    o->range(-10,10);
 	o->value(mpParams->GetStatResGain());
    o->callback((Fl_Callback*) sStatResGain,this);
  
//  Fl_Value_Slider* o1a = new Fl_Value_Slider(TEST_FADERS_X,TEST_FADERS_Y+20,190,20,"StatResonance");
  Fl_Dial* o1a = new Fl_Dial(TEST_FADERS_X+90,TEST_FADERS_Y,30,30,"StatResonance");
//    o1a->type(FL_HORIZONTAL);
    o1a->labelfont(FL_HELVETICA);
    o1a->labelsize(8);
    o1a->step(1);
    o1a->range(0,50);
 	o1a->value(5);
    o1a->callback((Fl_Callback*) sStatResonanceFreq,this);
#ifdef DEMO_VERSION
   	o1a->deactivate();
#endif //DEMO_VERSION 		
	
//  Fl_Value_Slider* o1 = new Fl_Value_Slider(TEST_FADERS_X,TEST_FADERS_Y+40,190,20,"Gain[dB]");
  Fl_Dial* o1 = new Fl_Dial(TEST_FADERS_X+150,TEST_FADERS_Y,30,30,"Gain[dB]");
//    o1->type(FL_HORIZONTAL);
    o1->labelfont(FL_HELVETICA);
    o1->labelsize(8);
    o1->step(1);
    o1->range(-10,10);
// 	o1->value(mpParams->GetGain());
    o1->callback((Fl_Callback*) sSetGain,this);
  
  Fl_Value_Slider* o2 = new Fl_Value_Slider(TEST_FADERS_X,TEST_FADERS_Y+60,190,20,"RandRange");
    o2->type(FL_HORIZONTAL);
    o2->labelfont(FL_HELVETICA);
    o2->labelsize(8);
    o2->step(0.02);
    o2->range(1,100);
 	o2->value(mpParams->GetRandomRange());
    o2->callback((Fl_Callback*) sSetTestSlider,this);
#ifdef DEMO_VERSION
   	o2->deactivate();
#endif //DEMO_VERSION 		
  
  // test button
  Fl_Button* oTest = new Fl_Button(TEST_FADERS_X-65 ,TEST_FADERS_Y,58,18,"FILE");
    oTest->selection_color(1);
    oTest->type(FL_TOGGLE_BUTTON);
	oTest->labelfont(FL_HELVETICA);
    oTest->labelsize(12);
    oTest->callback((Fl_Callback*) sSetTestButton,this);
#ifdef DEMO_VERSION
   	oTest->deactivate();
#endif //DEMO_VERSION 		

  // test button 1
  Fl_Button* oTest1 = new Fl_Button(TEST_FADERS_X-65 ,TEST_FADERS_Y+20,58,18,"MIDI-KB");
	oTest1->selection_color(1);
    oTest1->type(FL_TOGGLE_BUTTON);
	oTest1->labelfont(FL_HELVETICA);
    oTest1->labelsize(12);
    oTest1->callback((Fl_Callback*) sSetTestButton1,this);
#ifdef DEMO_VERSION
   	oTest1->deactivate();
#endif //DEMO_VERSION 		
  
  // test button 2 
  Fl_Button* oTest2 = new Fl_Button(TEST_FADERS_X-65 ,TEST_FADERS_Y+40,58,18,"C-Pitch");
	oTest2->selection_color(1);
    oTest2->type(FL_TOGGLE_BUTTON);
    oTest2->labelfont(FL_HELVETICA);
    oTest2->labelsize(12);
    oTest2->callback((Fl_Callback*) sSetTestButton2,this);
//  oTest2->value(mpParams->GetUsePitchCorrection());
#ifdef DEMO_VERSION
   	oTest2->deactivate();
#endif //DEMO_VERSION 		
	
	// test button 3 
  Fl_Button* oTest3 = new Fl_Button(TEST_FADERS_X-65 ,TEST_FADERS_Y+60,58,18,"Random");
	oTest3->selection_color(1);
    oTest3->type(FL_TOGGLE_BUTTON);
    oTest3->labelfont(FL_HELVETICA);
    oTest3->labelsize(12);
    oTest3->callback((Fl_Callback*) sSetTestButton3,this);
    oTest3->value(mpParams->GetUseRandomDeviations());
#ifdef DEMO_VERSION
   	oTest3->deactivate();
#endif //DEMO_VERSION 		
	
	// test button 4 
  Fl_Button* oTest4 = new Fl_Button(TEST_FADERS_X-125,TEST_FADERS_Y,58,18,"Tpose");
	oTest4->selection_color(1);
    oTest4->type(FL_TOGGLE_BUTTON);
    oTest4->labelfont(FL_HELVETICA);
    oTest4->labelsize(12);
    oTest4->callback((Fl_Callback*) sSetTestButton4,this);
    oTest4->value(mpParams->GetTranspose());

  // test button 5 
  //Fl_Button* oTest5 = new Fl_Button(TEST_FADERS_X-125,TEST_FADERS_Y+20,58,18,"KB-Mode");
	Fl_Button* oTest5 = new Fl_Button(TEST_FADERS_X-125,TEST_FADERS_Y+20,58,18,"Br-Ctrl");
	oTest5->selection_color(1);
    oTest5->type(FL_TOGGLE_BUTTON);
    oTest5->labelfont(FL_HELVETICA);
    oTest5->labelsize(12);
    oTest5->callback((Fl_Callback*) sSetTestButton5,this);
    oTest5->value(mpParams->GetUseBreathController());
  
  // test button 6 
  Fl_Button* oTest6 = new Fl_Button(TEST_FADERS_X-125,TEST_FADERS_Y+40,58,18,"Ran-Loop");
	oTest6->selection_color(1);
    oTest6->type(FL_TOGGLE_BUTTON);
    oTest6->labelfont(FL_HELVETICA);
    oTest6->labelsize(12);
    oTest6->callback((Fl_Callback*) sSetTestButton6,this);
    oTest6->value(mpParams->GetUseRandomLoop());
	
	mpEditorWindow->end();
	mpEditorWindow->show();
  
  // init the fader values for default position
  LoadLoopValues(0);
  mpParams->SetAttackTimbrePitch(0);
  mpParams->SetAttackTimbreAttack(0); 
  
}

CSaltoEditor::~CSaltoEditor()
{
	if(mpEditorWindow)
	{
		delete	mpEditorWindow;
		mpEditorWindow=NULL;
	}
}

//#pragma mark-
//----------------------------------------------------------------------------//
void CSaltoEditor::DisplayBufferOfDouble(TData *array,TSize numElements)
{
	mpWidgetWave->size = numElements;
	mpWidgetWave->ptr  = array;
	mpWidgetWave->redraw();
}
//----------------------------------------------------------------------------//
void CSaltoEditor::DisplayTmplArrayOfDouble(const DataArray &displayArray)
{
	mpWidgetWave->size = displayArray.Size();
	mpWidgetWave->ptr  = (TData*)displayArray.GetPtr();
	mpWidgetWave->redraw();
}

//----------------------------------------------------------------------------//
void CSaltoEditor::DisplayPeakArray(const SpectralPeakArray& peakArray/*Par*/) //pasa com copia i no petara!!
{
  //fer copia del peakArray to avoid inconsistences in the size, in case that to theads are accessing the same peakArray
//	SpectralPeakArray* peakArray = new SpectralPeakArray();
//	*peakArray = *peakArrayPar;

	TSize spectralFrameSize =  mSpectralFrameSize;
	double sampleFrequency = mSamplingRate;
	int i;
	
	Array<float> displayPeakArray(spectralFrameSize);
	displayPeakArray.Resize(spectralFrameSize); //BUG??
	displayPeakArray.SetSize(spectralFrameSize);
	
	// clear Array
	for (i=0;i<spectralFrameSize;i++)
	{
	  displayPeakArray[i]=-100; // NOT DISPLAYED....
	}

//  SpectralPeak  tempPeak;
  TIndex    binPos;
  float    logMag,mag;

	TSize nPeaks = peakArray.GetnPeaks();
	// set peaks in display array
	for (i=0;i<nPeaks;i++)  //BUG this chashes because nPeaks > peakArray.GetFreqBuffer().Size()
	{
		//std::cout << "nPeaks= " << nPeaks << " Freqsize: " << peakArray->GetFreqBuffer().Size() << " Magsize: " << peakArray->GetMagBuffer().Size() << " i= " << i << "\n";
		binPos = (TIndex)peakArray.GetFreqBuffer()[i]/sampleFrequency*2*spectralFrameSize;
		mag = peakArray.GetMagBuffer()[i];
		if (mag>0)
			logMag = 20*log10(mag);
		else
			logMag = -100;
		
		if (binPos>=0 && binPos<spectralFrameSize) // security
			displayPeakArray[binPos] = logMag;
	}

	mpWidgetPeaks->size = spectralFrameSize;
//  std::cout << "\ndisplayPeakArray: " <<	displayPeakArray;
//	mpWidgetPeaks->ptr = displayPeakArray.GetPtr(); //should be a copy
	mpWidgetPeaks->SetPtr(displayPeakArray);

	mpWidgetPeaks->redraw();
}

//----------------------------------------------------------------------------//


void CSaltoEditor::DisplayAttackResidual(const Spectrum& spec/*Par*/)
{
  //fer copia del spectrum to avoid inconsistences in the size, in case that to theads are accessing the same spectrum
//	Spectrum* spec = new Spectrum();
//	*spec = *specPar;

	TSize spectrumSize = spec.GetComplexArray().Size();

//	TSize spectrumSize = 511;
//  std::cout << "spectrumSize " << spectrumSize << "\n";

	Array<float> displaySpecArray(spectrumSize);
	displaySpecArray.Resize(spectrumSize);
	displaySpecArray.SetSize(spectrumSize);
	
	// clear Array
	for (int i=0;i<spectrumSize;i++)
	{
//	  displaySpecArray[i]=spec->GetComplexArray()[i].Real(); // NOT DISPLAYED....
		displaySpecArray[i]= 20.0*log10( sqrt(spec.GetComplexArray()[i].Real()*spec.GetComplexArray()[i].Real() +
																					spec.GetComplexArray()[i].Imag()*spec.GetComplexArray()[i].Imag())
																	 );
	}

	mpWidgetAttackResidual->size = spectrumSize;	
//  std::cout << "\ndisplaySpecArray: " << displaySpecArray;
//	mpWidgetResidual->ptr = displaySpecArray.GetPtr(); //should be a copy

	mpWidgetAttackResidual->SetPtr(displaySpecArray);

	mpWidgetAttackResidual->redraw();
}

void CSaltoEditor::DisplaySynthesizedSpectrum(const Spectrum& spec/*Par*/)
{
  //fer copia del spectrum to avoid inconsistences in the size, in case that to theads are accessing the same spectrum
//	Spectrum* spec = new Spectrum();
//	*spec = *specPar;

	TSize spectrumSize = spec.GetComplexArray().Size();

//	TSize spectrumSize = 511;
//  std::cout << "spectrumTestSize " << spectrumSize << "\n";

	Array<float> displayTestSpecArray(spectrumSize);
	displayTestSpecArray.Resize(spectrumSize);
	displayTestSpecArray.SetSize(spectrumSize);
	
	// clear Array
	for (int i=0;i<spectrumSize;i++)
	{
//	  displaySpecArray[i]=spec->GetComplexArray()[i].Real(); // NOT DISPLAYED....
		displayTestSpecArray[i]= 20.0*log10( sqrt(spec.GetComplexArray()[i].Real()*spec.GetComplexArray()[i].Real() +
																					    spec.GetComplexArray()[i].Imag()*spec.GetComplexArray()[i].Imag())
																	     );
	}

	mpWidgetSynthesizedSpectrum->size = spectrumSize;	
//  std::cout << "\ndisplaySpecArray: " << displaySpecArray;
//	mpWidgetResidual->ptr = displaySpecArray.GetPtr(); //should be a copy

	mpWidgetSynthesizedSpectrum->SetPtr(displayTestSpecArray);

	mpWidgetSynthesizedSpectrum->redraw();
}

//----------------------------------------------------------------------------//
void CSaltoEditor::FillDisplayPeakArray(const SpectralPeakArray& peakArray) //pasa com copia i no petara!!
{
  //fer copia del peakArray to avoid inconsistences in the size, in case that to theads are accessing the same peakArray
//	SpectralPeakArray* peakArray = new SpectralPeakArray();
//	*peakArray = *peakArrayPar;

	TSize spectralFrameSize =  mSpectralFrameSize;
	double sampleFrequency = mSamplingRate;
	int i;
	
	Array<float> displayPeakArray(spectralFrameSize);
	displayPeakArray.Resize(spectralFrameSize); //BUG??
	displayPeakArray.SetSize(spectralFrameSize);
		
	// clear Array
	for (i=0;i<spectralFrameSize;i++)
	{
	  displayPeakArray[i]=-100; // NOT DISPLAYED....
	}

//  SpectralPeak  tempPeak;
  TIndex    binPos;
  float    logMag,mag;

	TSize nPeaks = peakArray.GetnPeaks();
	// set peaks in display array
	for (i=0;i<nPeaks;i++)  //BUG this chashes because nPeaks > peakArray.GetFreqBuffer().Size()
	{
//std::cout << "nPeaks= " << nPeaks << " Freqsize: " << peakArray->GetFreqBuffer().Size() << " Magsize: " << peakArray->GetMagBuffer().Size() << " i= " << i << "\n";
			binPos = (TIndex)peakArray.GetFreqBuffer()[i]/sampleFrequency*2*spectralFrameSize;
      mag = peakArray.GetMagBuffer()[i];
      if (mag>0)
        logMag = 20*log10(mag);
      else
        logMag = -100;

      if (binPos>=0 && binPos<spectralFrameSize) // security
          displayPeakArray[binPos] = logMag;
	}

	mpWidgetPeaks->size = spectralFrameSize;
//  std::cout << "\ndisplayPeakArray: " <<	displayPeakArray;
//	mpWidgetPeaks->ptr = displayPeakArray.GetPtr(); //should be a copy
	mpWidgetPeaks->SetPtr(displayPeakArray);

}

//----------------------------------------------------------------------------//

void CSaltoEditor::FillDisplayAttackResidual(const Spectrum& spec/*Par*/)
{
  //fer copia del spectrum to avoid inconsistences in the size, in case that to theads are accessing the same spectrum
//	Spectrum* spec = new Spectrum();
//	*spec = *specPar;

	TSize spectrumSize = spec.GetComplexArray().Size();

//	TSize spectrumSize = 511;
//  std::cout << "spectrumSize " << spectrumSize << "\n";

	Array<float> displaySpecArray(spectrumSize);
	displaySpecArray.Resize(spectrumSize);
	displaySpecArray.SetSize(spectrumSize);
	
	// clear Array
	for (int i=0;i<spectrumSize;i++)
	{
//	  displaySpecArray[i]=spec->GetComplexArray()[i].Real(); // NOT DISPLAYED....
		displaySpecArray[i]= 20.0*log10( sqrt(spec.GetComplexArray()[i].Real()*spec.GetComplexArray()[i].Real() +
																					spec.GetComplexArray()[i].Imag()*spec.GetComplexArray()[i].Imag())
																		);
	}

	mpWidgetAttackResidual->size = spectrumSize;	
//  std::cout << "\ndisplaySpecArray: " << displaySpecArray;
//	mpWidgetResidual->ptr = displaySpecArray.GetPtr(); //should be a copy

	mpWidgetAttackResidual->SetPtr(displaySpecArray);

}

void CSaltoEditor::FillDisplayStationaryResidual(const Spectrum& spec/*Par*/)
{
  //fer copia del spectrum to avoid inconsistences in the size, in case that to theads are accessing the same spectrum
//	Spectrum* spec = new Spectrum();
//	*spec = *specPar;

	TSize spectrumSize = spec.GetComplexArray().Size();

//	TSize spectrumSize = 511;
//  std::cout << "spectrumSize " << spectrumSize << "\n";

	Array<float> displaySpecArray(spectrumSize);
	displaySpecArray.Resize(spectrumSize);
	displaySpecArray.SetSize(spectrumSize);
	
	// clear Array
	for (int i=0;i<spectrumSize;i++)
	{
//	  displaySpecArray[i]=spec->GetComplexArray()[i].Real(); // NOT DISPLAYED....
		displaySpecArray[i]= 20.0*log10( sqrt(spec.GetComplexArray()[i].Real()*spec.GetComplexArray()[i].Real() +
																					spec.GetComplexArray()[i].Imag()*spec.GetComplexArray()[i].Imag())
																		);
	}

	mpWidgetStationaryResidual->size = spectrumSize;	
//  std::cout << "\ndisplaySpecArray: " << displaySpecArray;
//	mpWidgetResidual->ptr = displaySpecArray.GetPtr(); //should be a copy

	mpWidgetStationaryResidual->SetPtr(displaySpecArray);

}

void CSaltoEditor::FillDisplaySynthesizedSpectrum(const Spectrum& spec/*Par*/)
{
  //fer copia del spectrum to avoid inconsistences in the size, in case that to theads are accessing the same spectrum
//	Spectrum* spec = new Spectrum();
//	*spec = *specPar;

	TSize spectrumSize = spec.GetComplexArray().Size();

//	TSize spectrumSize = 511;
//  std::cout << "spectrumTestSize " << spectrumSize << "\n";

	Array<float> displaySynthSpecArray(spectrumSize);
	displaySynthSpecArray.Resize(spectrumSize);
	displaySynthSpecArray.SetSize(spectrumSize);
	
	// clear Array
	for (int i=0;i<spectrumSize;i++)
	{
  //	  displaySpecArray[i]=spec->GetComplexArray()[i].Real(); // NOT DISPLAYED....
  		displaySynthSpecArray[i]= 20.0*log10( sqrt(spec.GetComplexArray()[i].Real()*spec.GetComplexArray()[i].Real() +
  																					     spec.GetComplexArray()[i].Imag()*spec.GetComplexArray()[i].Imag())
  																				);
	}

	mpWidgetSynthesizedSpectrum->size = spectrumSize;	
//  std::cout << "\ndisplaySpecArray: " << displaySpecArray;
//	mpWidgetResidual->ptr = displaySpecArray.GetPtr(); //should be a copy

	mpWidgetSynthesizedSpectrum->SetPtr(displaySynthSpecArray);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSaltoEditor::DisplayPeakArray()
{
	mpWidgetPeaks->redraw();
}

void CSaltoEditor::DisplayAttackResidual()
{
	mpWidgetAttackResidual->redraw();
}

void CSaltoEditor::DisplayStationaryResidual(const Spectrum& spec)
{
	//XA:this widget does nothing! mpWidgetStationaryResidual->redraw();
}

void CSaltoEditor::DisplayStationaryResidual()
{
	//XA:this widget does nothing! mpWidgetStationaryResidual->redraw();
}


void CSaltoEditor::DisplaySynthesizedSpectrum()
{
	mpWidgetSynthesizedSpectrum->redraw();
}


//----------------------------------------------------------------------------//
//#pragma mark-
void CSaltoEditor::LoadLoopValues (TIndex pos)
{
  CSaltoSegData dataSegment;
  dataSegment = mpParams->GetSegDataByPos(pos);
  
  TIndex segLength = dataSegment.GetSegLength();  // is set in DataManagment..

  mpTVektor1A->range(0,segLength); // set fader range
  mpTVektor1A->value(dataSegment.GetPlayBackStart());
  mpTVektor1B->range(-10,10);
  mpTVektor1B->value(dataSegment.GetIndividualVolume());
  mpTVektor1C->range(0,segLength);
  mpTVektor1C->value(dataSegment.GetLoopStart());
  mpTVektor2A->range(0,segLength);
  mpTVektor2A->value(dataSegment.GetLoopEnd());
  mpTVektor2B->range(0,segLength);
  mpTVektor2B->value(dataSegment.GetInterpolStart());
  mpTVektor2C->range(0,segLength);
  mpTVektor2C->value(dataSegment.GetInterpolEnd());          
  mpTVektor2D->range(0,segLength);
  mpTVektor2D->value(dataSegment.GetResFadeStart());          
  mpTVektor2E->range(0,segLength);
  mpTVektor2E->value(dataSegment.GetResFadeEnd());          
  mpTVektor2F->range(0,10);
  mpTVektor2F->value(dataSegment.GetAttackResVolume());          
  mpTVektor2G->range(0,10);
  mpTVektor2G->value(dataSegment.GetStatResVolume());          

  mpLoopControls->redraw();
}

//----------------------------------------------------------------------------//

void
CSaltoEditor::StoreLoopValues(TIndex pos)
{
  CSaltoSegData dataSegment;
  // update Segment Length
  dataSegment.SetSegLength((TIndex)mpTVektor1A->maximum());
  
  // read in Current Values
  dataSegment.SetPlayBackStart    ((TIndex)mpTVektor1A->value());
  dataSegment.SetIndividualVolume ((TIndex)mpTVektor1B->value());
  dataSegment.SetLoopStart        ((TIndex)mpTVektor1C->value());
  dataSegment.SetLoopEnd          ((TIndex)mpTVektor2A->value());
  dataSegment.SetInterpolStart    ((TIndex)mpTVektor2B->value());
  dataSegment.SetInterpolEnd      ((TIndex)mpTVektor2C->value());
  dataSegment.SetResFadeStart     ((TIndex)mpTVektor2D->value());
  dataSegment.SetResFadeEnd       ((TIndex)mpTVektor2E->value());
  dataSegment.SetAttackResVolume  ((TData)mpTVektor2F->value());
  dataSegment.SetStatResVolume    ((TData)mpTVektor2G->value());
  
  mpParams->SetSegDataByPos(pos,dataSegment);
}

//----------------------------------------------------------------------------//
void CSaltoEditor::LoadSDIFToEdit()
{
  char *p;
  p = fl_file_chooser("Pick a SDIF file",0,0);  // open file chooser
  
//  mpSaltoApp->SetSDIFToEdit(p);           // load SDIF for editing stat tmpl 
//  mpFrameNum->range(0,mpSaltoApp->GetnFramesInStatTmplSeg());
  mpOutput3->value(p);    // update filename display
}
//----------------------------------------------------------------------------//
void
CSaltoEditor::AddStationaryTmplToDataBase(TIndex pos)
{
  CSaltoStatTmplData* newElem=NULL;
  newElem = new CSaltoStatTmplData;
  newElem->SetStatPeakArray(*mpParams->GetStatPeakArrayPtr());
  newElem->SetFundFreq(mpParams->GetStatFundFreq());
//  mpSaltoApp->AddStatTmplToDatabase(*newElem,pos);
  
  // THIS HAS TO CHANGE , WHERE D WE ALLOCATE MEMORY FOR THIS
  // change the whole stat template memory organization !!
  // as soon as we can store template arrays of template arrays... ´¿
}
//----------------------------------------------------------------------------//

void CSaltoEditor::SetDisplayParameters( TData sampRate, TSize spectralFrameSize )
{
	mSamplingRate = sampRate;
	mSpectralFrameSize = spectralFrameSize;
}

}
