#include "SimpleTransformer.hxx"
#include "Spectrum.hxx"

namespace CLAM
{

SimpleTransformer::SimpleTransformer()
{
}

SimpleTransformer::~SimpleTransformer()
{
}

void SimpleTransformer::TransformFrequency(Frame* inFrame, float sourceFrequency, float targetFrequency)
{
	CLAM_ASSERT(inFrame != NULL, "Frame whose frequency is being transformed cannot be null.\n");
	CLAM_ASSERT(inFrame->HasSpectralPeakArray(), "Frame whose frequency is being transformed must have a SpectralPeakArray\n");

	SpectralPeakArray& theSpectralPeakArray = inFrame->GetSpectralPeakArray();
	DataArray& ifreqArray = theSpectralPeakArray.GetFreqBuffer();
	TSize nPeaks = theSpectralPeakArray.GetnPeaks();

	float amount = 1 + (targetFrequency - sourceFrequency) / sourceFrequency;
	for(int i=0;i<nPeaks;i++)
	{
		ifreqArray[i] = ifreqArray[i] * amount;
	}
}

void SimpleTransformer::TransformAmplitude(Frame* inFrame, float sourceAmplitude, float targetAmplitude)
{
	CLAM_ASSERT(inFrame != NULL, "Frame whose amplitude is being transformed cannot be null.\n");
	CLAM_ASSERT(inFrame->HasSpectralPeakArray(), "Frame whose amplitude is being transformed must have a SpectralPeakArray\n");

	SpectralPeakArray& theSpectralPeakArray = inFrame->GetSpectralPeakArray();
	DataArray& peaksMagArray = theSpectralPeakArray.GetMagBuffer();
	TSize nPeaks = theSpectralPeakArray.GetnPeaks();

	float scaleFactor = targetAmplitude / sourceAmplitude;
	for(int i=0; i<nPeaks; i++)
	{
		peaksMagArray[i] = peaksMagArray[i] * scaleFactor;
	}

	Spectrum& theResidual = inFrame->GetResidualSpec();
	int size = theResidual.GetSize();
	for (int i=0; i < size; i++)
	{
		theResidual.GetMagBuffer()[i] = scaleFactor * theResidual.GetComplexArray()[i].Mag();
		theResidual.GetPhaseBuffer()[i] = scaleFactor * theResidual.GetComplexArray()[i].Ang();
	}
	//theResidual.MagPhase2Complex();
}

} // end namespace CLAM

