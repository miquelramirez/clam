#include "AudioIO.hxx"
#include "SpectralAnalysisLite.hxx"
#include "SpectralSynthesisLite.hxx"
#include "NodeTmpl.hxx"
#include "CircularStreamImpl.hxx"
#include "FunkyPresentation.hxx"
#include "FunctionWindow.hxx"
#include "AudioFileOut.hxx"
#include "SpectralEnvelopeExtractor.hxx"

extern "C" {
#include<GL/glut.h>
};

using namespace CLAM;
using namespace CLAMGUI;

class Analizer {

	GlutFunctionWindow mWindow;
	FunkyPresentation mFunky;

	SpectralAnalysis mAnalysis1;
	SpectralAnalysis mAnalysis2;
	SpectralSynthesis mSynthesis1;
	SpectralSynthesis mSynthesis2;
	AudioIn mInput1;
//	AudioIn mInput2;
//	AudioOut mOutput1;
//	AudioOut mOutput2;
	AudioFileOut mDebug;

	SpectralEnvelopeExtractor mExtractor1;

	NodeTmpl<Audio,CircularStreamImpl<TData> > mInputNode1;
	NodeTmpl<Audio,CircularStreamImpl<TData> > mInputNode2;
	NodeTmpl<Audio,CircularStreamImpl<TData> > mOutputNode1;
	NodeTmpl<Audio,CircularStreamImpl<TData> > mOutputNode2;

	Spectrum mSpectrum1;
	Spectrum mSpectrum2;
	Spectrum mEnvelope1;

	int mAnalysisWindowSize;
	int mHopSize;
	int mTransformAudioSize;
	int mSampleRate;
	int mNPeaks;
	TData mMaskBandwidth;

	void ToDB();

public:
	Analizer();
	void Run();
};


Analizer::Analizer()
	: mFunky(MTPresentation::eRealTime),
	  mAnalysisWindowSize(512),
	  mHopSize(512),
	  mTransformAudioSize(1024),
	  mSampleRate(48000),
	  mNPeaks(10),
	  mMaskBandwidth(400.0)
{
	mSpectrum1.SetSize(mTransformAudioSize/2+1);
	mSpectrum2.SetSize(mTransformAudioSize/2+1);
	mEnvelope1.SetSize(mTransformAudioSize/2+1);
	mSpectrum1.SetSpectralRange(mSampleRate/2);
	mSpectrum2.SetSpectralRange(mSampleRate/2);
	mEnvelope1.SetSpectralRange(mSampleRate/2);

	SpectralAnalysisConfig acfg;
	acfg.SetAnalysisWindowSize(mAnalysisWindowSize);
	acfg.SetHopSize(mHopSize);
	acfg.SetTransformAudioSize(mTransformAudioSize);
	mAnalysis1.Configure(acfg);
	mAnalysis2.Configure(acfg);

// 	SpectralSynthesisConfig scfg;
// 	scfg.SetTransformAudioSize(mTransformAudioSize);
// 	scfg.SetAnalysisHopSize(mHopSize);
// 	mSynthesis1.Configure(scfg);
// 	mSynthesis2.Configure(scfg);
	
	AudioIOConfig icfg;
	icfg.SetFrameSize(mHopSize);
	icfg.SetSampleRate(mSampleRate);
	icfg.SetChannelID(0);
	mInput1.Configure(icfg);
//	mOutput1.Configure(icfg);
//	icfg.SetChannelID(1);
//	mInput2.Configure(icfg);
//	mOutput2.Configure(icfg);

	AudioFileConfig fcfg;
	fcfg.SetFilename("debug.wav");
	fcfg.SetFiletype(EAudioFileType::eWave);
	fcfg.SetSampleRate(mSampleRate);
	fcfg.SetFrameSize(mHopSize*2);
	mDebug.Configure(fcfg);

	SpectralEnvelopeExtractorConfig ecfg;
	ecfg.SetSpectralSize(mTransformAudioSize/2+1);
	ecfg.SetSpectralRange(mSampleRate/2);
	ecfg.SetnMaxPeaks(mNPeaks);
	ecfg.GetDetectConfig().SetMaskingBandwidth(mMaskBandwidth);
	mExtractor1.Configure(ecfg);

	mInputNode1.SetSampleRate(mSampleRate);
	mInput1.Output.Attach(mInputNode1);
	mAnalysis1.Input.Attach(mInputNode1);
	mAnalysis1.Output.Attach(mSpectrum1);

	mExtractor1.Input.Attach(mSpectrum1);
	mExtractor1.Output.Attach(mEnvelope1);

//	mInputNode2.SetSampleRate(mSampleRate);
//	mInput2.Output.Attach(mInputNode2);
//	mAnalysis2.Input.Attach(mInputNode2);
//	mAnalysis2.Output.Attach(mSpectrum2);

//	mSynthesis1.Input.Attach(mSpectrum1);
//	mSynthesis1.Output.Attach(mOutputNode1);

//	mSynthesis2.Input.Attach(mSpectrum2);
//	mSynthesis2.Output.Attach(mOutputNode2);

//	mOutput1.Input.Attach(mOutputNode1);
//	mOutput2.Input.Attach(mOutputNode2);
//	mDebug.Input.Attach(mOutputNode);

	mInputNode1.Configure(1024*8);
//	mOutputNode1.Configure(1024*8);
//	mInputNode2.Configure(1024*8);
//	mOutputNode2.Configure(1024*8);


	mFunky.BindSpectrum(mSpectrum1);
	mFunky.BindPeaks(mExtractor1.DebugGetPeaks());
	mFunky.BindSpectralEnvelope(mEnvelope1);
	mFunky.SetDisplay(mWindow);


}

