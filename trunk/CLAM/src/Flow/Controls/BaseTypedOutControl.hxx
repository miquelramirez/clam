#ifndef _BaseTypedOutControl_
#define _BaseTypedOutControl_

#include <string>
#include <list>
#include <typeinfo>
#include <CLAM/Assert.hxx>
#include <CLAM/BaseTypedInControl.hxx>


namespace CLAM {
	class Processing;
	class BaseTypedInControl;
	/**
	* \brief Processing out control base class.
	*
	*/
	class BaseTypedOutControl
	{
		std::string mName;
		Processing * mProcessing;
	protected:
		typedef std::list<BaseTypedInControl*> Peers;
		/// mLinks will store the pointers to the connected TypedInPorts
		Peers mLinks;
	public:
		BaseTypedOutControl(const std::string &name, Processing * proc = 0, bool publish=true);
		virtual ~BaseTypedOutControl();
		/**
			@pre You should call IsLinkable before using the AddLink function to avoid errors.
		*/
		virtual void AddLink(BaseTypedInControl& in)
		{
			mLinks.push_back(&in);
			in.OutControlInterface_AddLink(*this);
		}
		virtual void RemoveLink(BaseTypedInControl& in)
		{
			mLinks.remove( &in );
			in.OutControlInterface_RemoveLink(*this);
		}
		virtual bool IsLinkable(const BaseTypedInControl& in) = 0;
		virtual bool IsConnected()
		{
			return not mLinks.empty();
		}
		virtual	bool IsConnectedTo(BaseTypedInControl& in)
		{
				Peers::iterator it;
				for (it=mLinks.begin(); it!=mLinks.end(); it++) 
					if ((*it) == &in)
						return true;

				return false;
		}
		const std::string& GetName() const { return mName; }
		Processing * GetProcessing() const { return mProcessing; }
		virtual Peers::iterator BeginInControlsConnected();
		virtual Peers::iterator EndInControlsConnected();
	};
} // END NAMESPACE CLAM
#endif // _BaseTypedOutControl_
