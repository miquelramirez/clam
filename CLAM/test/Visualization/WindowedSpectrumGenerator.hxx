#ifndef __WINDOWEDSPECTRUMGENERATOR__
#define __WINDOWEDSPECTRUMGENERATOR__

#include "ProcessingComposite.hxx"
#include "AudioMultiplier.hxx"
#include "WindowGenerator.hxx"
#include "FFT_rfftw.hxx"
#include "Audio.hxx"

using namespace CLAM;

namespace CLAMTest
{
	class WindowedSpectrumGeneratorConfig
		: public ProcessingConfig
	{
	public:

		DYNAMIC_TYPE_USING_INTERFACE( WindowedSpectrumGeneratorConfig, 4, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, std::string, Name );
		DYN_ATTRIBUTE( 1, public, EWindowType, WindowType );
		DYN_ATTRIBUTE( 2, public, TData, SampleRate );
		DYN_ATTRIBUTE( 3, public, TSize, FrameSize );

		void DefaultInit();
	};
		

	class WindowedSpectrumGenerator 
		: public ProcessingComposite
	{
	public:

			WindowedSpectrumGenerator();

			WindowedSpectrumGenerator( const WindowedSpectrumGeneratorConfig& cfg );

			~WindowedSpectrumGenerator()
			{
			}


		virtual bool Do();
		virtual bool Do( Audio& inAudio, Spectrum& outSpectra );

		void SetWindowingType( EWindowType wtype );

		void SetLogOutput( )
		{
			mConvertToLog = true;
		}
		void SetLinearOutput( )
		{
			mConvertToLog = false;
		}

		inline const ProcessingConfig& GetConfig() const 
		{
			return mConfig;
		}

		virtual const char *GetClassName() const 
		{
			return "WindowedSpectrumGenerator";
		}

	protected:

		virtual bool ConcreteConfigure( const ProcessingConfig& cfg ); 

		virtual bool ConcreteStart();

		virtual bool ConcreteStop();

	private:

			WindowedSpectrumGeneratorConfig mConfig;

			// PO's

			WindowGenerator     mWindowGen;
			AudioMultiplier     mWindowApplier;
			FFT_rfftw           mAnalysis;
			Audio               mWindowedAudio;
			Audio               mWindow;

			bool                mConvertToLog;
	};
	
}

#endif // WindowedSpectrumGenerator.hxx

