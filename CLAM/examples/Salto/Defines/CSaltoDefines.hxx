#ifndef _SALTO_DEFINES_
#define _SALTO_DEFINES_

//#define AUDIO_TO_FILE
#define MONO
//#define CORRECT_PITCH
//#define DEMO_VERSION

// file

class InstrumentValues
{
public:
	char mSALTO_DATA_FILE_NAME[1024];
	char mSALTO_DATA_TXT_FNAME[1024];
	char mSALTO_STAT_TMPL_FILE_NAME[1024];
	char mSALTO_STAT_TMPL_TXT_FNAME[1024];
	char mSALTO_TEST_RESIDUAL_NAME[1024];
	char mSDIF_ATTACKS_FOLDER[1024];
	char* mVERYSOFT_ATTACKS;
	char* mSOFT_ATTACKS;
	char* mHARD_ATTACKS;
	char* mVERYHARD_ATTACKS;
	int   mMAX_SPECTRAL_SEGMENTS;
	int   mSPECTRAL_SEGMENTS_IN_USE;
	int   mMAX_PITCH_VALUE;
	int   mMAX_ATTACKS_VALUE;
	int   mVSOFT;
	int   mSOFT;
	int   mHARD;
	int   mVHARD;
	int   mBREATHONLY;
	int   mMAX_SMOOTHNESS_VALUE;
	float mLOWEST_PITCH;
	
	static InstrumentValues& GetInstance(void);
	static void Instantiate(int instr,char* dataFolder);

	InstrumentValues()
	{
		mInstrument = -1;
	}

	bool IsALTOSAX(void) { return mInstrument == 0; }
	bool IsTRUMPET(void) { return mInstrument == 1; }

	int   mInstrument;
private:
	static InstrumentValues& _GetInstance(int instr,char* dataFolder);
};

#define SALTO_DATA_FILE_NAME InstrumentValues::GetInstance().mSALTO_DATA_FILE_NAME
#define SALTO_DATA_TXT_FNAME InstrumentValues::GetInstance().mSALTO_DATA_TXT_FNAME
#define SALTO_STAT_TMPL_FILE_NAME InstrumentValues::GetInstance().mSALTO_STAT_TMPL_FILE_NAME
#define SALTO_STAT_TMPL_TXT_FNAME InstrumentValues::GetInstance().mSALTO_STAT_TMPL_TXT_FNAME
#define SALTO_TEST_RESIDUAL_NAME InstrumentValues::GetInstance().mSALTO_TEST_RESIDUAL_NAME
#define SDIF_ATTACKS_FOLDER InstrumentValues::GetInstance().mSDIF_ATTACKS_FOLDER
#define VERYSOFT_ATTACKS InstrumentValues::GetInstance().mVERYSOFT_ATTACKS
#define SOFT_ATTACKS InstrumentValues::GetInstance().mSOFT_ATTACKS
#define HARD_ATTACKS InstrumentValues::GetInstance().mHARD_ATTACKS
#define VERYHARD_ATTACKS InstrumentValues::GetInstance().mVERYHARD_ATTACKS

#define MAX_SPECTRAL_SEGMENTS (InstrumentValues::GetInstance().mMAX_SPECTRAL_SEGMENTS)
#define SPECTRAL_SEGMENTS_IN_USE (InstrumentValues::GetInstance().mSPECTRAL_SEGMENTS_IN_USE)
#define MAX_PITCH_VALUE (InstrumentValues::GetInstance().mMAX_PITCH_VALUE)
#define MAX_ATTACKS_VALUE (InstrumentValues::GetInstance().mMAX_ATTACKS_VALUE)
#define VSOFT (InstrumentValues::GetInstance().mVSOFT)
#define SOFT (InstrumentValues::GetInstance().mSOFT)
#define HARD (InstrumentValues::GetInstance().mHARD)
#define VHARD (InstrumentValues::GetInstance().mVHARD)
#define BREATHONLY (InstrumentValues::GetInstance().mBREATHONLY)
#define MAX_SMOOTHNESS_VALUE (InstrumentValues::GetInstance().mMAX_SMOOTHNESS_VALUE)
#define LOWEST_PITCH (InstrumentValues::GetInstance().mLOWEST_PITCH)

#define ALTOSAX (InstrumentValues::GetInstance().IsALTOSAX())
#define TRUMPET (InstrumentValues::GetInstance().IsTRUMPET())

// math constants
#define HALFPI    (PI/2)

// global macros
#undef MIN
#undef MAX
#define MIN(a,b)      ((a <= b) ? (a) : (b))
#define MAX(a,b)      ((a >= b) ? (a) : (b))
#define CLIP(a,b,c)   ((a<=b)?(b):(a>=c)?(c):(a))

//  GENERAL DEFINES
#define MIDI_DEVICE_NR            0       	// 0 is first input port in OMS
#define MIDI_EXT_CTRL_RANGE       127     	// resolution of external controller
#define MIDI_EXT_CTRL_OFFSET      64      	//
#define MAX_STAT_TEMPLATES        8
#define MAX_SINES									200				// max number of Peaks/Sines
//#define OSCILLOSCOPE_REFRESH_TIME 0.015 	// in secs
#define OSCILLOSCOPE_REFRESH_TIME 0.04    	// in secs = 25 frames/sec
//#define OSCILLOSCOPE_REFRESH_TIME 0.05    	// in secs = 20 frames/sec
//#define OSCILLOSCOPE_REFRESH_TIME 0.1    	// in secs = 10 frames/sec


// FlowControl Enums
enum
{
  FLOW_INTERPOLATE = 0,
  FLOW_GENERATE_SINE,
  FLOW_FOLLOW_TRACK,
  FLOW_SDIF_EDIT,
  NO_TRANS_MODE=0,
  INIT_TRANS_MODE,
  DO_TRANSITION
};
#endif




