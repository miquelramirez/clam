#include "ControlComparison.hxx"
#include "ProcessingFactory.hxx"
#include "math.h"
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
	SetConvertOps2IntegersFirst(false);
}

ControlComparison::ControlComparison()
	: _inOperator1(1, "Operator 1", this , &ControlComparison::InControlCallback )
	, _inOperator2(2, "Operator 2", this , &ControlComparison::InControlCallback )
	, _outControlBool( "Result of comparison (bool)", this )
	, _outControlFloat( "Result of comparison (float)", this )
	, _firstValueReceived(false)
{
	Configure( mConfig );	
}

ControlComparison::ControlComparison( const ControlComparisonConfig& cfg ) 
	: _inOperator1(1, "Operator 1", this , &ControlComparison::InControlCallback )
	, _inOperator2(2, "Operator 2", this , &ControlComparison::InControlCallback )
	, _outControlBool( "Result of comparison (bool)", this )
	, _outControlFloat( "Result of comparison (float)", this )
	, _firstValueReceived(false)
{ 
	Configure( cfg );
}

bool ControlComparison::ConcreteConfigure( const ProcessingConfig& cfg )
{
	CopyAsConcreteConfig( mConfig, cfg );
	_inOperator2.DoControl(mConfig.GetRightTerm());
	return true; 		
}

void ControlComparison::InControlCallback(unsigned controlId, const TControlData & value)
{

	if (controlId==1)
		_firstValueReceived=true;
	else if (controlId==2)
		if (not _firstValueReceived) return;

	TControlData op1 = _inOperator1.GetLastValue();
	TControlData op2 = _inOperator2.GetLastValue();
	bool equal = (op1 == op2);  
	if (mConfig.GetConvertOps2IntegersFirst())
		equal = (round(op1) == round(op2));
	_outControlBool.SendControl(equal);
	_outControlFloat.SendControl(equal ? 1 : 0 );
}

bool ControlComparison::Do()
{
	return true;
}
}

