#include "Analysis.hxx"

using namespace MTG;
using namespace TM;

//methods for configuration

void AnalysisConfig::Init()
{
	AddName();
	AddWindowSize();
	AddWindowType();
	AddFrameSize();
	AddUseZeroPhase();
	AddZeroPaddingFactor();
	AddCalculateFirstSpectrumWhenBufferIsFilled();

	UpdateData();
    
	SetWindowSize(0);
	SetWindowType(MTG::EWindowType::KAISERBESSEL20);
	SetFrameSize(0);
	SetUseZeroPhase(false);
	SetZeroPaddingFactor(1.0);
	SetCalculateFirstSpectrumWhenBufferIsFilled(true);
}

//methods for PO


void Analysis::AttachChildren()
{
	PO_WindowGen.SetParent(this);;
	PO_TDWindowing.SetParent(this);;
	PO_ZeroPhaseWindowing.SetParent(this);;
	PO_ZeroPadding.SetParent(this);;
	PO_FFT.SetParent(this);;
}

Analysis::Analysis() :
	InputAudio("Input Audio",this,1),
	OutputAudio("Output Audio",this,1)
{
	AttachChildren();
	Configure(AnalysisConfig());
}

Analysis::Analysis(AnalysisConfig& cfg) :
	InputAudio("Input Audio",this,1),
	OutputAudio("Output Audio",this,1)
{
	AttachChildren();
	Configure(cfg);
}

Analysis::~Analysis()
{
	delete mpCircBuffer;
}

bool Analysis::ConfigureChildren()
{
	static const char *fft_name = "Input FFT";
	static const char *zeropadding_name = "Input ZeroPadding";
	static const char *windowing_name = "Input Windowing";	
	static const char *windowgen_name = "Window Gen";	


	//instantiate zeropadding
	ZeroPaddingConfig ZeroPaddingCFG;
	ZeroPaddingCFG.SetName(zeropadding_name);
	ZeroPaddingCFG.SetForcePowerOfTwo(true);
	ZeroPaddingCFG.SetInSize(mAnalBufferSize);
	ZeroPaddingCFG.SetOutSize(mFFTSize);
	ZeroPaddingCFG.SetZeroPhaseUsed(mConfig.GetUseZeroPhase());
	if(!PO_ZeroPadding.Configure(ZeroPaddingCFG))
		return false;

	//instantiate window gen
	WindowGenConfig WindowGenCFG;
  WindowGenCFG.SetWindowType(mConfig.GetWindowType());
	WindowGenCFG.SetForceZeroPhaseWindow(true); 
	WindowGenCFG.SetInvertWindow(false);
	WindowGenCFG.SetOverlapWindow(false);
	WindowGenCFG.SetFrameSize(mFrameSize);
	if(!PO_WindowGen.Configure(WindowGenCFG))
		return false;

	if(mConfig.GetUseZeroPhase())
	{
		//instantiate zerophase windowing
		ZeroPhaseWindowingConfig ZeroPhaseWindowingCFG;
		ZeroPhaseWindowingCFG.SetName(windowing_name);
		ZeroPhaseWindowingCFG.SetWindowingMethod(MTG::EWindowingMethod::ANALYSIS);
		if(!PO_ZeroPhaseWindowing.Configure(ZeroPhaseWindowingCFG))
			return false;
	}
	else
	{
		TDWindowingConfig TDWindowingCFG;
		TDWindowingCFG.SetName(windowing_name);
		if(!PO_TDWindowing.Configure(TDWindowingCFG))
			return false;
	}
	
	//instantiate FFT
	FFTConfig FFTCFG;
	FFTCFG.SetName("fft_name");
	FFTCFG.SetAudioSize(mFFTSize);
	if(!PO_FFT.Configure(FFTCFG))
		return false;		

	return true;
}

void Analysis::ConfigureData()
{
	///////////////////////////////////
	//INSTANIATE PROCESSING DATA
	///////////////////////////////////

	//intantiate audio
	Audio0.SetSize(mAnalBufferSize);
	Audio1.SetSize(mAnalBufferSize);
	Audio2.SetSize(mAnalBufferSize);
	//instantiate analysis window
	WindowConfig AnalWindowCFG;
	AnalWindow.Configure(AnalWindowCFG);

	//instantiate circular buffer
	mpCircBuffer = new AudioCircularBuffer<TData>(mFrameSize, mAnalBufferSize+mFrameSize);
	mpCircBuffer[0].InitBuffer();
	mpCircBuffer[0].IncreaseWriteIndex((mWindowSize-1)/2);
}


bool Analysis::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
{
	mConfig = dynamic_cast<const AnalysisConfig&>(c);

	mFrameSize = mConfig.GetFrameSize();
	mWindowSize = mConfig.GetWindowSize();
	mWindowType = mConfig.GetWindowType();
	mZeroPaddingFactor = mConfig.GetZeroPaddingFactor();
	if(mWindowSize > 0) 
		mAnalBufferSize = mWindowSize-1;
	else 
		mAnalBufferSize = 0;
	mHopSize = mFrameSize;
	mSamplePosition = 0; //@new
	mAbsoluteWriteIndex = 0;

	//calculate fft size
	mFFTSize = CalculatePowerOfTwo(mAnalBufferSize*mZeroPaddingFactor);

	//configure children and data
	ConfigureChildren();

	ConfigureData();

	//init analysis window
	PO_WindowGen.Start();
  PO_WindowGen.Do(mWindowSize,AnalWindow);
	PO_WindowGen.Stop();
	return true;
}

bool Analysis::Do(void)
{
	return false;
}


bool Analysis::Do(Audio &in, Spectrum &out)
{
	mpCircBuffer[0].OverwriteBuffer(in); // add mFrameSize samples to the circular buffer
	if(mCalculateFirstSpectrumWhenBufferIsFilled) //start analysis only when buffer is filled with input frames
	{
		mAbsoluteWriteIndex += mFrameSize;	
		if(mAbsoluteWriteIndex <= (mSamplePosition + (mWindowSize-1)/2))
			return false;
	}
	mpCircBuffer[0].ReadFromBuffer(Audio0);
	if(mConfig.GetUseZeroPhase()){
		PO_ZeroPhaseWindowing.Do(Audio0, Audio1, AnalWindow);}
	else{
		PO_TDWindowing.Do(Audio0, Audio1, AnalWindow);}
	PO_ZeroPadding.Do(Audio1, Audio2);
	PO_FFT.Do(Audio2, out);
	if(mCalculateFirstSpectrumWhenBufferIsFilled)
		mSamplePosition += mFrameSize;
	return true;
}

bool Analysis::Do(Audio &in, Frame &out) //TimeMachine specific
{
	out.GetSpectralPeakAndPhaseData().SetnPeaks(0);
	return Do(in, out.GetSpectralData());
}

bool Analysis::Do(Audio &in, MultiBandFrame &out) //TimeMachine specific
{
	return Do(in, out.GetFrame(0,0));
}

bool Analysis::Do(Audio &in, MultiChannelFrame &out) //TimeMachine specific
{
	return Do(in, out.GetMultiBandFrame(0));
}

TInt32 Analysis::CalculatePowerOfTwo(TInt32 size)
{
	int tmp = size;
	int outputSize = 1;
	while (tmp) 
	{
	 	outputSize=outputSize << 1;
	 	tmp=tmp >> 1;
	}
	if(outputSize == size << 1)
		outputSize = outputSize >> 1;
	return outputSize;
}