
#include "ProcessingConfigPresentation.hxx"
#include "ProcessingConfig.hxx"

namespace NetworkGUI
{

ProcessingConfigPresentation::ProcessingConfigPresentation()
{
	SetConfig.Wrap( this, &ProcessingConfigPresentation::OnNewConfig );
}
/*
void ProcessingConfigPresentation::OnNewConfig( CLAM::ProcessingConfig * config)
{
	mConfig = config;
}
*/	

} // namespace NetworkGUI
