#ifndef AudioOutFilename_hxx
#define AudioOutFilename_hxx

#include "Filename.hxx"

namespace CLAM
{

class AudioOutFilename : public OutFilename
{
public:
	AudioOutFilename(const std::string & s="") : OutFilename(s) {}
	AudioOutFilename(const char * s) : OutFilename(s) {}
	virtual const char * TypeFamily() const { return "Audio"; }
	virtual const Filter * Filters() const
	{
		static const Filter filters[] = {
			{"Microsoft RIFF/WAVE files",        "*.wav"},
			{"Vorbis I files (Ogg/Vorbis)",      "*.ogg"},
	//		{"Mpeg Audio Layer 1",               "*.mp1 *.mpg"}, // Write not supported
	//		{"Mpeg Audio Layer 2",               "*.mp2 *.mpg"}, // Write not supported
	//		{"Mpeg Audio Layer 3",               "*.mp3 *.mpg"}, // Write not supported
			{"Apple/SGI AIFF files",             "*.aiff *.aif"},
			{"Sun/Next AU files",                "*.snd *.au"},
			{"RAW PCM files",                    "*.raw"},
			{"Ensoniq's PARIS Files",            "*.paf"},
			{"Amiga IFF files",                  "*.svx"},
			{"Sphere NIST files",                "*.nist"},
			{"Creative's VOC files",             "*.voc"},
			{"Berkeley/IRCAM/CARL files",        "*.ircam"},
			{"Sonic Foundry's 64 RIFF/WAV files","*.w64"},
			{"Matlab/GNU Octave files",          "*.mat4 *.mat5 *.mat"},
			{0,0}
		};
		return filters;
	}
};
CLAM_TYPEINFOGROUP(BasicCTypeInfo, AudioOutFilename);

}



#endif//AudioInFilename_hxx

