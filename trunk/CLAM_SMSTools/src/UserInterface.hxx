// generated by Fast Light User Interface Designer (fluid) version 1.0104

#ifndef UserInterface_hxx
#define UserInterface_hxx
#include <FL/Fl.H>
#include <FL/Fl_Tile.H>
#include "Fl_Smart_Tile.hxx"
class Fl_Smart_Tile;
#include "ForwardDeclarations.hxx"
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Box.H>

class UserInterface {
public:
  public: CLAMGUI::SMSTools* mSMS;
  UserInterface();
  Fl_Window *mWindow;
private:
  Fl_Menu_Bar *mMenuBar;
  static Fl_Menu_Item menu_mMenuBar[];
  static Fl_Menu_Item *mFileMenuItem;
  static Fl_Menu_Item *mConfigurationOpsMenuItem;
  static Fl_Menu_Item *mLoadCfgMenuItem;
  inline void cb_mLoadCfgMenuItem_i(Fl_Menu_*, void*);
  static void cb_mLoadCfgMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mEditCfgMenuItem;
  inline void cb_mEditCfgMenuItem_i(Fl_Menu_*, void*);
  static void cb_mEditCfgMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mStoreCfgMenuItem;
  inline void cb_mStoreCfgMenuItem_i(Fl_Menu_*, void*);
  static void cb_mStoreCfgMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mAnalysisFileOpsMenuItem;
  static Fl_Menu_Item *mLoadAnalysisMenuItem;
  inline void cb_mLoadAnalysisMenuItem_i(Fl_Menu_*, void*);
  static void cb_mLoadAnalysisMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mStoreAnalysisMenuItem;
  inline void cb_mStoreAnalysisMenuItem_i(Fl_Menu_*, void*);
  static void cb_mStoreAnalysisMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mMelodyFileOpsMenuItem;
  static Fl_Menu_Item *mStoreMelodyMenuItem;
  inline void cb_mStoreMelodyMenuItem_i(Fl_Menu_*, void*);
  static void cb_mStoreMelodyMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mSMSTransFileOpsMenuItem;
  static Fl_Menu_Item *mLoadSMSTransScoreMenuItem;
  inline void cb_mLoadSMSTransScoreMenuItem_i(Fl_Menu_*, void*);
  static void cb_mLoadSMSTransScoreMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mSMSSynthesisFileOpsMenuItem;
  static Fl_Menu_Item *mStoreSMSSynthSoundMenuItem;
  inline void cb_mStoreSMSSynthSoundMenuItem_i(Fl_Menu_*, void*);
  static void cb_mStoreSMSSynthSoundMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mStoreSMSSynthSinusoidalMenuItem;
  inline void cb_mStoreSMSSynthSinusoidalMenuItem_i(Fl_Menu_*, void*);
  static void cb_mStoreSMSSynthSinusoidalMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mStoreSMSSynthResidualMenuItem;
  inline void cb_mStoreSMSSynthResidualMenuItem_i(Fl_Menu_*, void*);
  static void cb_mStoreSMSSynthResidualMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mAppExitMenuItem;
  inline void cb_mAppExitMenuItem_i(Fl_Menu_*, void*);
  static void cb_mAppExitMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mSMSAnalysisMenuItem;
  static Fl_Menu_Item *mDoSMSAnalysisMenuItem;
  inline void cb_mDoSMSAnalysisMenuItem_i(Fl_Menu_*, void*);
  static void cb_mDoSMSAnalysisMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mMelodyExtractionMenuItem;
  inline void cb_mMelodyExtractionMenuItem_i(Fl_Menu_*, void*);
  static void cb_mMelodyExtractionMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mSMSTransformationMenuItem;
  static Fl_Menu_Item *mDoSMSTransMenuItem;
  inline void cb_mDoSMSTransMenuItem_i(Fl_Menu_*, void*);
  static void cb_mDoSMSTransMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mUndoTransMenuItem;
  inline void cb_mUndoTransMenuItem_i(Fl_Menu_*, void*);
  static void cb_mUndoTransMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mSMSSynthesisMenuItem;
  static Fl_Menu_Item *mDoSMSSynthesisMenuItem;
  inline void cb_mDoSMSSynthesisMenuItem_i(Fl_Menu_*, void*);
  static void cb_mDoSMSSynthesisMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mViewMenuItem;
  static Fl_Menu_Item *mShowOriginalAudioMenuItem;
  inline void cb_mShowOriginalAudioMenuItem_i(Fl_Menu_*, void*);
  static void cb_mShowOriginalAudioMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mShowAnalysisResultsMenuItem;
  static Fl_Menu_Item *mShowSinTracksMenuItem;
  inline void cb_mShowSinTracksMenuItem_i(Fl_Menu_*, void*);
  static void cb_mShowSinTracksMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mViewFrameDataMenuItem;
  static Fl_Menu_Item *mShowSpectrumAndPeaksMenuItem;
  inline void cb_mShowSpectrumAndPeaksMenuItem_i(Fl_Menu_*, void*);
  static void cb_mShowSpectrumAndPeaksMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mShowSinusoidalSpectrumMenuItem;
  inline void cb_mShowSinusoidalSpectrumMenuItem_i(Fl_Menu_*, void*);
  static void cb_mShowSinusoidalSpectrumMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mShowResidualSpectrumMenuItem;
  inline void cb_mShowResidualSpectrumMenuItem_i(Fl_Menu_*, void*);
  static void cb_mShowResidualSpectrumMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mViewSynthesisResultsMenuItem;
  static Fl_Menu_Item *mShowSynthesizedAudioMenuItem;
  inline void cb_mShowSynthesizedAudioMenuItem_i(Fl_Menu_*, void*);
  static void cb_mShowSynthesizedAudioMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mShowSynthesizedSinusoidalMenuItem;
  inline void cb_mShowSynthesizedSinusoidalMenuItem_i(Fl_Menu_*, void*);
  static void cb_mShowSynthesizedSinusoidalMenuItem(Fl_Menu_*, void*);
  static Fl_Menu_Item *mShowSynthesizedResidualMenuItem;
  inline void cb_mShowSynthesizedResidualMenuItem_i(Fl_Menu_*, void*);
  static void cb_mShowSynthesizedResidualMenuItem(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *mHelpMenuItem;
private:
  inline void cb_About_i(Fl_Menu_*, void*);
  static void cb_About(Fl_Menu_*, void*);
public:
  Fl_Counter *mCounter;
private:
  inline void cb_mCounter_i(Fl_Counter*, void*);
  static void cb_mCounter(Fl_Counter*, void*);
  Fl_Smart_Tile *mSmartTile;
  void AboutWindow();
  Fl_Window *mWindow2;
  inline void cb_mWindow2_i(Fl_Window*, void*);
  static void cb_mWindow2(Fl_Window*, void*);
public:
  void Init(void);
private:
  void LoadConfiguration(void);
  void EditConfiguration(void);
  void StoreConfiguration(void);
  void Analyze(void);
  void LoadAnalysisData(void);
  void StoreAnalysisData(void);
  void Synthesize(void);
  void AnalyzeMelody(void);
  void StoreMelody(void);
  void StoreOutputSound(void);
  void StoreOutputSoundResidual(void);
  void StoreOutputSoundSinusoidal(void);
  void LoadTransformation(void);
  void Transform(void);
  void Exit(void);
  bool LoadSound();
  void ChangeFrame();
  void ChangeTimeTag(double tag);
  void DisplayInputSound();
  void Update();
  void DisplaySinusoidalTracks();
  void DisplaySpectrumAndPeaks();
  void DisplaySinusoidalSpectrum();
  void DisplayResidualSpectrum();
  void DisplaySynthesizedAudio();
  void DisplaySynthesizedSinusoidal();
  void DisplaySynthesizedResidual();
  void ApplyInitialState();
  void ApplyReadyToAnalyzeState();
  void ApplyAnalysisAvailableState();
  void ApplyMelodyAvailableState();
  void ApplyTransformationReadyState();
  void ApplySynthesisAvailableState();
  void ApplyTransformationPerformedState();
  void DeactivateFrameDataMenuItems();
  bool mFrameDataAvailable;
public:
  bool FrameDataAvailable();
private:
  void UndoTransform();
};
#endif
