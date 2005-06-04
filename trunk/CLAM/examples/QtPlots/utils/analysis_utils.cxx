#include "SegmentDescriptors.hxx"
#include "Analyzer.hxx"
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

	TSize wSize = 2049;
	TSize hopSize = 256;
	TData sampleRate = out.GetSamplingRate();

	AnalyzerConfig cfg;
	Analyzer analyzer;
	
	cfg.SetWindowSize(wSize);
	cfg.SetSamplingRate(sampleRate);
	
	analyzer.Configure(cfg);

	analyzer.Start();

	TSize nSamples = out.GetAudio().GetSize();
	TSize frameSize = cfg.GetWindowSize();

	Frame frame;
	frame.AddAudioFrame();
	frame.UpdateData();

	frame.SetDuration(TData(hopSize/sampleRate));
	frame.GetAudioFrame().SetSize(frameSize);    
  
	TIndex leftIndex = 0;
	TIndex rightIndex = frameSize;

	while(leftIndex < nSamples)
	{
	    out.GetAudio().GetAudioChunk(leftIndex,rightIndex,frame.GetAudioFrame()); 
	    analyzer.Do(frame);         
	    out.AddFrame(frame);    

	    leftIndex += hopSize;
	    rightIndex = leftIndex + frameSize;
	}
	analyzer.Stop();
	out.SetEndTime(out.GetAudio().GetSize()/out.GetAudio().GetSampleRate());
    }

   

    void energy(const Audio& in, DataArray& out)
    {
	Segment seg;
	seg.AddAudio();
	seg.UpdateData();

	seg.SetAudio(in);
	seg.SetBeginTime(0);
	seg.SetSamplingRate(seg.GetAudio().GetSampleRate());

	TSize wSize = 513;
	TSize hopSize = 256;
	TData sampleRate = seg.GetSamplingRate();

	AnalyzerConfig cfg;
	Analyzer analyzer;
	
	cfg.SetWindowSize(wSize);
	cfg.SetZeroPadding(2);
	cfg.SetSamplingRate(sampleRate);
	
	analyzer.Configure(cfg);

	analyzer.Start();

	TSize nSamples = seg.GetAudio().GetSize();
	TSize frameSize = cfg.GetWindowSize();

	Frame frame;
	frame.AddAudioFrame();
	frame.UpdateData();

	frame.SetDuration(TData(hopSize/sampleRate));
	frame.GetAudioFrame().SetSize(frameSize);    
  
	TIndex leftIndex = 0;
	TIndex rightIndex = frameSize;

	while(leftIndex < nSamples)
	{
	    seg.GetAudio().GetAudioChunk(leftIndex,rightIndex,frame.GetAudioFrame()); 
	    analyzer.Do(frame);         
	    seg.AddFrame(frame);    

	    leftIndex += hopSize;
	    rightIndex = leftIndex + frameSize;
	}
	analyzer.Stop();

	SegmentDescriptors desc  = analyzer.GetDescriptors();
	TSize nFrames = desc.GetFramesD().Size();
	out.Resize(nFrames);  
	out.SetSize(nFrames);  
	for(TIndex i=0;i < nFrames; i++)
	{
	    out[i] = desc.GetFrameD(i).GetSpectrumD().GetEnergy();
	}
    }

  

   

   
}

// END

