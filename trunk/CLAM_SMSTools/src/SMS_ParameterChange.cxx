#include "SMS_ParameterChange.hxx"
#include <algorithm>

namespace SMS
{
	ParameterChange::ParameterChange()
		: mFieldKey( "noField" ), mNewFieldValue( "none" )
	{
	}

	ParameterChange::ParameterChange( const ParameterChange& obj )
	{
		SetFieldKey( obj.GetFieldKey() );
		SetNewFieldValue( obj.GetNewFieldValue() );
	}
	

	ParameterChange::ParameterChange( std::string fieldKey,
					  std::string fieldValue )
		: mFieldKey( fieldKey ), mNewFieldValue( fieldValue )
	{
	}

	ParameterChange::~ParameterChange()
	{
	}

	UnexistingParameter::UnexistingParameter( const ParameterChange& culpritObject )
	{
		SetMessageString( "Requested " + culpritObject.GetFieldKey() + " does not exist" ); 
	}

	UnexistingParameter::~UnexistingParameter()
	{
	}

	ParameterConstraintViolated::ParameterConstraintViolated()
	{
	}

	ParameterConstraintViolated::~ParameterConstraintViolated()
	{
	}

	ParameterConstraintViolated::ParameterConstraintViolated( const ParameterConstraintViolated& obj )
		: Error( obj )
	{

		mAffectedFields = obj.mAffectedFields;
	}

	MultipleParameterConstraintsViolated::MultipleParameterConstraintsViolated()
	{
	}

	MultipleParameterConstraintsViolated::MultipleParameterConstraintsViolated( const MultipleParameterConstraintsViolated& obj )
		: Error( obj )
	{

		mViolatedConstraints = obj.mViolatedConstraints;
	}

	MultipleParameterConstraintsViolated::~MultipleParameterConstraintsViolated()
	{
	}

}
