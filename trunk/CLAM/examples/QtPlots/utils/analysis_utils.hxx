#ifndef __ANALYSIS_UTILS__
#define __ANALYSIS_UTILS__

#include "Segment.hxx"

using CLAM::Audio;
using CLAM::Segment;
using CLAM::DataArray;

namespace qtvm_examples_utils
{
    void analyze(const Audio& in, Segment& out); 
    void energy(const Audio& in, DataArray& out);
}

#endif

