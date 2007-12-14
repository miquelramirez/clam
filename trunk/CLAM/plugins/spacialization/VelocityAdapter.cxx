#include "VelocityAdapter.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] = {
	"key", "VelocityAdapter",
	"category", "3D Audio",
	"description", "vx_out=vy_in vy_out=-vx_in",
	0
	};
static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, VelocityAdapter> registrator(metadata);

