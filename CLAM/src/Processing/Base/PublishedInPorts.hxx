#ifndef _PublishedInPorts_hxx_
#define _PublishedInPorts_hxx_

#include <string>
#include <vector>

namespace CLAM
{
	
//forward declarations:
class Processing;
class InPort;

class PublishedInPorts
{
public:
	typedef std::vector<InPort*>::iterator Iterator;
	PublishedInPorts(Processing* parent);

	InPort& GetByNumber(int index) const;
	InPort& Get( std::string name ) const;
	int Size() const;
	Iterator Begin();
	Iterator End();
	
	bool AreReadyForReading();
	
private:
	Processing& mParent;
};

} // namespace CLAM

#endif
