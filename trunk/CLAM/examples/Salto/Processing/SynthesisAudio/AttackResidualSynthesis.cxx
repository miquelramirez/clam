#include "AttackResidualSynthesis.hxx"

namespace SALTO
{
	void AttackResidualSynthesisConfig::DefaultInit()
	{
		AddAll();
		UpdateData();

		SetName( "Attack Residual Processor" );
	}

	AttackResidualSynthesis::AttackResidualSynthesis()
		: mAttackResGain( 0 ),
		  mInAttackResGain( "Attack Residual Gain Control", this, &AttackResidualSynthesis::AttackResGainCB )
	{
		AttackResidualSynthesisConfig cfg;

		Configure( cfg );
	}

	AttackResidualSynthesis::AttackResidualSynthesis( AttackResidualSynthesisConfig& cfg )
		: mAttackResGain( 0 ),
		  mInAttackResGain( "Attack Residual Gain Control", this, &AttackResidualSynthesis::AttackResGainCB )
	{
		Configure( cfg );
	}

	AttackResidualSynthesis::~AttackResidualSynthesis()
	{
	}

	bool AttackResidualSynthesis::ConcreteStart()
	{
		return true;
	}

	bool AttackResidualSynthesis::ConcreteStop()
	{
		return true;
	}

	bool AttackResidualSynthesis::ConcreteConfigure( const ProcessingConfig& c )
	{
		CopyAsConcreteConfig(mConfig, c);

		return true;
	}

	bool AttackResidualSynthesis::Do()
	{
		return true;
	}

	int AttackResidualSynthesis::AttackResGainCB( TControlData value )
	{
		mAttackResGain = value;

		return 0 ;
	}

	void AttackResidualSynthesis::DoAttackResidualSynthesis( CSaltoSynthFrame& synthFrame, TData attackResGain )
	{
		Spectrum *pCurrSpec = synthFrame.GetSpectrumPtr();
		Spectrum *pCurrResidual = synthFrame.GetResidualPtr();
		TSize	specSize = pCurrSpec->GetSize();
		
		if (pCurrResidual!=NULL&&pCurrResidual->GetSize()==pCurrSpec->GetSize())  // only if theres a attack residual !
		{
			if ( specSize != 0)
			{
				Complex* pSpecCmplx = pCurrSpec->GetComplexArray().GetPtr();
				Complex* pResCmplx = pCurrResidual->GetComplexArray().GetPtr();

				do
				{
					(*pSpecCmplx++)+=(*pResCmplx++)*attackResGain;
				}
				while ( --specSize );

  				//for (int i=0;i<specSize;i++)
  				//{
	//  		  pCurrSpec->Set(i,pCurrSpec->GetComplex(i)+pCurrResidual->GetComplex(i)*attackResGain);
 				// Buena: pCurrSpec->GetComplexArray()[i] = pCurrSpec->GetComplexArray()[i]+(pCurrResidual->GetComplexArray()[i])*attackResGain;
  				//}
			}
		}
	}


	bool AttackResidualSynthesis::Do( CSaltoSynthFrame& synthFrame )
	{
		Spectrum *pCurrSpec = synthFrame.GetSpectrumPtr();
		Spectrum *pCurrResidual = synthFrame.GetResidualPtr();
		TSize	specSize = pCurrSpec->GetSize();
		
		if (pCurrResidual!=NULL&&pCurrResidual->GetSize()==pCurrSpec->GetSize())  // only if theres a attack residual !
		{
			if ( specSize != 0)
			{
				Complex* pSpecCmplx = pCurrSpec->GetComplexArray().GetPtr();
				Complex* pResCmplx = pCurrResidual->GetComplexArray().GetPtr();

				do
				{
					(*pSpecCmplx++)+=(*pResCmplx++)*mAttackResGain;
				}
				while ( --specSize );

  				//for (int i=0;i<specSize;i++)
  				//{
	//  		  pCurrSpec->Set(i,pCurrSpec->GetComplex(i)+pCurrResidual->GetComplex(i)*attackResGain);
 				// Buena: pCurrSpec->GetComplexArray()[i] = pCurrSpec->GetComplexArray()[i]+(pCurrResidual->GetComplexArray()[i])*attackResGain;
  				//}
			}
		}
		
		return true;
	}


};
