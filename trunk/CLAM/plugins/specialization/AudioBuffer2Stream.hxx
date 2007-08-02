#ifndef AudioBuffer2Stream_hxx
#define AudioBuffer2Stream_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/AudioWindowingConfig.hxx>

namespace CLAM
{

class AudioBuffer2Stream : public Processing
{
	// TODO: This configuration is over crowed
	typedef AudioWindowingConfig Config;
	InPort<Audio> mIn;
	AudioOutPort mOut;
	unsigned mHopSize;
	unsigned mWindowSize;
	Config mConfig;
public:
	const char* GetClassName() const { return "AudioBuffer2Stream"; }
	AudioBuffer2Stream(const Config& config = Config()) 
		: mIn("Audio buffer", this)
		, mOut("Audio stream", this) 
	{
		Configure( config );
	}
	bool ConcreteConfigure(const ProcessingConfig & c)
	{
		CopyAsConcreteConfig(mConfig, c);
		mHopSize = mConfig.GetHopSize();
		mWindowSize = mConfig.GetFFTSize();
		mOut.SetSize( mWindowSize );
		mOut.SetHop( mHopSize );
		return true;
	}
	const ProcessingConfig & GetConfig() const { return mConfig; }
 
	bool Do()
	{
		const Audio& in = mIn.GetData();
		CLAM_ASSERT(mWindowSize==in.GetSize(),
			"AudioBuffer2Stream: Input does not provide the configured window size"); 
		Audio& out = mOut.GetAudio();
		const TData* inpointer = in.GetBuffer().GetPtr();
		TData* outpointer = out.GetBuffer().GetPtr();
		// Zero fill the new incomming hopSize
		std::fill(outpointer+mWindowSize-mHopSize, outpointer+mWindowSize, 0.0);
		// Add the input on the full window
		std::transform(inpointer, inpointer+mWindowSize, outpointer, outpointer, std::plus<TData>());
		
		// Tell the ports this is done
		mIn.Consume();
		mOut.Produce();
		return true;
	}


};

} // namespace CLAM
#endif // AudioBuffer2Stream_hxx
