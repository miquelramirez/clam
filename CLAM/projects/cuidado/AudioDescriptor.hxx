#ifndef __AUDIODESCRIPTOR_H__
#define __AUDIODESCRIPTOR_H__


//#include "AudioDescriptorFlags.hxx"
#include "DynamicType.hxx"
#include "EDataFormat.hxx"
#include "ProcessingData.hxx"
#include "ProcessingDataConfig.hxx"
#include "DataTypes.hxx"
#include "AudioDescriptorFlags.hxx"

/*
 * This class holds Descriptors computed from Audio data 
 * 
 *
 */

namespace MTG {

	class AudioDescriptorConfig : public ProcessingDataConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (AudioDescriptorConfig, 2, ProcessingDataConfig);

		void DefaultInit();

		REGISTER (0, Type,AudioDescriptorFlags);
		REGISTER (1, HoldsData, bool);
	};


	class AudioDescriptor : public ProcessingData {
	public:
		REG_NUM_ATTR_USING_INTERFACE (AudioDescriptor, 11, ProcessingData);
	private:
		REGISTER(0,Mean,TData);
		REGISTER(1,Variance,TData);
		REGISTER(2,TemporalCentroid,TData);
		REGISTER(3,Attack,TData);
		REGISTER(4,Decay,TData);
		REGISTER(5,Sustain,TData);
		REGISTER(6,Release,TData);
		REGISTER(7,LogAttackTime,TData);
		REGISTER(8,Energy,TData);
		REGISTER(9,ZeroCrossingRate,TData);
		REGISTER(10,RiseTime,TData);

	public:

		AudioDescriptor(AudioDescriptorConfig& c);
		void Configure(AudioDescriptorConfig& c);
		virtual const ProcessingDataConfig& GetProcessingDataConfig() const; 
		const AudioDescriptorConfig& GetConfig() const;

		AudioDescriptorConfig prConfig;
		friend class AudioDescriptorGen;

	};
	

};


#endif /* __AUDIODESCRIPTOR_H__ */
