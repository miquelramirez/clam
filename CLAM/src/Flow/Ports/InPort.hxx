#ifndef _InPort_hxx_
#define _InPort_hxx_

#include "Port.hxx"
#include <string>

namespace CLAM
{

class Processing;
class OutPort;

/** Input port */
class InPort: public Port
{
	/** True if the processing object may use the storage object
	 * in this port to write on it the data of an output port
	 */
	bool mCanDoInplace;

public:
	InPort(const std::string &n, Processing *o, int length, int hop = 0, bool inplace=false);
	virtual ~InPort() {}

	bool CanDoInplace(void)  {return mCanDoInplace;}
	
	bool IsConnectedTo( OutPort& out );
	virtual bool IsReadyForReading() = 0;
};



} // namespace CLAM

#endif
