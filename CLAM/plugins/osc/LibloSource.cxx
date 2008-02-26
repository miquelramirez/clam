#include "LibloSource.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] = {
	"key", "LibloSource",
	"category", "[plugin] Open Sound Control",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::LibloSource> registrator(metadata);

void CLAM::LibloSource::error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}


/* catch any incoming messages and display them. returning 1 means that the
 * message has not been fully handled and the server should try other methods */
int CLAM::LibloSource::generic_handler(const char *path, const char *types, lo_arg **argv,
		    int argc, void *data, void *user_data)
{
    int i;

    printf("path: <%s>\n", path);
    for (i=0; i<argc; i++) {
	printf("arg %d '%c' ", i, types[i]);
	lo_arg_pp((lo_type)types[i], argv[i]);
	printf("\n");
    }
    printf("\n");
    fflush(stdout);
    return 1;
}

int CLAM::LibloSource::controls_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 void *data, void *user_data)
{
    /* example showing pulling the argument values out of the argv array */
    printf("%s <- f:%f, f:%f, f:%f\n\n", path, argv[0]->f, argv[1]->f, argv[2]->f);
    fflush(stdout);
    CLAM::LibloSource & self = *((CLAM::LibloSource*)user_data);
    self._out1.SendControl( argv[0]->f );
    self._out2.SendControl( argv[1]->f );
    self._out3.SendControl( argv[2]->f );

    return 0;
}

int CLAM::LibloSource::quit_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 void *data, void *user_data)
{
    printf("quiting\n\n");
    fflush(stdout);

    return 0;
}


