#ifndef INIT_CLT_DESCRIPTOR
#define INIT_CLT_DESCRIPTOR

#include <ladspa.h>
#include <stdlib.h>
#include "CLT_Plugin.hxx"



CLT_Descriptor ** g_ppsRegisteredDescriptors = NULL;
unsigned long g_lPluginCapacity = 0;
unsigned long g_lPluginCount = 0;

void initialise_modules();
void finalise_modules();

/*****************************************************************************/

int 
pluginNameComparator(const void * pvDescriptor1, const void * pvDescriptor2) {

  const CLT_Descriptor * psDescriptor1
    = *(const CLT_Descriptor **)pvDescriptor1;
  const CLT_Descriptor * psDescriptor2
    = *(const CLT_Descriptor **)pvDescriptor2;

  int iResult = strcmp(psDescriptor1->Name, psDescriptor2->Name);
  if (iResult < 0)
    return -1;
  else if (iResult > 0)
    return 1;
  else
    return 0;
}


/*****************************************************************************/

#define CAPACITY_STEP 20

void registerNewPluginDescriptor(CLT_Descriptor * psDescriptor) 
{
		if (g_lPluginCapacity == g_lPluginCount) {
				/* Full. Enlarge capacity. */
				CLT_Descriptor ** ppsOldDescriptors
						= g_ppsRegisteredDescriptors;
				g_ppsRegisteredDescriptors
						= new CLT_Descriptor_ptr[g_lPluginCapacity + CAPACITY_STEP];
				if (g_lPluginCapacity > 0) {
						memcpy(g_ppsRegisteredDescriptors, 
							   ppsOldDescriptors,
							   g_lPluginCapacity * sizeof(CLT_Descriptor_ptr));
						delete [] ppsOldDescriptors;
				}
				g_lPluginCapacity += CAPACITY_STEP;
		}
		g_ppsRegisteredDescriptors[g_lPluginCount++] = psDescriptor;
}

/*****************************************************************************/



/** A global object of this class is used to perform initialisation
    and shutdown services for the entire library. The constructor is
    run when the library is loaded and the destructor when it is
    unloaded. */
class StartupShutdownHandler {
public:

  StartupShutdownHandler() {
    initialise_modules();
    qsort(g_ppsRegisteredDescriptors, 
	  g_lPluginCount,
	  sizeof(CLT_Descriptor_ptr),
	  pluginNameComparator);
  }

  ~StartupShutdownHandler() {
    if (g_ppsRegisteredDescriptors != NULL) {
      for (unsigned long lIndex = 0; lIndex < g_lPluginCount; lIndex++)
	  {
			  delete g_ppsRegisteredDescriptors[lIndex];
	  }
      delete [] g_ppsRegisteredDescriptors;

    }
    finalise_modules();
  }

} g_oStartupShutdownHandler;
  
/*****************************************************************************/

const LADSPA_Descriptor * 
ladspa_descriptor(unsigned long Index) 
{
		if (Index < g_lPluginCount)
				return g_ppsRegisteredDescriptors[Index];
		else
				return NULL;
}

#endif
/* EOF */


