
#ifndef __LadspaLoader_hxx__
#define __LadspaLoader_hxx__

#include "ProcessingConfig.hxx"
#include "Processing.hxx"
#include <ladspa.h>
#include <dlfcn.h>
#include <string>
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include "Audio.hxx"
#include "Filename.hxx"

namespace CLAM
{

class LadspaLoaderConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (LadspaLoaderConfig, 4, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, Filename, SharedObjectName );
	DYN_ATTRIBUTE (1, public, int, Index );
	DYN_ATTRIBUTE (2, public, unsigned long, SampleRate );
	DYN_ATTRIBUTE (3, public, unsigned long, Size );
protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
		SetSharedObjectName("");
		SetIndex(0);
		SetSampleRate(44100);
		SetSize(512);
	}
};

class LadspaLoader : public Processing
{
	LadspaLoaderConfig mConfig;
	typedef void * SOPointer;
	LADSPA_Handle mInstance;
	const LADSPA_Descriptor * mDescriptor;
	SOPointer mSharedObject;

	std::vector<Audio> mInputAudio;
	std::vector<Audio> mOutputAudio;
	std::vector< InPortTmpl<Audio>* > mInputPorts;
	std::vector< OutPortTmpl<Audio>* > mOutputPorts;

	std::vector< InControl* > mInputControls;
	std::vector< OutControl* > mOutputControls;
	std::vector< LADSPA_Data > mInputControlValues;
	std::vector< LADSPA_Data > mOutputControlValues;

	void ConfigurePortsAndControls();
	
public:
	LadspaLoader();
	LadspaLoader( const LadspaLoaderConfig & cfg);
	virtual ~LadspaLoader();
	bool Do();
	bool ConcreteStop();
	bool ConcreteStart();		
	bool ConcreteConfigure( const ProcessingConfig & cfg);

	bool ModifiesPortsAndControlsAtConfiguration()
	{
		return true;
	}
	
	const char* GetClassName() const
	{ 
		return "LadspaLoader";
	}

	const ProcessingConfig &GetConfig() const 
	{ 
		return mConfig;
	}
};
	
} // namespace CLAM

#endif // __LadspaLoader_hxx__
