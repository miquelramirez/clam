/* Note type definition */

#ifndef _Note_
#define _Note_

#include "DynamicType.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingDataConfig.hxx"
#include "ProcessingData.hxx"

#include "Enum.hxx"
#include "MediaTime.hxx"
#include "PitchNote.hxx"

namespace CLAM {

	// Class NoteConfig :	
	class NoteConfig : public ProcessingDataConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (NoteConfig, 2, ProcessingDataConfig);
		REGISTER(0, hasPitch, int); // If the pitch has been computed (by default, only fundFreq) BOOL!!!
		REGISTER(1, timeType, EMediaTimeType);
		void DefaultInit();
		void Default();
	};
	
	
	///////////////////////////////////////////////////////
	// Class Note:
	//
	class Note: public ProcessingData
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (Note, 7, ProcessingData);
	private:
		REGISTER (0, prConfig, NoteConfig); // Not configure
	public:
		REGISTER (1, pitchNote,    PitchNote);   
		REGISTER (2, fundFreq, TData); // Note fundamental frequency 
		REGISTER (3, time,     MediaTime); // Note location
		
		// Attack, sustain, release duration
		REGISTER (4, attack,  MediaTime);
		REGISTER (5, sustain, MediaTime);
		REGISTER (6, release, MediaTime);

		Note(const NoteConfig &newConfig);
		
		//* Definition of the pure virtual function declared at the interface ProcessingData
		const ProcessingDataConfig &GetProcessingDataConfig() const
		{ 
			return GetConfig();
		};
		
		void Configure(const NoteConfig &newConfig);
		
		const NoteConfig& GetConfig() const;
		
		void DefaultInit();	
		
	};

}; // namespace MTG


#endif // _Note_
