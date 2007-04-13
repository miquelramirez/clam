
#include "FilenameUsageExample.hxx"
#include <CLAM/OSDefines.hxx>
#include <CLAM/CLAM_Math.hxx>
#include <CLAM/Factory.hxx>
#include <ctime>
#include <cstdlib>

#define DEFAULTDELAYFACTOR 10


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<FilenameUsageExample> regtFilenameUsageExample( "FilenameUsageExample" );
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




