/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _ENUM_H_
#define _ENUM_H_
#include <exception>
#include <iosfwd>
#include <string>
#include "Assert.hxx"
#include "Component.hxx"

namespace CLAM {

// TODO: Integrate this excepcion class with CLAM exception standards
class IllegalValue : public std::exception {
	public:
		IllegalValue(const IllegalValue & e): msg(e.msg) {};
		IllegalValue(const std::string & s) : msg(s) {};
		virtual ~IllegalValue() throw() {};
		std::string msg;
};


/**
 * Instances of this class represents objects that can adquire 
 * a symbolic value from a limited set of symbolic values.
 *
 * Enum provides more features that the C \c enum with a
 * very little overhead:
 *
 * - Run-time symbolic representation
 * - Assignment to both simbolic and numeric values
 * - Run-time checking of values on debug mode
 * - Formated input and output to an std::stream as symbol
 * - Implements the Component interface
 * 
 * The way to define an enumerated type is by subclassing 
 * Enum.
 *
 * The subclass MUST redefine its constructors by 
 * providing the Enum constructor an array of tEnumValue's,
 * which defines the mapping between numeric and symbolic 
 * values, and an initialization value, than can be both a 
 * symbol (char* or std::string) or an integer.
 * 
@code
// EMyEnum.hxx
class EMyEnum : public CLAM::Enum {
public:
	EMyEnum() : CLAM::Enum(ValueTable(), eTwo) {}
	EMyEnum(tValue v) : CLAM::Enum(ValueTable(), v) {};
	EMyEnum(const std::string & s) : CLAM::Enum(ValueTable(), s) {};

	virtual CLAM::Component * Species() const {return new EMyEnum();}

	typedef enum {
		eZero=0,
		eTwo=2,
		eHundred=100
	} tEnum;
	static tEnumValue * ValueTable()
	{
		static tEnumValue sValueTable[] = 
		{
			{eZero,"zero"},
			{eTwo,"two"},
			{eHundred,"hundred"},
			{0,NULL}
		};
		return sValueTable;
	}
};
@endcode
 */
class Enum : public Component {

// Internal Types
public:
	typedef int tValue;
	typedef struct {tValue value; char*name;} tEnumValue;

// Attributes
private:
	const tEnumValue * mEnumValues;
	tValue mValue;

/** @name Construction/Destruction */
//@{

protected:
	/**
	 * Construction with a numeric value.
	 * @param values An array of tEnumValue structures in wich the last
	 * one has a NULL pointer as name.
	 * @param value An initialization numeric value.
	 * @throws A IllegalValue exception when the value is not valid.
	 */
	Enum (const tEnumValue * values, const tValue & value) /* throw IllegalValue */ {
		mEnumValues = values;
		SetValue(value);
	}
	/**
	 * Construction with a symbolic value.
	 * @param values An array of tEnumValue structures in wich the last
	 * one has a NULL pointer as name.
	 * @param value An initialization symbolic value.
	 * @throws A IllegalValue exception when the value is not valid.
	 */
	Enum (const tEnumValue * values, const std::string & value) /* throw IllegalValue */ {
		mEnumValues = values;
		SetValue(value);
	};
public:
	/** The required virtual destructor */
	virtual ~Enum ();
	const char * GetClassName() const {return NULL;}
//@}

// Operations
public:
	/**
	 * Returns the symbol map for the enum
	 */
	const tEnumValue * GetSymbolMap() {
		return mEnumValues;
	}

