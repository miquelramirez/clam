
#include "Node.hxx"
#include "InPort.hxx"
#include <vector>

namespace CLAM
{

const OutPort* NodeBase::GetWriter() const
{
	return mpDriver;
}

NodeBase::ReaderIterator NodeBase::BeginReaders()
{
	return mInputs.begin();
}

NodeBase::ReaderIterator NodeBase::EndReaders()
{
	return mInputs.end();
}


void  NodeBase::UnattachAll()
{
	std::list<InPort*>::iterator it;
	for(it=mInputs.begin(); it!=mInputs.end(); it++)
		(*it)->Unattach();
}

} // namespace CLAM
