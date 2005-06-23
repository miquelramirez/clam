#ifndef __AUDIOBUFFPLOTPROCESSING__
#define __AUDIOBUFFPLOTPROCESSING__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "AudioInPort.hxx"
#include "Audio.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetAudioBuffPlot;
	}
}

namespace CLAM
{

	class AudioBuffPlotProcessingConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (AudioBuffPlotProcessingConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, std::string, Caption);
		DYN_ATTRIBUTE (2, public, int, xpos);
		DYN_ATTRIBUTE (3, public, int, ypos);
		DYN_ATTRIBUTE (4, public, int, width);
		DYN_ATTRIBUTE (5, public, int, height);

	protected:
		void DefaultInit();
	};

	class AudioBuffPlotProcessing : public Processing
	{
	public:
		AudioBuffPlotProcessing();
		AudioBuffPlotProcessing(const AudioBuffPlotProcessingConfig& cfg);
		~AudioBuffPlotProcessing();

		bool Do();
		bool Do(const Audio& audio);

		const char * GetClassName() const {return "AudioBuffPlotProcessing";}
		
		inline const ProcessingConfig &GetConfig() const { return mConfig;}
		bool ConcreteConfigure(const ProcessingConfig& c);
		void SetPlot(VM::NetAudioBuffPlot * plot);

	protected:
		bool ConcreteStart();
		bool ConcreteStop();

	private:
		AudioBuffPlotProcessingConfig mConfig;
		VM::NetAudioBuffPlot*         mPlot;
		AudioInPort                   mInput;
		bool                          mOwnedPlot;

		void InitAudioBuffPlot();
	};
}

#endif

