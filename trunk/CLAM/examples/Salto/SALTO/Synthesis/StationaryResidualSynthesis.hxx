#ifndef _StationaryResidualSynthesis_
#define _StationaryResidualSynthesis_

#include "Processing.hxx"
#include "InControl.hxx"
#include "CSaltoSynthFrame.hxx"
#include "Frame.hxx"

using namespace CLAM;

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

		virtual const char* GetClassName()
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

		bool ConcreteConfigure( const ProcessingConfig& cfg ) throw ( std::bad_cast );

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