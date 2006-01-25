#ifndef __AUDIO_FILE_UTILS__
#define __AUDIO_FILE_UTILS__

#include <vector>
#include "Audio.hxx"

using CLAM::Audio;

namespace qtvm_examples_utils
{
    int load_audio(const char* fileName,Audio& out);
    int load_audio_st(const char* fileName,std::vector<Audio>& outputs);
}

#endif

