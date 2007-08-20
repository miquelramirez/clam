#include "DataUtil.hxx"
#include <iostream>

namespace CLAM
{

void DataUtil::CopyFrameMembers(Frame& sourceFrame,
										Frame& targetFrame)
{
	targetFrame.SetCenterTime(sourceFrame.GetCenterTime());

	// Copy SpectralPeakArray data
	SpectralPeakArray& sourceSpectralPeaks = sourceFrame.GetSpectralPeakArray();
	SpectralPeakArray& targetSpectralPeaks = targetFrame.GetSpectralPeakArray();
	CopySpectralPeakArraysMembers(sourceSpectralPeaks, targetSpectralPeaks);
	
	// Copy ResidualSpectrum data
	// (I would have moved this code into Spectrum as a CopyMembers method
	//  but that class is so convoluted that it would be a massive headache
	//  to make a general copy members class.)
	Spectrum& sourceResidualSpectrum = sourceFrame.GetResidualSpec();
	Spectrum& targetResidualSpectrum = targetFrame.GetResidualSpec();
	CopyResidualSpectrumsMembers(sourceResidualSpectrum, targetResidualSpectrum);

}

void DataUtil::CopySpectralPeakArraysMembers(SpectralPeakArray& sourceSpectralPeaks,
											SpectralPeakArray& targetSpectralPeaks )
{
	targetSpectralPeaks.AddAll();
	targetSpectralPeaks.UpdateData();

	targetSpectralPeaks.CopyMembers(sourceSpectralPeaks);	
}

void DataUtil::CopyResidualSpectrumsMembers(Spectrum& sourceResidualSpectrum,
											Spectrum& targetResidualSpectrum)
{
	targetResidualSpectrum.AddAll();
	targetResidualSpectrum.UpdateData();

	int size = sourceResidualSpectrum.GetSize();
	targetResidualSpectrum.SetSize( size );
	Array<Complex>& srcComplexBuffer = sourceResidualSpectrum.GetComplexArray();
	Array<Complex>& targetComplexBuffer = targetResidualSpectrum.GetComplexArray();
	for (int r=0; r < size; r++)
	{
		targetComplexBuffer[r] = srcComplexBuffer[r];
	}
}

bool DataUtil::CheckMembers(Frame& sourceFrame,
							Frame& targetFrame)
{
	// Copy SpectralPeakArray data
	SpectralPeakArray& sourceSpectralPeaks = sourceFrame.GetSpectralPeakArray();
	SpectralPeakArray& targetSpectralPeaks = targetFrame.GetSpectralPeakArray();

	//	CopySpectralPeakArraysMembers(sourceSpectralPeaks, targetSpectralPeaks);
	DataArray& srcFreqBuffer = sourceSpectralPeaks.GetFreqBuffer();
	DataArray& srcMagBuffer = sourceSpectralPeaks.GetMagBuffer();
	DataArray& srcPhaseBuffer = sourceSpectralPeaks.GetPhaseBuffer();
	DataArray& srcBinPosBuffer = sourceSpectralPeaks.GetBinPosBuffer();
	DataArray& srcBinWidthBuffer = sourceSpectralPeaks.GetBinWidthBuffer();
	IndexArray& srcIndexArray = sourceSpectralPeaks.GetIndexArray();

	DataArray& targetFreqBuffer = targetSpectralPeaks.GetFreqBuffer();
	DataArray& targetMagBuffer = targetSpectralPeaks.GetMagBuffer();
	DataArray& targetPhaseBuffer = targetSpectralPeaks.GetPhaseBuffer();
	DataArray& targetBinPosBuffer = targetSpectralPeaks.GetBinPosBuffer();
	DataArray& targetBinWidthBuffer = targetSpectralPeaks.GetBinWidthBuffer();
	IndexArray& targetIndexArray = targetSpectralPeaks.GetIndexArray();

	int numberOfPeaks = sourceSpectralPeaks.GetnPeaks();
	for (int r=0; r < numberOfPeaks;r++)
	{
		// get frequency , mag and phase
		if (targetFreqBuffer[r] != srcFreqBuffer[r] ||
		targetMagBuffer[r] != srcMagBuffer[r] ||
		targetPhaseBuffer[r] != srcPhaseBuffer[r] ||
		targetBinPosBuffer[r] != srcBinPosBuffer[r] ||
		targetBinWidthBuffer[r] != srcBinWidthBuffer[r] ||
		targetIndexArray[r] != srcIndexArray[r])
			return false;
	}

	// Copy ResidualSpectrum data
	// (I would have moved this code into Spectrum as a CopyMembers method
	//  but that class is so convoluted that it would be a massive headache
	//  to make a general copy members class.)
	Spectrum& sourceResidualSpectrum = sourceFrame.GetResidualSpec();
	Spectrum& targetResidualSpectrum = targetFrame.GetResidualSpec();
	int size = sourceResidualSpectrum.GetSize();
	targetResidualSpectrum.SetSize( size );
	Array<Complex>& srcComplexBuffer = sourceResidualSpectrum.GetComplexArray();
	Array<Complex>& targetComplexBuffer = targetResidualSpectrum.GetComplexArray();
	for (int r=0; r < size; r++)
	{
		if (targetComplexBuffer[r] != srcComplexBuffer[r])
			return false;
	}

	return true;
}

void DataUtil::PrintSpectralPeakArray(SpectralPeakArray& sourceSpectralPeaks)
{
	DataArray& srcFreqBuffer = sourceSpectralPeaks.GetFreqBuffer();
	DataArray& srcMagBuffer = sourceSpectralPeaks.GetMagBuffer();

	int numberOfPeaks = sourceSpectralPeaks.GetnPeaks();
	std::cout << "Printing out the contents of the spectral peak array." << std::endl;
	for (int r=0; r < numberOfPeaks;r++)
	{
		// get frequency , mag and phase
		std::cout << "Frequency: " << srcFreqBuffer[r];
		std::cout << ", Magnitude: " << srcMagBuffer[r] << std::endl;
	}
	std::cout << "Done." << std::endl;
}

} // end namespace CLAM


