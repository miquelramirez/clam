#ifndef _BaseTypedOutControl_
#define _BaseTypedOutControl_

#include <string>
#include <list>
#include <typeinfo>
#include <CLAM/Assert.hxx>
#include <CLAM/BaseTypedInControl.hxx>


namespace CLAM {
	class Processing;
	
	class BaseTypedOutControl{
		std::string mName;
		Processing * mProcessing;
	public:
		BaseTypedOutControl(const std::string &name, Processing * proc = 0);
		virtual ~BaseTypedOutControl();
		/**
			WARNING: You should call IsLinkable before using the AddLink function to avoid errors.
		*/
		virtual void AddLink(BaseTypedInControl& in) = 0;
		virtual void RemoveLink(BaseTypedInControl& in) = 0;
		virtual bool IsLinkable(const BaseTypedInControl& in) = 0;
		virtual bool IsConnected() = 0;
		virtual	bool IsConnectedTo(BaseTypedInControl& in) = 0;
		
		const std::string& GetName() const { return mName; }
		Processing * GetProcessing() const { return mProcessing; }
		virtual std::list<BaseTypedInControl*>::iterator BeginInControlsConnected() = 0;
		virtual std::list<BaseTypedInControl*>::iterator EndInControlsConnected() = 0;
	};
} // END NAMESPACE CLAM
#endif // _BaseTypedOutControl_
