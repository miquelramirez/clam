#ifndef _TypedInControl_
#define _TypedInControl_

#include <string>

namespace CLAM {
	
	template<class TypedControlData>
	class TypedInControl 
	{
	protected:
		std::string mName;
		TypedControlData mLastValue;
		
	public:
		TypedInControl(const std::string &name);
		~TypedInControl();
		
		void DoControl(const TypedControlData& val);
		const std::string& GetName() const { return mName; }
		const TypedControlData& GetLastValue();
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
	
	
} // End namespace CLAM
#endif // _TypedInControl_
