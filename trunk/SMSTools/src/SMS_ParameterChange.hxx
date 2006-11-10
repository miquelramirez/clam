#ifndef __SMS_PARAMETERCHANGE__
#define __SMS_PARAMETERCHANGE__

#include "SMS_Errors.hxx"
#include <string>
#include <list>

namespace SMS
{
	class ParameterChange
	{
	public:

		ParameterChange();
		ParameterChange( const ParameterChange& obj );
		ParameterChange( std::string fieldKey, std::string fieldValue );
		virtual ~ParameterChange();

		const std::string& GetFieldKey() const;
		void SetFieldKey( std::string key );
		const std::string& GetNewFieldValue() const;
		void SetNewFieldValue( std::string value );

	protected:
		std::string mFieldKey;
		std::string mNewFieldValue;
	};

	class UnexistingParameter
		: public Error
	{
	public:
		UnexistingParameter( const ParameterChange& culpritObject);
		virtual ~UnexistingParameter();
	};

	class ParameterConstraintViolated
		: public Error
	{
	public:
		ParameterConstraintViolated();
		ParameterConstraintViolated( const ParameterConstraintViolated& obj );
		virtual ~ParameterConstraintViolated();

		const std::list< std::string >& GetAffectedFieldsList() const;
		void AddAffectedField( std::string fieldName );

	protected:
		
		std::list< std::string >   mAffectedFields;
		
	};

	class MultipleParameterConstraintsViolated
		: public Error
	{
	public:
		typedef std::list< ParameterConstraintViolated > List;
		typedef std::list< ParameterConstraintViolated >::iterator ListIterator;
		typedef std::list< ParameterConstraintViolated >::const_iterator ConstListIterator;

		MultipleParameterConstraintsViolated();
		MultipleParameterConstraintsViolated( const MultipleParameterConstraintsViolated& obj );
		virtual ~MultipleParameterConstraintsViolated();


		ConstListIterator begin() const;
		ConstListIterator end() const;

		unsigned TotalConstraintsViolated() const;

		void Add( const ParameterConstraintViolated& error );

	protected:

		std::list< ParameterConstraintViolated > mViolatedConstraints;
	};

	// ParameterChange inline definitions
	inline const std::string& ParameterChange::GetFieldKey() const
	{
		return mFieldKey;
	}

	inline void ParameterChange::SetFieldKey( std::string fieldKey )
	{
		mFieldKey = fieldKey;
	}

	inline const std::string& ParameterChange::GetNewFieldValue() const
	{
		return mNewFieldValue;
	}

	inline void ParameterChange::SetNewFieldValue( std::string value )
	{
		mNewFieldValue = value;
	}

	// ParameterConstraintViolated inline definitions

	inline const std::list< std::string >& ParameterConstraintViolated::GetAffectedFieldsList() const
	{
		return mAffectedFields;
	}

	inline void ParameterConstraintViolated::AddAffectedField( std::string fieldname )
	{
		mAffectedFields.push_back( fieldname );
	}

	inline MultipleParameterConstraintsViolated::ConstListIterator MultipleParameterConstraintsViolated::begin() const
	{
		return mViolatedConstraints.begin();
	}

	inline MultipleParameterConstraintsViolated::ConstListIterator MultipleParameterConstraintsViolated::end() const
	{
		return mViolatedConstraints.end();
	}

	inline void MultipleParameterConstraintsViolated::Add( const ParameterConstraintViolated& error )
	{
		mViolatedConstraints.push_back( error );
	}

	inline unsigned MultipleParameterConstraintsViolated::TotalConstraintsViolated() const
	{
		return mViolatedConstraints.size();
	}
	
}


#endif // SMS_ParameterChange.hxx
