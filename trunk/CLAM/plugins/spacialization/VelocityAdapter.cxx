#include "VelocityAdapter.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] = {
	"key", "VelocityAdapter",
	"category", "[plugin] 3D Audio",
	"description", "Velocities and azimuth adapter",
	0
	};
static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, VelocityAdapter> registrator(metadata);

