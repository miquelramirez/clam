#ifndef _SALTO_DEFINES_
#define _SALTO_DEFINES_

//#define AUDIO_TO_FILE
#define MONO
//#define CORRECT_PITCH
//#define DEMO_VERSION

// file

#define ALTOSAX
//#define TRUMPET

#ifdef WIN32
	#define SALTO_DATA_DIR "c:"
#else
	#define SALTO_DATA_DIR ""
#endif

  #ifdef ALTOSAX
	
    #define SALTO_DATA_FILE_NAME      SALTO_DATA_DIR"/SaltoDataFolder/AltoSax/SALTO_DATA_FILE"
    #define SALTO_DATA_TXT_FNAME      SALTO_DATA_DIR"/SaltoDataFolder/AltoSax/SALTO_DATA_FILE.txt"
    #define SALTO_STAT_TMPL_FILE_NAME SALTO_DATA_DIR"/SaltoDataFolder/AltoSax/SALTO_STAT_TMPL_FILE"
    #define SALTO_STAT_TMPL_TXT_FNAME SALTO_DATA_DIR"/SaltoDataFolder/AltoSax/SALTO_STAT_TMPL_FILE.txt"
    #define SALTO_TEST_RESIDUAL_NAME  SALTO_DATA_DIR"/SaltoDataFolder/AltoSax/statresidual.sms.sdif" //"RT_a1_mf_2.sms.sdif"
    #define SDIF_ATTACKS_FOLDER       SALTO_DATA_DIR"/SaltoDataFolder/AltoSax/SDIFAttacks/"
    #define VERYSOFT_ATTACKS          "slow(sl)/"
    #define SOFT_ATTACKS              "mediumshort(ms)/"
    #define HARD_ATTACKS              "short(s)/"
    #define VERYHARD_ATTACKS          "short(s)/" // doesnt exist
//    #define SHORT_ATTACKS             SALTO_DATA_DIR"/temp/salto/SALTOApplication/SaltoDataFolder/short(s)/"
//    #define MEDIUM_SHORT_ATTACKS      SALTO_DATA_DIR"/temp/salto/SALTOApplication/SaltoDataFolder/mediumshort(ms)/"
//    #define SLOW_ATTACKS              SALTO_DATA_DIR"/temp/salto/SALTOApplication/SaltoDataFolder/slow(sl)/"
  #endif //ALTOSAX
  
  #ifdef TRUMPET
    #define SALTO_DATA_FILE_NAME      SALTO_DATA_DIR"/SaltoDataFolder/Trumpet/SALTO_DATA_FILE"
    #define SALTO_DATA_TXT_FNAME      SALTO_DATA_DIR"/SaltoDataFolder/Trumpet/SALTO_DATA_FILE.txt"
    #define SALTO_STAT_TMPL_FILE_NAME SALTO_DATA_DIR"/SaltoDataFolder/Trumpet/SALTO_STAT_TMPL_FILE"
    #define SALTO_STAT_TMPL_TXT_FNAME SALTO_DATA_DIR"/SaltoDataFolder/Trumpet/SALTO_STAT_TMPL_FILE.txt"
    #define SALTO_TEST_RESIDUAL_NAME  SALTO_DATA_DIR"/SaltoDataFolder/Trumpet/statresidual.sms.sdif" //"RT_a1_mf_2.sms.sdif"
    #define SDIF_ATTACKS_FOLDER       SALTO_DATA_DIR"/SaltoDataFolder/Trumpet/SDIFAttacks/"
    #define VERYSOFT_ATTACKS          "vs/"
    #define SOFT_ATTACKS              "s/"
    #define HARD_ATTACKS              "h/"
    #define VERYHARD_ATTACKS          "vh/"
  #endif //TRUMPET

//#endif //LINUX

                                   
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

#ifdef ALTOSAX  
  #define MAX_SPECTRAL_SEGMENTS			100			// spec Segments read from SDIF
  #define SPECTRAL_SEGMENTS_IN_USE  75      // current number of SpecSegs in memory
  #define MAX_PITCH_VALUE						25
  #define MAX_ATTACKS_VALUE         3 			//4 for breath-only
  #define VSOFT                     0       // define depends on MAX_ATTACKS_VALUE
  #define SOFT                      1       // define depends on MAX_ATTACKS_VALUE
  #define HARD                      1       // define depends on MAX_ATTACKS_VALUE
  #define VHARD				              2       // define depends on MAX_ATTACKS_VALUE
//  #define BREATHONLY                3
  #define MAX_SMOOTHNESS_VALUE			1
  #define LOWEST_PITCH              174.61  // F2 
#endif

#ifdef TRUMPET 
  #define	MAX_SPECTRAL_SEGMENTS			100			// spec Segments read from SDIF
  #define SPECTRAL_SEGMENTS_IN_USE  100     // current number of SpecSegs in memory
  #define MAX_PITCH_VALUE						25
  #define MAX_ATTACKS_VALUE         4 			//5 for breath-only
  #define VSOFT                     0       // define depends on MAX_ATTACKS_VALUE
  #define SOFT                      1       // define depends on MAX_ATTACKS_VALUE
  #define HARD                      2       // define depends on MAX_ATTACKS_VALUE 
  #define VHARD				              3       // define depends on MAX_ATTACKS_VALUE
//  #define BREATHONLY                4
  #define MAX_LOUDNESS_VALUE				1
  #define MAX_SMOOTHNESS_VALUE			1
  #define LOWEST_PITCH              233.08  // A#2
#endif

// math constants
#define HALFPI    (PI/2)

// global macros
#define MIN(a,b)      ((a <= b) ? (a) : (b))
#define MAX(a,b)      ((a >= b) ? (a) : (b))
#define CLIP(a,b,c)   ((a<=b)?(b):(a>=c)?(c):(a))

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
