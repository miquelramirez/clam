#include "PublishedInControls.hxx"
#include "Processing.hxx"
#include "InControl.hxx"
#include <string>

namespace CLAM
{

PublishedInControls::PublishedInControls(Processing* parent) :
		mParent(*parent)
{}

InControl& PublishedInControls::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Control must be >=0");
	return *mParent.GetInControl(index);
}

InControl& PublishedInControls::Get(std::string name) const
{
	Processing::InControlIterator it;
	for (it=mParent.FirstInControl(); it!=mParent.LastInControl(); it++)
	{
		std::string actualName( (*it)->GetName() );
		if (name == (*it)->GetName()) return *(*it);
	}
	CLAM_ASSERT(false, "name not found in InControls collection");

	return *(InControl*)NULL; // just to get rid of warnings
}
int PublishedInControls::Size() const
{
	Processing::InControlIterator it;
	int i=0;
	for (it=mParent.FirstInControl(); it!=mParent.LastInControl(); it++)
		i++;

	return i;
}

PublishedInControls::Iterator  PublishedInControls::Begin()
{
	return mParent.FirstInControl();
}

PublishedInControls::Iterator PublishedInControls::End()
{
	return mParent.LastInControl();
}

		
} // namespace CLAM

