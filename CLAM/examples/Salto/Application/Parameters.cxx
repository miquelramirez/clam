// Parameters.cxx: implementation of parameters class.
//
//////////////////////////////////////////////////////////////////////

#include "Parameters.hxx"
#include "GlobalEnums.hxx"

namespace CLAM
{
//template <class T> void InitArrayElement(T& t)

Parameters& Parameters::GetInstance()
{
	static Parameters instance;
	return instance;
}

//////////////////////////////////////////
// Parameters
//////////////////////////////////////////
void Parameters::DefaultInit()
{
//	Configure(ParametersConfig());

  mpFileName = new char*[MAX_SPECTRAL_SEGMENTS];
  mpPitchAttribute = new char*[MAX_PITCH_VALUE];
  mpAttackAttribute = new char*[MAX_ATTACKS_VALUE];
  mpStatTemplAttribute = new char*[MAX_STAT_TEMPLATES];

  AddAll();
  UpdateData();

  SetUseMelody(false);
  mSegData = MAX_SPECTRAL_SEGMENTS;
  SetCurrentStatTemplate(0);
	
  //XA: added this condition SetPlay(true)º
  SetPlay(false);
  SetExit(false);
  SetUseSines(true);
  SetUseAttackResidual(true);
  SetUseStatResidual(false);
  SetUsePhaseAlignment(true);
  SetLastAlignedFrame(false);
  SetPlayFrameOnly(false);
  SetUsePitchCorrection(true);
  SetTransitionMode(false);
  SetTransitionInit(false);
//  SetTransitionUpwards();
  SetUseRandomDeviations(false);
  SetDisplayedValuesChanged(false);
  SetInLoopSynthesis(false);
  mCurrSDIFPosition = 0;
  mCurrentResidual = 0;
  SetInterPolFactor(0.0);
	SetPitchModFactor(1.0);
  mMaxNumSines = MAX_SINES;
  SetStatResGain(1);
  mSelectedPitch = 0;
  mSelectedAttack = 0;
  SetSynthesisFlow(FLOW_INTERPOLATE);
  mCurrentStatFundFreq = 0.0;
  mCurrentStatFundFreq0 = 0.0;
  mCurrentStatFundFreq1 = 0.0;
  mCurrentStatFundFreq2 = 0.0;
  mCurrentStatFundFreq3 = 0.0;
  mCurrentStatFundFreq4 = 0.0;
  mCurrentStatFundFreq5 = 0.0;
  mCurrentStatFundFreq6 = 0.0;
  mCurrentStatFundFreq7 = 0.0;
  SetWriteToFile( false );
  SetRandomRange(8);
  SetStatResFreq(200.0);
  SetTranspose(0);
  SetUseMidiKeyboard(false);
  SetUseBreathController(false);
  SetUseRandomLoop(false);
  SetBreathOnlySound(false);
  SetTransitionInterval(0);
  SetPlayNote(false);
  SetPlayNoteOn(false);
  SetPlayNoteOff(false);

 
// Attacks
// mpFileName[6]="AT_a1_med_mf.sms.sdif";
	
	for (int i=0;i<MAX_SPECTRAL_SEGMENTS;i++)
	  mpFileName[i]=NULL;

//----------------------------------------------------------------------------//
	if (ALTOSAX)
		SetupSaxoDB();
	if (TRUMPET)
		SetupTrumpetDB();
//----------------------------------------------------------------------------//

}

void Parameters::SetupSaxoDB()
{
		// Stationary Template Attributes
	  mpStatTemplAttribute[0]="p F2-A#2";
	  mpStatTemplAttribute[1]="p B2-E3";
	  mpStatTemplAttribute[2]="p F3-A#3";
	  mpStatTemplAttribute[3]="p B#3-F4";
	  mpStatTemplAttribute[4]="f F2-A#2";
	  mpStatTemplAttribute[5]="f B2-E3";
	  mpStatTemplAttribute[6]="f F3-A#3";
	  mpStatTemplAttribute[7]="f B#3-F4";

	// SDIF Files
	// slow ( new: very soft)
	  mpFileName[0]="a_d1_sl.sms.sdif";
	  mpFileName[1]="a_dis1_sl.sms.sdif";
	  mpFileName[2]="a_e1_sl.sms.sdif";
	  mpFileName[3]="a_f1_sl.sms.sdif";
	  mpFileName[4]="a_fis1_sl.sms.sdif";
	  mpFileName[5]="a_g1_sl.sms.sdif";
	  mpFileName[6]="a_gis1_sl.sms.sdif";
	  mpFileName[7]="a_a1_sl.sms.sdif";
	  mpFileName[8]="a_b1_sl.sms.sdif";
	  mpFileName[9]="a_h1_sl.sms.sdif";
	  mpFileName[10]="a_c2_sl.sms.sdif";
	  mpFileName[11]="a_cis2_sl.sms.sdif";
	  mpFileName[12]="a_d2_sl.sms.sdif";
	  mpFileName[13]="a_dis2_sl.sms.sdif";
	  mpFileName[14]="a_e2_sl.sms.sdif";
	  mpFileName[15]="a_f2_sl.sms.sdif";
	  mpFileName[16]="a_fis2_sl.sms.sdif";
	  mpFileName[17]="a_g2_sl.sms.sdif";
	  mpFileName[18]="a_gis2_sl.sms.sdif";
	  mpFileName[19]="a_a2_sl.sms.sdif";
	  mpFileName[20]="a_b2_sl.sms.sdif";
	  mpFileName[21]="a_h2_sl.sms.sdif";
	  mpFileName[22]="a_c3_sl.sms.sdif";
	  mpFileName[23]="a_cis3_sl.sms.sdif";
	  mpFileName[24]="a_d3_sl.sms.sdif";

	// medium short ( new : soft)
	  mpFileName[25]="a_d1_ms.sms.sdif";
	  mpFileName[26]="a_dis1_ms.sms.sdif";
	  mpFileName[27]="a_e1_ms.sms.sdif";
	  mpFileName[28]="a_f1_ms.sms.sdif";
	  mpFileName[29]="a_fis1_ms.sms.sdif";
	  mpFileName[30]="a_g1_ms.sms.sdif";
	  mpFileName[31]="a_gis1_ms.sms.sdif";
	  mpFileName[32]="a_a1_ms.sms.sdif";
	  mpFileName[33]="a_b1_ms.sms.sdif";
	  mpFileName[34]="a_h1_ms.sms.sdif";
	  mpFileName[35]="a_c2_ms.sms.sdif";
	  mpFileName[36]="a_cis2_ms.sms.sdif";
	  mpFileName[37]="a_d2_ms.sms.sdif";
	  mpFileName[38]="a_dis2_ms.sms.sdif";
	  mpFileName[39]="a_e2_ms.sms.sdif";
	  mpFileName[40]="a_f2_ms.sms.sdif";
	  mpFileName[41]="a_fis2_ms.sms.sdif";
	  mpFileName[42]="a_g2_ms.sms.sdif";
	  mpFileName[43]="a_gis2_ms.sms.sdif";
	  mpFileName[44]="a_a2_ms.sms.sdif";
	  mpFileName[45]="a_b2_ms.sms.sdif";
	  mpFileName[46]="a_h2_ms.sms.sdif";
	  mpFileName[47]="a_c3_ms.sms.sdif";
	  mpFileName[48]="a_cis3_ms.sms.sdif";
	  mpFileName[49]="a_d3_ms.sms.sdif";

	// short (new : hard)
	  mpFileName[50]="a_d1_s.sms.sdif";
	  mpFileName[51]="a_dis1_s.sms.sdif";
	  mpFileName[52]="a_e1_s.sms.sdif";
	  mpFileName[53]="a_f1_s.sms.sdif";
	  mpFileName[54]="a_fis1_s.sms.sdif";
	  mpFileName[55]="a_g1_s.sms.sdif";
	  mpFileName[56]="a_gis1_s.sms.sdif";
	  mpFileName[57]="a_a1_s.sms.sdif";
	  mpFileName[58]="a_b1_s.sms.sdif";
	  mpFileName[59]="a_h1_s.sms.sdif";
	  mpFileName[60]="a_c2_s.sms.sdif";
	  mpFileName[61]="a_cis2_s.sms.sdif";
	  mpFileName[62]="a_d2_s.sms.sdif";
	  mpFileName[63]="a_dis2_s.sms.sdif";
	  mpFileName[64]="a_e2_s.sms.sdif";
	  mpFileName[65]="a_f2_s.sms.sdif";
	  mpFileName[66]="a_fis2_s.sms.sdif";
	  mpFileName[67]="a_g2_s.sms.sdif";
	  mpFileName[68]="a_gis2_s.sms.sdif";
	  mpFileName[69]="a_a2_s.sms.sdif";
	  mpFileName[70]="a_b2_s.sms.sdif";
	  mpFileName[71]="a_h2_s.sms.sdif";
	  mpFileName[72]="a_c3_s.sms.sdif";
	  mpFileName[73]="a_cis3_s.sms.sdif";
	  mpFileName[74]="a_d3_s.sms.sdif";

	  mpPitchAttribute[0]="F2";
	  mpPitchAttribute[1]="F#2";
	  mpPitchAttribute[2]="G2";
	  mpPitchAttribute[3]="G#2";
	  mpPitchAttribute[4]="A2";
	  mpPitchAttribute[5]="A#2";
	  mpPitchAttribute[6]="B2";
	  mpPitchAttribute[7]="C3";
	  mpPitchAttribute[8]="C#3";
	  mpPitchAttribute[9]="D3";
	  mpPitchAttribute[10]="D#3";
	  mpPitchAttribute[11]="E3";
	  mpPitchAttribute[12]="F3";
	  mpPitchAttribute[13]="F#3";
	  mpPitchAttribute[14]="G3";
	  mpPitchAttribute[15]="G#3";
	  mpPitchAttribute[16]="A3";
	  mpPitchAttribute[17]="A#3";
	  mpPitchAttribute[18]="B3";
	  mpPitchAttribute[19]="C4";
	  mpPitchAttribute[20]="C#4";
	  mpPitchAttribute[21]="D4";
	  mpPitchAttribute[22]="D#4";
	  mpPitchAttribute[23]="E4";
	  mpPitchAttribute[24]="F4";
		
	  mpAttackAttribute[0]="slow";
	  mpAttackAttribute[1]="medfast";
	  mpAttackAttribute[2]="fast";
	  mpAttackAttribute[4]="BreathOnly";	
}

void Parameters::SetupTrumpetDB()
{
		// Stationary Template Attributes
	  mpStatTemplAttribute[0]="p A#3-D#4";
	  mpStatTemplAttribute[1]="p E4-A4";
	  mpStatTemplAttribute[2]="p A#4-D#5";
	  mpStatTemplAttribute[3]="p E5-A5";
	  mpStatTemplAttribute[4]="f A#3-D#4";
	  mpStatTemplAttribute[5]="f E4-A4";
	  mpStatTemplAttribute[6]="f A#4-D#5";
	  mpStatTemplAttribute[7]="f E5-A5";

	// SDIF Files
	// very soft
	  mpFileName[0]="a_A#3_vs.sms.sdif";
	  mpFileName[1]="a_B3_vs.sms.sdif";
	  mpFileName[2]="a_C4_vs.sms.sdif";
	  mpFileName[3]="a_C#4_vs.sms.sdif";
	  mpFileName[4]="a_D4_vs.sms.sdif";
	  mpFileName[5]="a_D#4_vs.sms.sdif";
	  mpFileName[6]="a_E4_vs.sms.sdif";
	  mpFileName[7]="a_F4_vs.sms.sdif";
	  mpFileName[8]="a_F#4_vs.sms.sdif";
	  mpFileName[9]="a_G4_vs.sms.sdif";
	  mpFileName[10]="a_G#4_vs.sms.sdif";
	  mpFileName[11]="a_A4_vs.sms.sdif";
	  mpFileName[12]="a_A#4_vs.sms.sdif";
	  mpFileName[13]="a_B4_vs.sms.sdif";
	  mpFileName[14]="a_C5_vs.sms.sdif";
	  mpFileName[15]="a_C#5_vs.sms.sdif";
	  mpFileName[16]="a_D5_vs.sms.sdif";
	  mpFileName[17]="a_D#5_vs.sms.sdif";
	  mpFileName[18]="a_E5_vs.sms.sdif";
	  mpFileName[19]="a_F5_vs.sms.sdif";
	  mpFileName[20]="a_F#5_vs.sms.sdif";
	  mpFileName[21]="a_G5_vs.sms.sdif";
	  mpFileName[22]="a_G#5_vs.sms.sdif";
	  mpFileName[23]="a_G#5_vs.sms.sdif";
	  mpFileName[24]="a_G#5_vs.sms.sdif";

	// soft
	  mpFileName[25]="a_A#3_s.sms.sdif";
	  mpFileName[26]="a_B3_s.sms.sdif";
	  mpFileName[27]="a_C4_s.sms.sdif";
	  mpFileName[28]="a_C#4_s.sms.sdif";
	  mpFileName[29]="a_D4_s.sms.sdif";
	  mpFileName[30]="a_D#4_s.sms.sdif";
	  mpFileName[31]="a_E4_s.sms.sdif";
	  mpFileName[32]="a_F4_s.sms.sdif";
	  mpFileName[33]="a_F#4_s.sms.sdif";
	  mpFileName[34]="a_G4_s.sms.sdif";
	  mpFileName[35]="a_G#4_s.sms.sdif";
	  mpFileName[36]="a_A4_s.sms.sdif";
	  mpFileName[37]="a_A#4_s.sms.sdif";
	  mpFileName[38]="a_B4_s.sms.sdif";
	  mpFileName[39]="a_C5_s.sms.sdif";
	  mpFileName[40]="a_C#5_s.sms.sdif";
	  mpFileName[41]="a_D5_s.sms.sdif";
	  mpFileName[42]="a_D#5_s.sms.sdif";
	  mpFileName[43]="a_E5_s.sms.sdif";
	  mpFileName[44]="a_F5_s.sms.sdif";
	  mpFileName[45]="a_F#5_s.sms.sdif";
	  mpFileName[46]="a_G5_s.sms.sdif";
	  mpFileName[47]="a_G#5_s.sms.sdif";
	  mpFileName[48]="a_G#5_s.sms.sdif";
	  mpFileName[49]="a_G#5_s.sms.sdif";

	// hard
 		mpFileName[50]="a_A#3_h.sms.sdif";
	  mpFileName[51]="a_B3_h.sms.sdif";
	  mpFileName[52]="a_C4_h.sms.sdif";
	  mpFileName[53]="a_C#4_h.sms.sdif";
	  mpFileName[54]="a_D4_h.sms.sdif";
	  mpFileName[55]="a_D#4_h.sms.sdif";
	  mpFileName[56]="a_E4_h.sms.sdif";
	  mpFileName[57]="a_F4_h.sms.sdif";
	  mpFileName[58]="a_F#4_h.sms.sdif";
	  mpFileName[59]="a_G4_h.sms.sdif";
	  mpFileName[60]="a_G#4_h.sms.sdif";
	  mpFileName[61]="a_A4_h.sms.sdif";
	  mpFileName[62]="a_A#4_h.sms.sdif";
	  mpFileName[63]="a_B4_h.sms.sdif";
	  mpFileName[64]="a_C5_h.sms.sdif";
	  mpFileName[65]="a_C#5_h.sms.sdif";
	  mpFileName[66]="a_D5_h.sms.sdif";
	  mpFileName[67]="a_D#5_h.sms.sdif";
	  mpFileName[68]="a_E5_h.sms.sdif";
	  mpFileName[69]="a_F5_h.sms.sdif";
	  mpFileName[70]="a_F#5_h.sms.sdif";
	  mpFileName[71]="a_G5_h.sms.sdif";
	  mpFileName[72]="a_G#5_h.sms.sdif";
	  mpFileName[73]="a_G#5_h.sms.sdif";
	  mpFileName[74]="a_G#5_h.sms.sdif";

	 // very hard
	  mpFileName[75]="a_A#3_vh.sms.sdif";
	  mpFileName[76]="a_B3_vh.sms.sdif";
	  mpFileName[77]="a_C4_vh.sms.sdif";
	  mpFileName[78]="a_C#4_vh.sms.sdif";
	  mpFileName[79]="a_D4_vh.sms.sdif";
	  mpFileName[80]="a_D#4_vh.sms.sdif";
	  mpFileName[81]="a_E4_vh.sms.sdif";
	  mpFileName[82]="a_F4_vh.sms.sdif";
	  mpFileName[83]="a_F#4_vh.sms.sdif";
	  mpFileName[84]="a_G4_vh.sms.sdif";
	  mpFileName[85]="a_G#4_vh.sms.sdif";
	  mpFileName[86]="a_A4_vh.sms.sdif";
	  mpFileName[87]="a_A#4_vh.sms.sdif";
	  mpFileName[88]="a_B4_vh.sms.sdif";
	  mpFileName[89]="a_C5_vh.sms.sdif";
	  mpFileName[90]="a_C#5_vh.sms.sdif";
	  mpFileName[91]="a_D5_vh.sms.sdif";
	  mpFileName[92]="a_D#5_vh.sms.sdif";
	  mpFileName[93]="a_E5_vh.sms.sdif";
	  mpFileName[94]="a_F5_vh.sms.sdif";
	  mpFileName[95]="a_F#5_vh.sms.sdif";
	  mpFileName[96]="a_G5_vh.sms.sdif";
	  mpFileName[97]="a_G#5_vh.sms.sdif";
	  mpFileName[98]="a_G#5_vh.sms.sdif";
	  mpFileName[99]="a_G#5_vh.sms.sdif";

	  mpPitchAttribute[0]="A#2";
	  mpPitchAttribute[1]="B2";
	  mpPitchAttribute[2]="C3";
	  mpPitchAttribute[3]="C#3";
	  mpPitchAttribute[4]="D3";
	  mpPitchAttribute[5]="D#3";
	  mpPitchAttribute[6]="E3";
	  mpPitchAttribute[7]="F3";
	  mpPitchAttribute[8]="F#3'";
	  mpPitchAttribute[9]="G3";
	  mpPitchAttribute[10]="G#3";
	  mpPitchAttribute[11]="A3";
	  mpPitchAttribute[12]="A#3";
	  mpPitchAttribute[13]="B3";
	  mpPitchAttribute[14]="C4";
	  mpPitchAttribute[15]="C#4";
	  mpPitchAttribute[16]="D4";
	  mpPitchAttribute[17]="D#4";
	  mpPitchAttribute[18]="E4";
	  mpPitchAttribute[19]="F4";
	  mpPitchAttribute[20]="F#4";
	  mpPitchAttribute[21]="G4";
	  mpPitchAttribute[22]="G#4";
	  mpPitchAttribute[23]="A4";
	  mpPitchAttribute[24]="A#4";

	  mpAttackAttribute[0]="vSoft";
	  mpAttackAttribute[1]="soft";
	  mpAttackAttribute[2]="hard";
//	  mpAttackAttribute[3]="vHard";
	//  mpAttackAtribute[4]="BreathOnly";	
}

CSaltoSegData Parameters::GetSegDataByPos(TIndex pos)
{
  if (pos>=0 && pos <SPECTRAL_SEGMENTS_IN_USE)
    return mSegData[pos];
  else
    return mSegData[0]; // return no error ! Check when this happens !!!!
    //CLAM_ASSERT(false,"CSaltoDataManagment::GetSegDataByPos out of Bounds ");
}

//----------------------------------------------------------------------------//

void Parameters::SetSegDataByPos(TIndex pos,CSaltoSegData segData)
{
  if (pos>=0 && pos <SPECTRAL_SEGMENTS_IN_USE)
    mSegData[pos] = segData;
  else
    CLAM_ASSERT(false,"CSaltoDataManagment::SetSegDataByPos out of Bounds");
}
//----------------------------------------------------------------------------//
// there is a much nicer way to do this, but right now we can't save template
// arrays with template arrays inside. And I want to store the stat templates !
// This is the way around for now. later this could be brought as a proper
// structure ´¿

SpectralPeakArray* Parameters::GetCurrentStatTemplatePtr()
{
//  switch (mCurrentStatTemplate)
  switch (GetCurrentStatTemplate())
  {
    case 0:
    {
      return &mCurrentStatTmpl0;
      break;
    }
    case 1:
    {
      return &mCurrentStatTmpl1;
      break;
    }
    case 2:
    {
      return &mCurrentStatTmpl2;
      break;
    }
    case 3:
    {
      return &mCurrentStatTmpl3;
      break;
    }
    case 4:
    {
      return &mCurrentStatTmpl4;
      break;
    }
    case 5:
    {
      return &mCurrentStatTmpl5;
      break;
    }
    case 6:
    {
      return &mCurrentStatTmpl6;
      break;
    }
    case 7:
    default:
    {
      return &mCurrentStatTmpl7;
      break;
    }
  }
}
//----------------------------------------------------------------------------//
// ...the same as for 'GetCurrentStatTemplatePtr' ... ´¿
double Parameters::GetCurrentStatTemplateFundFreq()
{
  switch (GetCurrentStatTemplate())
  {
    case 0:
    {
      return mCurrentStatFundFreq0;
      break;
    }
    case 1:
    {
      return mCurrentStatFundFreq1;
      break;
    }
    case 2:
    {
      return mCurrentStatFundFreq2;
      break;
    }
    case 3:
    {
      return mCurrentStatFundFreq3;
      break;
    }
    case 4:
    {
      return mCurrentStatFundFreq4;
      break;
    }
    case 5:
    {
      return mCurrentStatFundFreq5;
      break;
    }
    case 6:
    {
      return mCurrentStatFundFreq6;
      break;
    }
    case 7:
    default:
    {
      return mCurrentStatFundFreq7;
      break;
    }
  }
}

//----------------------------------------------------------------------------//
const char* Parameters::GetFilePathOfSegmentPos(TIndex pos)
{
  strcpy(mpPathName,SDIF_ATTACKS_FOLDER);
  if (pos>=0 && pos<25)
  {
    strcat(mpPathName,VERYSOFT_ATTACKS);
  }
  else if (pos>=25 && pos<50)
  {
    strcat(mpPathName,SOFT_ATTACKS);
  }
  else if (pos>=50 && pos<75)
  {
    strcat(mpPathName,HARD_ATTACKS);
  }
   else if (pos>=75 && pos<100)
  {
    strcat(mpPathName,VERYHARD_ATTACKS);
  }

  return mpPathName;
}





} // end of namespace CLAM

