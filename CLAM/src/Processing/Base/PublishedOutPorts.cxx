#include "PublishedOutPorts.hxx"
#include "OutPort.hxx"

namespace CLAM
{

OutPort& PublishedOutPorts::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Port must be >=0");
	CLAM_ASSERT(index<Size(), "index for Port must be < than Size");

	ConstIterator it;
	int i;
	for (it=mOutPorts.begin(), i=0; it!=mOutPorts.end(); it++, i++)
	{
		if (i==index) return *(*it);
	}
	CLAM_ASSERT(false, "PublishedOutPorts::GetByNumber() index out of range");
	
	return *(OutPort*)NULL; // just to get rid of warnings
}
	
OutPort& PublishedOutPorts::Get(const std::string & name) const
{
	ConstIterator it;
	for (it=mOutPorts.begin(); it!=mOutPorts.end(); it++)
		if (name == (*it)->GetName()) 
			return **it;
	std::string error( "name not found in OutPorts collection: " );
	error += name;
	CLAM_ASSERT( false, error.c_str() );
	
	return *(OutPort*)NULL; // just to get rid of warnings
}
int PublishedOutPorts::Size() const
{
	return mOutPorts.size();
}

bool PublishedOutPorts::AreReadyForWriting()
{
	Iterator out;
	for ( out=mOutPorts.begin(); out!=mOutPorts.end(); out++)
		if (!(*out)->IsReadyForWriting()) return false;
	
	return true;
}

PublishedOutPorts::Iterator  PublishedOutPorts::Begin()
{
	return mOutPorts.begin();
}

PublishedOutPorts::Iterator PublishedOutPorts::End()
{
	return mOutPorts.end();
}

PublishedOutPorts::ConstIterator  PublishedOutPorts::Begin() const
{
	return mOutPorts.begin();
}

PublishedOutPorts::ConstIterator PublishedOutPorts::End() const
{
	return mOutPorts.end();
}

void PublishedOutPorts::Publish( OutPort * out )
{
	mOutPorts.push_back( out );
}

}// namespace CLAM
