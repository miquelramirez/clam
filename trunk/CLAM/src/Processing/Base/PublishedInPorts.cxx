#include "PublishedInPorts.hxx"
#include "InPort.hxx"

namespace CLAM
{

InPort& PublishedInPorts::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Port must be >=0");
	CLAM_ASSERT(index<Size(), "index for Port must be < than Size");
	ConstIterator it;
	int i;
	for (it=mInPorts.begin(), i=0; it!=mInPorts.end(); it++, i++)
	{
		if (i==index) return *(*it);
	}

	CLAM_ASSERT(false, "PublishedInPorts::GetByNumber() index out of range");

	return *(InPort*)NULL; // just to get rid of warnings
}
	
InPort& PublishedInPorts::Get(const std::string & name) const
{
	ConstIterator it;
	for (it=mInPorts.begin(); it!=mInPorts.end(); it++)
	{
		std::string actualName( (*it)->GetName() );
		if (name == (*it)->GetName()) return *(*it);
	}

	std::string error( "name not found in InPorts collection: " );
	error += name;
	CLAM_ASSERT( false, error.c_str() );

	return *(InPort*)NULL; // just to get rid of warnings
}

int PublishedInPorts::Size() const
{
	return mInPorts.size();
}

PublishedInPorts::Iterator  PublishedInPorts::Begin()
{
	return mInPorts.begin();
}

PublishedInPorts::Iterator PublishedInPorts::End()
{
	return mInPorts.end();
}

PublishedInPorts::ConstIterator  PublishedInPorts::Begin() const
{
	return mInPorts.begin();
}

PublishedInPorts::ConstIterator PublishedInPorts::End() const
{
	return mInPorts.end();
}


bool PublishedInPorts::AreReadyForReading()
{
	Iterator in;
	for ( in=mInPorts.begin(); in!=mInPorts.end(); in++)
		if (!(*in)->IsReadyForReading()) return false;

	return true;
}

void PublishedInPorts::Publish( InPort * in )
{
	mInPorts.push_back( in );
}
	
} // namespace CLAM

