#include "PublishedInPorts.hxx"
#include "InPort.hxx"
#include "Processing.hxx"
#include <string>

namespace CLAM
{

PublishedInPorts::PublishedInPorts(Processing* parent) :
		mParent(*parent)
{}

InPort& PublishedInPorts::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Port must be >=0");
	Processing::InPortIterator it;
	int i;
	for (it=mParent.FirstInput(), i=0; it!=mParent.LastInput(); it++, i++)
	{
		if (i==index) return *(*it);
	}
	CLAM_ASSERT(false, "PublishedInPorts::GetByNumber() index out of range");

	return *(InPort*)NULL; // just to get rid of warnings
}
	
InPort& PublishedInPorts::Get(std::string name) const
{
	Processing::InPortIterator it;
	for (it=mParent.FirstInput(); it!=mParent.LastInput(); it++)
	{
		std::string actualName( (*it)->GetName() );
		if (name == (*it)->GetName()) return *(*it);
	}
	CLAM_ASSERT(false, "name not found in InPorts collection");

	return *(InPort*)NULL; // just to get rid of warnings
}

int PublishedInPorts::Size() const
{
	Processing::InPortIterator it;
	int count=0;
	for (it=mParent.FirstInput(); it!=mParent.LastInput(); it++)
		++count;
	return count;
}

PublishedInPorts::Iterator  PublishedInPorts::Begin()
{
	return mParent.FirstInput();
}

PublishedInPorts::Iterator PublishedInPorts::End()
{
	return mParent.LastInput();
}

bool PublishedInPorts::AreReadyForReading()
{
	Iterator in;
	for ( in=Begin(); in!=End(); in++)
		if (!(*in)->IsReadyForReading()) return false;

	return true;
}

	
} // namespace CLAM
