

#ifndef _HARMONICDESCRIPTOR_H_
#define _HARMONICDESCRIPTOR_H_


#include "HarmonicDescriptorFlags.hxx"



namespace MTG {

	class HarmonicDescriptorConfig : public ProcessingDataConfig {

	public:
		REG_NUM_ATTR_USING_INTERFACE (HarmonicDescriptorConfig, 2, ProcessingDataConfig);

		void DefaultInit();

		REGISTER (0, Type,HarmonicDescriptorFlags);
		REGISTER (1, HoldsData, bool);

	};


	class HarmonicDescriptor : public ProcessingData {
	public:
		REG_NUM_ATTR_USING_INTERFACE (HarmonicDescriptor, 10, ProcessingData);
	private:
		REGISTER(0,TransientMidPoint,TData);

		REGISTER(1,Harmonic1RiseTime,TData);
		REGISTER(2,Harmonic234RiseTime,TData);
		REGISTER(3,HarmonicHighRiseTime,TData);

		REGISTER(4,Harmonic1Overshoot,TData);
		REGISTER(5,Harmonic2Overshoot,TData);
		REGISTER(6,Harmonic3Overshoot,TData);

		REGISTER(7,LowHarmonicsDelay,TData);
		REGISTER(8,HighHarmonicsDelay,TData);

		HarmonicDescriptorConfig prConfig;
	public:


		HarmonicDescriptor(HarmonicDescriptorConfig& c);
		void Configure(HarmonicDescriptorConfig& c);
		virtual const ProcessingDataConfig& GetProcessingDataConfig() const; 
		const HarmonicDescriptorConfig& GetConfig() const;
	};


}


#endif
