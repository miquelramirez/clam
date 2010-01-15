#ifndef _TypedOutControl_
#define _TypedOutControl_

#include "OutControl.hxx"

#warning TypedOutControl<T> is deprecated use OutControl<T> instead

namespace CLAM
{
	template<class ControlDataType>
	class TypedOutControl : public OutControl<ControlDataType>
	{
	public:
		TypedOutControl(const std::string &name = "unnamed typed in control", Processing * proc = 0)	
			: OutControl<ControlDataType>(name, proc)
		{
		}
	};
}

#endif // _TypedOutControl_
