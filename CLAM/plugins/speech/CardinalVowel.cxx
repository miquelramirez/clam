#include "CardinalVowel.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "CardinalVowel",
		"category", "Speech (Experimental - GSoC)",
		"description", "Cardinal Vowel",
		0
	};
	static FactoryRegistrator<ProcessingFactory, CardinalVowel> reg=metadata ;
}

void CardinalVowelConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	SetAmount( 1.0 );
}

CardinalVowel::CardinalVowel()
	: mVowelControl( "Vowel Position", this )
	, mStepControl( "Number of Steps", this )
	, mF1Control( "F1 Out", this )
	, mF2Control( "F2 Out", this )
{
	Configure( mConfig );	
}

CardinalVowel::CardinalVowel( const CardinalVowelConfig& cfg ) 
	: mVowelControl( "Vowel Position", this )
	, mStepControl( "Number of Steps", this )
	, mF1Control( "F1 Out", this )
	, mF2Control( "F2 Out", this )
{ 
	Configure( cfg );
}

bool CardinalVowel::ConcreteConfigure( const ProcessingConfig& cfg )
{
	CopyAsConcreteConfig( mConfig, cfg );
	mStepControl.DoControl(mConfig.GetAmount());
	return true; 		
}

bool CardinalVowel::Do()
{
	mF1Control.SendControl(CalcF1() );
	mF2Control.SendControl(CalcF2() );
	return true;
}

//function to approximate f1 through the cardinal vowel series
float CardinalVowel::CalcF1() 
{
	TControlData vowelLocation = mVowelControl.GetLastValue();
	TControlData numSteps = mStepControl.GetLastValue();
	//piecewise function: f1 rises from IY to AE, falls from AA to UW
	if(vowelLocation < .5)
	{
		return 200 + 539*vowelLocation;
	}
	else
	{
		return 715 - 462*vowelLocation;
	}
}

//function to approximate f2 through the cardinal vowel series
float CardinalVowel::CalcF2() 
{
	TControlData vowelLocation = mVowelControl.GetLastValue();
	TControlData numSteps = mStepControl.GetLastValue();
	return 2400 - 1679*vowelLocation;
}
	

}

