
#ifndef _CSaltoInterpolation_
#define _CSaltoInterpolation_

#include "CSaltoSynthFrame.hxx"
#include "Frame.hxx"
//#include "CSaltoParameter.hxx"
#include "Parameters.hxx"

namespace CLAM
{

	class CSaltoInterpolationConfig:public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (CSaltoInterpolationConfig, 2, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, double , SpectralRange );
	private:
		Parameters*			mpParams;
	protected:
		void DefaultInit(void);
	public:
		inline Parameters* GetParams() { return mpParams; }
		inline void SetParams( Parameters& params ){ mpParams = &params; }
	};

	class CSaltoInterpolation:public Processing
	{
	public:
	//CSaltoInterpolation(CSaltoParameter* pParams);		
	//CSaltoInterpolation(Parameters* pParams);	
		CSaltoInterpolation();

		CSaltoInterpolation( const CSaltoInterpolationConfig& cfg );

		~CSaltoInterpolation();

		const ProcessingConfig &GetConfig() const { return mConfig; }

		bool Do(void) { return true; }


		void DoInterpolation( Frame *pSpecFrameBase,
							  SpectralPeakArray *pTargetPeakArray,
                    			double targetFundFreq,
							  double magInterpolFactor,	
                    			double freqInterpolFactor,
                    			double magGain,
							  CSaltoSynthFrame *pSynthFrame,
							  bool doFreqIP=false,
							  bool doPhaseIP=false,
							  SpectralPeakArray *pTargetPeakArray2=NULL,
							  double magInterpolFactor2=0.0 );

		void DoInterpolation2( Frame *pSpecFrameBase,
							  SpectralPeakArray *pTargetPeakArray,
                    			double targetFundFreq,
							  double magInterpolFactor,	
                    			double freqInterpolFactor,
                    			double magGain,
							  CSaltoSynthFrame *pSynthFrame,
							  bool doFreqIP=false,
							  bool doPhaseIP=false,
							  SpectralPeakArray *pTargetPeakArray2=NULL,
							  double magInterpolFactor2=0.0 );
														
		void FollowOneTrack(Frame *pSpecFrameBase, CSaltoSynthFrame *pSynthFrame);
		void SetTrackIndex(TIndex trackId);
		void DoPitchMod(CSaltoSynthFrame *pSynthFrame,double pitchFactor);
		void DoPitchMod2(CSaltoSynthFrame *pSynthFrame,double pitchFactor);


	protected:
		bool ConcreteConfigure( const ProcessingConfig& cfg ) throw( std::bad_cast );
		bool ConcreteStart();
		bool ConcreteStop();

	private:
		//CSaltoParameter *mpParams;
		CSaltoInterpolationConfig mConfig;

		Parameters *mpParams;

		double mSpectralRange;

		TIndex mLastPosition;

		double mRandomIndex;

		bool mOneTrackAlreadyStarted;

		double mFirstFreq;

		TData mRandomNumber[MAX_SINES];

		double DoLinInterpolation(double base,double target,double ipFactor);
		void DoInterpolateToLessPeaks();
		void DoInterpolateToMorePeaks();

	};

} // end of namespace CLAM
#endif
