#include "PublishedOutControls.hxx"
#include "OutControl.hxx"

namespace CLAM
{

OutControl& PublishedOutControls::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Control must be >=0");
	return *mOutControls.at(index);
}

OutControl& PublishedOutControls::Get(const std::string & name) const
{
	ConstIterator it;
	for (it=mOutControls.begin(); it!=mOutControls.end(); it++)
	{
		std::string actualName( (*it)->GetName() );
		if (name == (*it)->GetName()) return *(*it);
	}
	CLAM_ASSERT(false, "name not found in OutControls collection");
	
	return *(OutControl*)NULL; // Just to get rid of warnings
}

int PublishedOutControls::Size() const
{
	return mOutControls.size();
}

PublishedOutControls::Iterator  PublishedOutControls::Begin()
{
	return mOutControls.begin();
}

PublishedOutControls::Iterator PublishedOutControls::End()
{
	return mOutControls.end();
}

PublishedOutControls::ConstIterator  PublishedOutControls::Begin() const
{
	return mOutControls.begin();
}

PublishedOutControls::ConstIterator PublishedOutControls::End() const
{
	return mOutControls.end();
}

void PublishedOutControls::Publish( OutControl * out )
{
	mOutControls.push_back( out );
}

} // namespace CLAM

