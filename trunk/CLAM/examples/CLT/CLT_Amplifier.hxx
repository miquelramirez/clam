#ifndef _CLT_AMPLIFIER
#define _CLT_AMPLIFIER

#include "CLT_MonoPlugin.hxx"

#define AMP_CONTROL 0
#define AMP_INPUT   1
#define AMP_OUTPUT  2

void initialise_CLT_amplifier();

class CLT_Amplifier : public CLT_MonoPlugin
{
public:

		TData* mControl;

		CLT_Amplifier()          {}
		virtual ~CLT_Amplifier() {}

		inline bool Do(Audio &in, Audio &out);

		static LADSPA_Handle Instantiate
		( const struct _LADSPA_Descriptor* Descriptor, 
		unsigned long SampleRate );
		static void ConnectPort (LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation);
		static void Run(LADSPA_Handle Instance, unsigned long SampleCount);

};

#endif
