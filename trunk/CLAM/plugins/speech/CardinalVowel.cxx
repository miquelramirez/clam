#include "CardinalVowel.hxx"
#include <CLAM/ProcessingFactory.hxx>

namespace CLAM
{
namespace Hidden
{
	static const char* metadata[] = {
		"key", "CardinalVowel",
		"category", "[plugin] Speech (experimental, GSoC)",
		"description", "Cardinal Vowel Formant Control Sender",
		"help", "Computes formant frequencies given f1 and f2 from the curve defined by the cardinal vowels for setting the controls of the vowel resonator.",
		0
	};
	static FactoryRegistrator<ProcessingFactory, CardinalVowel> reg=metadata ;
}

void CardinalVowel::Config::DefaultInit()
{
	AddAll();
	UpdateData();
	SetAmount( 1.0 );
}

CardinalVowel::CardinalVowel( const Config& cfg ) 
	: mVowelControl( "Vowel Position", this )
	, mStepControl( "Number of Steps", this )
	, mF1Control( "F1 Out", this )
	, mF2Control( "F2 Out", this )
	, mF3Control( "F3 Out", this )
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
	mF3Control.SendControl(CalcF3() );
	return true;
}

//function to approximate f1 through the cardinal vowel series
float CardinalVowel::CalcF1() 
{
	TControlData vowelLocation = mVowelControl.GetLastValue();
	//piecewise function: f1 rises from IY to AE, falls from AA to UW
	if(vowelLocation < .5)
		return 200 + 539*vowelLocation;
	else
		return 715 - 462*vowelLocation;
}

//function to approximate f2 through the cardinal vowel series
float CardinalVowel::CalcF2() 
{
	TControlData vowelLocation = mVowelControl.GetLastValue();
	return 2400 - 1679*vowelLocation;
}

//function to approximate f3 through the cardinal vowel series
float CardinalVowel::CalcF3() 
{
	TControlData vowelLocation = mVowelControl.GetLastValue();
	return 3300 - 900*vowelLocation;
}
	

}

