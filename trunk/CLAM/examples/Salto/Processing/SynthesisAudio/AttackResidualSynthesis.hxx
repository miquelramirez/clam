#ifndef _AttackResidualSynthesis_
#define _AttackResidualSynthesis_

#include "Processing.hxx"
#include "CSaltoSynthFrame.hxx"
#include "InControl.hxx"

namespace SALTO
{

	using CLAM::Processing;
	using CLAM::ProcessingConfig;
	using CLAM::InControlTmpl;
	using CLAM::DynamicType;
	using CLAM::TControlData;
	using CLAM::TData;
	using CLAM::Complex;
	using CLAM::Spectrum;
	using CLAM::TSize;

	using CLAM::CSaltoSynthFrame;



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

		const ProcessingConfig &GetConfig() const { return mConfig; }

		const char* GetClassName() const
		{
			return "SALTO::AttackResidualSynthesis";
		}

		virtual bool Do();

		bool Do( CSaltoSynthFrame& synthesisFrame );

		void DoAttackResidualSynthesis( CSaltoSynthFrame& synthFrame, TData attackResGain );

	protected:

		bool ConcreteStart();

		bool ConcreteStop();

		bool ConcreteConfigure( const ProcessingConfig& c );

		int AttackResGainCB( TControlData value );

	private:

		AttackResidualSynthesisConfig				mConfig;
	
		TData										mAttackResGain;

		InControlTmpl< AttackResidualSynthesis >	mInAttackResGain;
	};

};

#endif
