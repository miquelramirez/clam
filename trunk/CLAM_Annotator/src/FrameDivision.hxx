
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
};

}

#endif//FrameDivision_hxx
