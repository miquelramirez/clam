/* amp.c

   Free software by Richard W.E. Furse. Do with as you will. No
   warranty.

   This LADSPA plugin provides simple mono and stereo amplifiers.

   This file has poor memory protection. Failures during malloc() will
   not recover nicely. */

/*****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*****************************************************************************/

#include <ladspa.h>

/*****************************************************************************/

/* The port numbers for the plugin: */

#define CONTROL 0
#define INPUT  1
#define OUTPUT 2

/*****************************************************************************/

/* The structure used to hold port connection information and state
   (actually gain controls require no further state). */

typedef struct {

  /* Ports:
     ------ */

  LADSPA_Data * mControl;
  LADSPA_Data * mInput;
  LADSPA_Data * mOutput;

} Test;

/*****************************************************************************/

/* Construct a new plugin instance. */
LADSPA_Handle InstantiateTest(const LADSPA_Descriptor * Descriptor, unsigned long SampleRate) 
{
  return malloc(sizeof(Test));
}

/*****************************************************************************/

/* Connect a port to a data location. */
void ConnectPortToTest(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation) 
{
	Test* test;
	test = (Test*)Instance;
 	switch (Port) 
	{
		case CONTROL:
			test->mControl = DataLocation;
			break;
		case INPUT:
			test->mInput = DataLocation;
			break;
		case OUTPUT:
			test->mOutput = DataLocation;
			break;
	}
}

/*****************************************************************************/

void RunTest(LADSPA_Handle Instance, unsigned long SampleCount) 
{
	LADSPA_Data * input;
	LADSPA_Data * output;
	LADSPA_Data gain;
	Test * test;
	test = (Test*)Instance;
  
	input = test->mInput;
  	output = test->mOutput;
	gain = *(test->mControl);
 
	unsigned long lSampleIndex;
	for (lSampleIndex = 0; lSampleIndex < SampleCount; lSampleIndex++)
	{
		*output = *input; // * fGain;
		output++;
		input++;
	}
}

/*****************************************************************************/

/* Throw away a simple delay line. */
void CleanupTest(LADSPA_Handle Instance) 
{
	free(Instance);
}

/*****************************************************************************/

LADSPA_Descriptor * descriptor = NULL;

/*****************************************************************************/

/* _init() is called automatically when the plugin library is first
   loaded. */
void _init()
{
	char ** pcPortNames;
	LADSPA_PortDescriptor * piPortDescriptors;
	LADSPA_PortRangeHint * psPortRangeHints;

	descriptor = (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));
	if (descriptor) 
	{
		descriptor->UniqueID = 5000;
		descriptor->Label = strdup("testing plugin");
		descriptor->Name = strdup("Testing Plugin");
		descriptor->PortCount = 3;
		piPortDescriptors = (LADSPA_PortDescriptor *)calloc(3, sizeof(LADSPA_PortDescriptor));
		descriptor->PortDescriptors = (const LADSPA_PortDescriptor *)piPortDescriptors;
		piPortDescriptors[CONTROL] = LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[INPUT] = LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[OUTPUT] = LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		pcPortNames = (char **)calloc(3, sizeof(char *));
    		descriptor->PortNames = (const char **)pcPortNames;
    		pcPortNames[CONTROL] = strdup("Gain");
   		pcPortNames[INPUT] = strdup("Input");
    		pcPortNames[OUTPUT] = strdup("Output");
    		psPortRangeHints = ((LADSPA_PortRangeHint *)calloc(3, sizeof(LADSPA_PortRangeHint)));
    		descriptor->PortRangeHints = (const LADSPA_PortRangeHint *)psPortRangeHints;
    		psPortRangeHints[CONTROL].HintDescriptor = 0;
    		psPortRangeHints[INPUT].HintDescriptor = 0;
		psPortRangeHints[OUTPUT].HintDescriptor = 0;
	    	descriptor->instantiate = InstantiateTest;
		descriptor->connect_port = ConnectPortToTest;
		descriptor->activate = NULL;
		descriptor->run = RunTest;
		descriptor->run_adding = NULL;
		descriptor->set_run_adding_gain = NULL;
		descriptor->deactivate = NULL;
   		descriptor->cleanup = CleanupTest;
	}
}

/*****************************************************************************/

void deleteDescriptor(LADSPA_Descriptor * psDescriptor) 
{
  unsigned long lIndex;
  if (psDescriptor) {
    free((char *)psDescriptor->Label);
    free((char *)psDescriptor->Name);
    free((LADSPA_PortDescriptor *)psDescriptor->PortDescriptors);
    for (lIndex = 0; lIndex < psDescriptor->PortCount; lIndex++)
      free((char *)(psDescriptor->PortNames[lIndex]));
    free((char **)psDescriptor->PortNames);
    free((LADSPA_PortRangeHint *)psDescriptor->PortRangeHints);
    free(psDescriptor);
  }
}

/*****************************************************************************/

/* _fini() is called automatically when the library is unloaded. */
void _fini() 
{
  deleteDescriptor(descriptor);
}

/*****************************************************************************/

/* Return a descriptor of the requested plugin type. There are two
   plugin types available in this library (mono and stereo). */
const LADSPA_Descriptor * ladspa_descriptor(unsigned long Index) {
  /* Return the requested descriptor or null if the index is out of
     range. */
	switch (Index) 
	{
		case 0:
			return descriptor;
		default:
			return NULL;
	}
}

/*****************************************************************************/

/* EOF */
