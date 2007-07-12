#ifndef AudioBuffer2Stream_hxx
#define AudioBuffer2Stream_hxx

#include <CLAM/InPort.hxx>
#include <CLAM/OutPort.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/Processing.hxx>

namespace CLAM
{

class AudioBuffer2Stream : public Processing
{ 
	InPort<Audio> mIn;
	AudioOutPort mOut;
public:
	const char* GetClassName() const { return "AudioBuffer2Stream"; }
	AudioBuffer2Stream(const Config& config = Config()) 
		: mIn("Audio buffer", this)
		, mOut("Audio stream", this) 
	{
		Configure( config );
		mOut.SetSize( 2048 );
		mOut.SetHop( 2048 );
	}
 
	bool Do()
	{
		const Audio& in = mIn.GetData();
		std::cout << "doing " << in.GetSize() << std::endl;
		Audio& out = mOut.GetAudio();
		const TData* inpointer = in.GetBuffer().GetPtr();
		TData* outpointer = out.GetBuffer().GetPtr();
		std::copy(inpointer, inpointer+in.GetSize(), outpointer);
		
		// Tell the ports this is done
		mIn.Consume();
		mOut.Produce();
		return true;
	}


};

} // namespace CLAM
#endif // AudioBuffer2Stream_hxx
