
#ifndef __AudioOutWrapper_hxx__
#define __AudioOutWrapper_hxx__

#include "AudioOut.hxx"
#include "ProcessingComposite.hxx"
#include "InPortTmpl.hxx"

namespace CLAM
{

class AudioOutWrapperConfiguration : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (AudioOutWrapperConfiguration,4 , ProcessingConfig);
	DYN_ATTRIBUTE(0, public, std::string, Name);
	DYN_ATTRIBUTE (1, public, std::string, Device);
	DYN_ATTRIBUTE (2, public, int, FrameSize);
	DYN_ATTRIBUTE (3, public, int, SampleRate);

	void DefaultInit();
};

class AudioOutWrapper : public ProcessingComposite
{

	AudioOutWrapperConfiguration mConfig;

	AudioOut mLeft;
	AudioOut mRight;

	InPortTmpl<Audio> mInput;


	void AdoptChildren();
	bool ConfigureChildren(int size);
	const char *GetClassName() const {return "AudioOutWrapper";}
	bool ConcreteConfigure(const ProcessingConfig&);

public:
	AudioOutWrapper( AudioOutWrapperConfiguration & );
	AudioOutWrapper();

	const ProcessingConfig &GetConfig() const {return mConfig;}

	bool Do(void);
	bool Do(Audio& in);	
};

} // namespace CLAM

#endif // __AudioOutWrapper_hxx__
