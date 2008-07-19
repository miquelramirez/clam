#ifndef _BaseTypedInControl_
#define _BaseTypedInControl_

#include <string>
#include <list>
#include <typeinfo>

namespace CLAM {
	class Processing; 
	
	class BaseTypedInControl{
		std::string mName;
		Processing * mProcessing;

	public:
		BaseTypedInControl(const std::string &name, Processing * proc = 0);
		virtual ~BaseTypedInControl();
		virtual const std::type_info& ControlType() const = 0;
		const std::string& GetName() const { return mName; }
		Processing * GetProcessing() const { return mProcessing; }
	};
} // End namespace CLAM
#endif // _BaseTypedInControl_
