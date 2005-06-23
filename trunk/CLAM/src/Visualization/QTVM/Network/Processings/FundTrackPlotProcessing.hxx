#ifndef __FUNDTRACKPLOTPROCESSING__
#define __FUNDTRACKPLOTPROCESSING__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InPort.hxx"
#include "Fundamental.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetFundTrackPlot;
	}
}

namespace CLAM
{

	class FundTrackPlotProcessingConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (FundTrackPlotProcessingConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, std::string, Caption);
		DYN_ATTRIBUTE (2, public, int, xpos);
		DYN_ATTRIBUTE (3, public, int, ypos);
		DYN_ATTRIBUTE (4, public, int, width);
		DYN_ATTRIBUTE (5, public, int, height);

	protected:
		void DefaultInit();
	};

	class FundTrackPlotProcessing : public Processing
	{
	public:
		FundTrackPlotProcessing();
		FundTrackPlotProcessing(const FundTrackPlotProcessingConfig& cfg);
		~FundTrackPlotProcessing();

		bool Do();
		bool Do(const Fundamental& fund);

		const char * GetClassName() const {return "FundTrackPlotProcessing";}

		inline const ProcessingConfig &GetConfig() const { return mConfig;}
		bool ConcreteConfigure(const ProcessingConfig& c);
		void SetPlot(VM::NetFundTrackPlot * plot);

	protected:
		bool ConcreteStart();
		bool ConcreteStop();

	private:
		FundTrackPlotProcessingConfig mConfig;
		VM::NetFundTrackPlot*         mPlot;
		InPort<Fundamental>           mInput;
		bool                          mOwnedPlot;

		void InitFundTrackPlot();
	};
}

#endif

