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
			DYNAMIC_TYPE_USING_INTERFACE (SpectrumPlotProcessingConfig, 1, ProcessingConfig);
			DYN_ATTRIBUTE (0, public, std::string, Name);

		protected:
			void DefaultInit();
	};


	class SpectrumPlotProcessing : public Processing
	{
		public:
			SpectrumPlotProcessing();
			SpectrumPlotProcessing(const SpectrumPlotProcessingConfig& cfg);
			virtual ~SpectrumPlotProcessing();

			bool Do();
			bool Do(const Spectrum& spec);

			const char * GetClassName() const {return "SpectrumPlotProcessing";}
	
			inline const ProcessingConfig &GetConfig() const { return mConfig;}
			bool ConcreteConfigure(const ProcessingConfig& c);

		protected:
			bool ConcreteStart();
			bool ConcreteStop();

		private:
			SpectrumPlotProcessingConfig mConfig;
			VM::NetSpectrumPlot* mPlot;
			InPort<Spectrum> mInput;
			
			void InitSpectrumPlot();
	};
}

#endif

