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
	
	/**
	* \brief Processing typed out control template class.
	*
	*/
	template<class TypedControlData>
	class TypedOutControl : public BaseTypedOutControl
	{
		// This is required to solve the parsing problem with iterators.
		typedef TypedInControl<TypedControlData> PeerTypedInControl;
		typedef std::list< PeerTypedInControl * > ProperTypedInControlList;


	public:
		TypedOutControl(const std::string &name = "unnamed typed in control", Processing * proc = 0);

		void SendControl(const TypedControlData& val);
		bool IsLinkable(const BaseTypedInControl& in);
//		std::list<BaseTypedInControl*>::iterator BeginInControlsConnected();
//		std::list<BaseTypedInControl*>::iterator EndInControlsConnected();
		
		//bool DoTypedLink(BaseTypedInControl& in);

	};
	
	template<class TypedControlData>
	TypedOutControl<TypedControlData>::TypedOutControl(const std::string &name, Processing * proc)
		: BaseTypedOutControl(name,proc)
	{
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
	bool TypedOutControl<TypedControlData>::IsLinkable(const BaseTypedInControl& in)
	{
		return typeid(TypedControlData) == in.ControlType();
		
	}
/*
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
*/	
	
} // END NAMESPACE CLAM
#endif // _TypedOutControl_
