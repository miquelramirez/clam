#ifndef __SMS_ANALYSISTASK_INLINES__
#define __SMS_ANALYSISTASK_INLINES__

namespace SMS
{
	// SMS::AnalysisTask class inline method implementations
	inline CLAM::TData AnalysisTask::GetSampleRate()
	{
		return mSubjectAudio.GetSampleRate();
	}

	inline CLAM::TTime AnalysisTask::GetSoundLength()
	{
		return (mSubjectAudio.GetSize()/mSubjectAudio.GetSampleRate()) * 1000.;
	}

	inline const CLAM::SMSAnalysisSynthesisConfig& AnalysisTask::GetConfig() const 
	{
		return mSMSToolsConfig;
	}

}

#endif // SMS_AnalysisTask_inlines.hxx
