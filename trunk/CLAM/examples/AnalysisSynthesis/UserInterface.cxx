// generated by Fast Light User Interface Designer (fluid) version 1.0011

#include "UserInterface.hxx"
#include "SMSAnalysisSynthesisConfig.hxx"

inline void Configuration::cb_mWindow_i(Fl_Window*, void*) {
  delete mWindow;
}
void Configuration::cb_mWindow(Fl_Window* o, void* v) {
  ((Configuration*)(o->user_data()))->cb_mWindow_i(o,v);
}

inline void Configuration::cb_Hop_i(Fl_Value_Input* o, void*) {
  mConfig->SetAnalysisHopSize(o->value());
}
void Configuration::cb_Hop(Fl_Value_Input* o, void* v) {
  ((Configuration*)(o->parent()->user_data()))->cb_Hop_i(o,v);
}

inline void Configuration::cb_Analysis_i(Fl_Choice* o, void*) {
  printf("Value= %d\n",atoi(o->text()));
mConfig->SetAnalysisWindowSize(o->value()*512);
}
void Configuration::cb_Analysis(Fl_Choice* o, void* v) {
  ((Configuration*)(o->parent()->user_data()))->cb_Analysis_i(o,v);
}

Fl_Menu_Item Configuration::menu_Analysis[] = {
 {"512", 0,  0, 0, 0, 0, 0, 12, 0},
 {"1024", 0,  0, 0, 0, 0, 0, 12, 0},
 {"2048", 0,  0, 0, 0, 0, 0, 12, 0},
 {"4096", 0,  0, 0, 0, 0, 0, 12, 0},
 {"8192", 0,  0, 0, 0, 0, 0, 12, 0},
 {0}
};

