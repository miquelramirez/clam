#ifndef SMSREALTIMESYNTH_HXX
#define SMSREALTIMESYNTH_HXX

#include "SMSBase.hxx"
#include "StdOutProgress.hxx"
#include "StdOutWaitMessage.hxx"

namespace CLAM
{

class SMSRealtimeSynth : public SMSBase
{
	public:
			SMSRealtimeSynth( int sampleRate );
			void Run() {}
			void Play();
			CLAMGUI::Progress* CreateProgress(const char* title,float from,float to)
			{
				return new CLAMGUI::StdOutProgress(title,from,to);
			}

			CLAMGUI::WaitMessage* CreateWaitMessage(const char* title)
			{
				return new CLAMGUI::StdOutWaitMessage(title);
			}

			int GetSampleRate();

			void SetSampleRate( int sampleRate );

			void LoadAnalysisFile( char* analysisFile );

			void LoadConfigurationFile( char* configFile );

			void LoadScoreFile( const std::string& scoreFile );

			void DoTheTransform();

			void QueryState();

	private:
			int mSampleRate;
			CLAM::AudioIOConfig outLCfg;
		  	CLAM::AudioIOConfig outRCfg;
			CLAM::AudioOut outL;
			CLAM::AudioOut outR;
			

			void PlayAudio( const CLAM::Audio& audio );
			void TransformProcessing();
};

} // namespace CLAM

#endif
