#ifndef AudioStream2Buffer_hxx
#define AudioStream2Buffer_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/AudioInPort.hxx>
#include <CLAM/Audio.hxx>
#include <CLAM/OutPort.hxx>
#include <cstring>

class AudioStream2Buffer : public CLAM::Processing
{
public:
	class Config : public CLAM::ProcessingConfig
	{
		DYNAMIC_TYPE_USING_INTERFACE( Config, 2, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, unsigned, HopSize);
		DYN_ATTRIBUTE( 1, public, unsigned, BufferSize);
	public:
		void DefaultInit()
		{
			AddAll();
			UpdateData();
			SetHopSize(512);
			SetBufferSize(1024);
		}
	};
private:
	Config _config;
	CLAM::AudioInPort _in;
	CLAM::OutPort<CLAM::Audio> _out;
	unsigned _bufferSize;
	unsigned _hopSize;
public:
	const char* GetClassName() const { return "AudioStream2Buffer"; }
	AudioStream2Buffer(const Config& config = Config()) 
		: _in("Audio stream", this)
		, _out("Audio buffer", this) 
	{
		Configure( config );
	}
private:
	bool ConcreteConfigure(const CLAM::ProcessingConfig & c)
	{
		CopyAsConcreteConfig(_config, c);
		_bufferSize = _config.GetBufferSize();
		_hopSize = _config.GetHopSize();
//		_in.SetSize( _config.GetBufferSize() );
		_in.SetSize( _hopSize );
		_in.SetHop( _hopSize );
		return true;
	}
public:
	const CLAM::ProcessingConfig & GetConfig() const { return _config; }
	bool Do()
	{
		const CLAM::Audio& in = _in.GetAudio();
		CLAM::Audio& out = _out.GetData();

		CLAM_ASSERT(_hopSize==unsigned(in.GetSize()),
			"AudioBuffer2Stream: Input does not provide the configured hop size"); 

		const CLAM::TData* inpointer = in.GetBuffer().GetPtr();
		out.SetSize(_bufferSize);
		CLAM::TData* outpointer = out.GetBuffer().GetPtr();
		std::memcpy(outpointer, inpointer, sizeof(CLAM::TData)*_hopSize);

		_in.Consume();
		_out.Produce();
		return true;
	}


};

#endif//AudioStream2Buffer_hxx

