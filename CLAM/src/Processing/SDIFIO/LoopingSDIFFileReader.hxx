#ifndef _LoopingSDIFFileReader_
#define _LoopingSDIFFileReader_

#include "BufferedSDIFFileReader.hxx"
#include "SDIFInConfig.hxx"
#include "Frame.hxx"
#include "SpectralPeakArray.hxx"
#include "Spectrum.hxx"
#include "SimpleLoopMetadata.hxx"

#define DEFAULT_NUMBER_OF_CROSSFADE_FRAMES 100

namespace CLAM
{

/**
* This class extends the length of an SDIFFile indefinitely by looping between
* its frames. It supports looping between multiple loop points with crossfading.
*
* \author greg kellum [gkellum@iua.upf.edu] 7/30/07
* \since CLAM v1.1
*/
class LoopingSDIFFileReader : public BufferedSDIFFileReader
{
public:
	/**
	* You MUST give the instance the filename of the SDIF file to read.
	* in the SDIFInConfig object.
	*/
	LoopingSDIFFileReader(const SDIFInConfig& argSDIFInConfig);
	
	~LoopingSDIFFileReader();

	/**
	* You MUST to give the instance the filename of the SDIF file to read.
	* in the SDIFInConfig object. You can further specify which kinds of
	* objects to read from the SDIFFile. See the doc for SDIFInConfig.
	*/
	bool Configure(const SDIFInConfig& c);

	/**
	* This method returns the next frame from the internal buffer.
	*/
	virtual Frame* ReadFrame();

	/**
	* This returns true if and only if this class has been given some loop points
	*/
	bool isLooping();
	
	/**
	* Adds a single loop point for a sample.
	*/
	void AddLoop(SimpleLoopMetadata& aSimpleLoop);

	/**
	* Adds a list of loop points for a sample. (Note the data structure is actually
	* a vector rather than a list.
	*/
	void SetListOfLoops(std::vector<SimpleLoopMetadata>& theLoopList);
	
	/**
	* Clears all of the loop points previously set
	*/
	void ClearLoops();

	/**
	* Returns all of the loop points previously set
	*/
	std::vector<SimpleLoopMetadata>& GetListOfLoops();

	/**
	* Resets the SDIFFileReader to read the very first frame from the file again
	* thereby restarting the attack stage of the sample.
	*/
	void Reset();
	
private:
	/**
	* This crossfades between two given spectral peak arrays.
	*/
	void CrossfadeSpectralPeakArrays(SpectralPeakArray& sourceSpectralPeaks1,
									SpectralPeakArray& sourceSpectralPeaks2,
									SpectralPeakArray& targetSpectralPeaks,
									float crossfadeFactor);
	
	/**
	* This crossfades between two given residual spectrums.
	*/
	void CrossfadeResidualSpectrum(Spectrum& sourceSpectrum1,
									Spectrum& sourceSpectrum2,
									Spectrum& targetSpectrum,
									float crossfadeFactor);

	/**
	* This returns the index number of one loop selected at random from the
	* list of loops. But only those loops will be considered for which we have
	* already preloaded the buffers.
	*/
	int ChooseLoopRandomly(int frameBufferPosition, int indexOfCurrentLoop);
	
	// every time ReadFrame() is called, the frame is copied to this singleton
	// object which is returned rather than the frame from the BufferedSDIFReader
	Frame* outgoingFrame;
	// number of frames that should be crossfaded before looping
	int numberOfCrossfadeFrames;
	// a list of loop points for this sample
	std::vector<SimpleLoopMetadata> listOfLoops;
	// the index of the loop that we are currently using
	int indexOfCurrentLoop;
};

} // end namespace CLAM

#endif
