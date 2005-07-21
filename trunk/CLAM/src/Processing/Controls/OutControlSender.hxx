
#ifndef __OutControlSender_hxx__
#define __OutControlSender_hxx__

#include "OutControl.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "Enum.hxx"

namespace CLAM
{

class OutControlSenderConfig : public ProcessingConfig
{	
public:
	class EControlRepresentation : public Enum
	{
	public:

		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EControlRepresentation() : Enum(sEnumValues, sDefault) {}
		EControlRepresentation(tValue v) : Enum(sEnumValues, v) {};
		EControlRepresentation(std::string s) : Enum(sEnumValues, s) {};

		typedef enum {
			eUndetermined,
			eVerticalSlider,
			eHorizontalSlider,
			eKnot,
			eSpinBox		
		}; 
		
		Component* Species() const
		{
			return new EControlRepresentation;
		};
		
	};

	DYNAMIC_TYPE_USING_INTERFACE (OutControlSenderConfig, 5, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, TControlData, Min );
	DYN_ATTRIBUTE (1, public, TControlData, Default );
	DYN_ATTRIBUTE (2, public, TControlData, Max );
	DYN_ATTRIBUTE (3, public, TControlData, Step );
	DYN_ATTRIBUTE (4, public, EControlRepresentation, ControlRepresentation );


protected:
	void DefaultInit(void);
};

class OutControlSender : public Processing
{
	OutControlSenderConfig mConfig;
	OutControl mOutput;
public:
	OutControlSender();
	OutControlSender( const OutControlSenderConfig & );

	bool Do();
	const char * GetClassName() const {return "OutControlSender";}

	const ProcessingConfig &GetConfig() const { return mConfig;}
	bool ConcreteConfigure(const ProcessingConfig& c);
};

} //namespace CLAM

#endif //__OutControlSender_hxx__



