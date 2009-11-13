#include "AudioSinkBuffer.hxx"
#include "ProcessingFactory.hxx"
#include "Audio.hxx"

#include <iostream>

namespace CLAM
{

namespace 
{
	static const char* metadata[] = {
		"key", "AudioSinkBuffer",
		"category", "Audio I/O",
		"description", "AudioSinkBuffer",
		"icon", "sink.svg",
		"embedded_svg", "sink.svg",
		0
	};
	static FactoryRegistrator<ProcessingFactory, AudioSinkBuffer> reg = metadata;
}

bool AudioSinkBuffer::Do()
{
	for (Ports::iterator it = _ports.begin(); it != _ports.end(); ++it)
	{
		Port& port = (*it);
		InPort<Audio>* in = port.mPort;

		const CLAM::Audio& so=in->GetData();
		CLAM_DEBUG_ASSERT(port.mFloatBuffer, "No float buffer");
		CLAM_DEBUG_ASSERT(!port.mDoubleBuffer, "There should not be double buffer");
		CLAM_DEBUG_ASSERT(port.mBufferSize>0, "internal buffer size must be greater than 0");
		const CLAM::TData * audioBuffer = so.GetBuffer().GetPtr();

		for (unsigned i=0; i<port.mBufferSize; i++)
			port.mFloatBuffer[i] = audioBuffer[i];

		in->Consume();
	}

	return true;
}

void AudioSinkBuffer::SetExternalBuffer(float* buf, unsigned nframes, unsigned index)
{
	CLAM_ASSERT(index < _ports.size(), "InPort<Audio> index out of range");
	Port& port = _ports[index];
	port.mPort->SetSize(1);
	port.mPort->SetHop(1);
	port.mFloatBuffer = buf;
	port.mBufferSize = nframes;
	port.mDoubleBuffer = 0;

}

void AudioSinkBuffer::SetExternalBuffer(double* buf, unsigned nframes, unsigned index)
{
	CLAM_ASSERT(index < _ports.size(), "InPort<Audio> index out of range");
	Port& port = _ports[index];
	port.mPort->SetSize(1);
	port.mPort->SetHop(1);
	port.mDoubleBuffer = buf;
	port.mBufferSize = nframes;
	port.mFloatBuffer = 0;
}

} //namespace CLAM

