#ifndef _StationaryResidualSynthesis_
#define _StationaryResidualSynthesis_

#include "Processing.hxx"
#include "InControl.hxx"
#include "CSaltoSynthFrame.hxx"
#include "Frame.hxx"

using CLAM::Processing;
using CLAM::ProcessingConfig;
using CLAM::DynamicType;
using CLAM::InControlTmpl;
using CLAM::Frame;
using CLAM::TData;
using CLAM::TControlData;
using CLAM::Spectrum;
using CLAM::Complex;
using CLAM::TSize;

using CLAM::CSaltoSynthFrame;

namespace SALTO
{

	class StationaryResidualSynthesisConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( StationaryResidualSynthesisConfig, 1, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, std::string, Name );
	protected:
		void DefaultInit();
	};

	class StationaryResidualSynthesis: public Processing
	{
	public:

		StationaryResidualSynthesis();

		StationaryResidualSynthesis( StationaryResidualSynthesisConfig& cfg );

		virtual ~StationaryResidualSynthesis();

		const ProcessingConfig &GetConfig() const { return mConfig; }

		const char* GetClassName() const
		{
			return "SALTO::StationaryResidualSynthesis";
		}

		virtual bool Do();

		bool Do( CSaltoSynthFrame& synthesisFrame, Frame &pStatResFrame );

		void DoStationaryResidualSynthesis(CSaltoSynthFrame  &synthFrame,
	                              Frame *pStatResFrame,
								  TData            statResGain,
								  TData            resonanceFreq);

	protected:

		bool ConcreteStart();

		bool ConcreteStop();

		bool ConcreteConfigure( const ProcessingConfig& c );

		int StatResGainCB( TControlData value );

		int ResonanceFreqCB( TControlData value );

	private:

		StationaryResidualSynthesisConfig				mConfig;
		TData											mStatResGain;
		TData											mResonanceFreq;
		InControlTmpl< StationaryResidualSynthesis >	mInStatResGain;
		InControlTmpl< StationaryResidualSynthesis >	mInResonanceFreq;
	};
};

#endif
