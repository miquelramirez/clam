#ifndef _Parameters_
#define _Parameters_

#include "ProcessingDataConfig.hxx"
#include "ProcessingData.hxx"

#include "WindowGenerator.hxx"
#include "CSaltoSegData.hxx"
#include "SpectralPeakArray.hxx"
#include "CSaltoDefines.hxx"
#include "CSaltoTimbreVektor.hxx"
#include "Array.hxx"
#include "GlobalEnums.hxx"

namespace CLAM
{

  class Parameters: public ProcessingData
  {
     DYNAMIC_TYPE_USING_INTERFACE  (Parameters, 34, ProcessingData);

  public:
    DYN_ATTRIBUTE (0, public,bool,Play);
    DYN_ATTRIBUTE (1, public,bool,Exit);
    DYN_ATTRIBUTE (2, public,bool,UseSines);
    DYN_ATTRIBUTE (3, public,bool,UseAttackResidual);
    DYN_ATTRIBUTE (4, public,bool,UseStatResidual)
    DYN_ATTRIBUTE (5, public,bool,UsePhaseAlignment);
    DYN_ATTRIBUTE (6, public,bool,LastAlignedFrame);
    DYN_ATTRIBUTE (7, public,bool,PlayFrameOnly);
    DYN_ATTRIBUTE (8,public,bool,UsePitchCorrection);
    DYN_ATTRIBUTE (9,public,bool,TransitionMode);
    DYN_ATTRIBUTE (10,public,bool,TransitionInit);
    DYN_ATTRIBUTE (11,public,bool,TransitionUpwards);
    DYN_ATTRIBUTE (12,public,bool,UseRandomDeviations);
    DYN_ATTRIBUTE (13,public,bool,InLoopSynthesis);
    DYN_ATTRIBUTE (14,public,bool,UseMidiKeyboard);
    DYN_ATTRIBUTE (15,public,bool,UseBreathController);
    DYN_ATTRIBUTE (16,public,bool,UseRandomLoop);
    DYN_ATTRIBUTE (17,public,bool,BreathOnlySound);
    DYN_ATTRIBUTE (18,public,TIndex,SynthesisFlow);
    DYN_ATTRIBUTE (19,public,TIndex,CurrentStatTemplate);
    DYN_ATTRIBUTE (20,public,double,RandomRange);
    DYN_ATTRIBUTE (21,public,TIndex,Transpose);
    DYN_ATTRIBUTE (22,public,TIndex,TransitionInterval);
    DYN_ATTRIBUTE (23,public, double,InterPolFactor)
    DYN_ATTRIBUTE (24,public, double,PitchModFactor);
    DYN_ATTRIBUTE (25,public, double,TransitionFrequency);
    DYN_ATTRIBUTE (26,public, double,StatResFreq);
//runtime synthesis parameters

    DYN_ATTRIBUTE (27,public,double,StatResGain);
// runtime GUI Control
    DYN_ATTRIBUTE (28,public, bool,DisplayedValuesChanged);
// Salto SDIF Control Data Methods
    DYN_ATTRIBUTE (29,public, bool,UseMelody);
	DYN_ATTRIBUTE (30,public, bool,WriteToFile);
	DYN_ATTRIBUTE (31,public, bool,PlayNote);
	DYN_ATTRIBUTE (32,public, bool,PlayNoteOn);
	DYN_ATTRIBUTE (33,public, bool,PlayNoteOff);


/////////////////////////// THESE CAN BE DINAMIC BUT CHECK FIRST ////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
public:
	  
