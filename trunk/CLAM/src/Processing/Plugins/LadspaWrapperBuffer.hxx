
#ifndef _LadspaWrapperBuffer_hxx_
#define _LadspaWrapperBuffer_hxx_

#include "DataTypes.hxx"
#include "OutControl.hxx"
#include "InControl.hxx"
#include "Processing.hxx"
#include "RunTimeLibraryLoader.hxx"
#include "OutPort.hxx"
#include "InPort.hxx"
#include "Audio.hxx"

#include <ladspa.h>

#include <string>

namespace CLAM
{

class LadspaWrapperBuffer : public Processing
{
private:
	typedef void * SOPointer;
	LADSPA_Handle _instance;
	const LADSPA_Descriptor * _descriptor;
	SOPointer _sharedObject;
	std::string _libraryFileName;

	std::vector< InPort<Audio>* > _inputPorts;
	std::vector< OutPort<Audio>* > _outputPorts;

	std::vector< FloatInControl* > _inputControls;
	std::vector< FloatOutControl* > _outputControls;
	std::vector< LADSPA_Data > _outputControlValues;
	unsigned _bufferSize;
	std::string _factoryKey;
	void RemovePortsAndControls();
	void ConfigurePortsAndControls();
	void ConfigureControlsPointers();
	void DoUpdatePortsPointers();

public:
	LadspaWrapperBuffer( const Config & c = Config());
	LadspaWrapperBuffer( const std::string& libraryFileName, unsigned index, const std::string& factoryKey );
	bool ConcreteStart();
	bool ConcreteStop();
	bool ConcreteConfigure(const ProcessingConfig&);

	bool Do();
	virtual ~LadspaWrapperBuffer();

	const char * GetClassName() const;

	bool LoadLibraryFunction(const std::string& libraryFileName, unsigned index, const std::string& factoryKey);

};

} //namespace CLAM

#endif



