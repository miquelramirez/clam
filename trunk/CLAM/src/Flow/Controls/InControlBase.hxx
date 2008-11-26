#ifndef InControlBase_hxx
#define InControlBase_hxx

#include <string>
#include <list>
#include <typeinfo>

namespace CLAM {
	class Processing; 
	class OutControlBase;
	
	/// Control events type
	typedef float TControlData;

	/**
	* Base class for an inwards control connector.
	* InControls can be used to receive asynchronous data from
	* connected OutControls in other processings.
	* @see OutControlBase AddingControlsToProcessings Processing
	* @todo Remove the float services
	*/
	class InControlBase
	{
		std::string mName;
		Processing * mProcessing; 
	protected:
		typedef std::list<OutControlBase*> Peers;
		/// Stores the pointers to the connected outcontrols
		Peers mLinks;
		TControlData mDefaultValue;
		TControlData mUpperBound;
		TControlData mLowerBound;
		bool mBounded;
		bool mHasDefaultValue;

	public:
		/**
		 Class constructor.
		 @param name Unique name of the control within the processing.
		 @param processing Host processing (0 for a free control).
		 @param publish Whether to make the control visible to the outside.
		 @todo Is there any need for the publish parameter? If so, 
		       consider it when desregistering, If not remove it.
		*/
		InControlBase(const std::string &name, Processing * processing = 0);
		/**
		 Class destructor.
		 Destruction clears any connection left and unregisters the control from the processing.
		 Whatever it gets destructed first (In or Out) links are properly unset.
		*/
		virtual ~InControlBase();
		/**
		 Concrete InControls must overload this method by returning
		 the typeid of the kind of data they communicate.
		 This method is used internanlly in order to assure type
		 compatibility between an input an an output control.
		 There are some other uses like the NetworkEditor assigning
		 different colors to connectors depending on this type.
		 @todo Add a link to the 
		*/
		virtual const std::type_info& ControlType() const = 0;
		/// @returns The unique name that the control takes within the processing.
		const std::string& GetName() const { return mName; }
		/// @returns The hosting processing
		Processing * GetProcessing() const { return mProcessing; }
		/// @returns True if there is any linked out control
		bool IsConnected() const { return not mLinks.empty(); }
		/// @returns True if the control is linked to the parameter, an out control.
		bool IsConnectedTo(OutControlBase & out);

		/** @name Float specific services
		* @todo move them to the concrete float class
		*/
		//{@
		bool IsBounded() const;
		TControlData UpperBound() const;
		TControlData LowerBound() const;
		/** Returns the bounds mean or the value set with SetDefaultValue() if its the case */
		TControlData DefaultValue() const;
		void SetDefaultValue(TControlData val);
		void SetBounds(TControlData lower, TControlData upper);
		//@}

		/// Implementation detail just to be used just from OutControlBase
		void OutControlInterface_AddLink(OutControlBase & outControl)
		{
				mLinks.push_back(&outControl);
		}
		/// Implementation detail just to be used just from OutControlBase
		void OutControlInterface_RemoveLink(OutControlBase & outControl)
		{
				mLinks.remove(&outControl);
		}
	};
} // End namespace CLAM
#endif // InControlBase_hxx
