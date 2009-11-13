#include "AudioSourceBuffer.hxx"
#include "ProcessingFactory.hxx"
#include "Audio.hxx"

namespace CLAM
{

namespace
{
	static const char* metadata[] = {
		"key", "AudioSourceBuffer",
		"category", "Audio I/O",
		"description", "AudioSourceBuffer",
		"icon", "source.svg",
		"embedded_svg", "source.svg",
		0
	};
	static FactoryRegistrator<ProcessingFactory, AudioSourceBuffer> reg = metadata;
}

AudioSourceBuffer::Config::~Config() { }

bool AudioSourceBuffer::Do()
{
	for (Ports::iterator it = _ports.begin(); it != _ports.end(); ++it)
	{
		Port& port = (*it);
		OutPort<Audio>* out = port.mPort;

		CLAM::Audio& so=out->GetData();
		CLAM_DEBUG_ASSERT(!port.mFloatBuffer || !port.mDoubleBuffer, "AudioSourceBuffer: Just one buffer should be set");
		CLAM_DEBUG_ASSERT(port.mFloatBuffer || port.mDoubleBuffer, "AudioSourceBuffer: No external buffer set");
		CLAM_DEBUG_ASSERT(port.mBufferSize>0, "AudioSourceBuffer: internal buffer size must be greater than 0");
		
		so.GetBuffer().SetSize(port.mBufferSize);
		CLAM::TData * audioBuffer = so.GetBuffer().GetPtr();

		if (port.mFloatBuffer)
			for (unsigned i=0; i<port.mBufferSize; i++)
				audioBuffer[i] = port.mFloatBuffer[i];
		else
			for (unsigned i=0; i<port.mBufferSize; i++)
				audioBuffer[i] = port.mDoubleBuffer[i];

		for (unsigned i=port.mBufferSize; i<out->GetSize(); i++)
			audioBuffer[i] = 0.;

		out->Produce();
	}
	return true;
}

void AudioSourceBuffer::SetExternalBuffer(const float* buf, unsigned nframes, unsigned index)
{
	CLAM_ASSERT(index < _ports.size(), "OutPort<Audio> index out of range");
	Port& port = _ports[index];
	port.mPort->SetSize(1);
	port.mPort->SetHop(1);
	port.mFloatBuffer = buf;
	port.mBufferSize = nframes;
	port.mDoubleBuffer = 0;
}

void AudioSourceBuffer::SetExternalBuffer(const double* buf, unsigned nframes, unsigned index)
{
	CLAM_ASSERT(index < _ports.size(), "OutPort<Audio> index out of range");
	Port& port = _ports[index];
	port.mPort->SetSize(1);
	port.mPort->SetHop(1);
	port.mDoubleBuffer = buf;
	port.mBufferSize = nframes;
	port.mFloatBuffer = 0;
}

} //namespace CLAM

