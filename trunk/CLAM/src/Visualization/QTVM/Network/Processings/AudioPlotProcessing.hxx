#ifndef __AUDIOPLOTPROCESSING__
#define __AUDIOPLOTPROCESSING__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "AudioInPort.hxx"
#include "Audio.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetAudioPlot;
	}
}

namespace CLAM
{

	class AudioPlotProcessingConfig : public ProcessingConfig
	{
		public:
			DYNAMIC_TYPE_USING_INTERFACE (AudioPlotProcessingConfig, 1, ProcessingConfig);
			DYN_ATTRIBUTE (0, public, std::string, Name);

		protected:
			void DefaultInit();
	};


	class AudioPlotProcessing : public Processing
	{
		public:
			AudioPlotProcessing();
			AudioPlotProcessing(const AudioPlotProcessingConfig& cfg);
			virtual ~AudioPlotProcessing();

			bool Do();
			bool Do(const Audio& audio);

			const char * GetClassName() const {return "AudioPlotProcessing";}
	
			inline const ProcessingConfig &GetConfig() const { return mConfig;}
			bool ConcreteConfigure(const ProcessingConfig& c);

		private:
			AudioPlotProcessingConfig mConfig;
			VM::NetAudioPlot* mPlot;
			AudioInPort mInput;
			
			void InitAudioPlot();
	};
}

#endif

