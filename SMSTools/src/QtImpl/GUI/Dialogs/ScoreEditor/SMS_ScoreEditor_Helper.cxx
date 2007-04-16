#include "SMS_ScoreEditor_Helper.hxx"

namespace CLAMVM
{
	void clampBPFValues( CLAM::BPF& bpf, double min, double max )
	{
		
		for ( int i = 0; i < bpf.Size(); i++ )
		{
			if ( bpf.GetValueFromIndex( i ) < min )
				bpf.SetValue( i, min );
			else if ( bpf.GetValueFromIndex( i ) > max )
				bpf.SetValue( i, max );						
		}

	}

	void clearBPF( CLAM::BPF& bpf )
	{
		while( bpf.Size() )
			bpf.DeleteIndex(0);				
	}
	
}

