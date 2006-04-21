
#ifndef FrameDivision_hxx
#define FrameDivision_hxx

#include "DynamicType.hxx"

namespace Simac
{
class FrameDivision : public CLAM::DynamicType
{
	DYNAMIC_TYPE(FrameDivision,2);
	DYN_ATTRIBUTE(0, public, CLAM::TData, FirstCenter);
	DYN_ATTRIBUTE(1, public, CLAM::TData, InterCenterGap);

	void DefaultInit()
	{
		AddAll();
		UpdateData();
	}
public:
	CLAM::TData GetCenter(unsigned element)
	{
		return GetFirstCenter() + element* GetInterCenterGap();
	}
};

}

#endif//FrameDivision_hxx
