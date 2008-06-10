#ifndef _TypedInControl_
#define _TypedInControl_

#include <string>
#include <list>
#include <typeinfo>
#include <CLAM/Processing.hxx>

namespace CLAM {
	
	class BaseTypedInControl{
		std::string mName;
		Processing * mProcessing;

	public:
		BaseTypedInControl(const std::string &name, Processing * proc = 0);
		virtual ~BaseTypedInControl(){}
		virtual const std::type_info& ControlType() const = 0;
		const std::string& GetName() const { return mName; }
		Processing * GetProcessing() const { return mProcessing; }
	};
	
	
	// Template Class Declaration
	template<class TypedControlData>
	class TypedOutControl;
	
	template<class TypedControlData>
	class TypedInControl : public BaseTypedInControl
	{
		typedef TypedOutControl<TypedControlData> ProperTypedOutControl;
		typedef std::list< ProperTypedOutControl * > ProperTypedOutControlList;
		
	protected:
		TypedControlData mLastValue;
		ProperTypedOutControlList mLinks;
		
	public:
		TypedInControl(const std::string &name = "unnamed typed in control", Processing * proc = 0);
		~TypedInControl();
		
		void DoControl(const TypedControlData& val);
		const TypedControlData& GetLastValue();
		bool IsConnected() const;
		
		// For the typed linking check
		virtual const std::type_info& ControlType() const { return typeid(TypedControlData); };
		/// Implementation detail just to be used from OutControl
		void OutControlInterface_AddLink(ProperTypedOutControl & outControl);
		/// Implementation detail just to be used from OutControl
		void OutControlInterface_RemoveLink(ProperTypedOutControl & outControl);
	}; // End TypedInControl Class
	
	// TypedInControl Class Implementation
	template<class TypedControlData>
	TypedInControl<TypedControlData>::TypedInControl(const std::string &name, Processing * proc)
		: BaseTypedInControl(name,proc)
	{
	}
	
	template<class TypedControlData>
	TypedInControl<TypedControlData>::~TypedInControl()
	{
		while (!mLinks.empty())
			mLinks.front()->RemoveLink(*this);
	}

	template<class TypedControlData>
	void TypedInControl<TypedControlData>::DoControl(const TypedControlData& val)
	{
		mLastValue = val;
	}

	template<class TypedControlData>
	const TypedControlData& TypedInControl<TypedControlData>::GetLastValue()
	{
		return mLastValue;
	}

	template<class TypedControlData>
	bool TypedInControl<TypedControlData>::IsConnected() const
	{
		return !mLinks.empty();
	}

	template<class TypedControlData>
	void TypedInControl<TypedControlData>::OutControlInterface_AddLink(ProperTypedOutControl & outControl)
	{
		mLinks.push_back(&outControl);
	}

	template<class TypedControlData>
	void TypedInControl<TypedControlData>::OutControlInterface_RemoveLink(ProperTypedOutControl & outControl)
	{
		mLinks.remove(&outControl);
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
	
		//Constructor/Destructor
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
	//  Implementation of class InControlTmpl
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
