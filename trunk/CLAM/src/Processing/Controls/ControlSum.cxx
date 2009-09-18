#include "ControlSum.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "ControlSum",
		"category", "Controls",
		"description", "ControlSum",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ControlSum> reg = metadata;
}

void ControlSumConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	SetRightTerm( 0.0 );
}

ControlSum::ControlSum()
	: mInOperator1( "Operator 1", this , &ControlSum::InControlCallback )
	, mInOperator2( "Operator 2", this , &ControlSum::InControlCallback )
	, mOutControl( "Sum", this )
{
	Configure( mConfig );	
}

ControlSum::ControlSum( const ControlSumConfig& cfg ) 
	: mInOperator1( "Operator 1", this , &ControlSum::InControlCallback )
	, mInOperator2( "Operator 2", this , &ControlSum::InControlCallback )
	, mOutControl( "Sum", this )
{ 
	Configure( cfg );
}

bool ControlSum::ConcreteConfigure( const ProcessingConfig& cfg )
{
	CopyAsConcreteConfig( mConfig, cfg );
	mInOperator2.DoControl(mConfig.GetRightTerm());
	return true; 		
}

void ControlSum::InControlCallback(const TControlData & value)
{
	TControlData op1 = mInOperator1.GetLastValue();
	TControlData op2 = mInOperator2.GetLastValue();
	mOutControl.SendControl(op1+op2);
}

bool ControlSum::Do()
{
	return true;
}
}

