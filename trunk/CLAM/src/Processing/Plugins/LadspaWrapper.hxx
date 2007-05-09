
#ifndef _LadspaWrapper_hxx_
#define _LadspaWrapper_hxx_

#include <CLAM/DataTypes.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/Mapping.hxx>

#include <ladspa.h>

#include <string>

namespace CLAM
{
	class AudioInPort;
	class AudioOutPort;
	class InControl;
	class OutControl;

class LadspaWrapperConfig : public ProcessingConfig //TODO remove this config. pass params to constructor
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (LadspaWrapperConfig, 6, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, std::string, LibraryFileName);
	DYN_ATTRIBUTE (2, public, std::string, FactoryKey);
	DYN_ATTRIBUTE (3, public, int, Index);
	DYN_ATTRIBUTE (4, public, int, SampleRate);
	DYN_ATTRIBUTE (5, public, int, Size);

protected:
	void DefaultInit(void);
};

class LadspaWrapper : public Processing
{
private:
	LadspaWrapperConfig mConfig;
	typedef void * SOPointer;
	LADSPA_Handle mInstance;
	const LADSPA_Descriptor * mDescriptor;
	SOPointer mSharedObject;

	std::vector< AudioInPort* > mInputPorts;
	std::vector< AudioOutPort* > mOutputPorts;

	std::vector< InControl* > mInputControls;
	std::vector< OutControl* > mOutputControls;
	std::vector< LADSPA_Data > mOutputControlValues;

	void ConfigurePortsAndControls();
	void RemovePortsAndControls();
	void UpdateControlsPointers();
	void UpdatePortsPointers();

public:
	LadspaWrapper();
	LadspaWrapper( const LadspaWrapperConfig & );
	LadspaWrapper( const std::string& libraryFileName, unsigned index, const std::string& factoryKey );

	bool Do();
	virtual ~LadspaWrapper()
	{
		RemovePortsAndControls();
	}

	const char * GetClassName() const;
	
	inline const ProcessingConfig &GetConfig() const { return mConfig; }
	bool ConcreteConfigure(const ProcessingConfig& c);

};

} //namespace CLAM

#endif