	  static Parameters& GetInstance();

public:
  // this should change when the CLAM Classes allow storage of a SpectralPeakArray. Right now this
  // implementation is not really beautiful. The CSaltoStatTemplData Array contains no Template-
  // structures inside. Thats why it could be stored easily with the  DataFileIO. After reading
  // in this data from the HD, it has to be converted into 'real' PeakArrays and these are stored here..
  // Consequence is that we need double of the memory, and everything has to be done 'by hand'

SpectralPeakArray*  GetCurrentStatTemplatePtr();         // returns pointer to current StatTempl
double          GetCurrentStatTemplateFundFreq();             // returns current fund freq of the stat template

void            SetStatPeakArray0(SpectralPeakArray a)   {mCurrentStatTmpl0 = a;} // = copies
SpectralPeakArray*  GetStatPeakArrayPtr0()               {return &mCurrentStatTmpl0;}
void            SetStatFundFreq0(double f)              			{mCurrentStatFundFreq0=f;}
double          GetStatFundFreq0()                      			{return mCurrentStatFundFreq0;}

void            SetStatPeakArray1  (SpectralPeakArray a) {mCurrentStatTmpl1 = a;} // = copies
SpectralPeakArray*  GetStatPeakArrayPtr1()               {return &mCurrentStatTmpl1;}
void            SetStatFundFreq1(double f)                   	{mCurrentStatFundFreq1=f;}
double          GetStatFundFreq1()                           	{return mCurrentStatFundFreq1;}

void            SetStatPeakArray2  (SpectralPeakArray a) {mCurrentStatTmpl2 = a;} // = copies
SpectralPeakArray*  GetStatPeakArrayPtr2()               {return &mCurrentStatTmpl2;}
void            SetStatFundFreq2(double f)                   	{mCurrentStatFundFreq2=f;}
double          GetStatFundFreq2()                           	{return mCurrentStatFundFreq2;}

void            SetStatPeakArray3  (SpectralPeakArray a) {mCurrentStatTmpl3 = a;} // = copies
SpectralPeakArray*  GetStatPeakArrayPtr3()               {return &mCurrentStatTmpl3;}
void            SetStatFundFreq3(double f)                   	{mCurrentStatFundFreq3=f;}
double          GetStatFundFreq3()                           	{return mCurrentStatFundFreq3;}

void            SetStatPeakArray4   (SpectralPeakArray a){mCurrentStatTmpl4 = a;} // = copies
SpectralPeakArray*  GetStatPeakArrayPtr4()               {return &mCurrentStatTmpl4;}
void            SetStatFundFreq4(double f)                   	{mCurrentStatFundFreq4=f;}
double          GetStatFundFreq4()                           	{return mCurrentStatFundFreq4;}

void            SetStatPeakArray5  (SpectralPeakArray a) {mCurrentStatTmpl5 = a;} // = copies
SpectralPeakArray*  GetStatPeakArrayPtr5()               {return &mCurrentStatTmpl5;}
void            SetStatFundFreq5(double f)                   	{mCurrentStatFundFreq5=f;}
double          GetStatFundFreq5()                           	{return mCurrentStatFundFreq5;}

void            SetStatPeakArray6  (SpectralPeakArray a) {mCurrentStatTmpl6 = a;} // = copies
SpectralPeakArray*  GetStatPeakArrayPtr6()               {return &mCurrentStatTmpl6;}
void            SetStatFundFreq6(double f)                   	{mCurrentStatFundFreq6=f;}
double          GetStatFundFreq6()                           	{return mCurrentStatFundFreq6;}

void            SetStatPeakArray7  (SpectralPeakArray a) {mCurrentStatTmpl7 = a;} // = copies
SpectralPeakArray*  GetStatPeakArrayPtr7()               {return &mCurrentStatTmpl7;}
void            SetStatFundFreq7(double f)                   	{mCurrentStatFundFreq7=f;}
double          GetStatFundFreq7()                           	{return mCurrentStatFundFreq7;}


    // Stationary Template Handling members
    SpectralPeakArray mCurrentStatTmpl;   // controlled by fader for choosing process
    double mCurrentStatFundFreq;
    SpectralPeakArray mCurrentStatTmpl0;   // chosen with button Tpl0
    double mCurrentStatFundFreq0;
    SpectralPeakArray mCurrentStatTmpl1;   // chosen with button Tpl1
    double mCurrentStatFundFreq1;
    SpectralPeakArray mCurrentStatTmpl2;   // chosen with button Tpl2
    double mCurrentStatFundFreq2;
    SpectralPeakArray mCurrentStatTmpl3;   // chosen with button Tpl3
    double mCurrentStatFundFreq3;
    SpectralPeakArray mCurrentStatTmpl4;   // chosen with button Tpl4
    double mCurrentStatFundFreq4;
    SpectralPeakArray mCurrentStatTmpl5;   // chosen with button Tpl5
    double mCurrentStatFundFreq5;
    SpectralPeakArray mCurrentStatTmpl6;   // chosen with button Tpl6
    double mCurrentStatFundFreq6;
    SpectralPeakArray mCurrentStatTmpl7;   // chosen with button Tpl7
    double mCurrentStatFundFreq7;

