#ifndef _BaseTypedInControl_
#define _BaseTypedInControl_

#include <string>
#include <list>
#include <typeinfo>

namespace CLAM {
	class Processing; 
	class BaseTypedOutControl;
	
	/**
	* \brief Processing in control base class.
	*
	* Typed in controls base interface with some specific services for float ones.
	*/
	class BaseTypedInControl
	{
		std::string mName;
		Processing * mProcessing;
	protected:
		float mDefaultValue;
		float mUpperBound;
		float mLowerBound;
		bool mBounded;
		bool mHasDefaultValue;
		std::list< BaseTypedOutControl * > mLinks;

	public:
		BaseTypedInControl(const std::string &name, Processing * proc = 0);
		virtual ~BaseTypedInControl();
		virtual const std::type_info& ControlType() const = 0;
		const std::string& GetName() const { return mName; }
		Processing * GetProcessing() const { return mProcessing; }
		bool IsConnected() const
		{
			return not mLinks.empty();
		}
		/** @name Float specific services
		* 
		*/
		//{@
		bool IsBounded() const;
		float UpperBound() const;
		float LowerBound() const;
		/** Returns the bounds mean or the value set with SetDefaultValue() if its the case */
		float DefaultValue() const;
		void SetDefaultValue(float val);
		void SetBounds(float lower, float upper);
		//@}
		/// Implementation detail just to be used from OutControl
		void OutControlInterface_AddLink(BaseTypedOutControl & outControl)
		{
				mLinks.push_back(&outControl);
		}
		/// Implementation detail just to be used from OutControl
		void OutControlInterface_RemoveLink(BaseTypedOutControl & outControl)
		{
				mLinks.remove(&outControl);
		}
	};
} // End namespace CLAM
#endif // _BaseTypedInControl_
