#include "ControlComparison.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "ControlComparison",
		"category", "Controls",
		"description", "ControlComparison",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ControlComparison> reg = metadata;
}

void ControlComparisonConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	SetRightTerm( 0.0 );
}

ControlComparison::ControlComparison()
	: _inOperator1( "Operator 1", this , &ControlComparison::InControlCallback )
	, _inOperator2( "Operator 2", this , &ControlComparison::InControlCallback )
	, _outControlBool( "Result of comparison (bool)", this )
	, _outControlFloat( "Result of comparison (float)", this )
{
	Configure( mConfig );	
}

ControlComparison::ControlComparison( const ControlComparisonConfig& cfg ) 
	: _inOperator1( "Operator 1", this , &ControlComparison::InControlCallback )
	, _inOperator2( "Operator 2", this , &ControlComparison::InControlCallback )
	, _outControlBool( "Result of comparison (bool)", this )
	, _outControlFloat( "Result of comparison (float)", this )
{ 
	Configure( cfg );
}

bool ControlComparison::ConcreteConfigure( const ProcessingConfig& cfg )
{
	CopyAsConcreteConfig( mConfig, cfg );
	_inOperator2.DoControl(mConfig.GetRightTerm());
	return true; 		
}

void ControlComparison::InControlCallback(const TControlData & value)
{
	TControlData op1 = _inOperator1.GetLastValue();
	TControlData op2 = _inOperator2.GetLastValue();
	bool equal = (op1 == op2);
	_outControlBool.SendControl(equal);
	_outControlFloat.SendControl(equal ? 1 : 0 );
}

bool ControlComparison::Do()
{
	return true;
}
}

