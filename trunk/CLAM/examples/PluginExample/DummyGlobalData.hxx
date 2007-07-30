#ifndef DummyGlobalData_hxx
#define DummyGlobalData_hxx

#include "ComplexSpectrum.hxx"
#include <vector>

namespace CLAM
{

class DummyGlobalData
{
	typedef std::vector<ComplexSpectrum> Spectrums;
	static Spectrums * _responseSpectrums;

public:
	void static SetStaticResponseSpectrums(Spectrums * spectrums);
	static std::vector<ComplexSpectrum> & GetStaticResponseSpectrums();

};
} //namespace

#endif //DummyGlobalData_hxx

