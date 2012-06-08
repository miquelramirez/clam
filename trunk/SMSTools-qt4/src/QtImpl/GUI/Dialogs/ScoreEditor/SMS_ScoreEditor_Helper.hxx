#ifndef __SCOREEDITOR_HELPER__
#define __SCOREEDITOR_HELPER__

#include <CLAM/BPF.hxx>

namespace CLAMVM
{
	void clampBPFValues( CLAM::BPF& bpf, double min, double max );
	void clearBPF( CLAM::BPF& bpf );
}


#endif // SMS_ScoreEditor_Helper.hxx

