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
		InControl* mPublished; //TODO should be a list, in the future

		public:
			InControlPublisher() 
				: InControl( "InControlPublisher", 0 ) 
			{
				mPublished=NULL;
			} 

			InControlPublisher( const std::string& name, Processing* father )
				: InControl( name, father ) 
			{
				mPublished=NULL;
			}

			void PublishInControl( InControl& in )
			{
				mPublished = &in;
			}
			int DoControl(TControlData val)
			{
				if(mPublished)
					return mPublished->DoControl(val);
				else
					return InControl::DoControl(val);
			}
			TControlData GetLastValue() const 
			{ 
				if(mPublished)
					return mPublished->GetLastValue();
				else
					return mLastValue; 
			}

	};

} // namespace CLAM

#endif

