/* Pitch note type definition = pitch + octave */

#ifndef _PitchNote_
#define _PitchNote_

#include "DynamicType.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingDataConfig.hxx"
#include "ProcessingData.hxx"

#include "Enum.hxx"

namespace CLAM {

	// Class EPitch
	class EPitch : public Enum	{
	public:
		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EPitch() : Enum(sEnumValues, sDefault) {}
		EPitch(tValue v) : Enum(sEnumValues, v) {};
		EPitch(std::string s) : Enum(sEnumValues, s) {};
		
		typedef enum {
			eNone,eA,eB,eC,eD,eE,eF,eG
		};	

		virtual Component* Species() const
		{
			return (Component*) new EPitch();
		};
	};
	
	// Class PitchNoteConfig 
	class PitchNoteConfig : public ProcessingDataConfig
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (PitchNoteConfig, 0, ProcessingDataConfig);
		void DefaultInit();
		void Default();
	};

	///////////////////////////////////////////////////////
	// Class PitchNote:
	//
	
	class PitchNote: public ProcessingData
	{
	public:
		REG_NUM_ATTR_USING_INTERFACE (PitchNote, 3, ProcessingData);
	private:
		REGISTER (0, prConfig, PitchNoteConfig);
	public:
		REGISTER (1, pitch,    EPitch);   
		REGISTER (2, octave,   int);
		
		PitchNote(const PitchNoteConfig &newConfig);

		// Definition of the pure virtual function declared at the interface ProcessingData
		const ProcessingDataConfig &GetProcessingDataConfig() const
		{ 
			return GetConfig();
		};

		void Configure(const PitchNoteConfig &newConfig);

		const PitchNoteConfig& GetConfig() const;
		
		void DefaultInit();	
	};
	
}; // namespace MTG

#endif // _PitchNote_