    Array<CSaltoSegData>& 	GetSegDataReference() {return mSegData;}
    CSaltoSegData           GetSegDataByPos(TIndex pos);
    void                    SetSegDataByPos(TIndex pos,CSaltoSegData segData);
    void                    SetSegDataNumFrames (TIndex pos,TIndex nFrames)
    {
      mSegData[pos].SetSegLength(nFrames);
    }


    // FileNameHandling Methods
    const char*   GetFileNameOfSegmentPos(TIndex pos)       {return mpFileName[pos];}
    const char*   GetFilePathOfSegmentPos(TIndex pos);
    const char*   GetPitchAttribute(TIndex pos)             {return mpPitchAttribute[pos];}
    const char*   GetAttackAttribute(TIndex pos)            {return mpAttackAttribute[pos];}

////////////////////////////////////////////////////////////////////

    // Stationary Template Handling
    const char*     GetStatTemplAttribute(TIndex pos)         			{return mpStatTemplAttribute[pos];}

    /* StatPeakArray is set by the editor fader in TPL Edit Window */
    inline void     SetStatPeakArray  (SpectralPeakArray*& a)  {mCurrentStatTmpl = *a;} // = copies
		inline void			SetStatPeakArray  (SpectralPeakArray& a)  	{mCurrentStatTmpl = a;} // = copies
    SpectralPeakArray*  GetStatPeakArrayPtr()                  {return &mCurrentStatTmpl;}
    void            SetStatFundFreq(double f)                    		{mCurrentStatFundFreq=f;}
    double          GetStatFundFreq()                            		{return mCurrentStatFundFreq;}

    // Timbre Handling
    inline void SetAttackTimbrePitch(TIndex p)        {mAttackTimbreVektor.SetPitch(p);}
    inline void SetAttackTimbreAttack(TIndex p)       {mAttackTimbreVektor.SetAttack(p);}
    inline void SetAttackTimbreLevel(TIndex p)        {mAttackTimbreVektor.SetLevel(p);}
    inline void SetAttackTimbreSmoothness(TIndex p)   {mAttackTimbreVektor.SetSmoothness(p);}
    inline void SetAttackTimbreTransFrames(TIndex p)  {mAttackTimbreVektor.SetTransitionFrames(p);}

    inline CSaltoTimbreVektor GetAttackTimbre()       {return mAttackTimbreVektor;}

	void SetupSaxoDB();

	void SetupTrumpetDB();

/**************************************************************************************************/
  private:
    // Salto Control Data related to SDIF Database via index/timbrevektor
    Array<CSaltoSegData> mSegData;

    // SegData Handling
    char** mpFileName;
    char** mpPitchAttribute;
    char** mpAttackAttribute;
    char** mpStatTemplAttribute;

    char  mpPathName[255];

    TIndex mCurrSDIFPosition, mSelectedPitch, mSelectedAttack;

    // Residual Handling
    TIndex mCurrentResidual;

    // TIMBRE handling
    CSaltoTimbreVektor mAttackTimbreVektor;

    // runtime synthesis settings
    TSize mMaxNumSines;
    						
    // runtime flow control parameters
    bool mUseRandomPhases;
    						
    // runtime GUI control parameters
//    bool  mDisplayedValuesChanged;			// if this gets true the oscilloscope-timer updates  all
                                  // displays values stated there... sort of message-manager....
						
  private:
	  void DefaultInit(); // To be used from the dynamic-type-generated constructor.
  

  };

} // end of namespace CLAM

#endif // _Parameters_
