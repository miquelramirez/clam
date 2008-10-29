#ifndef _TypedInControl_
#define _TypedInControl_

#include <string>
#include <list>
#include <typeinfo>
#include <CLAM/BaseTypedInControl.hxx>
#include <CLAM/BaseTypedOutControl.hxx>

namespace CLAM {
	class Processing;
	class BaseTypedOutControl;

	template<class TypedControlData>
	class TypedOutControl;
	
	template<class TypedControlData>
	class TypedInControl : public BaseTypedInControl
	{
		
	protected:
		TypedControlData mLastValue;
		
	public:
		TypedInControl(const std::string &name = "unnamed in control", Processing * proc = 0);
		
		void DoControl(const TypedControlData& val);
		const TypedControlData& GetLastValue() const;
		/** ONLY TO USE WHEN TypedControlData == float. Returns the last TypedControlData (float) received interpreted as a bool. */
		bool GetLastValueAsBoolean() const 
		{ 
			return (mLastValue > 0) ? mLastValue>0.01 : mLastValue<-0.01;
		};
		/** ONLY TO USE WHEN TypedControlData == float. Returns the last TControlData (float) received interpireted as an integer */
		int GetLastValueAsInteger() const { return (int)(mLastValue+0.5f); };
		
		// For the typed linking check
		virtual const std::type_info& ControlType() const { return typeid(TypedControlData); };
	}; // End TypedInControl Class
	
	// TypedInControl Class Implementation
	template<class TypedControlData>
	TypedInControl<TypedControlData>::TypedInControl(const std::string &name, Processing * proc)
		: BaseTypedInControl(name,proc)
	{
	}
	
	template<class TypedControlData>
	void TypedInControl<TypedControlData>::DoControl(const TypedControlData& val)
	{
		mLastValue = val;
	}

	template<class TypedControlData>
	const TypedControlData& TypedInControl<TypedControlData>::GetLastValue() const
	{
		return mLastValue;
	}

	
	/**
	* Subclass of TypedInControl that provides the typedincontrol with a callback method
	* The method must be defined inside the parent \c Processing class.
	* See the \c CascadingTypedInControl constructors for learn how to provide
	* the callback to the \c CascadingTypedInControl
	*/
	template<class TypedControlData, class ProcObj>
	class CascadingTypedInControl : public TypedInControl<TypedControlData>
	{
		public:
			typedef int (ProcObj::*TPtrMemberFunc)(TypedControlData);
			typedef int (ProcObj::*TPtrMemberFuncId)(int,TypedControlData);

		private:
			TPtrMemberFunc   mFunc;
			TPtrMemberFuncId mFuncId;
			ProcObj* mProcObj;
			int mId;
		
		public:
			// redeclaration
			void DoControl(const TypedControlData& val);

			bool ExistMemberFunc() { return (mFunc==0); };
			void SetMemberFunc(TPtrMemberFunc f) { mFunc = f; };

			int GetId(void) const { return mId; }
	
		/**
		* Constructor of the CascadingTypedInControl with a member-service-function associated. 
		*
		* @param f The member function that will act as a service funtion each time
		* the DoControl method is invoqued.
		* @parent The processing object that owns the control object.
		*/
		CascadingTypedInControl(const std::string &name, ProcObj* processing, TPtrMemberFunc f = 0)	:
				TypedInControl<TypedControlData>(name,processing),
				mFunc(f),
				mFuncId(0),
				mProcObj(processing)
		{
		};
		
		CascadingTypedInControl(int id,const std::string &name, ProcObj* processing, TPtrMemberFuncId f)
			: TypedInControl<TypedControlData>(name,processing)
			, mFunc(0)
			, mFuncId(f)
			, mProcObj(processing)
			, mId(id)
		{
		};

		~CascadingTypedInControl(){};
		
	};

	/////////////////////////////////////////////////////////////////////////////////////////
	//  Implementation of class CascadingTypedInControl
	//
	template<class TypedControlData, class ProcObj>
	void CascadingTypedInControl<TypedControlData, ProcObj>::DoControl(const TypedControlData& val)
	{
		TypedInControl<TypedControlData>::DoControl(val);
		if(mFunc)
			(mProcObj->*mFunc)(val);
		else if (mFuncId)
			(mProcObj->*mFuncId)(mId,val);
	}
	
} // End namespace CLAM
#endif // _TypedInControl_