void InstrumentValues::Instantiate(int instr,char* dataFolder)
{
	_GetInstance(instr,dataFolder);
}

InstrumentValues& InstrumentValues::GetInstance(void)
{
	return _GetInstance(-1,0);
}

InstrumentValues& InstrumentValues::_GetInstance(int instr,char* dataFolder)
{
	static InstrumentValues instance;
	if (instance.mInstrument==-1)
	{
		instance.mInstrument = instr;
		if (instr==-1)
			CLAM_ASSERT(false,"InstrumentValues has not been instantiated yet\n");
		else if (instr==0)
		{
    	strcpy(instance.mSALTO_DATA_FILE_NAME,dataFolder);
			strcat(instance.mSALTO_DATA_FILE_NAME,"/AltoSax/SALTO_DATA_FILE");
    	strcpy(instance.mSALTO_DATA_TXT_FNAME,dataFolder);
			strcat(instance.mSALTO_DATA_TXT_FNAME,"/AltoSax/SALTO_DATA_FILE.txt");
    	strcpy(instance.mSALTO_STAT_TMPL_FILE_NAME,dataFolder);
			strcat(instance.mSALTO_STAT_TMPL_FILE_NAME,"/AltoSax/SALTO_STAT_TMPL_FILE");
    	strcpy(instance.mSALTO_STAT_TMPL_TXT_FNAME,dataFolder);
			strcat(instance.mSALTO_STAT_TMPL_TXT_FNAME,"/AltoSax/SALTO_STAT_TMPL_FILE.txt");
    	strcpy(instance.mSALTO_TEST_RESIDUAL_NAME,dataFolder);
			strcat(instance.mSALTO_TEST_RESIDUAL_NAME,"/AltoSax/statresidual.sms.sdif"); //"RT_a1_mf_2.sms.sdif"
    	strcpy(instance.mSDIF_ATTACKS_FOLDER,dataFolder);
			strcat(instance.mSDIF_ATTACKS_FOLDER,"/AltoSax/SDIFAttacks/");
    	instance.mVERYSOFT_ATTACKS =
				"slow(sl)/";
    	instance.mSOFT_ATTACKS =
				"mediumshort(ms)/";
    	instance.mHARD_ATTACKS =
				"short(s)/";
    	instance.mVERYHARD_ATTACKS =
				"short(s)/"; // doesnt exist

			instance.mMAX_SPECTRAL_SEGMENTS			= 100;	// spec Segments read from SDIF
			instance.mSPECTRAL_SEGMENTS_IN_USE  = 75;   // current number of SpecSegs in memory
			instance.mMAX_PITCH_VALUE						= 25;
			instance.mMAX_ATTACKS_VALUE         = 3;		//4 for breath-only
			instance.mVSOFT                     = 0;    // define depends on MAX_ATTACKS_VALUE
			instance.mSOFT                      = 1;    // define depends on MAX_ATTACKS_VALUE
			instance.mHARD                      = 1;    // define depends on MAX_ATTACKS_VALUE
			instance.mVHARD				              = 2;    // define depends on MAX_ATTACKS_VALUE
			instance.mBREATHONLY                = 3;
			instance.mMAX_SMOOTHNESS_VALUE			= 1;  
			instance.mLOWEST_PITCH              = 174.61; // F2 
		}else if (instr==1)
		{
    	strcpy(instance.mSALTO_DATA_FILE_NAME      ,dataFolder);
			strcat(instance.mSALTO_DATA_FILE_NAME      ,"/Trumpet/SALTO_DATA_FILE");
    	strcpy(instance.mSALTO_DATA_TXT_FNAME      ,dataFolder);
			strcat(instance.mSALTO_DATA_TXT_FNAME      ,"/Trumpet/SALTO_DATA_FILE.txt");
    	strcpy(instance.mSALTO_STAT_TMPL_FILE_NAME,dataFolder);
			strcat(instance.mSALTO_STAT_TMPL_FILE_NAME,"/Trumpet/SALTO_STAT_TMPL_FILE");
    	strcpy(instance.mSALTO_STAT_TMPL_TXT_FNAME,dataFolder);
			strcat(instance.mSALTO_STAT_TMPL_TXT_FNAME,"/Trumpet/SALTO_STAT_TMPL_FILE.txt");
    	strcpy(instance.mSALTO_TEST_RESIDUAL_NAME ,dataFolder);
			strcat(instance.mSALTO_TEST_RESIDUAL_NAME ,"/Trumpet/statresidual.sms.sdif"); //"RT_a1_mf_2.sms.sdif"
    	strcpy(instance.mSDIF_ATTACKS_FOLDER      ,dataFolder);
			strcat(instance.mSDIF_ATTACKS_FOLDER      ,"/Trumpet/SDIFAttacks/");
    	instance.mVERYSOFT_ATTACKS          =
				"vs/";
    	instance.mSOFT_ATTACKS              =
				"s/";
    	instance.mHARD_ATTACKS              =
				"h/";
    	instance.mVERYHARD_ATTACKS          =
				"vh/";
  		instance.mMAX_SPECTRAL_SEGMENTS			= 100;		// spec Segments read from SDIF;
  		instance.mSPECTRAL_SEGMENTS_IN_USE  = 100;    // current number of SpecSegs in memory;
  		instance.mMAX_PITCH_VALUE						= 25;
  		instance.mMAX_ATTACKS_VALUE         = 4; 			//5 for breath-only;
  		instance.mVSOFT                     = 0;      // define depends on MAX_ATTACKS_VALUE;
  		instance.mSOFT                      = 1;      // define depends on MAX_ATTACKS_VALUE;
  		instance.mHARD                      = 2;      // define depends on MAX_ATTACKS_VALUE ;
  		instance.mVHARD				              = 3;      // define depends on MAX_ATTACKS_VALUE;
  		instance.mBREATHONLY                = 4;
  		instance.mMAX_SMOOTHNESS_VALUE			= 1;
  		instance.mLOWEST_PITCH              = 233.08;  // A#2;
		}else{
			CLAM_ASSERT(false,"Invalid instrument while instatiating InstrumentValues\n");
		}
	}
	return instance;
}
