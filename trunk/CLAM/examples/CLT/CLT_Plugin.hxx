#ifndef CLT_PLUGIN
#define CLT_PLUGIN

#include "ladspa_types.h"
#include "ProcessingComposite.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"

#include "CLT_Descriptor.hxx"
#include "Audio.hxx"

#define CONNECT(DESCR, CLASS_NAME)\
 DESCR->cleanup = CLASS_NAME::CleanUp;\
 DESCR->instantiate = CLASS_NAME::Instantiate;\
 DESCR->connect_port = CLASS_NAME::ConnectPort;\
 DESCR->run = CLASS_NAME::Run;\
 CLASS_NAME::ConcreteConnect(DESCR);

using namespace CLAM;

class CLT_Descriptor;
class CLT_Plugin;
class CLT_PluginConfig;

class CLT_PluginConfig : public ProcessingConfig
{
public:
		DYNAMIC_TYPE_USING_INTERFACE (CLT_PluginConfig, 1, ProcessingConfig);
		
		DYN_ATTRIBUTE (0,public, std::string, Name);
		
protected:
		void DefaultInit(void);
};


class CLT_Plugin : public ProcessingComposite
{
		
public:
		CLT_PluginConfig   mConfig;
		InPortTmpl<Audio>  mInput;
		OutPortTmpl<Audio> mOutput;
		
		CLT_Plugin();
		CLT_Plugin(const CLT_PluginConfig&);
		virtual ~CLT_Plugin();
		
//////////////////////////////
/// CLAM DEDICATED METHODS ///
//////////////////////////////
protected:		
		virtual void Convert(Audio &audio);
		virtual void Deconvert(Audio &audio);
public:
		
		
		virtual bool ConcreteConfigure(const ProcessingConfig& c);
		virtual const ProcessingConfig &GetConfig() const { return mConfig; }
//to implement in son classes
		virtual bool Do(void){return true;} //a implementar pels fills
		
		
////////////////////////////////
/// LADSPA DEDICATED METHODS ///
////////////////////////////////

//to implement if there is necessity		
		static void ConcreteConnect(CLT_Descriptor* descr);

//to implement always
		static LADSPA_Handle CLT_Plugin::Instantiate
		( const struct _LADSPA_Descriptor* Descriptor, unsigned long SampleRate );
//to implement always
		static  void ConnectPort 
		(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation){}
//to implement if there is necessity		
		static void Activate
		(LADSPA_Handle Instance){}
//to implement always
		static void Run
		(LADSPA_Handle Instance, unsigned long SampleCount){}
//to implement if there is necessity		
		static void RunAdding 
		(LADSPA_Handle Instance, unsigned long SampleCount){}
//to implement if there is necessity		
		static void SetRunAddingGain
		(LADSPA_Handle Instance,LADSPA_Data Gain){}
//to implement if there is necessity				
		static void Deactivate
		(LADSPA_Handle Instance){}
//implemented here		
		static void CleanUp
		(LADSPA_Handle Instance);
		
};


#endif
