#include "Synthesis.hxx"

using namespace MTG;

//methods for configuration

void SynthesisConfig::Init()
{
  AddName();
	AddFrameSize();
	AddWindowSize();
	AddWindowType();
	AddUseZeroPhase();
	AddZeroPaddingFactor();
  UpdateData();
 
	SetFrameSize(0);
	SetWindowSize(0);
	SetWindowType(MTG::EWindowType::KAISERBESSEL20);
	SetUseZeroPhase(true);
	SetZeroPaddingFactor(1.0);
}

//methods for PO

void Synthesis::AttachChildren()
{
	PO_WindowGen.SetParent(this);;
	PO_TDWindowing.SetParent(this);;
	PO_ZeroPhaseWindowing.SetParent(this);;
	PO_ZeroPadding.SetParent(this);;
	PO_IFFT.SetParent(this);;
	PO_OverlapAdd.SetParent(this);;
}

Synthesis::Synthesis() :
	InputAudio("Input Audio",this,1),
	OutputAudio("Output Audio",this,1)
{
	AttachChildren();
	Configure(SynthesisConfig());
}

Synthesis::Synthesis(SynthesisConfig& cfg) :
	InputAudio("Input Audio",this,1),
	OutputAudio("Output Audio",this,1)
{
	AttachChildren();
	Configure(cfg);
}

Synthesis::~Synthesis()
{
	delete mpCircBuffer;
}

bool Synthesis::ConfigureChildren()
{
	static const char *ifft_name = "Output IFFT";
	static const char *zeropadding_name = "Output ZeroPadding";
	static const char *windowing_name = "Output Windowing";	
	static const char *windowgen_name = "Synth Window Gen";	
	static const char *overlapadd_name = "OverlapAdd";	


	//instantiate zeropadding
	ZeroPaddingConfig ZeroPaddingCFG;
	ZeroPaddingCFG.SetName(zeropadding_name);
	ZeroPaddingCFG.SetForcePowerOfTwo(false);
	ZeroPaddingCFG.SetInSize(mFFTSize);
	ZeroPaddingCFG.SetOutSize(mSynthBufferSize);
	ZeroPaddingCFG.SetZeroPhaseUsed(mConfig.GetUseZeroPhase());
	if(!PO_ZeroPadding.Configure(ZeroPaddingCFG))
		return false;

	//instantiate window gen
	WindowGenConfig WindowGenCFG;
  WindowGenCFG.SetWindowType(mConfig.GetWindowType());
	WindowGenCFG.SetForceZeroPhaseWindow(false); 
	WindowGenCFG.SetInvertWindow(true);
	WindowGenCFG.SetOverlapWindow(true);
	WindowGenCFG.SetFrameSize(mFrameSize);
	if(!PO_WindowGen.Configure(WindowGenCFG))
		return false;

	if(mConfig.GetUseZeroPhase())
	{
		//instantiate zerophase windowing
		ZeroPhaseWindowingConfig ZeroPhaseWindowingCFG;
		ZeroPhaseWindowingCFG.SetName(windowing_name);
		ZeroPhaseWindowingCFG.SetWindowingMethod(MTG::EWindowingMethod::SYNTHESIS);
		ZeroPhaseWindowingCFG.SetOverlapSize(mHopSize);	
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
	
	OverlapAddConfig OverlapAddCFG;
	OverlapAddCFG.SetHopSize(mHopSize);
	OverlapAddCFG.SetFrameSize(mFrameSize);
	OverlapAddCFG.SetBufferSize(mOverlapWindowSize);
	if(!PO_OverlapAdd.Configure(OverlapAddCFG))
		return false;

	//instantiate FFT
	IFFTConfig IFFTCFG;
	IFFTCFG.SetName("ifft_name");
	IFFTCFG.SetAudioSize(mFFTSize);
	if(!PO_IFFT.Configure(IFFTCFG))
		return false;		

	return true;
}

void Synthesis::ConfigureData()
{
	///////////////////////////////////
	//INSTANIATE PROCESSING DATA
	///////////////////////////////////

	//intantiate audio
	Audio0.SetSize(mFFTSize);
	Audio1.SetSize(mSynthBufferSize);
	Audio2.SetSize(mOverlapWindowSize);

	//instantiate analysis window
	WindowConfig SynthWindowCFG;
	SynthWindow.Configure(SynthWindowCFG);

	//instantiate circular buffer
	mpCircBuffer = new AudioCircularBuffer<TData>(mFrameSize, mOverlapWindowSize);
	mpCircBuffer[0].InitBuffer();

	mtypeMagPhase.bMagPhase = 1; 
	mtypeMagPhase.bComplex = 0; 
}


bool Synthesis::ConcreteConfigure(const ProcessingObjectConfig& c) throw(std::bad_cast)
{
	mConfig = dynamic_cast<const SynthesisConfig&>(c);


	mFrameSize = mConfig.GetFrameSize();
	mWindowSize = mConfig.GetWindowSize();
	mWindowType = mConfig.GetWindowType();
	mZeroPaddingFactor = mConfig.GetZeroPaddingFactor();
	mHopSize = mFrameSize;
	mSynthBufferSize = mWindowSize-1;
	mOverlapWindowSize = mHopSize << 1; //mOverlapWindowSize = 2*mHopSize

	//calculate fft size
	mFFTSize = CalculatePowerOfTwo(mSynthBufferSize*mZeroPaddingFactor);

	//configure children and data
	ConfigureChildren();

	ConfigureData();

	//init synthesis window
	PO_WindowGen.Start();
  PO_WindowGen.Do(mWindowSize,SynthWindow);
	PO_WindowGen.Stop();
	return true;
}

bool Synthesis::Do(void)
{
	return false;
}

bool Synthesis::Do(Spectrum& in, Audio& out)
{
	in.SynchronizeTo(mtypeMagPhase); //convert MagPhase data to ComplexData
	PO_IFFT.Do(in, Audio0);
	PO_ZeroPadding.Do(Audio0, Audio1);
	if(mConfig.GetUseZeroPhase()){
		PO_ZeroPhaseWindowing.Do(Audio1, Audio2, SynthWindow);}
	else{
		PO_TDWindowing.Do(Audio1, Audio2, SynthWindow);}
	PO_OverlapAdd.Do(Audio2, mpCircBuffer[0], out);
	return true;
}

bool Synthesis::Do(TM::Frame& in, Audio& out)
{
	return Do(in.GetSpectralData(), out);
}

bool Synthesis::Do(TM::MultiBandFrame& in, Audio& out)
{
	return Do(in.GetFrame(0, 0), out);
}

bool Synthesis::Do(TM::MultiChannelFrame& in, Audio& out)
{
	return Do(in.GetMultiBandFrame(0), out);
}


TInt32 Synthesis::CalculatePowerOfTwo(TInt32 size)
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