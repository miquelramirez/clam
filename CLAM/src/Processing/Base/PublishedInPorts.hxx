#ifndef _PublishedInPorts_hxx_
#define _PublishedInPorts_hxx_

#include <string>
#include <vector>

namespace CLAM
{
	
//forward declarations:
class InPort;

class PublishedInPorts
{
	std::vector<InPort*> mInPorts;
public:
	typedef std::vector<InPort*>::iterator Iterator;
	typedef std::vector<InPort*>::const_iterator ConstIterator;

	InPort& GetByNumber(int index) const;
	InPort& Get( const std::string & name ) const;
	int Size() const;
	bool AreReadyForReading();
	void Publish(InPort *);
	void Clear(){ mInPorts.clear(); }
	Iterator Begin();
	Iterator End();
	ConstIterator Begin() const;
	ConstIterator End() const;

};

} // namespace CLAM

#endif
