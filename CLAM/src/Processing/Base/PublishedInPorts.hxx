#ifndef _PublishedInPorts_hxx_
#define _PublishedInPorts_hxx_

#include <string>
#include <vector>

namespace CLAM
{
	
//forward declarations:
class InPortBase;

class PublishedInPorts
{
	std::vector<InPortBase*> mInPorts;
public:
	typedef std::vector<InPortBase*>::iterator Iterator;
	typedef std::vector<InPortBase*>::const_iterator ConstIterator;

	InPortBase& GetByNumber(int index) const;
	InPortBase& Get( const std::string & name ) const;
	int Size() const;
	bool AreReadyForReading();
	void Publish(InPortBase *);
	void Clear(){ mInPorts.clear(); }
	Iterator Begin();
	Iterator End();
	ConstIterator Begin() const;
	ConstIterator End() const;

private:
	std::string AvailableNames() const;
};

} // namespace CLAM

#endif
