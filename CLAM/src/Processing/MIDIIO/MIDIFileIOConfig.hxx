#ifndef __MIDIFILEIOCONFIG__
#define __MIDIFILEIOCONFIG__
#include <typeinfo>
#include <string>
#include "ProcessingConfig.hxx"

namespace CLAM
{
	class MIDIFileIOConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDIFileIOConfig, 1, ProcessingConfig);
		/* Filename */
		DYN_ATTRIBUTE(0,public, std::string, FileName);

	private:
		void DefaultInit();
	};
}

#endif

