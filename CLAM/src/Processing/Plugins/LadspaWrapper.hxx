
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

class LadspaWrapper : public Processing
{
private:
	typedef void * SOPointer;
	LADSPA_Handle mInstance;
	const LADSPA_Descriptor * mDescriptor;
	SOPointer mSharedObject;

	std::string mFactoryKey;

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
	LadspaWrapper( const Config & );
	LadspaWrapper( const std::string& libraryFileName, unsigned index, const std::string& factoryKey );

	bool Do();
	virtual ~LadspaWrapper()
	{
		RemovePortsAndControls();
	}

	const char * GetClassName() const;
	
	bool ConcreteConfigure(const std::string& libraryFileName, unsigned index, const std::string& factoryKey);

};

} //namespace CLAM

#endif



