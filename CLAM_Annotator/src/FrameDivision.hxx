
#ifndef FrameDivision_hxx
#define FrameDivision_hxx

#include <CLAM/DynamicType.hxx>

namespace CLAM_Annotator
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
	CLAM::TData GetCenter(unsigned element) const
	{
		return GetFirstCenter() + element* GetInterCenterGap();
	}
	unsigned GetItem(CLAM::TData position) const
	{
		if (position < GetFirstCenter()) return 0;
		return (position-GetFirstCenter())/GetInterCenterGap();
	}
};

}

#endif//FrameDivision_hxx
