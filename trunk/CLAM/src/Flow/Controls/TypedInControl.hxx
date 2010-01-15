#ifndef _TypedInControl_
#define _TypedInControl_

#include <CLAM/InControl.hxx>
#warning TypedInControl<T> is deprecated use InControl<T> instead

namespace CLAM
{

/**  Transition class, use InControl instead */
template<class ControlDataType>
class TypedInControl : public InControl<ControlDataType>
{
public:
		/// Constructor to use when no callback is used
		TypedInControl(const std::string &name = "unnamed in control", Processing * proc = 0)
			: InControl<ControlDataType>(name,proc)
		{
		}
		/// Constructor to use a callback by const reference
		template <typename ProcessingType, typename ParameterType>
		TypedInControl(const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(const ParameterType&))
			: InControl<ControlDataType>(name,proc,callback)
		{
		}
		/// Constructor to use a callback by const reference plus a port id to distinguish different caller controls in a single serving callback
		template <typename ProcessingType, typename ParameterType>
		TypedInControl(unsigned id, const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(unsigned, const ParameterType&))
			: InControl<ControlDataType>(id,name,proc,callback)
		{
		}
		/// Constructor to use a callback by copy
		template <typename ProcessingType, typename ParameterType>
		TypedInControl(const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(ParameterType))
			: InControl<ControlDataType>(name,proc,callback)
		{
		}
		/// Constructor to use a callback by copy plus a port id to distinguish different caller controls in a single serving callback
		template <typename ProcessingType, typename ParameterType>
		TypedInControl(unsigned id, const std::string &name, ProcessingType * proc, void (ProcessingType::*callback)(unsigned, ParameterType))
			: InControlBase(id,name,proc,callback)
		{
		}

};

}

#endif // _TypedInControl_
