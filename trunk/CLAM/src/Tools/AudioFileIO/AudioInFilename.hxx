#ifndef AudioInFilename_hxx
#define AudioInFilename_hxx

#include "Filename.hxx"

namespace CLAM
{

class AudioInFilename : public InFilename
{
public:
	AudioInFilename(const std::string & s="") : InFilename(s) {}
	AudioInFilename(const char * s) : InFilename(s) {}
	virtual const char * TypeFamily() const { return "Image"; }
	virtual const Filter * Filters() const
	{
		static const Filter filters[] = {
			{"Microsoft RIFF/WAVE files",        "*.wav"},
			{"Vorbis I files (Ogg/Vorbis)",      "*.ogg"},
			{"Mpeg Audio Layer 1",               "*.mp1 *.mpg"},
			{"Mpeg Audio Layer 2",               "*.mp2 *.mpg"},
			{"Mpeg Audio Layer 3",               "*.mp3 *.mpg"},
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

CLAM_TYPEINFOGROUP(BasicCTypeInfo, AudioInFilename);

}



#endif//AudioInFilename_hxx

