#include "SpectralAnalysisLite.hxx"
#include "SpectralSynthesisLite.hxx"
#include "NodeTmpl.hxx"
#include "CircularStreamImpl.hxx"
#include "FunkyPresentation.hxx"
#include "FunctionWindow.hxx"
#include "AudioFileIn.hxx"
#include "AudioFileOut.hxx"
#include "SpectralEnvelopeExtractor.hxx"
#include "SpectrumProduct.hxx"

extern "C" {
#include<GL/glut.h>
};

using namespace CLAM;
using namespace CLAMGUI;

class SEModulator {

//	GlutFunctionWindow mWindow;
//	FunkyPresentation mFunky;

	SpectralAnalysis mAnalysis1;
	SpectralAnalysis mAnalysis2;
	SpectralSynthesis mSynthesis1;
	SpectralSynthesis mSynthesis2;
	AudioFileIn mInput1;
	AudioFileIn mInput2;
//	AudioFileOut mOutput1;
//	AudioOut mOutput2;
	AudioFileOut mDebug;

	SpectralEnvelopeExtractor mExtractor2;

	SpectrumProduct mModulator;

	NodeTmpl<Audio,CircularStreamImpl<TData> > mInputNode1;
	NodeTmpl<Audio,CircularStreamImpl<TData> > mInputNode2;
	NodeTmpl<Audio,CircularStreamImpl<TData> > mOutputNode1;
	NodeTmpl<Audio,CircularStreamImpl<TData> > mOutputNode2;

	Spectrum mSpectrum1;
	Spectrum mSpectrum2;
	Spectrum mEnvelope2;
	Spectrum mSpectrumOut;

	int mAnalysisWindowSize;
	int mHopSize;
	int mTransformAudioSize;
	int mSampleRate;
	int mNPeaks;
	TData mMaskBandwidth;

public:
	SEModulator();
	void Run();
};


