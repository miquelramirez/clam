
#include "Node.hxx"
#include "InPort.hxx"
#include <vector>

namespace CLAM
{

const OutPort* NodeBase::GetWriter() const
{
	return mpDriver;
}

const std::list<InPort*> NodeBase::GetReaders() const
{
	return mInputs;
}

void  NodeBase::UnattachAll()
{
	std::list<InPort*>::iterator it;
	for(it=mInputs.begin(); it!=mInputs.end(); it++)
		(*it)->Unattach();
}

} // namespace CLAM
