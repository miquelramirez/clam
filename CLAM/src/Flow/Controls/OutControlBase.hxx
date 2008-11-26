#ifndef _BaseTypedOutControl_
#define _BaseTypedOutControl_

#include <string>
#include <list>
#include <typeinfo>
#include <CLAM/Assert.hxx>
#include <CLAM/InControlBase.hxx>


namespace CLAM {
	class Processing;
	class InControlBase;
	/**
	* Base class for an outwards control connector.
	* OutControls can be used to send asynchronous data to
	* connected InControls in other processings.
	* @see InControlBase AddingControlsToProcessings Processing
	*/
	class OutControlBase
	{
		std::string mName;
		Processing * mProcessing;
	protected:
		typedef std::list<InControlBase*> Peers;
		/// Stores the pointers to the connected incontrols
		Peers mLinks;
		/** Private copy constructor to avoid copies */
		OutControlBase(const OutControlBase & toBeCopied ) {}
	public:
		/**
		 Class constructor.
		 @param name Unique name of the control within the processing.
		 @param processing Host processing (0 for a free control).
		*/
		OutControlBase(const std::string &name, Processing * proc = 0);
		/**
		 Class destructor.
		 Destruction clears any connection left and unregisters the control from the processing.
		 Whatever it gets destructed first (In or Out) links are properly unset.
		*/
		virtual ~OutControlBase();
		/// @returns The unique name that the control takes within the processing.
		const std::string& GetName() const { return mName; }
		/// @returns The hosting processing
		Processing * GetProcessing() const { return mProcessing; }
		/// @returns True if there is any linked out control
		bool IsConnected() { return not mLinks.empty(); }
		/// @returns True if the control is linked to the parameter, an out control.
		bool IsConnectedTo(InControlBase& in)
		{
				Peers::iterator it;
				for (it=mLinks.begin(); it!=mLinks.end(); it++) 
					if ((*it) == &in) return true;
				return false;
		}
		/**
		 Returns whether the control can be connected to the provided in control.
		 Concrete in controls redefine this function to provide or extend the set
		 of conditions to be linkable.
		 @todo move here a base implementation by checking the typeid.
		*/
		virtual bool IsLinkable(const InControlBase& in) = 0;
		/**
			Set a connection with an in control.
			@pre You should call IsLinkable before using the AddLink function to avoid errors.
		*/
		void AddLink(InControlBase& in)
		{
			CLAM_ASSERT(IsLinkable(in),
				"Connecting controls which are not linkable (different types?). "
				"You can use IsLinkable before connecting to ensure that they are compatible.");
			mLinks.push_back(&in);
			in.OutControlInterface_AddLink(*this);
		}
		/**
		 Unsets an existing link with the provided in control.
		 @pre The link must exist. (Check that with IsConnectedTo).
		*/
		void RemoveLink(InControlBase& in)
		{
			CLAM_ASSERT(IsConnectedTo(in),
				"Removing a control connection that doesn't exist."
				"You can check that with IsConnectedTo to ensure that this condition is meet.");
			mLinks.remove( &in );
			in.OutControlInterface_RemoveLink(*this);
		}
		Peers::iterator BeginInControlsConnected();
		Peers::iterator EndInControlsConnected();
	};
} // END NAMESPACE CLAM
#endif // _BaseTypedOutControl_
