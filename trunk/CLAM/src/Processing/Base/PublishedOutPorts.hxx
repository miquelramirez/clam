#ifndef _PublishedOutPorts_hxx_
#define _PublishedOutPorts_hxx_

#include <string>
#include <vector>

namespace CLAM
{

//foward declarations:
class Processing;
class OutPort;

class PublishedOutPorts
{
public:
	typedef std::vector<OutPort*>::iterator Iterator;
	PublishedOutPorts(Processing* parent);

	OutPort& GetByNumber(int index) const;
	OutPort& Get(std::string name) const;
	int Size() const;
    Iterator Begin();
	Iterator End();
	bool AreReadyForWriting();

private:
	Processing& mParent;
};

} // namespace CLAM

#endif
