#ifndef _CSaltoMIDITarget_
#define _CSaltoMIDITarget_

//#include "CMTGMIDIIn.hxx"
#include "CMTGMIDITarget.hxx"
//#include "CSaltoParameter.hxx"
#include "Parameters.hxx"

class CSaltoMIDITarget:public CMTGMIDITarget
{
public:	
//	CSaltoMIDITarget(CSaltoParameter* pParams);
	CSaltoMIDITarget(MTG::Parameters* pParams);
	virtual ~CSaltoMIDITarget();

	void Handle(TMIDITime timetag, const TMIDIByte bytes[], TInt32 size);

private:	
//	CSaltoParameter *mpParams;
	MTG::Parameters *mpParams;
	int mLastPitch;
};
#endif //_CSaltoMIDITarget_

