
#ifndef _AudioMixer_hxx__
#define _AudioMixer_hxx__

#include "Processing.hxx"
#include "AudioOutPort.hxx"
#include "AudioInPort.hxx"
#include "Audio.hxx"
#include "InControl.hxx"

namespace CLAM
{

class AudioMixerConfig: public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (AudioMixerConfig, 2, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, int, NumberOfInPorts);
	DYN_ATTRIBUTE (1, public, int, FrameSize);

protected:
	void DefaultInit(void)
	{
		AddAll();
		UpdateData();
		SetNumberOfInPorts(0);
		SetFrameSize(512);
	}

};

class AudioMixer: public Processing
{
private:
	AudioMixerConfig  mConfig;

	std::vector< AudioInPort* > mInputPorts;
	AudioOutPort     mOutputPort;		
	std::vector< InControl* > mInputControls;
	
	void RemovePortsAndControls();
	void CreatePortsAndControls();

public:
	bool ConcreteConfigure(const ProcessingConfig& c);
	AudioMixer();
	virtual ~AudioMixer()
	{
		RemovePortsAndControls();
	}
	bool ModifiesPortsAndControlsAtConfiguration()
	{
		return true;
	}

	const char * GetClassName() const {return "AudioMixer";}
	const ProcessingConfig &GetConfig() const { return mConfig;}
	bool Do();
};

} // namespace CLAM

#endif // __AudioMixer_hxx__
