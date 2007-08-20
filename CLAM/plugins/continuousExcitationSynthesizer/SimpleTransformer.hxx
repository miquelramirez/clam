#ifndef _SimpleTransformer_
#define _SimpleTransformer_

#include "Frame.hxx"

namespace CLAM
{

/**
* This class can be used to transform the contents of a given frame to fit
* a desired value for pitch or amplitude.
*
* \author greg kellum [gkellum@iua.upf.edu] 7/15/07
* \since  CLAM v1.1.
*/
class SimpleTransformer
{
public:
	SimpleTransformer();
	~SimpleTransformer();

	/**
	* This method transforms the frequency of a frame from that of the source
	* frequency to that of the target frequency.  It does this my multiplying
	* the spectral peaks of the frame by a scaling factor. The residual portion
	* of the signal is left untouched.
	*/
	void TransformFrequency(Frame* inFrame, float sourceFrequency, float targetFrequency);

	/**
	* This method transforms the amplitude of a frame from that of the source
	* amplitude to that of the target amplitude.  It does this my multiplying
	* the magnitudes of the spectral peaks of the frame by a scaling factor.
	*/
	void TransformAmplitude(Frame* inFrame, float sourceAmplitude, float targetAmplitude);

private:
};

} // end namespace CLAM

#endif
