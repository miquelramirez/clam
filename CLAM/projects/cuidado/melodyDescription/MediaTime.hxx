
/* Media Time type definition                    *
 * This type propose a flexible way of defining  *
 * the temporal location of an audio segment     *
 * Types: {begin, end}, {begin, duration}        *
 * This type is thought to be used to define     *
 * some kind of probability when defining segment*
 * boundaries                                    *
 *                                               */

#ifndef _MediaTime_
#define _MediaTime_

#include "DynamicType.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingDataConfig.hxx"
#include "ProcessingData.hxx"

#include "Enum.hxx"

namespace CLAM {
	
	// Class eMediaTimeType
	class EMediaTimeType : public Enum {
	public:
		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EMediaTimeType() : Enum(sEnumValues, sDefault) {}
		EMediaTimeType(tValue v) : Enum(sEnumValues, v) {};
		EMediaTimeType(std::string s) : Enum(sEnumValues, s) {};
		
		typedef enum {
			eBeginEnd,eBeginDuration
		};	

		virtual Component* Species() const
		{
			return (Component*) new EMediaTimeType();
		};
	};

	// Class MediaTimeConfig 
	class MediaTimeConfig : public ProcessingDataConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (MediaTimeConfig, 1, ProcessingDataConfig);
		REGISTER(0, type, EMediaTimeType);
		void DefaultInit();
		void Default();
	};
	
	
	///////////////////////////////////////////////////////
	// Class MediaTime:
	//
	
	class MediaTime: public ProcessingData
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (MediaTime, 4, ProcessingData);
	private:
		REGISTER (0, prConfig, MediaTimeConfig); 
	public:
		REGISTER (1, begin,    TData);   
		REGISTER (2, end,      TData);
		REGISTER (3, duration, TData);
		
	MediaTime(const MediaTimeConfig &newConfig);
 
	//* Definition of the pure virtual function declared at the interface ProcessingData
	const ProcessingDataConfig &GetProcessingDataConfig() const
	{ 
		return GetConfig();
	};

	void Configure(const MediaTimeConfig &newConfig);

	const MediaTimeConfig& GetConfig() const;

	void DefaultInit();	
};

}; // namespace MTG

#endif


