#ifndef __SMS_ANALYSISTASK__
#define __SMS_ANALYSISTASK__

#include "Audio.hxx"
#include "SMSAnalysisSynthesisConfig.hxx"
#include "SMS_ParameterChange.hxx"
#include <string>
#include <list>
#include <vector>

namespace SMS
{
	class AnalysisTask
	{
	public:

		AnalysisTask();
		~AnalysisTask();

		bool         SetSoundSource( std::string filename );
		CLAM::TData  GetSampleRate();
		CLAM::TTime  GetSoundLength();
		bool         SetAnalysisFrameLength( CLAM::TTime value );
		bool         SetAnalysisFrameOverlapFactor( double percentage );
		bool         SetAnalysisWindowFunction( std::string funcName );
		bool         SetZeroPaddingFactor( CLAM::TSize factor );
		bool         SetPeakDetectMagThreshold( CLAM::TData threshold );
		bool         SetMaximumPeakDeviation( CLAM::TData percentage );
		bool         SetPeakFrequencyCutoff( CLAM::TData maxFreq );
		bool         SetFundamentalFreqSearchRange( CLAM::TData freqLow,
							    CLAM::TData freqHigh );
		bool         AssumeHarmonicity( bool status );
		bool         ActivateTrackCleaning( bool status );
		bool         SetToleranceToTrackDiscontinuation( CLAM::TSize maxDiscont );
		bool         SetMinimumTrackLength( CLAM::TSize minLength );

		void         GetAvailableWindowFunctions( std::list< std::string >& winFuncs );

		const CLAM::SMSAnalysisSynthesisConfig& GetConfig() const;

		void Process( const ParameterChange& cmd );
		void Process( const std::vector< ParameterChange >& collectionOfCmds );
		
	protected:
		
		void UpdateExpectedNumberOfFrames();
	protected:
		
		CLAM::Audio  mSubjectAudio;
		std::string  mSubjectAudioFilename;
		CLAM::TTime  mAnalysisFrameLength;
		double       mAnalysisFrameOverlapFactor;
		std::string  mAnalysisWindowFunction;
		CLAM::TSize  mZeroPaddingFactor;
		CLAM::TData  mPeakDetectMagThreshold;
		CLAM::TData  mPeakFrequencyCutoff;
		CLAM::TData  mMaxPeakDeviation;
		CLAM::TData  mF0SearchLowerBound;
		CLAM::TData  mF0SearchUpperBound;
		bool         mMustAssumeHarmonicity;
		bool         mTrackCleaningIsActivated;
		CLAM::TSize  mTrackDiscTolerance;
		CLAM::TSize  mTrackMinimumLength;
		CLAM::TSize  mExpectedNumberOfFrames;
		CLAM::SMSAnalysisSynthesisConfig mSMSToolsConfig;
		
	};

}

#include "SMS_AnalysisTask_inlines.hxx"

#endif // SMS_AnalysisTask.hxx
