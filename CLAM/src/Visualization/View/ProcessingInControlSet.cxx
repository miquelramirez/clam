#include "ProcessingInControlSet.hxx"
#include "Assert.hxx"

namespace CLAMVM
{
	ProcessingInControlSet::ProcessingInControlSet()
	{
	}

	ProcessingInControlSet::~ProcessingInControlSet()
	{
	}

	void ProcessingInControlSet::Insert( std::string name, InputControlModel& a )
	{
			mChildren.insert( tChildvalue_type( name, &a ) );
	}
	
	InputControlModel& ProcessingInControlSet::Retrieve( std::string name )
	{
			tChildTbl::iterator i = mChildren.find( name );
			
			CLAM_ASSERT( i!=mChildren.end() ,"Requested children unavailable" );

			return *( i->second );
	}

}
