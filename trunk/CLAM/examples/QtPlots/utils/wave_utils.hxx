#ifndef __WAVE_UTILS__
#define __WAVE_UTILS__

#include "Audio.hxx"

using CLAM::TData;
using CLAM::Audio;

namespace qtvm_examples_utils
{
    void chirp(const double& freq0, const double& freq1, const double& duration, const double& sampleRate, Audio& out);
    void sine(const TData& freq, const TData& amplitude, const TData& duration, const TData& sampleRate, Audio& out);
}

#endif
