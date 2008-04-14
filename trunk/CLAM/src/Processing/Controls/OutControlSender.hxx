
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

	class EMapping : public Enum
	{
	public:

		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EMapping() : Enum(sEnumValues, sDefault) {}
		EMapping(tValue v) : Enum(sEnumValues, v) {};
		EMapping(std::string s) : Enum(sEnumValues, s) {};

		typedef enum {
			eLinear,		///< 1:1 relationship (default)
			eInverted,		///< Inverted linear relationship
			eLog, 			///< More resolution in the low range
			eReverseLog		///< More resolution in the upper range
		}; 
		
		Component* Species() const
		{
			return new EMapping;
		};
		
	};

	DYNAMIC_TYPE_USING_INTERFACE (OutControlSenderConfig, 6, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, TControlData, Min );
	DYN_ATTRIBUTE (1, public, TControlData, Default );
	DYN_ATTRIBUTE (2, public, TControlData, Max );
	DYN_ATTRIBUTE (3, public, TControlData, Step );
	DYN_ATTRIBUTE (4, public, EControlRepresentation, ControlRepresentation );
	DYN_ATTRIBUTE (5, public, EMapping, Mapping );


protected:
	void DefaultInit(void);
};

class OutControlSender : public Processing
{
	OutControlSenderConfig mConfig;
	OutControl mOutput;
	TControlData mLastValue;
	bool mFirstDoAfterStart;
public:
	OutControlSender();
	OutControlSender( const OutControlSenderConfig & );

	bool Do();
	const char * GetClassName() const {return "OutControlSender";}

	const ProcessingConfig &GetConfig() const { return mConfig;}
	void SendControl(TControlData value);
protected:
	bool ConcreteConfigure(const ProcessingConfig& c);
	bool ConcreteStart();
};

} //namespace CLAM

#endif //__OutControlSender_hxx__



