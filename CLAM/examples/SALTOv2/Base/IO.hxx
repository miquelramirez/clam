#ifndef _IO_
#define _IO_

#include "EIOMode.hxx"

namespace CLAM
{

class IO
{
/* base class for data I/O */
protected:
	EIOMode mMode;
public:
	IO(const EIOMode& mode);
};

} // end of namesapce CLAM

#endif
