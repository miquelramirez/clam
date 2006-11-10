#ifndef PARAMS
#define PARAMS
#include <math.h>

/* Spectrum analysis parameters */

#define kDR 	 	2       // Decimation ratio applied
#define kRES	 	8       // winsize=winsize*res Zeroes are added at the end for getting more frequency resolution
#define kPERCENTAGE	50	// Overlapping percentage
#define kN		2048	// Input window size
#define DIVFACTOR	2147483000.0	// 2^21 Divide input (time-signal) between this value before FFT computation

/* Sinusoidal likeness parameters */

#define kSINUSOIDAL	true	// If true, sinusoidal peak picking is activated. If false, normal spectrum peak piking will be computed 
#define kW		200.0	// Bandwidth (Hz) for sinusoidal likeness
#define kMINPEAKVALUE	0.005	// 0.4 Minimum frequency spectrum value for considering as a peak.
#define kMINSINLIKEVAL  0.1	// 0.1 Minimum sinusoidal likeness value for considering peaks. Not used if kSINUSOIDAL=false 

/* Processing parameters */

#define AVERAGE		3
#define MAVP		0.01

#define kHARMONICS	30	// Max number of harmonics of a spectral pattern
#define LIMSPSEARCH	5	// Max time frames analysed in order to get the best spectral pattern, since the last best pattern was found
#define ERRORALLOWED	0.5	// Error allowed for matching a harmonic (0.3=30 %)
#define HARMONICSALLOWED 0.2	// Maximum number of missing harmonics allowed in processing (0.3=30%)
#define STABLEFRAMES	0	// Peak picking is done n frames after change (if possible).
#define kMINSPLOUD 	4	// Minimum loudness of a spectral pattern for apply it to the whole melody

#define kLOUDNESS  	0.1	// Minimum note loudness relative to the maximum note in surrounding area
#define kMINLOUDNESS	2	// Minimum loudness for a note
#define kDISCARDH	0.00	// If 0.1, harmonics lower to 10% of maximum spectral pattern value will be discarded

/* Parameters for onset detection */

#define kSILENCE	7.0     // Silence limit
#define NEWPAR		0.18	// Threshold at the end of onset detection stage
#define NEWPARCONTEXT	0.2

/* Band parameters */

#define kMINBANDFREQ	48.9994087219		// MidiNote= G0 (1st note of piano-roll)
const double stepinterbands=pow(2.0,1.0/24.0);	// Bands separation (Hz)
const double stepinterharm=pow(2.0,1.0/48.0);
const double freqrange=pow(2.0,1.0/72.0);

/* Notes array (used only for debugging) */

static const char *Notas[]={"0_SOL","0_SOL#",
		"0_LA","0_LA#","0_SI","1_DO","1_DO#","1_RE","1_RE#","1_MI","1_FA","1_FA#","1_SOL","1_SOL#",
		"1_LA","1_LA#","1_SI","2_DO","2_DO#","2_RE","2_RE#","2_MI","2_FA","2_FA#","2_SOL","2_SOL#",
		"2_LA","2_LA#","2_SI","3_DO","3_DO#","3_RE","3_RE#","3_MI","3_FA","3_FA#","3_SOL","3_SOL#",
		"3_LA","3_LA#","3_SI","4_DO","4_DO#","4_RE","4_RE#","4_MI","4_FA","4_FA#","4_SOL","4_SOL#",
		"4_LA","4_LA#","4_SI","5_DO","5_DO#","5_RE","5_RE#","5_MI","5_FA","5_FA#","5_SOL","5_SOL#",
		"5_LA","5_LA#","5_SI","6_DO","6_DO#","6_RE","6_RE#","6_MI","6_FA","6_FA#","6_SOL","6_SOL#",
		"6_LA","6_LA#","6_SI","7_DO","7_DO#","7_RE","7_RE#","7_MI","7_FA","7_FA#","7_SOL","7_SOL#",
		"7_LA","7_LA#","7_SI","8_DO","8_DO#","8_RE","8_RE#","8_MI"
};


#endif
