
#ifndef __OutControlSender_hxx__
#define __OutControlSender_hxx__

#include "OutControl.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include <list>

namespace CLAM
{

class OutControlSenderConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (OutControlSenderConfig, 1, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, int, NumControls ); 
protected:
	void DefaultInit(void);
};

class OutControlSender : public Processing
{
	OutControlSenderConfig mConfig;
	std::list<OutControl*> mOutputs;
public:
	OutControlSender();
	OutControlSender( const OutControlSenderConfig & );

	bool Do();
	const char * GetClassName() const {return "OutControlSender";}
	bool ModifiesPortsAndControlsAtConfiguration()
	{
		return true;
	}

	const ProcessingConfig &GetConfig() const { return mConfig;}
	bool ConcreteConfigure(const ProcessingConfig& c);
};

} //namespace CLAM

#endif //__OutControlSender_hxx__



