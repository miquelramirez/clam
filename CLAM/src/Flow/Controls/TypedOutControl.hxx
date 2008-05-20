#ifndef _TypedOutControl_
#define _TypedOutControl_

#include <string>
#include <list>
#include <typeinfo>

namespace CLAM {
	
	class BaseTypedOutControl{
	public:
		virtual ~BaseTypedOutControl(){}
		virtual bool IsLinkable(const BaseTypedInControl& in) = 0;
	};
	
	template<class TypedControlData>
	class TypedOutControl : public BaseTypedOutControl
	{
		// This is required to solve the parsing problem with iterators.
		typedef TypedInControl<TypedControlData> ProperTypedInControl;
		typedef std::list< ProperTypedInControl * > ProperTypedInControlList;

	protected:
		std::string mName;
		// mLinks will store the pointers to the connected TypedInPorts
		ProperTypedInControlList mLinks;

	public:
		TypedOutControl(const std::string &name);
		~TypedOutControl();

		void AddLink(TypedInControl<TypedControlData>& in);
		void RemoveLink(TypedInControl<TypedControlData>& in);
		void SendControl(const TypedControlData& val);
		bool IsConnected();
		bool IsConnectedTo( TypedInControl<TypedControlData> & );
		bool IsLinkable(const BaseTypedInControl& in);

	};
	
	template<class TypedControlData>
	TypedOutControl<TypedControlData>::TypedOutControl(const std::string &name)
		: mName(name)
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
	
} // END NAMESPACE CLAM
#endif // _TypedOutControl_
