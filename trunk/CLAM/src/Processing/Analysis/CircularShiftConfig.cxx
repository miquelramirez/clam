#include "CircularShiftConfig.hxx"

namespace CLAM
{
	/* The  Configuration object has at least to have a name */

	void CircularShiftConfig::DefaultInit()
	{
		/* the dynamic type takes care if we add an existing attr .. */
		AddAmount();
		/* All Attributes are added */
		UpdateData();
		SetAmount(0);

	}



}
