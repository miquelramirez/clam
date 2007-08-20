#ifndef _DataUtil_
#define _DataUtil_

#include "Frame.hxx"
#include "SpectralPeakArray.hxx"
#include "Spectrum.hxx"

namespace CLAM
{
	
/**
* This class simply holds a few miscellaneous utility methods that are used
* by multiple classes, but don't fit very well in any class.
*
* \author greg kellum [gkellum@iua.upf.edu] 08/20/2007
* \since CLAM v1.1
*/
class DataUtil
{
public:
	/**
	* This copies the frame center time, the spectral peak array, and the 
	* residual from the source frame to the target frame.
	*/
	static void CopyFrameMembers(Frame& sourceFrame, Frame& targetFrame);

	/**
	* This copies the spectral peak array values from the source to the target
	*/
	static void CopySpectralPeakArraysMembers(SpectralPeakArray& sourceSpectralPeaks,
											SpectralPeakArray& targetSpectralPeaks );

	/**
	* This copies the residual values from the source to the target
	*/
	static void CopyResidualSpectrumsMembers(Spectrum& sourceResidualSpectrum,
											Spectrum& targetResidualSpectrum);

	/**
	* This checks that the frame center time, the spectral peak array, and the 
	* residual of the source frame and the target frame are identical.
	*/
	static bool CheckMembers(Frame& sourceFrame, Frame& targetFrame);
	
	static void PrintSpectralPeakArray(SpectralPeakArray& sourceSpectralPeaks);
};

} // end namespace CLAM 

#endif
