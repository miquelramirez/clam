#ifndef _InterpolatingSynthesis_
#define _InterpolatingSynthesis_

#include "Processing.hxx"
#include "InControl.hxx"
#include "CSaltoSynthFrame.hxx"
#include "CSaltoDefines.hxx"
#include "Frame.hxx"


namespace SALTO
{
	using CLAM::Processing;
	using CLAM::ProcessingConfig;
	using CLAM::DynamicType;
	using CLAM::InControlTmpl;
	using CLAM::Frame;
	using CLAM::SpectralPeakArray;
	using CLAM::SpectralPeak;
	using CLAM::TIndex;
	using CLAM::TData;
	using CLAM::TSize;
	using CLAM::TFloat32;
	using CLAM::TControlData;

	using CLAM::CSaltoSynthFrame;

	class InterpolatingSynthesisConfig: public ProcessingConfig
	{
		public:
			DYNAMIC_TYPE_USING_INTERFACE (InterpolatingSynthesisConfig, 2, ProcessingConfig);
			DYN_ATTRIBUTE (0, public, std::string, Name);
			DYN_ATTRIBUTE (1, public, double , SpectralRange );
		protected:
			void DefaultInit(void);
	};

	class InterpolatingSynthesis: public Processing
	{
	public:

		InterpolatingSynthesis();

		InterpolatingSynthesis( const InterpolatingSynthesisConfig& cfg );

		virtual ~InterpolatingSynthesis();

		const ProcessingConfig &GetConfig() const { return mConfig; }

		const char* GetClassName() const
		{
			return "SALTO::InterpolatingSynthesis";
		}

		virtual bool Do() ;

		void DoMagInterpolation(Frame &pSpecFrameBase,	
							  SpectralPeakArray &pTargetPeakArray,
	//						  double targetFundFreq,      
	//						  double magInterpolFactor,
	//						  double magGain,             
							  CSaltoSynthFrame &pSynthFrame );

		void DoInterpolation( Frame &pSpecFrameBase,	
							  SpectralPeakArray &pTargetPeakArray,
	//						  double targetFundFreq,      
	//						  double magInterpolFactor,
	//						  double freqInterpolFactor,  
	//						  double magGain,             
							  CSaltoSynthFrame &pSynthFrame,
							  SpectralPeakArray &pTargetPeakArray2);
	//						  double magInterpolFactor2 );

		void DoPitchMod( CSaltoSynthFrame &pSynthFrame );

	protected:

		bool ConcreteConfigure( const ProcessingConfig& c );

		bool ConcreteStart();

		bool ConcreteStop();

		int InLoopSynthesisCB( TControlData value );

		int UseRandomLoopCB( TControlData value );

		int UseRandomDeviationsCB( TControlData value );

		int PitchFactorCB( TControlData value );

		int RandomRangeCB( TControlData value );

		int TargetFreqCB( TControlData value );

		int MagInterpolFactorCB( TControlData value );

		int MagGainCB( TControlData value );

		int FreqInterpolFactorCB( TControlData value );

		int MagInterpolFactor2CB( TControlData value );

	private:

		double DoLinInterpolation(double base,double target,double ipFactor);

		InterpolatingSynthesisConfig				mConfig;

/*		double							mTargetFundFreq;

		double							mMagInterpolFactor;

		double							mFreqInterpolFactor;

		double							mMagGain;

		bool							mDoFreqIP;

		bool							mDoPhase

*/
		double										mSpectralRange;

		TData										mRandomRange;

		bool										mUseRandomDeviations;

		bool										mInLoopSynthesis;

		bool										mUseRandomLoop;

		double										mPitchFactor;

		double										mTargetFreq;

		double										mMagInterpolFactor;

		double										mMagGain;

		double										mFreqInterpolFactor;

		double										mMagInterpolFactor2;

		InControlTmpl< InterpolatingSynthesis >		mIn_InLoopSynthesis;

		InControlTmpl< InterpolatingSynthesis >		mInUseRandomDeviations;

		InControlTmpl< InterpolatingSynthesis >		mInUseRandomLoop;

		InControlTmpl< InterpolatingSynthesis >		mInPitchFactor;

		InControlTmpl< InterpolatingSynthesis >		mInRandomRange;

		InControlTmpl< InterpolatingSynthesis >		mInTargetFreq;

		InControlTmpl< InterpolatingSynthesis >		mInMagInterpolFactor;

		InControlTmpl< InterpolatingSynthesis >		mInMagGain;

		InControlTmpl< InterpolatingSynthesis >		mInFreqInterpolFactor;

		InControlTmpl< InterpolatingSynthesis >		mInMagInterpolFactor2;

		TIndex										mLastPosition;

		double										mRandomIndex;

		bool										mOneTrackAlreadyStarted;

		double										mFirstFreq;

		TData										mRandomNumber[ MAX_SINES ];

	};
};

#endif
