#ifndef CLT_DESCRIPTOR
#define CLT_DESCRIPTOR

//#include <ladspa_types.h>
#include <ladspa.h>

class CLT_Descriptor;
class CLT_Port;

typedef CLT_Descriptor * CLT_Descriptor_ptr;
void registerNewPluginDescriptor(CLT_Descriptor * psDescriptor);


class CLT_Descriptor : public _LADSPA_Descriptor
{
public:
		
		CLT_Descriptor(){}
		CLT_Descriptor (const unsigned long uniqueID,
						const char* label,
						const char* name,
						const char* maker,
						const char* copyright,
						const unsigned long portCount);
		virtual ~CLT_Descriptor();
		static int ConnectPort(CLT_Port* port);
		bool SetProperties (bool realTime, bool inplaceBroken, bool hardRTCapable);
		bool SetProperties (LADSPA_Properties properties);
		
/////////////////////
//    GETTERS      //
/////////////////////
		
		const char* GetName(){ return Name; }
		int GetPortCount(){ return PortCount;}
		int GetUniqueID(){return UniqueID;}
		
		
private:
		static int sNumPort;
		static char ** sPortNames;
		static LADSPA_PortDescriptor * sPortDescriptors;
		static LADSPA_PortRangeHint * sPortRangeHints;
		
};





class CLT_Port
{
public:
		CLT_Port(): mProperties(0), mName(""){CreateHint(0);}
		~CLT_Port(){delete &mHint;}
		CLT_Port (bool input, bool output, bool audio, bool control, char* name);
		CLT_Port (LADSPA_PortDescriptor properties, char* name):
				mProperties (properties),
				mName (name)
		{}
		bool CreateHint(bool boundedBelow, bool boundedAbove, bool toggled, 
						bool sampleRate, bool logarithmic, bool integer);
		bool CreateHint(LADSPA_PortRangeHintDescriptor hint){mHint.HintDescriptor = hint; return 1;}
		bool SetLowerBound(LADSPA_Data bound){mHint.LowerBound = bound; return 1;}
		bool SetUpperBound(LADSPA_Data bound){mHint.UpperBound = bound; return 1;}
		
/////////////////////
/// Getters       ///
/////////////////////
		
		LADSPA_PortDescriptor GetProperties(){ return mProperties; }
		char* GetName(){ return mName; }
		LADSPA_PortRangeHint GetPortRangeHint (){return mHint;}
		
private:				
		LADSPA_PortDescriptor mProperties;
		char* mName;
		LADSPA_PortRangeHint mHint;
};


#endif
