#ifndef _CSaltoEditor_
#define _CSaltoEditor_

#include "DataTypes.hxx"
#include "Array.hxx"
#include "CSaltoSegData.hxx"

using CLAM::TData;
using CLAM::TSize;
using CLAM::TIndex;
using CLAM::Array;

// MRJ: As soon as these are inside the SALTO namespace
// this will be changed
namespace CLAM
{
class CSaltoSpectrumDisplay;
class CSaltoSpectralDisplay;
class CSaltoDisplay;
class Parameters;
class BaseAudioApplication;
class MelodyTranslator;
class Spectrum;
class SpectralPeakArray;
class SaltoSynth;

}

using CLAM::SaltoSynth;
using CLAM::DataArray;
using CLAM::SpectralPeakArray;
using CLAM::Spectrum;
using CLAM::CSaltoSpectrumDisplay;
using CLAM::CSaltoSpectralDisplay;
using CLAM::CSaltoDisplay;
using CLAM::Parameters;
using CLAM::BaseAudioApplication;
using CLAM::MelodyTranslator;

// FLTK widgets forward declarations
class Fl_Group;
class Fl_Window;
class Fl_Double_Window;
class Fl_Button;
class Fl_Slider;
class Fl_Value_Slider;
class Fl_Value_Output;
class Fl_Scrollbar;
class Fl_Widget;
class Fl_Output;
class Fl_Light_Button;
class Fl_Counter;
class Fl_Adjuster;
class Fl_Box;
class Fl_Dial;

namespace SALTO
{


class CSaltoEditor
{
public:

	CSaltoEditor(
		Parameters *pParams,
		SaltoSynth *pSaltoApp,
		MelodyTranslator *melody, 
		BaseAudioApplication* pApp);

	~CSaltoEditor();

	// public methods


	void DisplayBufferOfDouble(TData *array,TSize numElements);
	void DisplayTmplArrayOfDouble(const DataArray &displayArray);

//	void DisplayPeakArray(const SpectralPeakArray* peakArray);
//	void DisplayResidual(const Spectrum* spec);
	void DisplayPeakArray(const SpectralPeakArray& peakArray);
	void DisplayAttackResidual(const Spectrum& spec);
	void DisplayStationaryResidual(const Spectrum& spec);
	void DisplaySynthesizedSpectrum(const Spectrum& spec);
	void FillDisplayPeakArray(const SpectralPeakArray& peakArray );
	void FillDisplayAttackResidual(const Spectrum& spec);
	void FillDisplayStationaryResidual(const Spectrum& spec);
	void FillDisplaySynthesizedSpectrum(const Spectrum& spec);
	void DisplayPeakArray();
	void DisplayAttackResidual();
	void DisplayStationaryResidual();
	void DisplaySynthesizedSpectrum();

	void SetValOut1(TData val);

	void LoadLoopValues(TIndex pos);
	void StoreLoopValues(TIndex pos);
	void LoadLoopValuesStat(TIndex pos);
	void StoreLoopValuesStat(TIndex pos);
	void LoadSDIFToEdit();
//	void StoreStationaryTmpl();
	void AddStationaryTmplToDataBase(TIndex pos);

	void SetDisplayParameters( TData sampRate, TSize spectralFrameSize );

// callbacks

	static void sStoreAll(Fl_Light_Button* pButton, CSaltoEditor* pThis);

	static void sStart(Fl_Light_Button* pButton, CSaltoEditor* pThis);

	/** Triggers a single note synthesis
	 */
	static void sPlay(Fl_Button* pButton, CSaltoEditor* pThis);

	/** Terminates the application
	 */
	static void sExit(Fl_Button* pButton, CSaltoEditor* pThis);

	/** Allows to tell the Synthesis to use or not sinusoidal component
	 *  synthesis
	 */

	static void sUseSines(Fl_Button* pButton, CSaltoEditor* pThis);

	/** Enables/disable usage of special residual component synthesis
	 *  for notes' attack
	 */

	static void sUseAttackResidual(Fl_Button* pButton, CSaltoEditor* pThis);

	/** Enables/disable residual component synthesis
	 */

	static void sUseStatResidual(Fl_Button* pButton, CSaltoEditor* pThis);

	/** Enables/disables phase alignment enforcement for the analysis input
	 */

	static void sSetUsePhaseAlignment(Fl_Button* pButton, CSaltoEditor* pThis);

	/** Sets manually the interpolation factor 
	 */
	static void sSetIPValue(Fl_Slider* pSlider, CSaltoEditor* pThis);

