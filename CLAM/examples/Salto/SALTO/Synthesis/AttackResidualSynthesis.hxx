#ifndef _AttackResidualSynthesis_
#define _AttackResidualSynthesis_

#include "Processing.hxx"
#include "CSaltoSynthFrame.hxx"
#include "InControl.hxx"

using namespace CLAM;

namespace SALTO
{
	class AttackResidualSynthesisConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE( AttackResidualSynthesisConfig, 1, ProcessingConfig );
		DYN_ATTRIBUTE( 0, public, std::string, Name );
	protected:
		void DefaultInit();
	};

	class AttackResidualSynthesis: public Processing
	{
	public:

		AttackResidualSynthesis();

		AttackResidualSynthesis( AttackResidualSynthesisConfig& cfg );

		virtual ~AttackResidualSynthesis();

		virtual const char* GetClassName()
		{
			return "SALTO::AttackResidualSynthesis";
		}

		virtual bool Do();

		bool Do( CSaltoSynthFrame& synthesisFrame );

		void DoAttackResidualSynthesis( CSaltoSynthFrame& synthFrame, TData attackResGain );

	protected:

		bool ConcreteStart();

		bool ConcreteStop();

		bool ConcreteConfigure( const ProcessingConfig& cfg ) throw( std::bad_cast );

		int AttackResGainCB( TControlData value );

	private:

		AttackResidualSynthesisConfig				mConfig;
	
		TData										mAttackResGain;

		InControlTmpl< AttackResidualSynthesis >	mInAttackResGain;
	};

};

#endif