SEModulator::SEModulator() :
//	  mFunky(MTPresentation::eBlocking),
	  mAnalysisWindowSize(512),
	  mHopSize(256),
	  mTransformAudioSize(1022),
	  mSampleRate(44100),
	  mNPeaks(10),
	  mMaskBandwidth(400.0)
{
	mSpectrum1.AddComplexArray();
	mSpectrumOut.AddComplexArray();
	mSpectrum2.AddComplexArray();
	mSpectrum1.SetSize(mTransformAudioSize/2+1);
	mSpectrum2.SetSize(mTransformAudioSize/2+1);
	mSpectrumOut.SetSize(mTransformAudioSize/2+1);
	mEnvelope2.SetSize(mTransformAudioSize/2+1);
	mSpectrum1.SetSpectralRange(mSampleRate/2);
	mSpectrum2.SetSpectralRange(mSampleRate/2);
	mSpectrumOut.SetSpectralRange(mSampleRate/2);
	mEnvelope2.SetSpectralRange(mSampleRate/2);

	SpectralAnalysisConfig acfg;
	acfg.SetAnalysisWindowSize(mAnalysisWindowSize);
	acfg.SetHopSize(mHopSize);
	acfg.SetTransformAudioSize(mTransformAudioSize);
	mAnalysis1.Configure(acfg);
	mAnalysis2.Configure(acfg);

 	SpectralSynthesisConfig scfg;
 	scfg.SetTransformAudioSize(mTransformAudioSize);
 	scfg.SetAnalysisHopSize(mHopSize);
 	mSynthesis1.Configure(scfg);
// 	mSynthesis2.Configure(scfg);
	
	AudioFileConfig fcfg;
	fcfg.SetFiletype(EAudioFileType::eWave);
	fcfg.SetSampleRate(mSampleRate);
	fcfg.SetFrameSize(mHopSize*2);
	fcfg.SetKeepFrameSizes(true);
	fcfg.SetFilename("in.wav");
	mInput1.Configure(fcfg);
	fcfg.SetFilename("mod.wav");
	mInput2.Configure(fcfg);
	fcfg.SetFilename("out.wav");
	mDebug.Configure(fcfg);

	SpectralEnvelopeExtractorConfig ecfg;
	ecfg.SetSpectralSize(mTransformAudioSize/2+1);
	ecfg.SetSpectralRange(mSampleRate/2);
	ecfg.SetnMaxPeaks(mNPeaks);
	ecfg.GetDetectConfig().SetMaskingBandwidth(mMaskBandwidth);
	mExtractor2.Configure(ecfg);

	mInputNode1.SetSampleRate(mSampleRate);
	mInput1.Output.Attach(mInputNode1);
	mAnalysis1.Input.Attach(mInputNode1);
	mAnalysis1.Output.Attach(mSpectrum1);

	mInputNode2.SetSampleRate(mSampleRate);
	mInput2.Output.Attach(mInputNode2);
	mAnalysis2.Input.Attach(mInputNode2);
	mAnalysis2.Output.Attach(mSpectrum2);

	mExtractor2.Input.Attach(mSpectrum2);
	mExtractor2.Output.Attach(mEnvelope2);

	mModulator.Input1.Attach(mEnvelope2);
	mModulator.Input2.Attach(mSpectrum1);
	mModulator.Output.Attach(mSpectrumOut);

	mSynthesis1.Input.Attach(mSpectrum1);
//	mSynthesis1.Input.Attach(mSpectrumOut);
	mSynthesis1.Output.Attach(mOutputNode1);

//	mSynthesis2.Input.Attach(mSpectrum2);
//	mSynthesis2.Output.Attach(mOutputNode2);

//	mOutput1.Input.Attach(mOutputNode1);
//	mOutput2.Input.Attach(mOutputNode2);
	mDebug.Input.Attach(mOutputNode1);

	mInputNode1.Configure(1024*8);
	mOutputNode1.Configure(1024*8);
	mInputNode2.Configure(1024*8);
//	mOutputNode2.Configure(1024*8);


// 	mFunky.BindSpectrum(mSpectrum1);
// 	mFunky.BindPeaks(mExtractor2.DebugGetPeaks());
// 	mFunky.BindSpectralEnvelope(mEnvelope2);
// 	mFunky.SetDisplay(mWindow);


}

void SEModulator::Run()
{
//	mWindow.RunInBackground();

	int dropouts = 0;
	mAnalysis1.Start();
	mInput1.Start();
	mExtractor2.Start();
	mSynthesis1.Start();
	mAnalysis2.Start();
	mInput2.Start();
	mModulator.SetPrototypes(mEnvelope2,mSpectrum1,mSpectrumOut);
	mModulator.Start();
//	mSynthesis2.Start();
//	mOutput1.Start();
//	mOutput2.Start();
	mDebug.Start();

//	int iterations = 100;

	bool odd = false;

	mInput1.Do();
	mInput2.Do();
	bool ok=true;
	while (ok)
	{
		odd = !odd;

		if (odd){
			ok &= mInput1.Do();
			ok &= mInput2.Do();
		}

		mAnalysis1.Do();
		mAnalysis2.Do();
		mExtractor2.Do();
		mModulator.Do();
		mSynthesis1.Do();
//		mSynthesis2.Do();

//		if (!mFunky.Refresh())
//			dropouts++;

		if (odd)
			continue;

//		mOutput1.Do();
//		mOutput2.Do();
		mDebug.Do();

	}
	std::cout << "Graphic droputs: " << dropouts << std::endl;
	mAnalysis1.Stop();
	mInput1.Stop();
	mSynthesis1.Stop();
	mExtractor2.Stop();
	mAnalysis2.Stop();
	mInput2.Stop();
//	mSynthesis2.Stop();
//	mOutput1.Stop();
//	mOutput2.Stop();
	mDebug.Stop();
}


int main(int argc, char* argv[])
{
	try {
//		glutInit(&argc, argv);
		SEModulator modulator;
		modulator.Run();
	}
	catch (Err& e) {
		std::cout << e.what();
		return 1;
	}
	return 0;
}
