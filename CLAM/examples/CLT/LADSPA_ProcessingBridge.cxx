
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "LADSPA_ProcessingBridge.hxx"

extern LADSPABridge * Instance(void);

extern "C"
{
	
LADSPA_Handle Instantiate(const struct _LADSPA_Descriptor * Descriptor,  unsigned long SampleRate)
{
	LADSPABridge * test = Instance();
	

	(test->mInControlsList).resize(test->GetInControlsSize());
	(test->mOutControlsList).resize(test->GetOutControlsSize());

	(test->mInAudioList).resize(test->GetInPortsSize());
	(test->mOutAudioList).resize(test->GetOutPortsSize());
	return test;
}

void ConnectPort(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
{
	LADSPABridge * test = (LADSPABridge*)Instance;

	if(Port < test->GetInControlsIndex() + test->GetInControlsSize()) // is an "in control"
	{
		test->mInControlsList[Port] = DataLocation;
		return;
	}
	if(Port < test->GetOutControlsIndex() + test->GetOutControlsSize()) // is an "out control"
	{
		test->mOutControlsList[Port-test->GetOutControlsIndex()] = DataLocation;
		return;
	}
	if(Port < test->GetInPortsIndex() + test->GetInPortsSize())
	{
		test->mInAudioList[Port-test->GetInPortsIndex()].GetBuffer().SetPtr((CLAM::TData*)DataLocation, 1);
		return;
	}
	if(Port < test->GetOutPortsIndex() + test->GetOutPortsSize()) 
	{
		test->mOutAudioList[Port-test->GetOutPortsIndex()].GetBuffer().SetPtr((CLAM::TData*)DataLocation, 1);
		return;
	}
	CLAM_ASSERT(false, "port out of range");
}

void Activate(LADSPA_Handle Instance)
{
	LADSPABridge* test = (LADSPABridge*)Instance;
	test->Start();
}
  
void Run(LADSPA_Handle Instance, unsigned long SampleCount)
{
	LADSPABridge * test = (LADSPABridge*)Instance;
	test->DoControls();
	test->DoSizeCheck(SampleCount);
	test->DoProc();
}

void DeActivate(LADSPA_Handle Instance)
{
	LADSPABridge * test = (LADSPABridge*)Instance;
	test->Stop();
}

void CleanUp(LADSPA_Handle Instance)
{
	LADSPABridge * test = (LADSPABridge*)Instance;
	delete test;
}

LADSPA_Descriptor * g_psDescriptor;

}

class StartupShutdownHandler 
{
	LADSPABridge* test;
	char ** pcPortNames;
	LADSPA_PortDescriptor * piPortDescriptors;
	LADSPA_PortRangeHint * psPortRangeHints;
	int numPorts;

	void CreateDescriptor()
	{
		if (g_psDescriptor) 
		{
		    g_psDescriptor->UniqueID  = test->GetId(); //?
	    
		    g_psDescriptor->Label = strdup(test->GetClassName());
		    g_psDescriptor->Name = strdup(test->GetClassName());
		    g_psDescriptor->Maker = strdup("CLAM-dev");
		    g_psDescriptor->Copyright = strdup("None");
		    g_psDescriptor->Properties = LADSPA_PROPERTY_HARD_RT_CAPABLE; //?
		    g_psDescriptor->PortCount = numPorts;
		    g_psDescriptor->instantiate  = Instantiate;
		    g_psDescriptor->connect_port = ConnectPort;
		    g_psDescriptor->activate = Activate;
		    g_psDescriptor->run = Run;
		    g_psDescriptor->run_adding = 0;
		    g_psDescriptor->set_run_adding_gain = 0;
		    g_psDescriptor->deactivate = DeActivate;
		    g_psDescriptor->cleanup = CleanUp;
		}

	}

	
	void SetPortsAndControls()
	{
	   int i=0;
	   for(int j=0; i<test->GetInControlsIndex()+test->GetInControlsSize() ; i++, j++)
	   {
		piPortDescriptors[i] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL; 
		pcPortNames[i] = strdup(test->GetInControlName(j));
	   }

	   for(int j=0; i<test->GetOutControlsIndex()+test->GetOutControlsSize() ; i++, j++)
	   {
		piPortDescriptors[i] = LADSPA_PORT_OUTPUT | LADSPA_PORT_CONTROL; 
		pcPortNames[i] =  strdup(test->GetOutControlName(j));
	   }
	   for(int j=0; i<test->GetInPortsIndex()+test->GetInPortsSize() ; i++, j++)
	   {
		piPortDescriptors[i] = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO; 
		pcPortNames[i] =  strdup(test->GetInPortName(j));
           }
	   for(int j=0; i<test->GetOutPortsIndex()+test->GetOutPortsSize() ; i++, j++)
	   {
		piPortDescriptors[i] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO; 
		pcPortNames[i] =  strdup(test->GetOutPortName(j));
	   }

	}

	void SetPortRangeHints()
	{
	   for(int i=0; i<numPorts; i++)
		psPortRangeHints[i].HintDescriptor = 0;
	}
	
		
	
public:
	~StartupShutdownHandler() 
	{
	  long lIndex;
	  if (g_psDescriptor) 
	  {
	    free((char *)g_psDescriptor->Label);
	    free((char *)g_psDescriptor->Name);
	    free((char *)g_psDescriptor->Maker);
	    free((char *)g_psDescriptor->Copyright);
	    
	    free((LADSPA_PortDescriptor *)g_psDescriptor->PortDescriptors);
	    for (lIndex = 0; lIndex < g_psDescriptor->PortCount; lIndex++)
	         free((char *)(g_psDescriptor->PortNames[lIndex]));
	    free((char **)g_psDescriptor->PortNames);
	    free((LADSPA_PortRangeHint *)g_psDescriptor->PortRangeHints);
	    free(g_psDescriptor);   
	  }
	}


	StartupShutdownHandler():
		test(0), numPorts(0)
		
	{
		test = Instance();

		numPorts = test->GetInControlsSize()+test->GetOutControlsSize() 
		    + test->GetInPortsSize() + test->GetOutPortsSize();

		std::cout << "numports: " << numPorts << std::endl;
		g_psDescriptor = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));
		CreateDescriptor();
    	   piPortDescriptors = (LADSPA_PortDescriptor *)calloc(numPorts, sizeof(LADSPA_PortDescriptor));
	   g_psDescriptor->PortDescriptors = (const LADSPA_PortDescriptor *)piPortDescriptors;
	   pcPortNames = (char **)calloc(numPorts, sizeof(char *));	   
	   g_psDescriptor->PortNames = (const char **)pcPortNames;

		  SetPortsAndControls();
    	   psPortRangeHints = ((LADSPA_PortRangeHint *)calloc(numPorts, sizeof(LADSPA_PortRangeHint)));
           g_psDescriptor->PortRangeHints = (const LADSPA_PortRangeHint *)psPortRangeHints;

	   SetPortRangeHints();
	  
	   /*
	   
	   // pitch
	    
    	   psPortRangeHints[0].HintDescriptor = (LADSPA_HINT_BOUNDED_BELOW 
			   			| LADSPA_HINT_BOUNDED_ABOVE
					        | LADSPA_HINT_SAMPLE_RATE
           					| LADSPA_HINT_LOGARITHMIC
			 	 	        | LADSPA_HINT_DEFAULT_440);
	   psPortRangeHints[0].LowerBound = 0;
	   psPortRangeHints[0].UpperBound = 0.5;
	   
	   // amplitude
	   
   	   psPortRangeHints[1].HintDescriptor = (LADSPA_HINT_BOUNDED_BELOW 
			   			| LADSPA_HINT_BOUNDED_ABOVE
			 	 	        | LADSPA_HINT_DEFAULT_1);
	   psPortRangeHints[1].LowerBound = 0;
	   psPortRangeHints[1].UpperBound = 1;
		
	   // audio output
	   psPortRangeHints[2] = 0;
	   */
	}
} g_oShutdownStartupHandler;

extern "C"
{
  
const LADSPA_Descriptor * 
ladspa_descriptor(unsigned long Index) {
  if (Index == 0)
    return g_psDescriptor;
  else
    return NULL;
}

}

