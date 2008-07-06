#ifndef _TypedOutControl_
#define _TypedOutControl_

#include <string>
#include <list>
#include <typeinfo>
#include "Assert.hxx"
#include "TypedInControl.hxx"
#include "Processing.hxx"

namespace CLAM
{

	class BaseTypedOutControl{
		std::string mName;
		Processing * mProcessing;
	public:
		BaseTypedOutControl(const std::string &name, Processing * proc = 0);
		virtual ~BaseTypedOutControl(){}
		virtual bool IsLinkable(const BaseTypedInControl& in) = 0;
		void Link(BaseTypedInControl& in);
		virtual bool DoTypedLink(BaseTypedInControl& in) = 0;
		virtual bool IsConnected() = 0;
		virtual	bool IsConnectedTo(BaseTypedInControl& in) = 0;
		
		const std::string& GetName() const { return mName; }
		Processing * GetProcessing() const { return mProcessing; }
		
	};
	
	template<class TypedControlData>
	class TypedOutControl : public BaseTypedOutControl
	{
		// This is required to solve the parsing problem with iterators.
		typedef TypedInControl<TypedControlData> ProperTypedInControl;
		typedef std::list< ProperTypedInControl * > ProperTypedInControlList;

	protected:
		// mLinks will store the pointers to the connected TypedInPorts
		ProperTypedInControlList mLinks;

	public:
		TypedOutControl(const std::string &name = "unnamed typed in control", Processing * proc = 0);
		~TypedOutControl();

		void AddLink(TypedInControl<TypedControlData>& in);
		void RemoveLink(TypedInControl<TypedControlData>& in);
		void SendControl(const TypedControlData& val);
		bool IsConnected();
		bool IsConnectedTo( TypedInControl<TypedControlData> & );
		bool IsConnectedTo(BaseTypedInControl& in);
		bool IsLinkable(const BaseTypedInControl& in);
		bool DoTypedLink(BaseTypedInControl& in);

	};
	
	template<class TypedControlData>
	TypedOutControl<TypedControlData>::TypedOutControl(const std::string &name, Processing * proc)
		: BaseTypedOutControl(name,proc)
	{
	}

	template<class TypedControlData>
	TypedOutControl<TypedControlData>::~TypedOutControl()
	{
		while (!mLinks.empty())
			RemoveLink(*mLinks.front());
	}

	
	template<class TypedControlData>
	void TypedOutControl<TypedControlData>::AddLink(TypedInControl<TypedControlData>& in)
	{
		mLinks.push_back(&in);
		in.OutControlInterface_AddLink(*this);
	}

	template<class TypedControlData>
	void TypedOutControl<TypedControlData>::RemoveLink(TypedInControl<TypedControlData>& in)
	{
		mLinks.remove( &in );
		in.OutControlInterface_RemoveLink(*this);
	}

	template<class TypedControlData>
	void TypedOutControl<TypedControlData>::SendControl(const TypedControlData& val)
	{
		typename ProperTypedInControlList::iterator it;
		
		for (it=mLinks.begin(); it!=mLinks.end(); it++) 
		{
			(*it)->DoControl(val);
		}
	}

	template<class TypedControlData>
	bool TypedOutControl<TypedControlData>::IsConnected()
	{
		return ! mLinks.empty();
	}

	template<class TypedControlData>
	bool TypedOutControl<TypedControlData>::IsConnectedTo( TypedInControl<TypedControlData> & in)
	{
		typename ProperTypedInControlList::iterator it;
		for (it=mLinks.begin(); it!=mLinks.end(); it++) 
			if ((*it) == &in)
				return true;

		return false;
	}
	
	template<class TypedControlData>
	bool TypedOutControl<TypedControlData>::IsLinkable(const BaseTypedInControl& in)
	{
		return typeid(TypedControlData) == in.ControlType();
		
	}

	template<class TypedControlData>
	bool TypedOutControl<TypedControlData>::DoTypedLink(BaseTypedInControl& in)
	{
		bool result = false;
		try
		{
			AddLink(dynamic_cast< ProperTypedInControl& >(in));	
			result = true;
		}
		catch(...)
		{
			CLAM_ASSERT( false, "TypedOutControl<TypedControlData>::DoTypedLink could not cast BaseTypedInControl to TypedInControl<TypedControlData>" );
		}
		
		return result;
	}
	
	template<class TypedControlData>
	bool TypedOutControl<TypedControlData>::IsConnectedTo(BaseTypedInControl& in)
	{
		bool result = false;
		try
		{
			result = IsConnectedTo(dynamic_cast< ProperTypedInControl& >(in));	
		}
		catch(...)
		{
			CLAM_ASSERT( false, "TypedOutControl<TypedControlData>::IsConnectedTo(BaseTypedInControl& in) could not cast BaseTypedInControl to TypedInControl<TypedControlData>" );
		}
		
		return result;
		
	}
	
} // END NAMESPACE CLAM
#endif // _TypedOutControl_
