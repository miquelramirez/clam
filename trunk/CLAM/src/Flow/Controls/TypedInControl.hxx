#ifndef _TypedInControl_
#define _TypedInControl_

#include <string>
#include <list>
#include <typeinfo>

namespace CLAM {
	
	class BaseTypedInControl{
	public:
		virtual ~BaseTypedInControl(){}
		virtual const std::type_info& ControlType() const = 0;
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
		std::string mName;
		TypedControlData mLastValue;
		ProperTypedOutControlList mLinks;
		
	public:
		TypedInControl(const std::string &name);
		~TypedInControl();
		
		void DoControl(const TypedControlData& val);
		const std::string& GetName() const { return mName; }
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
	TypedInControl<TypedControlData>::TypedInControl(const std::string &name)
		: mName(name)
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
	
} // End namespace CLAM
#endif // _TypedInControl_
