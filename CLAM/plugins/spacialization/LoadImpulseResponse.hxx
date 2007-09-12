#ifndef LoadImpulseResponse_hxx
#define LoadImpulseResponse_hxx

#include "ComplexSpectrum.hxx"
#include <string>
#include <vector>

namespace CLAM
{

bool ComputeResponseSpectrums(const std::string & wavfile, std::vector<ComplexSpectrum> & responseSpectrums, unsigned framesize, std::string & errorMsg);

}
#endif

