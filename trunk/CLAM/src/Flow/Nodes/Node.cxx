
#include "Node.hxx"
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

} // namespace CLAM
