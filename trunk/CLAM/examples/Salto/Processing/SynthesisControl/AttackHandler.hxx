#ifndef _AttackHandler_
#define _AttackHandler_

#include "StationaryHandler.hxx"

namespace SALTO
{
	class AttackHandler: public StationaryHandler
	{
	public:

		AttackHandler();

		AttackHandler( StationaryHandlerConfig& cfg );

		virtual ~AttackHandler();

		const ProcessingConfig &GetConfig() const { return mConfig; }

		const char* GetClassName() const
		{
			return "SALTO::AttackHandler";
		}

		virtual bool Do();

		bool Do( CSaltoSynthFrame  &pSynthFrame,
				   Frame			 *pCurrSpectralFrameBase,										
				   SynthesisState    &synthState,
				   Frame			 *pCurrSpectralFrameResidual,
				   SpectralPeakArray *pCurrPeakArrayTarget);

	protected:

		bool ConcreteStart();

		bool ConcreteStop();

		bool ConcreteConfigure( const ProcessingConfig& c );

		void InitInterpolatingSynthesis( SynthesisState &synthState );

	private:

		OutControl			mOutResetSineSynth;
	
	};
}

#endif
