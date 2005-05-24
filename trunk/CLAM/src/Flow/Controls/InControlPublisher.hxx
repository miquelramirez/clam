#ifndef _INCONTROL_PUBLISHER_HXX_
#define _INCONTROL_PUBLISHER_HXX_

#include "InControl.hxx"
#include "InControl.hxx"
#include <string>

// TODO: Refactoring...
// Extract common interface from InControlPublisher, InControl and create a common base class

namespace CLAM
{
    class InControlPublisher : public InControl
	{
			InControl* mPublished;

		public:
			InControlPublisher() 
				: InControl( "InControlPublisher", 0 ) {} 

			InControlPublisher( const std::string& name, Processing* father )
				: InControl( name, father ) {}

			void PublishInControl( InControl& in )
			{
				mPublished = &in;
			}
	                int DoControl(TControlData val)
	                {
			        InControl::DoControl(val);
			        return mPublished->DoControl(val);
			}

	};

} // namespace CLAM

#endif
