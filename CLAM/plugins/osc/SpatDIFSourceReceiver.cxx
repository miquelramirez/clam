#include "SpatDIFSourceReceiver.hxx"
#include <CLAM/ProcessingFactory.hxx>

static const char * metadata[] = {
	"key", "SpatDIFSourceReceiver",
	"category", "[plugin] Open Sound Control",
	0
	};

static CLAM::FactoryRegistrator<CLAM::ProcessingFactory, CLAM::SpatDIFSourceReceiver> registrator(metadata);

int CLAM::SpatDIFSourceReceiver::controls_handler(const char *path, const char *types, lo_arg **argv, int argc,
		 void *data, void *user_data)
{
	CLAM::SpatDIFSourceReceiver & self = *((CLAM::SpatDIFSourceReceiver*)user_data);


	std::string cuttedPath=path;
	if (cuttedPath.substr(cuttedPath.size()-1,1)=="/")
		cuttedPath=cuttedPath.substr(0,cuttedPath.size()-1);
	size_t lastBar=cuttedPath.rfind("/");
	if (lastBar!=-1)
		cuttedPath=cuttedPath.substr(lastBar+1,1+cuttedPath.size()-lastBar);
//	std::cout<<cuttedPath<<std::endl;


	unsigned int baseOutControlNumber=0;
	if (cuttedPath=="ypr") baseOutControlNumber=3;
	if (cuttedPath=="velocity") baseOutControlNumber=6;
	if (SplitPath(path,3)=="sampler") 
	{
		CLAM::MultiSampler::SamplerMessage samplerMessage;
		samplerMessage.voice=atoi(SplitPath(path,4).c_str());
		if (cuttedPath=="play" && argc==2)
		{
			samplerMessage.play = (argv[0]->i == 1);
			samplerMessage.loop= (argv[1]->i == 1);
			dynamic_cast<TypedOutControl <CLAM::MultiSampler::SamplerMessage> * > (&self.GetOutControl("Sampler typed messages"))->SendControl(samplerMessage);
		}
		return 0;
	}

	for (int i=0;i<argc;i++)
	{
std::cout<< "spatdifreceiver - argc: "<<argc<< " - GetNOutControls: " <<self.GetNOutControls() << " - i: "<<i<<std::endl;

		sendControl(&self.GetOutControl(baseOutControlNumber+i),argv[i]);
	}
	return 0;
}


