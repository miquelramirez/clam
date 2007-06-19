
#include "FilenameUsageExample.hxx"
#include <CLAM/OSDefines.hxx>
#include <CLAM/CLAM_Math.hxx>
#include <CLAM/ProcessingFactory.hxx>
#include <ctime>
#include <cstdlib>

#define DEFAULTDELAYFACTOR 10


namespace CLAM
{

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, FilenameUsageExample> regFilenameUsageExample("FilenameUsageExample");
}
	
	
void FilenameUsageExampleConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
}

FilenameUsageExample::FilenameUsageExample()
{
	FilenameUsageExampleConfig cfg;
	Configure(cfg);
}

FilenameUsageExample::FilenameUsageExample( const FilenameUsageExampleConfig & cfg)
{
	Configure(cfg);
}


bool FilenameUsageExample::Do()
{

	return true;
}

bool FilenameUsageExample::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	return true;
}

} // namespace CLAM




