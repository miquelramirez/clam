/* Definition of the Melody Type as an array of notes */

#ifndef _Melody_
#define _Melody_

#include "DynamicType.hxx"
#include "Array.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingDataConfig.hxx"
#include "ProcessingData.hxx"

#include "Note.hxx"

namespace CLAM{

	//
	// Class MelodyConfig :
	//
	
 class MelodyConfig : public ProcessingDataConfig
 {
 public:
	 REG_NUM_ATTR_USING_INTERFACE (MelodyConfig, 0, ProcessingDataConfig);
	 void DefaultInit();
	 void Default();
 };
	
	
	///////////////////////////////////////////////////////
 // Class Melody:
 //
 
 class Melody: public ProcessingData
 {
 public:
	 REG_NUM_ATTR_USING_INTERFACE (Melody, 2, ProcessingData);
 private:
	 REGISTER (0, prConfig, MelodyConfig); 
 public:
	 REGISTER (1, noteArray, Array<Note>); // Melody fundamental frequency 
	 
	 Melody(const MelodyConfig &newConfig);
	 
	 //* Definition of the pure virtual function declared at the interface ProcessingData
	 const ProcessingDataConfig &GetProcessingDataConfig() const
	 { 
		 return GetConfig();
	 };
	 
	 void Configure(const MelodyConfig &newConfig);
	 
	 const MelodyConfig& GetConfig() const;
	 
	 void DefaultInit();	
 };
	
}; // namespace MTG


#endif // _Melody _
