#ifndef _SynthesisState_
#define _SynthesisState_

#include "ProcessingData.hxx"
#include "SpectralPeakArray.hxx"
#include "CSaltoTimbreVektor.hxx"
#include "Array.hxx"

#include "CSaltoSegData.hxx"

using CLAM::ProcessingData;
using CLAM::DynamicType;
using CLAM::TIndex;
using CLAM::SpectralPeakArray;
using CLAM::Array;

using CLAM::CSaltoTimbreVektor;
using CLAM::CSaltoSegData;

namespace SALTO
{
	class SynthesisState: public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SynthesisState, 31, ProcessingData);
		DYN_ATTRIBUTE (0, public, bool, InLoopSynthesis);
		DYN_ATTRIBUTE (1, public, bool, LoopDirectionFW);
		DYN_ATTRIBUTE (2, public, bool, StatResLoopDirectionFW);
		DYN_ATTRIBUTE (3, public, TIndex, FrameCounterTransition);
		DYN_ATTRIBUTE (4, public, TIndex, SegPositionBase);  // locales
		DYN_ATTRIBUTE (5, public, TIndex, NumFramesBase);
		DYN_ATTRIBUTE (6, public, TIndex, FrameCounterBase);
		DYN_ATTRIBUTE (7, public, double, IndividualGain);
		DYN_ATTRIBUTE (8, public, TIndex, LimitLoopStart);	//locales
		DYN_ATTRIBUTE (9, public, TIndex, LimitLoopEnd);	//locales
		DYN_ATTRIBUTE (10, public, TIndex, LoopStart);
		DYN_ATTRIBUTE (11, public, TIndex, InterpolStart);
		DYN_ATTRIBUTE (12, public, TIndex, InterpolEnd);
		DYN_ATTRIBUTE (13, public, TIndex, InterpolDuration);
		DYN_ATTRIBUTE (14, public, TIndex, ResFadeStart);
		DYN_ATTRIBUTE (15, public, TIndex, ResFadeEnd);
		DYN_ATTRIBUTE (16, public, double, AttackResVolume);
		DYN_ATTRIBUTE (17, public, TIndex, FrameCounterStatRes);
		DYN_ATTRIBUTE (18, public, TIndex, NumFramesStatRes);
		DYN_ATTRIBUTE (19, public, TIndex, StatResFadeInFrom);
		DYN_ATTRIBUTE (20, public, TIndex, StatResFadeInTo);
		DYN_ATTRIBUTE (21, public, double, StatResVolume);
		DYN_ATTRIBUTE (22, public, TIndex, FrameCounterAttackResidual);
		DYN_ATTRIBUTE (23, public, TIndex, LoopEnd);
		DYN_ATTRIBUTE (24, public, TIndex, CurrentStatTemplate);
		DYN_ATTRIBUTE (25, public, double, IPFactor);
		DYN_ATTRIBUTE (26, public, double, PitchReference);
		DYN_ATTRIBUTE (27, public, double, LastPitchCorrectionFactor);
		DYN_ATTRIBUTE (28, public, double, PitchModFactor);
		DYN_ATTRIBUTE (29, public, bool, LastAlignedFrame);
		DYN_ATTRIBUTE (30, public, bool, TransitionMode);
	protected:
		void DefaultInit(void);

	private:

		CSaltoTimbreVektor mAttackTimbreVektor;

		Array<CSaltoSegData> mSegData;

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

		// Timbre Handling
		inline void SetAttackTimbrePitch(TIndex p)        {mAttackTimbreVektor.SetPitch(p);}
		inline void SetAttackTimbreAttack(TIndex p)       {mAttackTimbreVektor.SetAttack(p);}
		inline void SetAttackTimbreLevel(TIndex p)        {mAttackTimbreVektor.SetLevel(p);}
		inline void SetAttackTimbreSmoothness(TIndex p)   {mAttackTimbreVektor.SetSmoothness(p);}
		inline void SetAttackTimbreTransFrames(TIndex p)  {mAttackTimbreVektor.SetTransitionFrames(p);}

		inline CSaltoTimbreVektor GetAttackTimbre()       {return mAttackTimbreVektor;}

	    CSaltoSegData           GetSegDataByPos(TIndex pos);

	};
}

#endif