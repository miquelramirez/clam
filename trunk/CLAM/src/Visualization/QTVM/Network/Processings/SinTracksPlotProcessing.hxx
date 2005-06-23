#ifndef __SINTRACKSPLOTPROCESSING__
#define __SINTRACKSPLOTPROCESSING__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InPort.hxx"
#include "SpectralPeakArray.hxx"

namespace CLAM
{
    namespace VM
    {
	class NetSinTracksPlot;
    }
}

namespace CLAM
{
    class SinTracksPlotProcessingConfig : public ProcessingConfig
    {
    public:
		DYNAMIC_TYPE_USING_INTERFACE (SinTracksPlotProcessingConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, std::string, Caption);
		DYN_ATTRIBUTE (2, public, int, xpos);
		DYN_ATTRIBUTE (3, public, int, ypos);
		DYN_ATTRIBUTE (4, public, int, width);
		DYN_ATTRIBUTE (5, public, int, height);

    protected:
		void DefaultInit();
    };

    class SinTracksPlotProcessing : public Processing
    {
    public:
		SinTracksPlotProcessing();
		SinTracksPlotProcessing(const SinTracksPlotProcessingConfig& cfg);
		~SinTracksPlotProcessing();

		bool Do();
		bool Do(const SpectralPeakArray& peaks);

		const char * GetClassName() const {return "SinTracksPlotProcessing";}

		inline const ProcessingConfig &GetConfig() const { return mConfig;}
		bool ConcreteConfigure(const ProcessingConfig& c);
		void SetPlot(VM::NetSinTracksPlot * plot);

    protected:
		bool ConcreteStart();
		bool ConcreteStop();

    private:
		SinTracksPlotProcessingConfig mConfig;
		VM::NetSinTracksPlot*         mPlot;
		InPort<SpectralPeakArray>     mInput;
		bool                          mOwnedPlot;

		void InitSinTracksPlot();
    };
}

#endif

