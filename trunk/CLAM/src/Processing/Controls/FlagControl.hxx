
#ifndef _FlagControl_hxx_
#define _FlagControl_hxx_

#include "DataTypes.hxx"
#include "OutControl.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include <string>

namespace CLAM
{

class FlagControlConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (FlagControlConfig, 2, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, bool, Flag );
protected:
	void DefaultInit(void);
};

class FlagControl : public Processing
{
private:
	
	FlagControlConfig mConfig;
	FloatOutControl mFlagOutControl;

public:
	FlagControl();
	FlagControl( const FlagControlConfig & );

	bool Do();
	virtual ~FlagControl(){}
	const char * GetClassName() const {return "FlagControl";}
	
	inline const ProcessingConfig &GetConfig() const { return mConfig;}
	bool ConcreteConfigure(const ProcessingConfig& c);

};

} //namespace CLAM

#endif

