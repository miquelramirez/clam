

#include "SpectralEnvelopeGenerator.hxx"

namespace CLAM {


	void SpectralEnvelopeGeneratorConfig::DefaultInit(void)
 	{
		AddAll();
		UpdateData();
		GetMemoryFactor().SetInitValue(0.0);
		GetMemoryFactor().SetMinValue(0.0);
		GetMemoryFactor().SetMaxValue(0.99);
		SetnMaxPeaks(1024);
		SetSpectralRange(48000/2);
		SetSpectralSize(1024);
		SetInterpolationType(EInterpolation::eLinear);
	}

	SpectralEnvelopeGenerator::SpectralEnvelopeGenerator(const SpectralEnvelopeGeneratorConfig& c)
		: Input("Input",this,1),
		  Output("Output",this,1),
		  MemoryFactor("MemoryFactor",this,&SpectralEnvelopeGenerator::MemoryFactorChange)
	{
		Configure(c);
	}

	int SpectralEnvelopeGenerator::MemoryFactorChange(TControlData value)
	{
		mMemoryFactor = mMinMemoryFactor + value * ( mMaxMemoryFactor - mMinMemoryFactor );
		return 0;
	}

	bool SpectralEnvelopeGenerator::ConcreteConfigure(const ProcessingConfig& cfg) throw(std::bad_cast)
	{
		mConfig = dynamic_cast<const SpectralEnvelopeGeneratorConfig&>(cfg);

		mMemoryFactor = mConfig.GetMemoryFactor().GetInitValue();
		mMinMemoryFactor = mConfig.GetMemoryFactor().GetMinValue();
		mMaxMemoryFactor = mConfig.GetMemoryFactor().GetMaxValue();

		mSize = mConfig.GetSpectralSize();

		mDelta = mConfig.GetSpectralRange() / (mSize - 1);

		mMemory.Resize(mSize);
		mMemory.SetSize(mSize);

		mBPF.SetIntpType(mConfig.GetInterpolationType());
		mIsSpline = (mConfig.GetInterpolationType() == EInterpolation::eSpline);

		mBPF.Resize(mConfig.GetnMaxPeaks());

		return true;
	}

	bool SpectralEnvelopeGenerator::Do(void)
	{
		bool res = Do(Input.GetData(),Output.GetData());
		Input.LeaveData();
		Output.LeaveData();
		return res;
	}

	void SpectralEnvelopeGenerator::CheckData(const SpectralPeakArray& inp, Spectrum &out)
	{
		CLAM_DEBUG_ASSERT(out.GetSpectralRange() == mConfig.GetSpectralRange(),
						  "Spectral range mismatch");
		CLAM_DEBUG_ASSERT(out.GetSize() == mSize,
						  "Spectral size mismatch");
		CLAM_DEBUG_ASSERT(out.HasMagBuffer(),
						  "Missing attributes in Spectrum");
		CLAM_DEBUG_ASSERT(inp.HasMagBuffer()  &&
						  inp.HasFreqBuffer() &&
						  inp.HasnPeaks(),
						  "Missing attributes in SpectralPeakArray");
						  
	}

	void SpectralEnvelopeGenerator::ConfigureBPF(int n_peaks, const Array<TData> &mags)
	{
		if (n_peaks < 3) {
			mBPF.SetSize(3);
			if (n_peaks == 2) {
				mBPF.SetXValue(2,mDelta*mSize);
				mBPF.SetValue(2,mags[1]);
			}
			else if (n_peaks == 1) {
				mBPF.SetXValue(2,mDelta*mSize);
				mBPF.SetValue(2,mags[0]);
				mBPF.SetXValue(1,mDelta*mSize* 0.99);
				mBPF.SetValue(1,mags[0]);
			}
			else {
				mBPF.SetXValue(2,mDelta*mSize);
				mBPF.SetValue(2,0.0);
				mBPF.SetXValue(1,mDelta*mSize/2);
				mBPF.SetValue(1,0.0);
				mBPF.SetXValue(0,0.0);
				mBPF.SetValue(0,0.0);
			}
		}
		else
		{
			mBPF.SetSize(n_peaks);
		}
	}

	bool SpectralEnvelopeGenerator::Do(const SpectralPeakArray& inp, Spectrum &out)
	{
		int i;

		CheckData(inp,out);

		int n_peaks = inp.GetnPeaks();

		Array<TData> &freqs = inp.GetFreqBuffer();
		Array<TData> &mags = inp.GetMagBuffer();

		Array<TData> &out_mag = out.GetMagBuffer();

		ConfigureBPF(n_peaks,mags);

		for (i=0; i<n_peaks; i++) {
			mBPF.SetXValue(i,freqs[i]);
			mBPF.SetValue(i,mags[i]);
		}

		if (mIsSpline)
			mBPF.UpdateSplineTable();

		TData freq = 0.0;

		for (i=0; i<mSize; i++)
		{
			mMemory[i] =       mMemoryFactor   * mMemory[i]        +  
			             ( 1 - mMemoryFactor ) * mBPF.GetValue(freq);

			out_mag[i] = mMemory[i];

			freq += mDelta;
		}

		return true;
	}



}
