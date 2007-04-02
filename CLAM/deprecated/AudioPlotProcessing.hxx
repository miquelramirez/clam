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
		DYNAMIC_TYPE_USING_INTERFACE (AudioPlotProcessingConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, std::string, Caption);
		DYN_ATTRIBUTE (2, public, int, xpos);
		DYN_ATTRIBUTE (3, public, int, ypos);
		DYN_ATTRIBUTE (4, public, int, width);
		DYN_ATTRIBUTE (5, public, int, height);

	protected:
		void DefaultInit();
	};

	class AudioPlotProcessing : public Processing
	{
	public:
		AudioPlotProcessing();
		AudioPlotProcessing(const AudioPlotProcessingConfig& cfg);
		~AudioPlotProcessing();

		bool Do();
		bool Do(const Audio& audio);

		const char * GetClassName() const {return "AudioPlotProcessing";}

		inline const ProcessingConfig &GetConfig() const { return mConfig;}
		bool ConcreteConfigure(const ProcessingConfig& c);
		void SetPlot(VM::NetAudioPlot * plot);

	protected:
		bool ConcreteStart();
		bool ConcreteStop();

	private:
		AudioPlotProcessingConfig mConfig;
		VM::NetAudioPlot*         mPlot;
		AudioInPort               mInput;
		bool                      mOwnedPlot;

		void InitAudioPlot();
	};
}

#endif

