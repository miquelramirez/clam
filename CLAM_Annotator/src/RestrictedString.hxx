
#ifndef _RestrictedString_
#define _RestrictedString_

#include "DynamicType.hxx"

#include <string>

namespace CLAM_Annotator{

	/* We need this stupid class in order to differentiate from a regular string
	when overloading functions*/
	class RestrictedString:public CLAM::DynamicType
	{
		DYNAMIC_TYPE(RestrictedString,1);
		DYN_ATTRIBUTE(0, public, std::string, String);

		void DefaultInit()
		{
			AddAll();
			UpdateData();
		}
		public:
		void operator = (const std::string& value)
		{
			SetString(value);
		}
		bool operator == (const std::string& value)
		{
			return (value == GetString());
		}
	};
};

#endif
