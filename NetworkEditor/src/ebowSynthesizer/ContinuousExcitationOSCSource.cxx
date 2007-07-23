#include "ContinuousExcitationOSCSource.hxx"

namespace CLAM
{

ContinuousExcitationOSCSource::ContinuousExcitationOSCSource(std::string targetName)
 :	mTargetName(targetName),
	mOSCSource()
{
	mOSCSource.Start();
}

ContinuousExcitationOSCSource::~ContinuousExcitationOSCSource()
{
	mOSCSource.Stop();
}

bool ContinuousExcitationOSCSource::GetControlValues(float& pitch, float& amplitude)
{
	std::vector<float>* valuesPtr = mOSCSource.GetLastValuesForTarget(mTargetName);
	if (valuesPtr == NULL || valuesPtr->size() < 2)
	{
		pitch = 0.0;
		amplitude = 0.0;
	}
	else
	{
		pitch = valuesPtr->at(0);
		amplitude = valuesPtr->at(1);
	}

	return true;
}

} // end namespace CLAM


