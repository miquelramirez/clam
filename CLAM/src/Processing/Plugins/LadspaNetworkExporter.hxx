#ifndef LadspaNetworkExporter_hxx
#define LadspaNetworkExporter_hxx

#include <ladspa.h>
#include <cstring>
#include <cstdlib>
#include <iostream>

#include "Network.hxx"
#include "PushFlowControl.hxx"
#include "XMLStorage.hxx"
#include "AudioSource.hxx"
#include "AudioSink.hxx"
#include "ControlSource.hxx"
#include "ControlSink.hxx"
#include "LadspaLibrary.hxx"

/// Include a given file as binary data
/// @arg varname is the variable that will hold the var (const char[])
/// @arg file is an absolute path or a relative path from the build point (not the source file!!)
/// @todo Try to make CLAM_EXTERNAL_FILE_DATA work with path relatives to the source that uses the macro.
#define CLAM_EXTERNAL_FILE_DATA(varname, file) \
asm ( \
"  .section \".rodata\"\n" \
"   .type " #varname ", @object\n" \
" " #varname ":\n" \
"   .incbin \"" file "\"\n" \
"   .byte 0\n" \
"   .size " #varname ", .-" #varname "\n" \
); \
extern const char varname[]; \

namespace CLAM
{

class LadspaNetworkExporter
{
public:
	LadspaNetworkExporter(LadspaLibrary & library, const std::string & networkXmlContent,
		unsigned id,
		const std::string & label,
		const std::string & name,
		const std::string & maker,
		const std::string & copyright
		);
};

} //namespace CLAM


#endif//LadspaNetworkExporter_hxx

