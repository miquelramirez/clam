#include "PublishedOutControls.hxx"
#include "OutControl.hxx"

namespace CLAM
{

OutControl& PublishedOutControls::GetByNumber(int index) const
{
	CLAM_ASSERT(index>=0, "index for Control must be >=0");
	CLAM_ASSERT(index<Size(), "index for Control must be < than Size");
	
	return *mOutControls.at(index);
}

OutControl& PublishedOutControls::Get(const std::string & name) const
{
	ConstIterator it;
	for (it=mOutControls.begin(); it!=mOutControls.end(); it++)
		if (name == (*it)->GetName()) 
			return **it;
	
	std::string error( "name not found in OutControls collection: " );
	error += name;
	CLAM_ASSERT( false, error.c_str() );

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

