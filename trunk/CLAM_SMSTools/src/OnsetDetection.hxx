#ifndef _OnsetDetection_
#define _OnsetDetection_

#include "ProcessingComposite.hxx"
#include "Segment.hxx"
#include "AudioFileIn.hxx"
#include "FFT_rfftw.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "Matrix.hxx"
#include "TimeIndex.hxx"
#include "MFCCGen.hxx"
#include "Point.hxx"

using namespace CLAM;


/** OnsetDetection configuration class **/
class OnsetDetectionConfig : public ProcessingConfig 
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (OnsetDetectionConfig, 7,ProcessingConfig);
	DYN_ATTRIBUTE (0, public, TSize, FrameSize);
	DYN_ATTRIBUTE (1, public, TSize, nBands);
	DYN_ATTRIBUTE (2, public, TSize, SmoothFiltSize);
	DYN_ATTRIBUTE (3, public, TData, BandThreshold);
	DYN_ATTRIBUTE (4, public, TSize, MinPeakDist);
	DYN_ATTRIBUTE (5, public, TData, GlobalThreshold);
	DYN_ATTRIBUTE (6, public, TSize, DifSize);

	void DefaultInit();
};


/** OnsetDetection class **/
class OnsetDetection : public ProcessingComposite 
{
		
public:
	
	OnsetDetection();
	OnsetDetection(OnsetDetectionConfig& c);
	//~OnsetDetection();

	const ProcessingConfig &GetConfig() const {return mConfig;}

	/** Supervised mode execution */
	bool Do(void){return false;}

	/** Unsupervised mode execution */
	bool Do(Segment& originalSegment); //inplace processing only?


private:
	
	// Configuration data
	OnsetDetectionConfig mConfig;
	
	// The internal Processing Objects
	FFT_rfftw	mFFT;
	MFCCGen		mMFCCGen;


	Audio mAudioFrame;			// smaller chunk of audio that will be processed thru the FFT
	Audio mAudioFrameZeroed;	 //ZeroPadded Audio Chunk
	Spectrum mSpec;
	Frame mFrame;
	
	
	// Internal convenience methods.

	void AttachChildren();
	bool ConfigureChildren();
	void ConfigureData();


	// Processing Object compliance methods.

	const char *GetClassName() const {return "OnsetDetection";}

	/** Configuration method */
	bool ConcreteConfigure(const ProcessingConfig&);

	
	bool DescriptorsCalculation( Audio &in );
	bool AnalyseFrame(Audio& in,int frameIndex);
	void Algorithm(Segment& s, Array< Array<TData> >& values);
	void Smoothing(DataArray energy, DataArray &smoothedEnergy );
	void DetectPosition(DataArray in, DataArray &ret);
	void AutoThresh(DataArray in, int retPerCent, float &ret);
	void PeakPick(DataArray in, TData threshold , DataArray &ret);
	void DetectIntensity(DataArray positionArray , DataArray weightArray , DataArray &ret );
	void DeleteWeakOnsets(Array<TimeIndex> &in , char* type );
	void PeakDeletion( Array<TimeIndex> in , TimeIndex &ret );
	void PeakSummation( Array<TimeIndex> in , TimeIndex &ret );
	//void CheckOffset( Array<TimeIndex> in , DataArray energy, Array< Point<int,int> > &ret , int type);
	void CheckPitch(Segment& s , Array<TimeIndex> onsets, Array<int>& ret );
	//void CheckPitch(Segment& s , DataArray fund, Array<TimeIndex> onsets , Array<int>& ret );


	//Member Data
	TSize mnBands;							//Number of Bands
	TSize mnFrames;							//Number of Frames in the segment	
	TSize mFrameSize;						//FrameSize
	TSize mEnergyThreshold;
	TSize mFFTSize;							//FFT Size
	TSize mWinSize;							//Smoothing Filter Size
	TSize mDifSize;							//Num of points taken for computing the derivative
	TData mBandThreshold;					//Threshold for Band peak picking 
	TData mMinPeakDist;						//Minimum distacne between picked peaks
	TData mGlobalThreshold;					
	DataArray mWinCoef;						//Smoothing Filter Coefficients
	TData mSampleRate;
	TData mOffsetPercentil;
	Array< Array<TData> > mDescriptors;
};

#endif
