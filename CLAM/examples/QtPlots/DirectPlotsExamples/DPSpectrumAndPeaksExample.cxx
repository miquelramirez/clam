#include "XMLStorage.hxx"
#include "DPSpectrumAndPeaks.hxx"

int main()
{
	// load data from XML file

	CLAM::Spectrum spec;
	CLAM::XMLStorage::Restore(spec,"../../data/spectrum_data.xml");

	CLAM::SpectralPeakArray peaks;
	CLAM::XMLStorage::Restore(peaks,"../../data/spectralpeaks_data.xml");

	// plot data
	CLAM::VM::PlotSpectrumAndPeaks(spec,peaks,"Spectrum and Peaks"); 

	return 0;
}


// END