Fl_Menu_Item Configuration::menu_Analysis1[] = {
 {"Kaiser-Bessel 17", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 18", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 19", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 20", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 25", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 30", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 35", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 74", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 62", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 70", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 92", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Hamming", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Triangular", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 92 Trans Main Lobe", 0,  0, 0, 0, 0, 0, 12, 0},
 {0}
};

inline void Configuration::cb_Residual_i(Fl_Choice* o, void*) {
  printf("Value= %d\n",atoi(o->text()));
mConfig->SetAnalysisWindowSize(o->value()*512);
}
void Configuration::cb_Residual(Fl_Choice* o, void* v) {
  ((Configuration*)(o->parent()->user_data()))->cb_Residual_i(o,v);
}

Fl_Menu_Item Configuration::menu_Residual[] = {
 {"512", 0,  0, 0, 0, 0, 0, 12, 0},
 {"1024", 0,  0, 0, 0, 0, 0, 12, 0},
 {"2048", 0,  0, 0, 0, 0, 0, 12, 0},
 {"4096", 0,  0, 0, 0, 0, 0, 12, 0},
 {"8192", 0,  0, 0, 0, 0, 0, 12, 0},
 {0}
};

Fl_Menu_Item Configuration::menu_Residual1[] = {
 {"Kaiser-Bessel 17", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 18", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 19", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 20", 0,  0, 0, 0, 0, 0, 13, 0},
 {"Kaiser-Bessel 25", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 30", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 35", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 74", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 62", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 70", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 92", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Hamming", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Triangular", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 92 Trans Main Lobe", 0,  0, 0, 0, 0, 0, 12, 0},
 {0}
};

inline void Configuration::cb_Window_i(Fl_Choice* o, void*) {
  printf("Value= %d\n",atoi(o->text()));
mConfig->SetAnalysisWindowSize(o->value()*512);
}
void Configuration::cb_Window(Fl_Choice* o, void* v) {
  ((Configuration*)(o->parent()->user_data()))->cb_Window_i(o,v);
}

Fl_Menu_Item Configuration::menu_Window[] = {
 {"512", 0,  0, 0, 0, 0, 0, 12, 0},
 {"1024", 0,  0, 0, 0, 0, 0, 12, 0},
 {"2048", 0,  0, 0, 0, 0, 0, 12, 0},
 {"4096", 0,  0, 0, 0, 0, 0, 12, 0},
 {"8192", 0,  0, 0, 0, 0, 0, 12, 0},
 {0}
};

Fl_Menu_Item Configuration::menu_Window1[] = {
 {"Kaiser-Bessel 17", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 18", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 19", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 20", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 25", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 30", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Kaiser-Bessel 35", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 74", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 62", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 70", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 92", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Hamming", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Triangular", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Blackman-Harris 92 Trans Main Lobe", 0,  0, 0, 0, 0, 0, 12, 0},
 {0}
};

inline void Configuration::cb_Hop1_i(Fl_Value_Input* o, void*) {
  mConfig->SetAnalysisHopSize(o->value());
}
void Configuration::cb_Hop1(Fl_Value_Input* o, void* v) {
  ((Configuration*)(o->parent()->user_data()))->cb_Hop1_i(o,v);
}

inline void Configuration::cb_Zero_i(Fl_Value_Input* o, void*) {
  mConfig->SetAnalysisHopSize(o->value());
}
void Configuration::cb_Zero(Fl_Value_Input* o, void* v) {
  ((Configuration*)(o->parent()->user_data()))->cb_Zero_i(o,v);
}

Fl_Menu_Item Configuration::menu_Phase[] = {
 {"Align", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Random", 0,  0, 0, 0, 0, 0, 12, 0},
 {"Continuation", 0,  0, 0, 0, 0, 0, 12, 0},
 {0}
};

Configuration::Configuration(CLAM::SMSAnalysisSynthesisConfig* config) {
  Fl_Window* w;
  mConfig = config;
  { Fl_Window* o = mWindow = new Fl_Window(536, 740, "Configure the Analysis/Synthesis parameters");
    w = o;
    o->color(16);
    o->callback((Fl_Callback*)cb_mWindow, (void*)(this));
    { Fl_Box* o = new Fl_Box(10, 15, 515, 115);
      o->box(FL_SHADOW_BOX);
      o->color(16);
      o->labeltype(FL_NO_LABEL);
    }
    { Fl_Box* o = new Fl_Box(15, 5, 120, 20, "Input & Output Files");
      o->box(FL_FLAT_BOX);
      o->color(16);
      o->labelsize(12);
      o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    }
    { Fl_Input* o = mFileInName = new Fl_Input(115, 25, 355, 20, "Input Sound File:");
      o->labelsize(12);
      o->textsize(12);
    }
    { Fl_Input* o = mFileOutName = new Fl_Input(125, 50, 345, 20, "Output Sound File:");
      o->labelsize(12);
      o->textsize(12);
    }
    { Fl_Input* o = mXMLInName = new Fl_Input(105, 75, 365, 20, "Input XML File:");
      o->labelsize(12);
      o->textsize(12);
    }
    { Fl_Input* o = mXMLOutName = new Fl_Input(115, 100, 355, 20, "Output XML File:");
      o->labelsize(12);
      o->textsize(12);
    }
    { Fl_Box* o = new Fl_Box(10, 145, 515, 325);
      o->box(FL_SHADOW_BOX);
      o->color(16);
      o->labeltype(FL_NO_LABEL);
    }
    { Fl_Box* o = new Fl_Box(15, 135, 120, 20, "Analysis Parameters");
      o->box(FL_FLAT_BOX);
      o->color(16);
      o->labelsize(12);
      o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    }
    { Fl_Value_Input* o = new Fl_Value_Input(75, 240, 80, 20, "Hop size:");
      o->labelsize(12);
      o->textsize(12);
      o->callback((Fl_Callback*)cb_Hop);
    }
    { Fl_Choice* o = new Fl_Choice(20, 170, 235, 20, "Analysis Window Size:");
      o->labelsize(12);
      o->textsize(12);
      o->callback((Fl_Callback*)cb_Analysis);
      o->align(FL_ALIGN_TOP_LEFT);
      o->menu(menu_Analysis);
    }
    { Fl_Choice* o = new Fl_Choice(20, 210, 235, 20, "Analysis Window Type:");
      o->labelsize(12);
      o->textsize(12);
      o->align(FL_ALIGN_TOP_LEFT);
      o->menu(menu_Analysis1);
    }
    { Fl_Choice* o = new Fl_Choice(280, 170, 235, 20, "Residual Window Size:");
      o->labelsize(12);
      o->textsize(12);
      o->callback((Fl_Callback*)cb_Residual);
      o->align(FL_ALIGN_TOP_LEFT);
      o->menu(menu_Residual);
    }
    { Fl_Choice* o = new Fl_Choice(280, 210, 235, 20, "Residual Window Type:");
      o->labelsize(12);
      o->textsize(12);
      o->align(FL_ALIGN_TOP_LEFT);
      o->menu(menu_Residual1);
    }
    { Fl_Value_Slider* o = new Fl_Value_Slider(140, 270, 375, 20, "Zero Padding Factor:");
      o->type(3);
      o->color(16);
      o->selection_color(24);
      o->labelsize(12);
      o->maximum(10000);
      o->step(10);
      o->align(FL_ALIGN_LEFT);
    }
    { Fl_Light_Button* o = new Fl_Light_Button(20, 440, 130, 20, "Analysis Harmonic");
      o->labelsize(12);
    }
    { Fl_Light_Button* o = new Fl_Light_Button(395, 440, 120, 20, "Do Clean Tracks");
      o->labelsize(12);
    }
    { Fl_Box* o = new Fl_Box(10, 485, 515, 140);
      o->box(FL_SHADOW_BOX);
      o->color(16);
      o->labeltype(FL_NO_LABEL);
    }
    { Fl_Box* o = new Fl_Box(20, 475, 125, 20, "Synthesis Parameters");
      o->box(FL_FLAT_BOX);
      o->color(16);
      o->labelsize(12);
      o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    }
    { Fl_Choice* o = new Fl_Choice(95, 495, 235, 20, "Window Size:");
      o->labelsize(12);
      o->textsize(12);
      o->callback((Fl_Callback*)cb_Window);
      o->menu(menu_Window);
    }
    { Fl_Choice* o = new Fl_Choice(100, 520, 235, 20, "Window Type:");
      o->labelsize(12);
      o->textsize(12);
      o->menu(menu_Window1);
    }
    { Fl_Value_Input* o = new Fl_Value_Input(75, 545, 80, 20, "Hop size: ");
      o->labelsize(12);
      o->textsize(12);
      o->callback((Fl_Callback*)cb_Hop1);
    }
    { Fl_Value_Input* o = new Fl_Value_Input(140, 570, 80, 20, "Zero Padding Factor:");
      o->labelsize(12);
      o->textsize(12);
      o->callback((Fl_Callback*)cb_Zero);
    }
    { Fl_Choice* o = new Fl_Choice(165, 595, 235, 20, "Phase Management Type:");
      o->labelsize(12);
      o->textsize(12);
      o->menu(menu_Phase);
    }
    { Fl_Button* o = new Fl_Button(215, 705, 155, 25, "Dump on an XML File...");
      o->labelsize(12);
    }
    { Fl_Button* o = new Fl_Button(390, 705, 135, 25, "Set Configuration");
      o->labelsize(12);
    }
    { Fl_Button* o = new Fl_Button(35, 705, 155, 25, "Load from an XML File...");
      o->labelsize(12);
    }
    mWindow->show();
    o->end();
  }
}

inline void UserInterface::cb_Configure_i(Fl_Menu_*, void*) {
  LoadConfiguration();
//EditConfiguration();
}
void UserInterface::cb_Configure(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_Configure_i(o,v);
}

inline void UserInterface::cb_Load_i(Fl_Menu_*, void*) {
  LoadAnalysisData();
}
void UserInterface::cb_Load(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_Load_i(o,v);
}

inline void UserInterface::cb_mStoreAnalysisData_i(Fl_Menu_*, void*) {
  StoreAnalysisData();
}
void UserInterface::cb_mStoreAnalysisData(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_mStoreAnalysisData_i(o,v);
}

inline void UserInterface::cb_Load1_i(Fl_Menu_*, void*) {
  LoadTransformation();
}
void UserInterface::cb_Load1(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_Load1_i(o,v);
}

inline void UserInterface::cb_mDoTransformation_i(Fl_Menu_*, void*) {
  Transform();
}
void UserInterface::cb_mDoTransformation(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_mDoTransformation_i(o,v);
}

inline void UserInterface::cb_mMelodyAnalyze_i(Fl_Menu_*, void*) {
  AnalyzeMelody();
}
void UserInterface::cb_mMelodyAnalyze(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_mMelodyAnalyze_i(o,v);
}

inline void UserInterface::cb_mMelodyStore_i(Fl_Menu_*, void*) {
  StoreMelody();
}
void UserInterface::cb_mMelodyStore(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_mMelodyStore_i(o,v);
}

inline void UserInterface::cb_mDisplayInSound_i(Fl_Menu_*, void*) {
  DisplayInputSound();
}
void UserInterface::cb_mDisplayInSound(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_mDisplayInSound_i(o,v);
}

inline void UserInterface::cb_mDisplayInSpec_i(Fl_Menu_*, void*) {
  DisplayInputSpectrum();
}
void UserInterface::cb_mDisplayInSpec(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_mDisplayInSpec_i(o,v);
}

inline void UserInterface::cb_mAnalyze_i(Fl_Menu_*, void*) {
  Analyze();
}
void UserInterface::cb_mAnalyze(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_mAnalyze_i(o,v);
}

inline void UserInterface::cb_mSynthesize_i(Fl_Menu_*, void*) {
  Synthesize();
}
void UserInterface::cb_mSynthesize(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_mSynthesize_i(o,v);
}

inline void UserInterface::cb_Sound_i(Fl_Menu_*, void*) {
  StoreOutputSound();
}
void UserInterface::cb_Sound(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_Sound_i(o,v);
}

inline void UserInterface::cb_Sinusoidal_i(Fl_Menu_*, void*) {
  StoreOutputSoundSinusoidal();
}
void UserInterface::cb_Sinusoidal(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_Sinusoidal_i(o,v);
}

inline void UserInterface::cb_Residual1_i(Fl_Menu_*, void*) {
  StoreOutputSoundResidual();
}
void UserInterface::cb_Residual1(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_Residual1_i(o,v);
}

inline void UserInterface::cb_Sound1_i(Fl_Menu_*, void*) {
  DisplayOutputSound();
}
void UserInterface::cb_Sound1(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_Sound1_i(o,v);
}

inline void UserInterface::cb_Sinusoidal1_i(Fl_Menu_*, void*) {
  DisplayOutputSoundSinusoidal();
}
void UserInterface::cb_Sinusoidal1(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_Sinusoidal1_i(o,v);
}

inline void UserInterface::cb_Residual2_i(Fl_Menu_*, void*) {
  DisplayOutputSoundResidual();
}
void UserInterface::cb_Residual2(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_Residual2_i(o,v);
}

inline void UserInterface::cb_Exit_i(Fl_Menu_*, void*) {
  if(mWindow2!=NULL) delete mWindow2;
Exit();
}
void UserInterface::cb_Exit(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_Exit_i(o,v);
}

inline void UserInterface::cb_About_i(Fl_Menu_*, void*) {
  AboutWindow();
}
void UserInterface::cb_About(Fl_Menu_* o, void* v) {
  ((UserInterface*)(o->parent()->user_data()))->cb_About_i(o,v);
}

Fl_Menu_Item UserInterface::menu_mMenuBar[] = {
 {"Input", 0,  0, 0, 64, 0, 0, 12, 0},
 {"Configure...", 0,  (Fl_Callback*)UserInterface::cb_Configure, 0, 0, 0, 0, 12, 0},
 {"Analysis Data", 0,  0, 0, 64, 0, 0, 12, 0},
 {"Load...", 0,  (Fl_Callback*)UserInterface::cb_Load, 0, 0, 0, 0, 12, 0},
 {"Store...", 0,  (Fl_Callback*)UserInterface::cb_mStoreAnalysisData, 0, 1, 0, 0, 12, 0},
 {0},
 {"Transformation", 0,  0, 0, 64, 0, 0, 12, 0},
 {"Load...", 0,  (Fl_Callback*)UserInterface::cb_Load1, 0, 0, 0, 0, 12, 0},
 {"Do", 0,  (Fl_Callback*)UserInterface::cb_mDoTransformation, 0, 1, 0, 0, 12, 0},
 {0},
 {"Melody", 0,  0, 0, 65, 0, 0, 12, 0},
 {"Analyze", 0,  (Fl_Callback*)UserInterface::cb_mMelodyAnalyze, 0, 0, 0, 0, 12, 0},
 {"Store", 0,  (Fl_Callback*)UserInterface::cb_mMelodyStore, 0, 1, 0, 0, 12, 0},
 {0},
 {"Display", 0,  0, 0, 65, 0, 0, 12, 0},
 {"Sound", 0,  (Fl_Callback*)UserInterface::cb_mDisplayInSound, 0, 1, 0, 0, 12, 0},
 {"Spectrum", 0,  (Fl_Callback*)UserInterface::cb_mDisplayInSpec, 0, 1, 0, 0, 12, 0},
 {0},
 {0},
 {"Analyze", 0x40061,  (Fl_Callback*)UserInterface::cb_mAnalyze, 0, 1, 0, 0, 12, 0},
 {"Synthesize", 0x40073,  (Fl_Callback*)UserInterface::cb_mSynthesize, 0, 1, 0, 0, 12, 0},
 {"Output", 0,  0, 0, 65, 0, 0, 12, 0},
 {"Store", 0,  0, 0, 64, 0, 0, 12, 0},
 {"Sound", 0,  (Fl_Callback*)UserInterface::cb_Sound, 0, 0, 0, 0, 12, 0},
 {"Sinusoidal", 0,  (Fl_Callback*)UserInterface::cb_Sinusoidal, 0, 0, 0, 0, 12, 0},
 {"Residual", 0,  (Fl_Callback*)UserInterface::cb_Residual1, 0, 0, 0, 0, 12, 0},
 {0},
 {"Display", 0,  0, 0, 64, 0, 0, 12, 0},
 {"Sound", 0,  (Fl_Callback*)UserInterface::cb_Sound1, 0, 0, 0, 0, 12, 0},
 {"Sinusoidal", 0,  (Fl_Callback*)UserInterface::cb_Sinusoidal1, 0, 0, 0, 0, 12, 0},
 {"Residual", 0,  (Fl_Callback*)UserInterface::cb_Residual2, 0, 0, 0, 0, 12, 0},
 {0},
 {0},
 {"Exit", 0x40071,  (Fl_Callback*)UserInterface::cb_Exit, 0, 0, 0, 0, 12, 0},
 {"About", 0,  (Fl_Callback*)UserInterface::cb_About, 0, 0, 0, 13, 12, 0},
 {0}
};
Fl_Menu_Item* UserInterface::mStoreAnalysisData = UserInterface::menu_mMenuBar + 4;
Fl_Menu_Item* UserInterface::mDoTransformation = UserInterface::menu_mMenuBar + 8;
Fl_Menu_Item* UserInterface::mMelodySM = UserInterface::menu_mMenuBar + 10;
Fl_Menu_Item* UserInterface::mMelodyAnalyze = UserInterface::menu_mMenuBar + 11;
Fl_Menu_Item* UserInterface::mMelodyStore = UserInterface::menu_mMenuBar + 12;
Fl_Menu_Item* UserInterface::mDisplayInSM = UserInterface::menu_mMenuBar + 14;
Fl_Menu_Item* UserInterface::mDisplayInSound = UserInterface::menu_mMenuBar + 15;
Fl_Menu_Item* UserInterface::mDisplayInSpec = UserInterface::menu_mMenuBar + 16;
Fl_Menu_Item* UserInterface::mAnalyze = UserInterface::menu_mMenuBar + 19;
Fl_Menu_Item* UserInterface::mSynthesize = UserInterface::menu_mMenuBar + 20;
Fl_Menu_Item* UserInterface::mOutputSM = UserInterface::menu_mMenuBar + 21;
Fl_Menu_Item* UserInterface::mStoreOutputs = UserInterface::menu_mMenuBar + 22;
Fl_Menu_Item* UserInterface::mVisualizeOutputs = UserInterface::menu_mMenuBar + 27;

inline void UserInterface::cb_mWindow2_i(Fl_Window*, void*) {
  delete mWindow2;
mWindow2=NULL;
}
void UserInterface::cb_mWindow2(Fl_Window* o, void* v) {
  ((UserInterface*)(o->user_data()))->cb_mWindow2_i(o,v);
}

#include <FL/Fl_Pixmap.H>
static const char *image_iua[] = {
"45 49 153 2",
"  \tc None",
". \tc #F8ACAF",
"+ \tc #F8A7AB",
"@ \tc #F15358",
"# \tc #F79DA0",
"$ \tc #F9AEB0",
"% \tc #FAB9BB",
"& \tc #F69697",
"* \tc #F58489",
"= \tc #F6878B",
"- \tc #F9B7B9",
"; \tc #F37D82",
"> \tc #F15056",
", \tc #F46B72",
"\' \tc #F89EA2",
") \tc #F9BFC2",
"! \tc #F48489",
"~ \tc #F7A0A4",
"{ \tc #EF2A2D",
"] \tc #F69497",
"^ \tc #F16366",
"/ \tc #F79295",
"( \tc #F68A8F",
"_ \tc #F7A7AB",
": \tc #F16263",
"< \tc #EA0000",
"[ \tc #EB0000",
"} \tc #EA0007",
"| \tc #EC0B0F",
"1 \tc #EC050E",
"2 \tc #EC1115",
"3 \tc #EB1619",
"4 \tc #ED2229",
"5 \tc #ED262C",
"6 \tc #EF4D4E",
"7 \tc #F58588",
"8 \tc #EC1619",
"9 \tc #E80000",
"0 \tc #EC2328",
"a \tc #F47778",
"b \tc #F7999D",
"c \tc #F9ABAF",
"d \tc #F79DA1",
"e \tc #F69091",
"f \tc #F68F8F",
"g \tc #F3676C",
"h \tc #F36D71",
"i \tc #F36364",
"j \tc #F15E5E",
"k \tc #EC171E",
"l \tc #F15255",
"m \tc #ED3436",
"n \tc #F36268",
"o \tc #F2666A",
"p \tc #F14B50",
"q \tc #F9B2B4",
"r \tc #F68D90",
"s \tc #F58389",
"t \tc #F1484F",
"u \tc #F3747A",
"v \tc #EA0D0D",
"w \tc #EF3A42",
"x \tc #F16064",
"y \tc #EF343C",
"z \tc #F6888D",
"A \tc #F7979B",
"B \tc #F0474F",
"C \tc #E90A14",
"D \tc #F26165",
"E \tc #F47072",
"F \tc #F79A9D",
"G \tc #F25A5E",
"H \tc #F25F65",
"I \tc #EE212A",
"J \tc #EF363E",
"K \tc #E90002",
"L \tc #EB1A19",
"M \tc #F79EA3",
"N \tc #F8A8AC",
"O \tc #F15A60",
"P \tc #ED282F",
"Q \tc #F37175",
"R \tc #EF363C",
"S \tc #F04F51",
"T \tc #E70707",
"U \tc #EE141C",
"V \tc #E90000",
"W \tc #EF3738",
"X \tc #F78F93",
"Y \tc #EB151C",
"Z \tc #F36C70",
"` \tc #EA0808",
" .\tc #ED2D35",
"..\tc #ED1119",
"+.\tc #F0343C",
"@.\tc #F7A1A2",
"#.\tc #E90001",
"$.\tc #F79398",
"%.\tc #FABCBC",
"&.\tc #E90606",
"*.\tc #F36066",
"=.\tc #FABABC",
"-.\tc #E90303",
";.\tc #F59296",
">.\tc #F9BBBE",
",.\tc #ED2329",
"\'.\tc #E70000",
").\tc #F35C63",
"!.\tc #F47B7F",
"~.\tc #EE3234",
"{.\tc #EA0003",
"].\tc #F15558",
"^.\tc #E70405",
"/.\tc #F57D7E",
"(.\tc #F0484D",
"_.\tc #F9AFB2",
":.\tc #F36B71",
"<.\tc #EF474E",
"[.\tc #ED1A22",
"}.\tc #F15058",
"|.\tc #F6898E",
"1.\tc #F8A4A7",
"2.\tc #F8B0B3",
"3.\tc #ED282B",
"4.\tc #EA0001",
"5.\tc #EC0E16",
"6.\tc #F8A2A5",
"7.\tc #ED252A",
"8.\tc #EF4346",
"9.\tc #E90003",
"0.\tc #EA2C2D",
"a.\tc #F57377",
"b.\tc #EC151C",
"c.\tc #F3696B",
"d.\tc #F56D73",
"e.\tc #EC2426",
"f.\tc #EB0912",
"g.\tc #EE2930",
"h.\tc #EE222A",
"i.\tc #EC3032",
"j.\tc #F58183",
"k.\tc #F37278",
"l.\tc #F5898D",
"m.\tc #F68E91",
"n.\tc #F58689",
"o.\tc #EE3138",
"p.\tc #ED2C33",
"q.\tc #F79A9E",
"r.\tc #F69094",
"s.\tc #EC1E26",
"t.\tc #EB1820",
"u.\tc #EE2F38",
"v.\tc #F14D51",
"                                                                             \
             ",
"                                                                             \
             ",
"                                                                             \
             ",
"                                                                             \
             ",
"                                                                             \
             ",
"                                                                             \
             ",
"                                                                             \
             ",
"                                                    .       +                \
             ",
"                                                          + +                \
             ",
"                                                                             \
             ",
"                                      +             + @   #             $ %  \
             ",
"                                      +             & *   = -           ; >  \
             ",
"                                      ,             + \'   ) +         , , ! \
,             ",
"                                      ,             ~ { ] ^ / , , , , , , , (\
             ",
"                                      +       , _ : < [ } | 1 2 3 4 5 6 7    \
             ",
"                                            ] 8 9 0 a b , c d e f g h 6 < i  \
             ",
"                                          j k l + ,                       m n\
             ",
"                                        o p q +                             r\
             ",
"                                        % +                                  \
             ",
"                                                                             \
             ",
"                  s t u                                                      \
             ",
"              t v w x y k                                                    \
             ",
"              < z       A B                                                  \
             ",
"          , C D           E                             F G H +              \
             ",
"          I J             :                     + G K L M + + N O P Q +      \
             ",
"          R S           e T                 / U k V W               X Y ,    \
             ",
"          Z `        .....+             , +.R + @.#.$.      %.n +     j V    \
             ",
"            &.*.                      ,  .=.    d -.;.    >.,.\'.l +   ).B   \
              ",
"            !.~.                                o {.,     + ].^./.    p +   (\
.n           ",
"              3                         , ,     _.< u       + :.+     <., , [\
.}.          ",
"              } |.                      1.H 2.    V 3.+           + A L 4.y ,\
             ",
"              5.D                           h b 6.V } 7., , , , , 8.9.0.a.+  \
             ",
"              b.c.                            @.O d.R e.f.g.h.< V i.j.+      \
             ",
"              f.k.                                  , , , l.l.m., +          \
             ",
"              5.n.                                                           \
             ",
"              [.,                                                            \
             ",
"              o.6.                                                           \
             ",
"              R g                                                            \
             ",
"              p.t           q.,                                              \
             ",
"              r.K ,       , s.,                                              \
             ",
"                , t.m., !.u.b                                                \
             ",
"                    , v.a.                                                   \
             ",
"                                                                             \
             ",
"                                                                             \
             ",
"                                                                             \
             ",
"                                                                             \
             ",
"                                                                             \
             ",
"                                                                             \
             ",
"                                                                             \
             "
};
static Fl_Pixmap pixmap_iua(image_iua);

static const char *image_mtg[] = {
"72 49 234 2",
"  \tc None",
". \tc #FCDFDF",
"+ \tc #EF5E60",
"@ \tc #EB3234",
"# \tc #F06567",
"$ \tc #E81A1C",
"% \tc #F27F81",
"& \tc #F17273",
"* \tc #7A749F",
"= \tc #665E90",
"- \tc #76709C",
"; \tc #756F9C",
"> \tc #746C9A",
", \tc #5D5D92",
"\' \tc #FFA4A2",
") \tc #F5A1A2",
"! \tc #F49091",
"~ \tc #F38687",
"{ \tc #FBD9DA",
"] \tc #F9C3C3",
"^ \tc #F17678",
"/ \tc #F17374",
"( \tc #F8B5B6",
"_ \tc #F7ADAE",
": \tc #EC4547",
"< \tc #F6A3A4",
"[ \tc #F6A8A9",
"} \tc #F38788",
"| \tc #FBD2D3",
"1 \tc #F7B4B4",
"2 \tc #F17475",
"3 \tc #F06E6F",
"4 \tc #ED4A4C",
"5 \tc #706998",
"6 \tc #585186",
"7 \tc #686292",
"8 \tc #666091",
"9 \tc #676192",
"0 \tc #4E4D88",
"a \tc #FF1C13",
"b \tc #E61A1D",
"c \tc #EB3839",
"d \tc #F6A7A8",
"e \tc #F06A6B",
"f \tc #ED4849",
"g \tc #FF3932",
"h \tc #F38889",
"i \tc #F8BABA",
"j \tc #F06B6C",
"k \tc #EC4345",
"l \tc #F59FA0",
"m \tc #EA2C2E",
"n \tc #F3898A",
"o \tc #F38B8C",
"p \tc #F17071",
"q \tc #F27C7D",
"r \tc #FBD7D8",
"s \tc #F59798",
"t \tc #A39FBC",
"u \tc #9590B3",
"v \tc #A19DBB",
"w \tc #A09CBA",
"x \tc #9E99B9",
"y \tc #8D8DB2",
"z \tc #FF3C34",
"A \tc #F49596",
"B \tc #E92022",
"C \tc #F38384",
"D \tc #F48F90",
"E \tc #4F4780",
"F \tc #5F588C",
"G \tc #5E568A",
"H \tc #484784",
"I \tc #F17879",
"J \tc #E92728",
"K \tc #EB3C3E",
"L \tc #EA292B",
"M \tc #E92325",
"N \tc #ED4D4F",
"O \tc #EC3D3F",
"P \tc #B1AEC7",
"Q \tc #A8A4C0",
"R \tc #B2AFC8",
"S \tc #B2AEC7",
"T \tc #A1A1C0",
"U \tc #EE5759",
"V \tc #443C78",
"W \tc #534C83",
"X \tc #524B82",
"Y \tc #524881",
"Z \tc #39387A",
"` \tc #FF3B33",
" .\tc #F27B7C",
"..\tc #F8B7B8",
"+.\tc #ED4F51",
"@.\tc #EA2B2D",
"#.\tc #EF6364",
"$.\tc #AFABC5",
"%.\tc #B0ADC6",
"&.\tc #B0ABC5",
"*.\tc #9F9FBF",
"=.\tc #EA2F30",
"-.\tc #251B62",
";.\tc #393070",
">.\tc #372E6F",
",.\tc #382F6F",
"\'.\tc #372B6D",
").\tc #1B1A66",
"!.\tc #FF3831",
"~.\tc #F8BDBD",
"{.\tc #E71B1D",
"].\tc #F59A9A",
"^.\tc #D5D3E0",
"/.\tc #CDCCDC",
"(.\tc #D7D6E2",
"_.\tc #D6D5E2",
":.\tc #D5D4E1",
"<.\tc #D6D4E1",
"[.\tc #FF2016",
"}.\tc #E41C1F",
"|.\tc #F35152",
"1.\tc #E61C1E",
"2.\tc #E61D1F",
"3.\tc #E71D1F",
"4.\tc #F44A4B",
"5.\tc #E71C1E",
"6.\tc #36366A",
"7.\tc #12124E",
"8.\tc #27275E",
"9.\tc #262661",
"0.\tc #32316F",
"a.\tc #2A2A67",
"b.\tc #272767",
"c.\tc #14145C",
"d.\tc #FF6C6C",
"e.\tc #FF1D1D",
"f.\tc #FF9595",
"g.\tc #FF3030",
"h.\tc #FF2F2F",
"i.\tc #FF5D5D",
"j.\tc #FE1515",
"k.\tc #F31214",
"l.\tc #FD0A0A",
"m.\tc #FFB5B5",
"n.\tc #EE5557",
"o.\tc #F28283",
"p.\tc #F9FBFB",
"q.\tc #F6F9F9",
"r.\tc #F1F1EC",
"s.\tc #EEEDEA",
"t.\tc #F8F8F6",
"u.\tc #FEFDFD",
"v.\tc #F6F6F3",
"w.\tc #E2E2DD",
"x.\tc #FCFCFC",
"y.\tc #E7E8D9",
"z.\tc #EBEDED",
"A.\tc #EDF4F4",
"B.\tc #DAE5E4",
"C.\tc #B6B6B6",
"D.\tc #EDEDED",
"E.\tc #E1E7E7",
"F.\tc #FCFBFB",
"G.\tc #E7F2F2",
"H.\tc #DFDFDE",
"I.\tc #E6E5E5",
"J.\tc #E6EDED",
"K.\tc #E2EEEE",
"L.\tc #FBFCFC",
"M.\tc #E8E8E8",
"N.\tc #EBEAEA",
"O.\tc #F0F0F0",
"P.\tc #EAF0F0",
"Q.\tc #FCC6C7",
"R.\tc #F6A6A7",
"S.\tc #BAB9B9",
"T.\tc #A4A2A3",
"U.\tc #C3C1C3",
"V.\tc #C0BEBF",
"W.\tc #C9C8C8",
"X.\tc #A8A6A8",
"Y.\tc #DADADA",
"Z.\tc #CCCBCC",
"`.\tc #C8C7C7",
" +\tc #CAC8C5",
".+\tc #ACABAB",
"++\tc #BCC2C2",
"@+\tc #C7C5C5",
"#+\tc #E3E2E2",
"$+\tc #B1AEAE",
"%+\tc #C5C4C4",
"&+\tc #C1C6C6",
"*+\tc #C8CDCD",
"=+\tc #C5C0C0",
"-+\tc #CCC9C9",
";+\tc #E1E0E0",
">+\tc #CDCCCC",
",+\tc #C3C1C1",
"\'+\tc #BDB9B9",
")+\tc #B6B0B0",
"!+\tc #CDCFCF",
"~+\tc #C6CBCB",
"{+\tc #F3F3F3",
"]+\tc #AFADAD",
"^+\tc #BFBDBD",
"/+\tc #CBC9C9",
"(+\tc #C2C3C3",
"_+\tc #BABEBD",
":+\tc #E3EAEA",
"<+\tc #F3080A",
"[+\tc #DDDDDD",
"}+\tc #D6D5D5",
"|+\tc #B4B3B3",
"1+\tc #D0CFCF",
"2+\tc #CBCACA",
"3+\tc #A8A7A7",
"4+\tc #D2D1D1",
"5+\tc #B1AFB0",
"6+\tc #CCCBCB",
"7+\tc #B3B1B1",
"8+\tc #AAA9A9",
"9+\tc #9D9B9B",
"0+\tc #B8B7B7",
"a+\tc #CED1D0",
"b+\tc #FEF6F6",
"c+\tc #F41F21",
"d+\tc #D4D3D3",
"e+\tc #CECDCD",
"f+\tc #F9F9F9",
"g+\tc #EEEDED",
"h+\tc #EFEEEE",
"i+\tc #FAF9F9",
"j+\tc #EC4041",
"k+\tc #EF5C5D",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
             . + @                                                 ",
"                                                                             \
             # $ $                                                 ",
"                                                                             \
             % $ $                                 &               ",
"          * = - ; ; ; ; ; ; ; ; ; - > ,     \' )     ! # # ~ {     ] ^ + / ( \
          & _ : $ $ < [ } |             1 2 + 3 ( & $ 4 &           ",
"          5 6 7 8 8 8 8 8 8 8 8 8 9 = 0     a b [ c $ $ $ $ $ d ^ $ $ $ $ $ e\
         & $ $ $ $ $ $ $ e         & f $ $ $ $ $ $ $ @ &           ",
"                                            g b $ $ h i j $ $ $ $ k 1 l m $ $\
 )       & $ $ $ $ n o p ]         k $ $ q r s $ $ $ &             ",
"          t u v w w w w w w w w w w x y     z b $ &       A $ $ p       & B $\
 :           C $ $               ] $ $ A       ] $ $ D             ",
"          7 E F F F F F F F F F F F G H     z b I         & $ $ &         e $\
 m           2 $ J               C $ $           K $ k             ",
"                                            g b &         & $ $ &         h $\
 L           & $ J               ^ $ M           N $ O             ",
"          P Q R P P P P P P P P P P S T     g b 3         & $ $ &         n $\
 m           & $ J               < $ $ &         B $ U             ",
"          G V W X X X X X X X X X X Y Z     ` b 3         & $ $ &         h $\
 m           & $ J               & $ $ :        .$ $ ..            ",
"                                            g b 3         & $ $ &         h $\
 m           & $ J                 $ $ $ M +.@.$ $ #.              ",
"          $.Q P %.%.%.%.%.%.%.%.%.%.&.*.    ` b 3         & $ $ &         h $\
 m           3 $ =.                  $ $ $ $ $ $ $                 ",
"          V -.;.>.>.>.>.>.,.>.>.>.,.\'.).    ` b 3         & $ $ &         h \
$ m            .$ $                 n $ !                           ",
"                                            !.b 3         & $ $ &         ~ $\
 @.          ~.{.$ +.& & & &     & $ $     ].e +.U [               ",
"          ^./.(._._._._.:.^.^.<._._.<./.    [.}.|.        & 1.2.&         2 3\
.3.            4.2.3.5.3.3.l       L $ $ $ $ $ $ $ $ +             ",
"          6.7.8.9.9.8.8.8.0.a.8.b.b.9.c.    d.e.f.        & g.h.&         \' \
e.i.              d.j.k.l.j.m.      & & 4 n.o.1 1 +.$ $ i           ",
"                                                  p.              q.         \
                                                   M $ <           ",
"          r.s.t.    u.v.v.w.x.r.t.  y.s.  z.  A.B.C.D.  E.u.F.G.z.H.F.I.  J.K\
.  x.L.  M.N.O.z.A.A.      P.E.    Q.A           & $ $ R.          ",
"          S.T.U.V.D.W.X.Y.Z.V.`.O.  V.H. +.+++@+#+$+%+W.S.&+*+=+-+;+>+%+,+\'+\
)+!+~+t.{+]+^+S./+=+(+&+I.-+\'+_+:+  <+{.=.! i ( I $ $ $ &           ",
"          [+`.}+|+1+,+`.`.2+3+I.    4+[+5+`.V.4+{+6+Y.7+I.%+,+}+\'+W.@+}+|+8+\
I.,+|+  {+9+6+1+O.6+4+|+>+0+/+a+B.b+c+{.$ $ $ $ $ $ M |             ",
"          N.[+M.#+%+4+d+}+M.I.V.I.  ;+1+;+e+f+@+e+#+N.}+g+h+Y.@+h+H.}+1+N.T.6\
+>+1+  i+8+C.H.  }+>+h+%+e+]+++p.    & o.j+M m k+R.                ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   ",
"                                                                             \
                                                                   "
};
static Fl_Pixmap pixmap_mtg(image_mtg);

UserInterface::UserInterface() {
  Fl_Double_Window* w;
  { Fl_Double_Window* o = mWindow = new Fl_Double_Window(1108, 779, "SMS Analysis/Synthesis Application | MTG-UPF (Barcelona)");
    w = o;
    o->color(23);
    o->user_data((void*)(this));
    { Fl_Input* o = mConfigurationText = new Fl_Input(105, 25, 985, 20, "Configuration File");
      o->box(FL_THIN_DOWN_BOX);
      o->labelsize(12);
      o->textsize(12);
      o->align(132);
    }
    { Fl_Menu_Bar* o = mMenuBar = new Fl_Menu_Bar(0, 0, 1110, 20);
      o->box(FL_THIN_UP_BOX);
      o->labelsize(13);
      o->textsize(12);
      o->menu(menu_mMenuBar);
    }
    { Fl_Smart_Tile* o = mSmartTile = new Fl_Smart_Tile(165, 100, 800, 665);
      o->color(23);
      Fl_Group::current()->resizable(o);
    }
    mWindow2=NULL;
    o->end();
  }
}

void UserInterface::AboutWindow() {
  Fl_Window* w;
  { Fl_Window* o = mWindow2 = new Fl_Window(417, 100, "About this program...");
    w = o;
    o->box(FL_ENGRAVED_BOX);
    o->color(17);
    o->labeltype(FL_NORMAL_LABEL);
    o->labelsize(10);
    o->callback((Fl_Callback*)cb_mWindow2, (void*)(this));
    { Fl_Box* o = new Fl_Box(85, 15, 265, 20, "SMS Analysis/Synthesis Application");
      o->labelfont(3);
      o->labelsize(12);
    }
    { Fl_Box* o = new Fl_Box(85, 40, 265, 20, "by MTG - IUA - UPF (Barcelona, Spain)");
      o->labelfont(2);
      o->labelsize(12);
    }
    { Fl_Box* o = new Fl_Box(360, 15, 50, 60);
      pixmap_iua.label(o);
    }
    { Fl_Box* o = new Fl_Box(90, 65, 260, 20, "please visit us at http://www.iua.upf.es/mtg for more info");
      o->labelfont(2);
      o->labelsize(11);
    }
    { Fl_Box* o = new Fl_Box(10, 20, 75, 55);
      pixmap_mtg.label(o);
    }
    mWindow2->show();
    o->clear_border();
    o->end();
  }
}
