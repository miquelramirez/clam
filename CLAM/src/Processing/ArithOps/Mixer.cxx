#include "Mixer.hxx"

using namespace CLAM;

bool Mixer::Do( Array< Audio >& AudioArray, Audio& out) 
{
	int array_size = AudioArray.Size();
	int i ;

	TData* out_ptr    = out.GetBuffer().GetPtr();
	TData* out_endptr = out_ptr + out.GetSize();

	TData* audio_ptr  = AudioArray[ 0 ].GetBuffer().GetPtr();
	
	while ( out_ptr != out_endptr )
	{
		( *out_ptr++ ) = ( *audio_ptr++ ) ;
	}

	for( i = 1; i < array_size ; i++ )
	{
		out_ptr = out.GetBuffer().GetPtr();
		audio_ptr  = AudioArray[ i ].GetBuffer().GetPtr();

		while ( out_ptr != out_endptr )
		{
			( *out_ptr++ ) += ( *audio_ptr++ ) ;
		}
	}

	out_ptr = out.GetBuffer().GetPtr();

	while ( out_ptr != out_endptr )
	{
		( *out_ptr++ ) /= (TData) ( array_size );
	}

	return true;
}
