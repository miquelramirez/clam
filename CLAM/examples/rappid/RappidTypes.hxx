#ifndef _RAPPID_TYPES_H
#define _RAPPID_TYPES_H

#include "DynamicType.hxx"

using namespace CLAM;

class RappidAppConfig: public DynamicType {
public:
	DYNAMIC_TYPE(RappidAppConfig,4);
	DYN_ATTRIBUTE (0,  public, int,   FrameSize);
	DYN_ATTRIBUTE (1,  public, TData, SampleRate);
	DYN_ATTRIBUTE (2,  public, bool,  Verbose);
	DYN_ATTRIBUTE (3,  public, bool,  ADAT);
	void DefaultInit(void);
	void Dump(void);
};


#endif
