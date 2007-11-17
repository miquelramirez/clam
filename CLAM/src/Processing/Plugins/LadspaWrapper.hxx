
#ifndef _LadspaWrapper_hxx_
#define _LadspaWrapper_hxx_

#include <CLAM/DataTypes.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/Processing.hxx>

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
	LADSPA_Handle _instance;
	const LADSPA_Descriptor * _descriptor;
	SOPointer _sharedObject;

	std::vector< AudioInPort* > _inputPorts;
	std::vector< AudioOutPort* > outputPorts;

	std::vector< InControl* > _inputControls;
	std::vector< OutControl* > _outputControls;
	std::vector< LADSPA_Data > _outputControlValues;
	unsigned _bufferSize;
	std::string _factoryKey;
	void ConfigurePortsAndControls();
	void RemovePortsAndControls();
	void UpdateControlsPointers();
	void UpdatePortsPointers();

public:
	LadspaWrapper( const Config & c = Config());
	LadspaWrapper( const std::string& libraryFileName, unsigned index, const std::string& factoryKey );
	bool ConcreteStart();
	bool ConcreteStop();
	bool ConcreteConfigure(const ProcessingConfig&);

	bool Do();
	virtual ~LadspaWrapper();

	const char * GetClassName() const;
	
	bool LoadLibraryFunction(const std::string& libraryFileName, unsigned index, const std::string& factoryKey);

};

} //namespace CLAM

#endif



