#include "PublishedInControls.hxx"
#include "InControl.hxx"

namespace CLAM
{

InControl& PublishedInControls::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Control must be >=0");
	CLAM_ASSERT(index<Size(), "index for Control must be < than Size");
	return *mInControls.at(index);
}

InControl& PublishedInControls::Get(const std::string & name) const
{
	ConstIterator it;
	for (it=mInControls.begin(); it!=mInControls.end(); it++)
	{
		std::string actualName( (*it)->GetName() );
		if (name == (*it)->GetName()) return *(*it);
	}
	std::string error( "name not found in InControls collection: " );
	error += name;
	CLAM_ASSERT( false, error.c_str() );

	return *(InControl*)NULL; // just to get rid of warnings
}
int PublishedInControls::Size() const
{
	return mInControls.size();
}

PublishedInControls::Iterator  PublishedInControls::Begin()
{
	return mInControls.begin();
}

PublishedInControls::Iterator PublishedInControls::End()
{
	return mInControls.end();
}

PublishedInControls::ConstIterator  PublishedInControls::Begin() const
{
	return mInControls.begin();
}

PublishedInControls::ConstIterator PublishedInControls::End() const
{
	return mInControls.end();
}

void PublishedInControls::Publish( InControl * in )
{
	mInControls.push_back( in );
}
		
} // namespace CLAM

