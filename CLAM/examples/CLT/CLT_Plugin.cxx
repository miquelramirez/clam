#include "CLT_Plugin.hxx"

using namespace CLAM;


void CLT_PluginConfig::DefaultInit (void)
{
		AddName();
		UpdateData();
}

void CLT_Plugin::ConcreteConnect (CLT_Descriptor* descr)
{
		descr->activate =            NULL;
		descr->deactivate =          NULL;
		descr->run_adding =          NULL;
		descr->set_run_adding_gain = NULL;
}

CLT_Plugin::CLT_Plugin()
		: mInput ("Input",this,1),
		  mOutput("Output",this,1)
{
		Configure(CLT_PluginConfig());
}

CLT_Plugin::CLT_Plugin(const CLT_PluginConfig& cfg)
		: mInput ("Input",this,1),
		  mOutput("Output",this,1)
{
		Configure(cfg);
}

CLT_Plugin::~CLT_Plugin()
{
}

bool CLT_Plugin::ConcreteConfigure(const ProcessingConfig& c)
{
		CopyAsConcreteConfig(mConfig, c);
		//configurar els fills
		return true;
}

		
void CLT_Plugin::CleanUp
(LADSPA_Handle Instance)
{
    delete (CLT_Plugin *)Instance;
}


void CLT_Plugin::Convert(Audio &audio)
{

		for (int i=0;i<audio.GetSize();i++)
		{
				audio.GetBuffer()[i] /= 32000;
		}
}

void CLT_Plugin::Deconvert(Audio &audio)
{
		for (int i=0;i<audio.GetSize();i++)
		{
				audio.GetBuffer()[i] *= 32000;
		}
}

