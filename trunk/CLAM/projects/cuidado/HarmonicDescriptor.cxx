#include "ProcessingData.hxx"
#include "HarmonicDescriptor.hxx"





namespace MTG {
	
	void HarmonicDescriptorConfig::Init()
	{
		AddType();
		UpdateData();
	}


	HarmonicDescriptor::HarmonicDescriptor(HarmonicDescriptorConfig& c): ProcessingData(10)
	{
		MandatoryInit();
		Configure(c);
	}

	void HarmonicDescriptor::Configure(HarmonicDescriptorConfig& c)
	{

		if (c.GetType().transientMidPoint)
			AddTransientMidPoint();


		if (c.GetType().harmonicRiseTimes) {
			AddHarmonic1RiseTime();
			AddHarmonic234RiseTime();
			AddHarmonicHighRiseTime();
		}

		if (c.GetType().overshoots) {
			AddHarmonic1Overshoot();
			AddHarmonic2Overshoot();
			AddHarmonic3Overshoot();
		}

		if (c.GetType().lowHarmonicsDelay)
			AddLowHarmonicsDelay();

		if (c.GetType().highHarmonicsDelay)
			AddHighHarmonicsDelay();

		UpdateData();
		prConfig = c;
	}


	const ProcessingDataConfig& HarmonicDescriptor::GetProcessingDataConfig() const 
	{
		return	GetConfig();
	}
	
	const HarmonicDescriptorConfig& HarmonicDescriptor::GetConfig() const {
		return prConfig;

	}
	



};
