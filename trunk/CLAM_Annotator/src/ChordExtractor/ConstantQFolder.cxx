#include <iostream>
#include <cmath>
#include "ConstantQFolder.hxx"

namespace Simac
{

/**
 * returns the absolute value of complex number real + i*imag
 */
static double complexModule(const double & real, const double & imag)
{
	return std::sqrt(real*real + imag*imag);
}

ConstantQFolder::ConstantQFolder(unsigned nConstantQBins, int binsPerOctave) 
	: _binsPerOctave(binsPerOctave)
	, _nConstantQBins(nConstantQBins)
{
	_chromadata.resize(_binsPerOctave);
}

ConstantQFolder::~ConstantQFolder()
{
}

void ConstantQFolder::doIt(const std::vector<double> & constantQData)
{
	//initialise _chromadata to 0
	for (unsigned i=0; i<_binsPerOctave; i++) _chromadata[i]=0;

	// add each octave of cq data into chromagram
	const unsigned nOctaves = (int)floor(double(_nConstantQBins/_binsPerOctave))-1;
	unsigned constantQBin = 0;
	for (unsigned octave=0; octave<=nOctaves; octave++) {
		for (unsigned i=0; i<_binsPerOctave; i++) {
			const double & real = constantQData[constantQBin++];
			const double & imag = constantQData[constantQBin++];
			_chromadata[i] += complexModule(real, imag);
		}
	}
}

}

