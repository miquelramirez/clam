#ifndef __SPECGRAMPLOTPROCESSING__
#define __SPECGRAMPLOTPROCESSING__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "InPort.hxx"
#include "Spectrum.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetSpecgramPlot;
	}
}

namespace CLAM
{

	class SpecgramPlotProcessingConfig : public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (SpecgramPlotProcessingConfig, 6, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, std::string, Caption);
		DYN_ATTRIBUTE (2, public, int, xpos);
		DYN_ATTRIBUTE (3, public, int, ypos);
		DYN_ATTRIBUTE (4, public, int, width);
		DYN_ATTRIBUTE (5, public, int, height);

	protected:
		void DefaultInit();
	};

	class SpecgramPlotProcessing : public Processing
	{
	public:
		SpecgramPlotProcessing();
		SpecgramPlotProcessing(const SpecgramPlotProcessingConfig& cfg);
		~SpecgramPlotProcessing();

		bool Do();
		bool Do(const Spectrum& spec);

		const char * GetClassName() const {return "SpecgramPlotProcessing";}

		inline const ProcessingConfig &GetConfig() const { return mConfig;}
		bool ConcreteConfigure(const ProcessingConfig& c);
		void SetPlot(VM::NetSpecgramPlot * plot);

	protected:
		bool ConcreteStart();
		bool ConcreteStop();

	private:
		SpecgramPlotProcessingConfig mConfig;
		VM::NetSpecgramPlot*         mPlot;
		InPort<Spectrum>             mInput;
		bool                         mOwnedPlot;

		void InitSpecgramPlot();
	};
}

#endif

