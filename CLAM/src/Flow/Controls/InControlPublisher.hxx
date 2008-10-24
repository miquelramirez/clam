#ifndef _INCONTROL_PUBLISHER_HXX_
#define _INCONTROL_PUBLISHER_HXX_

#include "InControl.hxx"
#include "InControl.hxx"
#include <string>

// TODO: Refactoring...
// Extract common interface from InControlPublisher, InControl and create a common base class
namespace CLAM
{
    class InControlPublisher : public FloatInControl
	{
		FloatInControl* mPublished; //TODO should be a list, in the future

		public:
			InControlPublisher() 
				: FloatInControl( "InControlPublisher", 0 ) 
			{
				mPublished=NULL;
			} 

			InControlPublisher( const std::string& name, Processing* father )
				: FloatInControl( name, father ) 
			{
				mPublished=NULL;
			}

			void PublishInControl( FloatInControl& in )
			{
				mPublished = &in;
			}
			int DoControl(TControlData val)
			{
				if(mPublished)
					return mPublished->DoControl(val);
				return FloatInControl::DoControl(val);
			}
			const TControlData& GetLastValue() const 
			{ 
				if(mPublished)
					return mPublished->GetLastValue();
				return mLastValue; 
			}

	};

} // namespace CLAM

#endif