	/**
	 *  Unknown purpose
	 */	
	static void sShowIPEditor(Fl_Button* pSlider, CSaltoEditor* pThis);


	/**
	 *  Unknown purpose
	 */

	static void sShowTemplateEditor(Fl_Button* pSlider, CSaltoEditor* pThis);
	
	/** Unknown purpose
	 */
	static void sShowPlayEditor(Fl_Button* pSlider, CSaltoEditor* pThis);

//----------------------------------------------------------------------------//
// salto control parameter editing ATTACK
//----------------------------------------------------------------------------//
	static void sSelectPitch(Fl_Counter* pCounter, CSaltoEditor* pThis);

	static void sSelectAttack(Fl_Counter* pCounter, CSaltoEditor* pThis);

/* chooser buttons for stat templates */
	static void sStatTmplChoose0(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sStatTmplChoose1(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sStatTmplChoose2(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sStatTmplChoose3(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sStatTmplChoose4(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sStatTmplChoose5(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sStatTmplChoose6(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sStatTmplChoose7(Fl_Button* pButton, CSaltoEditor* pThis);

	// load / store loop params
	static void sLoadLoopParams(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sStoreLoopParams(Fl_Button* pButton, CSaltoEditor* pThis);


//----------------------------------------------------------------------------//
// salto control parameter Stationary Template Editor
//----------------------------------------------------------------------------//
	static void sLoadSDIFToEdit(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sStoreStationaryTmpl(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sStatFrameFader(Fl_Value_Slider* pSlider, CSaltoEditor* pThis);
	
	static void sAddStationaryTmpl(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sAddStationaryTmpl0(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sAddStationaryTmpl1(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sAddStationaryTmpl2(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sAddStationaryTmpl3(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sAddStationaryTmpl4(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sAddStationaryTmpl5(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sAddStationaryTmpl6(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sAddStationaryTmpl7(Fl_Button* pButton, CSaltoEditor* pThis);

//----------------------------------------------------------------------------//
// Timbre Vektor Display
//----------------------------------------------------------------------------//
	static void sdisplay (Fl_Button* pButton, CSaltoEditor* pThis);

//----------------------------------------------------------------------------//
// TIMER FOR SALTO DISPLAY
//----------------------------------------------------------------------------//

	static void sOldTimer(void* p);

	static void sTimer(void* p);

//----------------------------------------------------------------------------//
// test display
//----------------------------------------------------------------------------//

	static void sStatResGain(Fl_Value_Slider* pSlider, CSaltoEditor* pThis);

	static void sStatResonanceFreq(Fl_Value_Slider* pSlider, CSaltoEditor* pThis);

	static void sSetGain(Fl_Value_Slider* pSlider, CSaltoEditor* pThis);

	static void sSetTestSlider(Fl_Value_Slider* pSlider, CSaltoEditor* pThis);

	/**  Enables, for debugging purposes, SALTO to write the synthesized audio
	 *   into a WAVE file
	 */
	static void sSetTestButton(Fl_Button* pButton, CSaltoEditor* pThis);

	/**  Activates MIDI-Keyboard support 
	 */
	static void sSetTestButton1(Fl_Button* pButton, CSaltoEditor* pThis);

	/**  Activates Pitch Correction algorithms
	 */
	static void sSetTestButton2(Fl_Button* pButton, CSaltoEditor* pThis);

	/** Allows random deviations
	 */
	static void sSetTestButton3(Fl_Button* pButton, CSaltoEditor* pThis);

	/** Allows note transposing
	 */
	static void sSetTestButton4(Fl_Button* pButton, CSaltoEditor* pThis);

	/** Enables YAMAHA(c) WX-5 MIDI breath controller support
	 */
	static void sSetTestButton5(Fl_Button* pButton, CSaltoEditor* pThis);

	/** Activates Synthesis random loop over the Spectral sampled frames
	 */
	static void sSetTestButton6(Fl_Button* pButton, CSaltoEditor* pThis);

	/** Loads an XML Melody description file
	 */
	static void sLoadMelody(Fl_Button* pButton, CSaltoEditor* pThis);

	static void sPlayMelody(Fl_Button* pButton, CSaltoEditor* pThis);

	Fl_Double_Window  *mpEditorWindow;

private:

	CSaltoDisplay   *mpWidgetWave;
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
	Parameters *mpParams;
	SaltoSynth *mpSaltoApp;
	MelodyTranslator *mMelody;
	BaseAudioApplication* mpApp;

	TData mSamplingRate;
	TSize mSpectralFrameSize;

};

} // end of namespace SALTO
#endif
