

#include "OverlapAddConfig.hxx"

namespace CLAM
{

void OverlapAddConfig::DefaultInit()
{
	/* the dynamic type takes care if we add an existing attr .. */
	AddHopSize();
	AddFrameSize();

	/* all attributes are added */
	UpdateData();
	DefaultValues();
}

void OverlapAddConfig::DefaultValues()
{
	/* set default values */
	SetHopSize(256);
	SetFrameSize(256);
}

} // namespace CLAM

