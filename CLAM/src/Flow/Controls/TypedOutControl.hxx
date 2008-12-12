#ifndef _TypedOutControl_
#define _TypedOutControl_

#include <string>
#include <list>
#include <typeinfo>
#include "Assert.hxx"
#include "OutControl.hxx"
#include "TypedInControl.hxx"

namespace CLAM {
	class Processing;
	
	/**
	* \brief Processing typed out control template class.
	*
	*/
	template<class TypedControlData>
	class TypedOutControl : public OutControlBase
	{
		// This is required to solve the parsing problem with iterators.
		typedef TypedInControl<TypedControlData> PeerTypedInControl;
		typedef std::list< PeerTypedInControl * > ProperTypedInControlList;


	public:
		TypedOutControl(const std::string &name = "unnamed typed in control", Processing * proc = 0);

		void SendControl(const TypedControlData& val);
		bool IsLinkable(const InControlBase& in);
		virtual const std::type_info & GetTypeId() const 
		{
			return typeid(TypedControlData);
		};
	};
	
	template<class TypedControlData>
	TypedOutControl<TypedControlData>::TypedOutControl(const std::string &name, Processing * proc)
		: OutControlBase(name,proc)
	{
	}

	template<class TypedControlData>
	void TypedOutControl<TypedControlData>::SendControl(const TypedControlData& val)
	{
		typename std::list< InControlBase * >::iterator it;
		
		for (it=mLinks.begin(); it!=mLinks.end(); it++) 
		{
			((dynamic_cast<TypedInControl<TypedControlData>*>(*it)))->DoControl(val);
		}
	}

	template<class TypedControlData>
	bool TypedOutControl<TypedControlData>::IsLinkable(const InControlBase& in)
	{
		return typeid(TypedControlData) == in.GetTypeId();
		
	}

} // END NAMESPACE CLAM
#endif // _TypedOutControl_
