#ifndef __MIXER__
#define __MIXER__

#include "Processing.hxx"
#include "Array.hxx"
#include "Audio.hxx"

namespace CLAM
{
	class MixerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE ( MixerConfig, 1, ProcessingConfig );
		DYN_ATTRIBUTE (0, public, std::string, Name);

	protected:
		void DefaultInit(void)
		{
			AddName();
	
			UpdateData();
		}
	};

	class Mixer: public Processing
	{
	private:
		MixerConfig			mConfig;
	public:
		Mixer()
		{
			MixerConfig cfg;

			Configure( cfg );
		}

		Mixer( const MixerConfig& c)
		{
			  Configure( c );
		}

		~Mixer(){}

		const ProcessingConfig &GetConfig() const { return mConfig; }

		bool ConcreteConfigure( const ProcessingConfig& cfg ) throw(std::bad_cast)
		{
			try {
				mConfig = dynamic_cast<const MixerConfig&>(cfg);
			}
			catch (std::bad_cast) {
				throw(ErrProcessingObj("Mixer::ConcreteConfigure(): Wrong argument type.",this));
			}
			return true;
		}

		bool Do(void) { return true; }

		bool Do( Array< Audio >& AudioArray, Audio& out); 
	};
}

#endif