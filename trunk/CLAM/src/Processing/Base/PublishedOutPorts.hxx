#ifndef _PublishedOutPorts_hxx_
#define _PublishedOutPorts_hxx_

#include <string>
#include <vector>

namespace CLAM
{

//forward declarations:
class OutPortBase;

class PublishedOutPorts
{
	std::vector<OutPortBase*> mOutPorts;
public:
	typedef std::vector<OutPortBase*>::iterator Iterator;
	typedef std::vector<OutPortBase*>::const_iterator ConstIterator;

	OutPortBase & GetByNumber(int index) const;
	OutPortBase & Get(const std::string & name) const;
	int Size() const;
	bool AreReadyForWriting();
	void Publish( OutPortBase * );
	void Clear(){ mOutPorts.clear(); }
	Iterator Begin();
	Iterator End();
	ConstIterator Begin() const;
	ConstIterator End() const;
};

} // namespace CLAM

#endif
