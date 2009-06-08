#include "MultiLibloSource.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] = {
	"key", "MultiLibloSource",
	"category", "[plugin] Open Sound Control",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::MultiLibloSource> registrator(metadata);

int CLAM::MultiLibloSource::controls_handler(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
{
	CLAM::MultiLibloSource & self = *((CLAM::MultiLibloSource*)user_data);
	/* example showing pulling the argument values out of the argv array */
//	printf("%s <- f:",path);
	for (int i=0;i<argc;i++)
	{
		//TODO: replace by ASSERT?
		if (self._outControls.size()>i)
			CLAM::MultiLibloSource::sendControl(self._outControls[i],argv[i]);
	}
//	printf("\n\n");
//	fflush(stdout);
	return 0;
}

