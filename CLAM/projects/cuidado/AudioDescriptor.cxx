


#include "ProcessingData.hxx"
#include "AudioDescriptor.hxx"





namespace MTG {

	void AudioDescriptorConfig::Init()
	{
		AddType();
		UpdateData();
	}
	

	AudioDescriptor::AudioDescriptor(AudioDescriptorConfig& c): ProcessingData(12)
	{
		MandatoryInit();
		Configure(c);
	}

	void AudioDescriptor::Configure(AudioDescriptorConfig& c)
	{
		if (c.GetType().mean)
			AddMean();
		if(c.GetType().variance)
			AddVariance();
		if (c.GetType().temporalCentroid)
			AddTemporalCentroid();
		if (c.GetType().attack)
			AddAttack();
		if (c.GetType().decay)
			AddDecay();
		if (c.GetType().sustain)
			AddSustain();
		if (c.GetType().release)
			AddRelease();
		if (c.GetType().logAttackTime)
			AddLogAttackTime();
		if (c.GetType().energy)
			AddEnergy();
		if (c.GetType().zeroCrossingRate)
			AddZeroCrossingRate();
		if (c.GetType().riseTime)
			AddRiseTime();
		UpdateData();
		prConfig = c;
	}



	const ProcessingDataConfig& AudioDescriptor::GetProcessingDataConfig() const 
	{
		return  GetConfig();
	}
	
	const AudioDescriptorConfig& AudioDescriptor::GetConfig() const {
		return prConfig;
	}
	



};
