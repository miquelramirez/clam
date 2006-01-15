#include "SMSAnalysis.hxx"
#include "analysis_utils.hxx"

using namespace CLAM;

namespace qtvm_examples_utils
{
    void analyze(const Audio& in, Segment& out)
    {
		out.AddAudio();
		out.UpdateData();

		out.SetAudio(in);
		out.SetBeginTime(0);
		out.SetSamplingRate(out.GetAudio().GetSampleRate());
		out.mCurrentFrameIndex=0;

		SMSAnalysisConfig cfg;
		cfg.SetSinWindowType(EWindowType::eBlackmanHarris92);
		cfg.SetResWindowType(EWindowType::eBlackmanHarris92);
		cfg.SetSinWindowSize(2049);
		cfg.SetResWindowSize(1025);
		cfg.SetHopSize(256);
		cfg.SetSinZeroPadding(2);
		cfg.SetResZeroPadding(0);
		cfg.SetSamplingRate(out.GetSamplingRate());
		cfg.GetPeakDetect().SetMagThreshold(-120.0);
		cfg.GetSinTracking().SetIsHarmonic(true);

		SMSAnalysis analyzer;
		analyzer.Configure(cfg);
	
		analyzer.Start();
		while(analyzer.Do(out))
		{
			printf(".");
		}         
		analyzer.Stop();
		out.SetEndTime(out.GetAudio().GetSize()/out.GetAudio().GetSampleRate());
    }   
}

// END

