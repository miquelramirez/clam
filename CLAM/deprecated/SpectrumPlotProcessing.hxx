#ifndef __SPECTRUMPLOTPROCESSING__
#define __SPECTRUMPLOTPROCESSING__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InPort.hxx"
#include "Spectrum.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetSpectrumPlot;
	}
}

namespace CLAM
{

	class SpectrumPlotProcessingConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpectrumPlotProcessingConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, std::string, Caption);
		DYN_ATTRIBUTE (2, public, int, xpos);
		DYN_ATTRIBUTE (3, public, int, ypos);
		DYN_ATTRIBUTE (4, public, int, width);
		DYN_ATTRIBUTE (5, public, int, height);

	protected:
		void DefaultInit();
	};

	class SpectrumPlotProcessing : public Processing
	{
	public:
		SpectrumPlotProcessing();
		SpectrumPlotProcessing(const SpectrumPlotProcessingConfig& cfg);
		~SpectrumPlotProcessing();

		bool Do();
		bool Do(const Spectrum& spec);
		
		const char * GetClassName() const {return "SpectrumPlotProcessing";}

		inline const ProcessingConfig &GetConfig() const { return mConfig;}
		bool ConcreteConfigure(const ProcessingConfig& c);
		void SetPlot(VM::NetSpectrumPlot * plot);

	protected:
		bool ConcreteStart();
		bool ConcreteStop();

	private:
		SpectrumPlotProcessingConfig mConfig;
		VM::NetSpectrumPlot*         mPlot;
		InPort<Spectrum>             mInput;
		bool                         mOwnedPlot;

		void InitSpectrumPlot();
	};
}

#endif

