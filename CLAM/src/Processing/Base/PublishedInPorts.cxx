
#include "PublishedInPorts.hxx"
#include "InPort.hxx"

namespace CLAM
{

InPortBase& PublishedInPorts::GetByNumber(int index) const
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

	return *(InPortBase*)NULL; // just to get rid of warnings
}
	
InPortBase& PublishedInPorts::Get(const std::string & name) const
{
	ConstIterator it;
	for (it=mInPorts.begin(); it!=mInPorts.end(); it++)
		if(name == (*it)->GetName()) 
			return **it;

	std::string error( "name not found in InPorts collection: " );
	error += "'" +  name + "'" + std::string(". In ports availables: ") + AvailableNames();
	CLAM_ASSERT( false, error.c_str() );

	return *(InPortBase*)NULL; // just to get rid of warnings
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
		if (!(*in)->CanConsume()) return false;

	return true;
}

void PublishedInPorts::Publish( InPortBase * in )
{
	mInPorts.push_back( in );
}
	
std::string PublishedInPorts::AvailableNames() const
{
	std::string result;
	ConstIterator it;
	bool first=true;
	for (it=mInPorts.begin(); it!=mInPorts.end(); it++)
	{
		if (!first) 
			result += ", ";
		else 
			first = false;
		InPortBase & port = *(*it);
		result += "'";
		result += port.GetName();
		result += "'";
	}
	return result;
}
} // namespace CLAM