	/**
	 * Changes the value.
	 * @param v The new numeric value
	 */
	void SetValue(const tValue v) {
		CLAM_BEGIN_DEBUG_CHECK
			for (int i = 0; mEnumValues[i].name; i++) {
				if (v==mEnumValues[i].value) {
					mValue = v;
					return;
				}
			}
			CLAM_ASSERT(false, "Invalid value for an Enum");
		CLAM_END_DEBUG_CHECK
		mValue = v; 
	}
	/**
	 * Changes the value safely. 
	 * That is it checks the value is ok for the enum and throws a
	 * catchable exception if not.
	 * @param v The new numeric value
	 * @throws IllegalValue when the value is not valid for the enum
	 * @todo Fill IllegalValue with useful information to recover 
	 * instead a insightfull string.
	 */
	void SetValueSafely(const tValue v) throw (IllegalValue) {
		for (int i = 0; mEnumValues[i].name; i++) {
			if (v==mEnumValues[i].value) {
				mValue = v;
				return;
			}
		}
		throw IllegalValue("Invalid value for an Enum");
	}
	/**
	 * Changes the value.
	 * @param s The new symbolic value
	 */
	void SetValue(const std::string & s) 
	{
		for (int i = 0; mEnumValues[i].name; i++) {
			if (s.compare(mEnumValues[i].name)==0) {
				mValue = mEnumValues[i].value;
				return;
			}
		}
		CLAM_ASSERT(false, "Illegal literal for an Enum");
	}
	/*
	 * Changes the value safely. 
	 * That is it checks the value is ok for the enum and throws a
	 * catchable exception if not.
	 * @param s The new symbolic value
	 * @throws IllegalValue when the value is not valid for the enum
	 * @todo Fill IllegalValue with useful information to recover 
	 * instead a insightfull string.
	 */
	void SetValueSafely(const std::string & s) throw (IllegalValue) 
	{
		for (int i = 0; mEnumValues[i].name; i++) {
			if (s.compare(mEnumValues[i].name)==0) {
				mValue = mEnumValues[i].value;
				return;
			}
		}
		throw IllegalValue("Illegal literal for an Enum.");
	}

	/**
	 * Returns the numeric value.
	 * @returns The numeric value
	 */
	tValue GetValue() const {
		return mValue;
	}

	/**
	 * Returns the symbolic value.
	 * @returns The symbolic value
	 */
	std::string GetString() const throw (IllegalValue)
	{
		for (int i = 0; mEnumValues[i].name; i++) {
			if (mValue==mEnumValues[i].value) 
				return mEnumValues[i].name;
		}
		CLAM_ASSERT(false, "Illegal numeric value for an Enum");
		return "IllegalValue";
	}

	Enum & operator = (const tValue & v) throw (IllegalValue) {
		SetValue(v);
		return *this;
	}

	Enum & operator = (const std::string & v) throw (IllegalValue) {
		SetValue(v);
		return *this;
	}

	Enum & operator = (const Enum & v) throw (IllegalValue) {
		SetValue(tValue(v));
		return *this;
	}

	/**
	 * Conversion operiation as a number.
	 * @returns The numeric value
	 */
	operator tValue() const {
		return mValue;
	}

// Component Protocol
public:
	/**
	 * Returns a new object of the same class than the receiver
	 * object. To be reimplemented by subclasses.
	 * @returns a new allocated component. The pointer belongs
	 * to the caller.
	 */
	virtual Component * Species () const = 0;

	/**
	 * TODO: Copy documentation for this method from Component 
	 */
	virtual Component * DeepCopy () const {
		Enum * pe = (Enum*)Species();
		pe->SetValue(mValue);
		return pe;
	}

	/**
	 * TODO: Copy documentation for this method from Component 
	 */
	virtual Component * ShallowCopy () const {
		Enum * pe = (Enum*)Species();
		pe->SetValue(mValue);
		return pe;
	}

	/** 
	 * Stores component's subitems on the given Storage
	 * @param storage The given storage where the subitem will be stored
	 * @see Storage
	 * TODO: This method can throw and IllegalValue exception
	 */

	virtual void StoreOn (Storage & storage) const;

	/** 
	 * Loads component's subitems from the given Storage
	 * @param storage The given storage where the subitem will be stored
	 * @see Storage
	 * TODO: This method can throw and IllegalValue exception
	 */

	virtual void LoadFrom (Storage & storage);


};

/**
 * Stores the symbolic value of an Enum to the output stream.
 * @param os The output stream
 * @param e The Enum
 * @returns The output stream
 */
std::ostream & operator << (std::ostream & os, const Enum & e) throw (IllegalValue);

/**
 * Loads a symbolic value from the input stream onto an Enum.
 * @param os The input stream
 * @param e The Enum
 * @returns The input stream
 */
std::istream & operator >> (std::istream & os, Enum & e) throw (IllegalValue);

}
#endif // _ENUM_H_

