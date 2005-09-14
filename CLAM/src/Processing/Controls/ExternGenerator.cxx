#include "ExternGenerator.hxx"
#include "Factory.hxx"
#include "Audio.hxx"

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<ExternGenerator> regtExternGenerator( "ExternGenerator" );
	}
	
	ExternGenerator::ExternGenerator()
		: mOut("AudioOut",this)
	{
		//After being dropped it is ready to run as it does not need any configuration at all
		mExecState=Ready;
	}

	ExternGenerator::ExternGenerator(const ProcessingConfig & conf)
		: mOut("AudioOut",this)
	{
		//After being dropped it is ready to run as it does not need any configuration at all
		mExecState=Ready;
	}

	ExternGenerator::~ExternGenerator()
	{
	}

	bool ExternGenerator::Do()
	{
		return true;
	}

	bool ExternGenerator::Do( TData* buf, int nframes)
	{		
		if (!mOut.CanProduce())
		{
			//std::cout << "_outport cant produce" << std::endl;
			return true;
		}

		CLAM::Audio& so=mOut.GetAudio();

		//We assume that external audio type is the same as ours, range (-1,1) in a float
		std::copy(buf, buf+nframes, so.GetBuffer().GetPtr());
		mOut.Produce();

		return true;
	}
} //namespace CLAM
