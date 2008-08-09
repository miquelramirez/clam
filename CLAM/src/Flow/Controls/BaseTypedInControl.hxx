#ifndef _BaseTypedInControl_
#define _BaseTypedInControl_

#include <string>
#include <list>
#include <typeinfo>

namespace CLAM {
	class Processing; 
	
	/**
	* \brief Processing in control base class.
	*
	* Typed in controls base interface with some specific services for float ones.
	*/
	class BaseTypedInControl{
		std::string mName;
		Processing * mProcessing;
	protected:
		float mDefaultValue;
		float mUpperBound;
		float mLowerBound;
		bool mBounded;
		bool mHasDefaultValue;
	
	public:
		BaseTypedInControl(const std::string &name, Processing * proc = 0);
		virtual ~BaseTypedInControl();
		virtual const std::type_info& ControlType() const = 0;
		const std::string& GetName() const { return mName; }
		Processing * GetProcessing() const { return mProcessing; }
		
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
	};
} // End namespace CLAM
#endif // _BaseTypedInControl_
