#include "PublishedOutPorts.hxx"
#include "Processing.hxx"
#include "OutPort.hxx"

namespace CLAM
{

PublishedOutPorts::PublishedOutPorts(Processing* parent) :
		mParent(*parent)
{}

OutPort& PublishedOutPorts::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Port must be >=0");
	Processing::OutPortIterator it;
	int i;
	for (it=mParent.FirstOutput(), i=0; it!=mParent.LastOutput(); it++, i++)
	{
		if (i==index) return *(*it);
	}
	CLAM_ASSERT(false, "PublishedOutPorts::GetByNumber() index out of range");
	
	return *(OutPort*)NULL; // just to get rid of warnings
}
	
OutPort& PublishedOutPorts::Get(std::string name) const
{
	Processing::OutPortIterator it;
	for (it=mParent.FirstOutput(); it!=mParent.LastOutput(); it++)
	{
		std::string actualName( (*it)->GetName() );
		if (name == (*it)->GetName()) return *(*it);
	}
	CLAM_ASSERT(false, "name not found in OutPorts collection");
	
	return *(OutPort*)NULL; // just to get rid of warnings
}
int PublishedOutPorts::Size() const
{
	Processing::OutPortIterator it;
	int count=0;
	for (it=mParent.FirstOutput(); it!=mParent.LastOutput(); it++)
		++count;
	return count;
}

PublishedOutPorts::Iterator  PublishedOutPorts::Begin()
{
	return mParent.FirstOutput();
}

PublishedOutPorts::Iterator PublishedOutPorts::End()
{
	return mParent.LastOutput();
}
bool PublishedOutPorts::AreReadyForWriting()
{
	Iterator out;
	for ( out=Begin(); out!=End(); out++)
		if (!(*out)->IsReadyForWriting()) return false;
	
	return true;
}

}// namespace CLAM
