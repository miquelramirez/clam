#include "SinTrackingConfig.hxx"

namespace CLAM
{
void SinTrackingConfig::DefaultInit()
{
	AddAll();
	UpdateData();

	DefaultValues();
	
}

void SinTrackingConfig::DefaultValues()
{
	SetThreshold(20);//in percents
	SetnMaxSines(250);
	SetIsHarmonic(0);
}

}
