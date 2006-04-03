
#ifndef Enumerated_hxx
#define Enumerated_hxx

#include <CLAM/DynamicType.hxx>
#include <CLAM/XMLAdapter.hxx>
#include <CLAM/TypeInfo.hxx>
#include <string>

namespace CLAM_Annotator
{
	/* We need this stupid class in order to differentiate from a regular string
	when overloading functions*/
	class Enumerated:public CLAM::DynamicType
	{
		DYNAMIC_TYPE(Enumerated,1);
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
		void StoreOn(CLAM::Storage & storage) const
		{
			CLAM::XMLAdapter<std::string> adapter(GetString(), 0, false);
			storage.Store(adapter);
		}
		void LoadFrom(CLAM::Storage & storage)
		{
			CLAM::XMLAdapter<std::string> adapter(GetString(), 0, false);
			if (!storage.Load(adapter))
				RemoveString();
		}
	};
};
namespace CLAM
{
	CLAM_TYPEINFOGROUP(CLAM::BasicCTypeInfo, CLAM_Annotator::Enumerated);
}
#endif//Enumerated_hxx
