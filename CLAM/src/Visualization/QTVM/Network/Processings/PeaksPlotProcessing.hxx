#ifndef __PEAKSPLOTPROCESSING__
#define __PEAKSPLOTPROCESSING__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InPort.hxx"
#include "SpectralPeakArray.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetPeaksPlot;
	}
}

namespace CLAM
{

	class PeaksPlotProcessingConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (PeaksPlotProcessingConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, std::string, Caption);
		DYN_ATTRIBUTE (2, public, int, xpos);
		DYN_ATTRIBUTE (3, public, int, ypos);
		DYN_ATTRIBUTE (4, public, int, width);
		DYN_ATTRIBUTE (5, public, int, height);

	protected:
		void DefaultInit();
	};

	class PeaksPlotProcessing : public Processing
	{
	public:
		PeaksPlotProcessing();
		PeaksPlotProcessing(const PeaksPlotProcessingConfig& cfg);
		~PeaksPlotProcessing();

		bool Do();
		bool Do(const SpectralPeakArray& peaks);

		const char * GetClassName() const {return "PeaksPlotProcessing";}

		inline const ProcessingConfig &GetConfig() const { return mConfig;}
		bool ConcreteConfigure(const ProcessingConfig& c);
		void SetPlot(VM::NetPeaksPlot * plot);

	protected:
		bool ConcreteStart();
		bool ConcreteStop();

	private:
		PeaksPlotProcessingConfig mConfig;
		VM::NetPeaksPlot*         mPlot;
		InPort<SpectralPeakArray> mInput;
		bool                      mOwnedPlot;

		void InitPeaksPlot();
	};
}

#endif

