#ifndef _OUTCONTROL_PUBLISHER_HXX_
#define _OUTCONTROL_PUBLISHER_HXX_

#include "InControl.hxx"
#include "OutControl.hxx"
#include <string>

// TODO: Refactoring...
// Extract common interface from OutControlPublisher, OutControl and create a common base class

namespace CLAM
{
    class OutControlPublisher : public OutControl
	{
			OutControl* mPublished;

		public:
			OutControlPublisher() 
				: OutControl( "OutControlPublisher", 0 ) 
			{
				mPublished=NULL;
			} 

			OutControlPublisher( const std::string& name, Processing* father )
				: OutControl( name, father ) 
			{
				mPublished=NULL;
			}

			void PublishOutControl( OutControl& out )
			{
				mPublished = &out;
			}

			void AddLink( InControl& in )
			{
				OutControl::AddLink( in );
				if(mPublished) mPublished->AddLink( in );
			}
	};

} // namespace CLAM

#endif

