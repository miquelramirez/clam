#ifndef _PublishedOutPorts_hxx_
#define _PublishedOutPorts_hxx_

#include <string>
#include <vector>

namespace CLAM
{

//foward declarations:
class OutPort;

class PublishedOutPorts
{
	std::vector<OutPort*> mOutPorts;
public:
	typedef std::vector<OutPort*>::iterator Iterator;
	typedef std::vector<OutPort*>::const_iterator ConstIterator;

	OutPort& GetByNumber(int index) const;
	OutPort& Get(const std::string & name) const;
	int Size() const;
	bool AreReadyForWriting();
	void Publish( OutPort * );
	void Clear(){ mOutPorts.clear(); }
	Iterator Begin();
	Iterator End();
	ConstIterator Begin() const;
	ConstIterator End() const;
};

} // namespace CLAM

#endif
