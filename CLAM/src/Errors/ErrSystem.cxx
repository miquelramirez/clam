#include "ErrSystem.hxx"

namespace CLAM
{

LockError::LockError( const char* msg )
	: Err( msg )
{
}

ThreadResourceError::ThreadResourceError( const char* msg )
	: Err( msg )
{
}

} // end of namespace CLAM
