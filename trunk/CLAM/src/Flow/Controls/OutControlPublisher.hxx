#ifndef _OUTCONTROL_PUBLISHER_HXX_
#define _OUTCONTROL_PUBLISHER_HXX_

#include "InControl.hxx"
#include "OutControl.hxx"
#include <string>

// TODO: Refactoring...
// Extract common interface from OutControlPublisher, OutControl and create a common base class

namespace CLAM
{
    class OutControlPublisher : public FloatOutControl
	{
			FloatOutControl* mPublished;

		public:
			OutControlPublisher() 
				: FloatOutControl( "OutControlPublisher", 0 ) 
			{
				mPublished=NULL;
			} 

			OutControlPublisher( const std::string& name, Processing* father )
				: FloatOutControl( name, father ) 
			{
				mPublished=NULL;
			}

			void PublishOutControl( FloatOutControl& out )
			{
				mPublished = &out;
			}

			void AddLink( InControlBase& in )
			{
				FloatOutControl::AddLink( in );
				if(mPublished) mPublished->AddLink( in );
			}
	};

} // namespace CLAM

#endif