void Analizer::ToDB()
{
	Array<TData> &mag = mSpectrum1.GetMagBuffer();
	int size = mTransformAudioSize/2+1;
	TData max = 0;
	for (int i=0; i<size; i++)
	{
		if (mag[i] > max)
			max = mag[i];
	}
	for (int i=0; i<size; i++)
	{
		mag[i] = 10 + log(mag[i]/max);
	}
}

void Analizer::Run()
{
	mWindow.RunInBackground();

	int dropouts = 0;
	mAnalysis1.Start();
	mInput1.Start();
	mExtractor1.Start();
//	mSynthesis1.Start();
//	mOutput1.Start();
//	mAnalysis2.Start();
//	mInput2.Start();
//	mSynthesis2.Start();
//	mOutput2.Start();
//	mDebug.Start();

//	int iterations = 100;

	bool odd = false;

	mInput1.Do();
//	mInput2.Do();
	while (1)
	{
		odd = !odd;

//		if (odd){
			mInput1.Do();
//			mInput2.Do();
//		}

		mAnalysis1.Do();

		ToDB();

		mExtractor1.Do();
//		mSynthesis1.Do();
//		mAnalysis2.Do();
//		mSynthesis2.Do();

		if (!mFunky.Refresh())
			dropouts++;

//		if (odd)
//			continue;

//		mOutput1.Do();
//		mOutput2.Do();
//		mDebug.Do();

	}
	std::cout << "Graphic droputs: " << dropouts << std::endl;
	mAnalysis1.Stop();
	mInput1.Stop();
	mExtractor1.Stop();
//	mSynthesis1.Stop();
//	mOutput1.Stop();
//	mAnalysis2.Stop();
//	mInput2.Stop();
//	mSynthesis2.Stop();
//	mOutput2.Stop();
//	mDebug.Stop();
}


int main(int argc, char* argv[])
{
	AudioManager manager;
	try {
		glutInit(&argc, argv);
		Analizer analizer;
		analizer.Run();
	}
	catch (Err& e) {
		std::cout << e.what();
		return 1;
	}
	return 0;
}
