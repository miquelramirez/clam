#include "ExternSink.hxx"
#include "Factory.hxx"
#include "Audio.hxx"

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<ExternSink> regtExternSink( "ExternSink" );
	}
	
	ExternSink::ExternSink()
		: mIn("AudioIn",this)
	{
	}

	ExternSink::ExternSink(const ProcessingConfig & conf)
		: mIn("AudioIn",this)
	{
	}

	ExternSink::~ExternSink()
	{
	}

	bool ExternSink::Do()
	{
		return true;
	}

	bool ExternSink::Do( TData* buf, int nframes)
	{
		if (!mIn.CanConsume())
		{
			//std::cout << "_inport cant consume" << std::endl;
			return true;
		}

		const CLAM::Audio& so=mIn.GetAudio();
		
		//We assume that external audio type is the same as ours, range (-1,1) in a float
		std::copy(so.GetBuffer().GetPtr(),so.GetBuffer().GetPtr()+nframes, buf);
		mIn.Consume();

		return true;
	}
} //namespace CLAM
