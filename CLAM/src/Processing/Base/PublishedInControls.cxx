#include "PublishedInControls.hxx"
#include "InControl.hxx"

namespace CLAM
{

InControl& PublishedInControls::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Control must be >=0");
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
	CLAM_ASSERT(false, "name not found in InControls collection");

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

