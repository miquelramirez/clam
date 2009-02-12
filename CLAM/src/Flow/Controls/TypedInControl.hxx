#ifndef _TypedInControl_
#define _TypedInControl_

#include <string>
#include <list>
#include <typeinfo>
#include <CLAM/TypeInfo.hxx>
#include <CLAM/InControlBase.hxx>
#include <CLAM/OutControlBase.hxx>
#include <sstream>

namespace CLAM {
	class Processing;
	class OutControlBase;

	template<class TypedControlData>
	class TypedOutControl;
	
	template<class TypedControlData>
	class TypedInControl : public InControlBase
	{
		typedef typename TypeInfo<TypedControlData>::StorableAsLeaf TokenIsStorableAsLeaf;
	protected:
		TypedControlData mLastValue;
		
	public:
		TypedInControl(const std::string &name = "unnamed in control", Processing * proc = 0);
		
		virtual void DoControl(const TypedControlData& val) 
		{
			mLastValue = val;
			_hasBeenRead=false;
		};
		const TypedControlData& GetLastValue() const 
		{
			_hasBeenRead=true;
			return mLastValue; 
		};
	private:
		std::string GetLastValueAsString(StaticFalse* /*isStreamable*/) const
		{
			return "Not printable";
		}
		/** @return A string with the extracted XML content */
		std::string GetLastValueAsString(StaticTrue* /*isStreamable*/) const
		{
			std::ostringstream valueStream;
			valueStream << GetLastValue();
			return valueStream.str();
		}
	public:
		const std::string GetLastValueAsString() // TODO: Use plugins as soon we start to use non streamable types
		{
			return GetLastValueAsString((TokenIsStorableAsLeaf*)0);
		}
		// For the typed linking check
		virtual const std::type_info& GetTypeId() const { return typeid(TypedControlData); };
	};
	
	// TypedInControl Class Implementation
	template<class TypedControlData>
	TypedInControl<TypedControlData>::TypedInControl(const std::string &name, Processing * proc)
		: InControlBase(name,proc)
	{
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
