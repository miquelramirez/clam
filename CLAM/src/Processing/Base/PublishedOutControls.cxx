#include "PublishedOutControls.hxx"
#include "Processing.hxx"
#include "OutControl.hxx"
#include <string>

namespace CLAM
{

PublishedOutControls::PublishedOutControls(Processing* parent) :
		mParent(*parent)
{}

OutControl& PublishedOutControls::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Control must be >=0");
	return *mParent.GetOutControl(index);
}

OutControl& PublishedOutControls::Get(std::string name) const
{
	Processing::OutControlIterator it;
	for (it=mParent.FirstOutControl(); it!=mParent.LastOutControl(); it++)
	{
		std::string actualName( (*it)->GetName() );
		if (name == (*it)->GetName()) return *(*it);
	}
	CLAM_ASSERT(false, "name not found in OutControls collection");
	
	return *(OutControl*)NULL; // Just to get rid of warnings
}

int PublishedOutControls::Size() const
{
	Processing::OutControlIterator it;
	int count=0;
	for (it=mParent.FirstOutControl(); it!=mParent.LastOutControl(); it++)
		++count;
	return count;
}

PublishedOutControls::Iterator  PublishedOutControls::Begin()
{
	return mParent.FirstOutControl();
}

PublishedOutControls::Iterator PublishedOutControls::End()
{
	return mParent.LastOutControl();
}

} // namespace CLAM

