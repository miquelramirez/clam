#ifndef _TypedOutControl_
#define _TypedOutControl_

#include <string>
#include <list>
#include <typeinfo>
#include <CLAM/Assert.hxx>
#include <CLAM/BaseTypedOutControl.hxx>
#include <CLAM/TypedInControl.hxx>

namespace CLAM {
	class Processing;
	
	template<class TypedControlData>
	class TypedOutControl : public BaseTypedOutControl
	{
		// This is required to solve the parsing problem with iterators.
		typedef TypedInControl<TypedControlData> ProperTypedInControl;
		typedef std::list< ProperTypedInControl * > ProperTypedInControlList;

	protected:
		// mLinks will store the pointers to the connected TypedInPorts
		std::list< BaseTypedInControl * > mLinks;

	public:
		TypedOutControl(const std::string &name = "unnamed typed in control", Processing * proc = 0);
		~TypedOutControl();

		void AddLink(TypedInControl<TypedControlData>& in);
		/**
			WARNING: You should call IsLinkable before using the Link function or you'll get an assert failure if In and Out Control types are different.
		*/
		void AddLink(BaseTypedInControl& in);
		void RemoveLink(TypedInControl<TypedControlData>& in);
		void RemoveLink(BaseTypedInControl& in);
		void SendControl(const TypedControlData& val);
		bool IsConnected();
		bool IsConnectedTo(TypedInControl<TypedControlData> & );
		bool IsConnectedTo(BaseTypedInControl& in);
		bool IsLinkable(const BaseTypedInControl& in);
		std::list<BaseTypedInControl*>::iterator BeginInControlsConnected();
		std::list<BaseTypedInControl*>::iterator EndInControlsConnected();
		
		//bool DoTypedLink(BaseTypedInControl& in);

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

	/**
		WARNING: You should call IsLinkable before using the Link function or you'll get an assert failure if In and Out Control types are different.
	*/
	template<class TypedControlData>
	void TypedOutControl<TypedControlData>::AddLink(BaseTypedInControl& in)
	{
		try
		{
			AddLink(dynamic_cast< ProperTypedInControl& >(in));	
		}
		catch(...)
		{
			CLAM_ASSERT( false, "TypedOutControl<TypedControlData>::AddLink(BaseTypedInControl&) could not cast BaseTypedInControl to TypedInControl<TypedControlData>" );
		}
	}

	template<class TypedControlData>
	void TypedOutControl<TypedControlData>::RemoveLink(TypedInControl<TypedControlData>& in)
	{
		mLinks.remove( &in );
		in.OutControlInterface_RemoveLink(*this);
	}
	template<class TypedControlData>
	void TypedOutControl<TypedControlData>::RemoveLink(BaseTypedInControl& in)
	{
		try
		{
			RemoveLink(dynamic_cast< ProperTypedInControl& >(in));	
		}
		catch(...)
		{
			CLAM_ASSERT( false, "TypedOutControl<TypedControlData>::RemoveLink(BaseTypedInControl&) could not cast BaseTypedInControl to TypedInControl<TypedControlData>" );
		}
	}
	template<class TypedControlData>
	void TypedOutControl<TypedControlData>::SendControl(const TypedControlData& val)
	{
		typename std::list< BaseTypedInControl * >::iterator it;
		
		for (it=mLinks.begin(); it!=mLinks.end(); it++) 
		{
			((dynamic_cast<TypedInControl<TypedControlData>*>(*it)))->DoControl(val);
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
		typename std::list< BaseTypedInControl * >::iterator it;
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
	
	template<class TypedControlData>
	std::list<BaseTypedInControl*>::iterator TypedOutControl<TypedControlData>::BeginInControlsConnected()
	{
		return mLinks.begin();
	}

	template<class TypedControlData>
	std::list<BaseTypedInControl*>::iterator TypedOutControl<TypedControlData>::EndInControlsConnected()
	{
		return mLinks.end();
	}
	
	
} // END NAMESPACE CLAM
#endif // _TypedOutControl_
