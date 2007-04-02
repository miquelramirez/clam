#ifndef __FUNDPLOTPROCESSING__
#define __FUNDPLOTPROCESSING__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InPort.hxx"
#include "Fundamental.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetFundPlot;
	}
}

namespace CLAM
{

	class FundPlotProcessingConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (FundPlotProcessingConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, std::string, Caption);
		DYN_ATTRIBUTE (2, public, int, xpos);
		DYN_ATTRIBUTE (3, public, int, ypos);
		DYN_ATTRIBUTE (4, public, int, width);
		DYN_ATTRIBUTE (5, public, int, height);

	protected:
		void DefaultInit();
	};

	class FundPlotProcessing : public Processing
	{
	public:
		FundPlotProcessing();
		FundPlotProcessing(const FundPlotProcessingConfig& cfg);
		~FundPlotProcessing();

		bool Do();
		bool Do(const Fundamental& fund);
		
		const char * GetClassName() const {return "FundPlotProcessing";}

		inline const ProcessingConfig &GetConfig() const { return mConfig;}
		bool ConcreteConfigure(const ProcessingConfig& c);
		void SetPlot(VM::NetFundPlot * plot);

	protected:
		bool ConcreteStart();
		bool ConcreteStop();

	private:
		FundPlotProcessingConfig mConfig;
		VM::NetFundPlot*         mPlot;
		InPort<Fundamental>      mInput;
		bool                     mOwnedPlot;

		void InitFundPlot();
	};
}

#endif

