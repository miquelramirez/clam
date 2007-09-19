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

#ifndef _Flags_hxx_
#define _Flags_hxx_

#include <bitset>
#include <string>
#include <iosfwd>
#include "Component.hxx"
// TODO: This is provisional to share the IllegalValue exception
#include "Enum.hxx"

namespace CLAM {


/**
* Abstract class from which any Flag<N> instantiation is derived.
* It provides some clever pure virtual functions and shared
* implementations for the symbol managing.
*/
class FlagsBase : public Component {
// Internal Types
public:
	/** The type of the positional indexes */
	typedef unsigned int tValue;
	/** The type of the name table rows */
	typedef struct {tValue value; const char*name;} tFlagValue;

// Attributes
protected:
	/**
	* A pointer to the user specific flag class table of
	* value-symbol pairs for the flag.
	*/
	const tFlagValue * mFlagValues;

// Operations
public:
	/**
	 * @returns the number of flags contained
	 */
	virtual unsigned int GetNFlags() const=0;
	virtual void SetFlag(unsigned int whichOne, bool value)=0;
	virtual bool IsSetFlag(unsigned int whichOne) const =0;
	/**
	* Retrieves the name of the flag at a position
	* @param whichOne The position of the selected flag
	* @returns The symbolic name of the selected flag as stream
	*/
	std::string GetFlagString(unsigned int whichOne) const throw (IllegalValue);

	/**
	* Retrieves the position of the named flag
	* @param whichOne The string containing the simbolic name
	* @returns The symbolic name of the selected flag as stream
	*/
	unsigned int GetFlagPosition(const std::string & whichOne) const throw (IllegalValue);

	/*
	 * Stores component's subitems on the given Storage
	 * @param store The given storage where the subitem will be stored
	 * @see Storage
	 * @todo TODO: This method can throw and IllegalValue exception
	 */
	virtual void StoreOn (Storage & storage) const;

	/*
	 * Loads component's subitems from the given Storage
	 * @param store The given storage where the subitem will be stored
	 * @see Storage
	 * @todo TODO: This method can throw and IllegalValue exception
	 */
	virtual void LoadFrom (Storage & storage);

// Debug
public:
	/**
	* Checks that the object is internally consistent.
	* It tests that: 
	* - the names array is not a NULL pointer (it still could be invalid)
	* - the names array is shorter than the number of flags plus one for the NULL name
	* - a NULL name is present at the end of the names array
	* - the names array has no value replication
	* - the names array has no name replication
	* @todo TODO: Check blanks in names
	*/
	inline bool CheckInvariant();
};

std::istream & operator >> (std::istream & is, FlagsBase & f);
std::ostream & operator << (std::ostream & os, const FlagsBase & f);

/**
* Instances of this class represents objects containing a set
* of booleans values (flags) that can be accessed by their
* symbolic name.
* 
* Because Flags derives from std::bitset<N> it provides all the
* standard operations like bitwise operators and so.
* This interface has been fattened to provide symbolic
* representation (name string) for each flag.
* If symbolic names for flags are not useful to you, consider
* use std::bitset instead which has a pure positional approach.
* 
* - Bitwise operators
* - Several ways to access to individual flags
* - Formated input and output to an std::stream as symbols
* with the insertion (&lt;&lt;) and extraction (&gt;&gt;)
* operators
* - Implements the Component interface: StoreOn, DeepCopy,
* ShallowCopy...
* - Runtime symbolic representation
* - Runtime checking of the values
*
* @section ind Accessing individual flags
* 
* You can acces an individual flag of the Flags object in
* different ways:
* <table border='0'>
* <tr><td>Access by name:</td>	<td>flags.bFlagName</td></tr>
* <tr><td>Access by indexation plus enums:</td>	<td>flags[MyFlagsClass::eFlagName]</td></tr>
* <tr><td>Access by symbol:</td>	<td>flags["FlagName"]</td></tr>
* </table>
*
* You can use references this individual flags like any reference to a boolean.
* Moreover, you can use some extra operations like <tt>flip</tt> that inverses
* the actual value of the flag:
* @code
* 	bool isTrue= flags.bFlagName;
* 	bool isFalse= ~(flags.bFlagName);
* 	flags.bFlagName=true;
* 	flags.bFlagName.flip();
* @endcode
* @section FlagsBitwise Bitwise operations
* @section FlagsNonStandard Non Standard operations
*
* Because older versions of this class didn't derive from bitfield,
* a different interface for a few functions was used. This old interface
* has the advantage that uses the same function name convention as the
* rest of the CLAM classes.
* <table>
* <tr><td>CLAMFlags</td><td>mtg::Flags</td><td>Comments</td></tr>
* <tr><td>Size</td><td>size</td><td>Returns the number of flags</td></tr>
* <tr><td>NSet</td><td>count</td><td>Counts the set flags</td></tr>
* <tr><td>Reset</td><td>reset</td><td></td>Sets to 0 all the flags</tr>
* <tr><td>NFlags</td><td>not present</td><td>Revise This!!!!</td></tr>
* <tr><td>operator int</td><td>operator int</td><td>The interface is unchanged but throws a </td></tr>
* </table>
* @section CustomFlags Creating your own flags
* The way to define a new Flags type is by subclassing
* <tt>Flags<N></tt>.
* The subclass MUST redefine its constructors by
* providing the Flags constructor an array of tFlagsValue's,
* which defines the mapping between numeric and symbolic
* values, and an initialization value, than can be both a
* symbol (char* or std::string) or an integer.
* 
* @code
	// MyFlags.hxx
	class MyFlags : public Flags<5> {
	// Construction/Destruction
	public:
		static tFlagValue sFlagValues[];
		static tValue sDefault;
		virtual Component * Species() const {
			return new MyFlags();
		}
		typedef enum {
			eFlag0=0,
			eFlag1=1,
			eFlag2=2,
			eFlag3=3,
			eFlag4=4
		} tFlag;
		MyFlags () :
			Flags<5>(sFlagValues),
			flag0(operator[](eFlag0)),
			flag1(operator[](eFlag1)),
			flag2(operator[](eFlag2)),
			flag3(operator[](eFlag3)),
			flag4(operator[](eFlag4))
		{
			// The default flag configuration is set here
			flag4=true;
		};
		MyFlags (const MyFlags& someFlags) :
			Flags<5>(sFlagValues,someFlags),
			flag0(operator[](eFlag0)),
			flag1(operator[](eFlag1)),
			flag2(operator[](eFlag2)),
			flag3(operator[](eFlag3)),
			flag4(operator[](eFlag4))
		{};
		template <class T>
		MyFlags(const T &t) :
			Flags<5>(sFlagValues,t),
			flag0(operator[](eFlag0)),
			flag1(operator[](eFlag1)),
			flag2(operator[](eFlag2)),
			flag3(operator[](eFlag3)),
			flag4(operator[](eFlag4))
		{};
		template <class T1, class T2>
		MyFlags(const T1 &t1, const T2 &t2) :
			Flags<5>(sFlagValues,t1,t2),
			flag0(operator[](eFlag0)),
			flag1(operator[](eFlag1)),
			flag2(operator[](eFlag2)),
			flag3(operator[](eFlag3)),
			flag4(operator[](eFlag4))
		{}
		reference flag0;
		reference flag1;
		reference flag2;
		reference flag3;
		reference flag4;
	};
@endcode

@code
	// MyFlags.cxx
	Flags<5>::tFlagValue MyFlags::sFlagValues[] = {
		{MyFlags::eFlag0, "flag0"},
		{MyFlags::eFlag1, "flag1"},
		{MyFlags::eFlag2, "flag2"},
		{MyFlags::eFlag3, "flag3"},
		{MyFlags::eFlag4, "flag4"},
		{0,NULL}
	};
@endcode
*/
template <unsigned int N> class Flags : public FlagsBase, public std::bitset<N>
{
// Construction/Destruction
protected:
	/** The default constructor */
	Flags(tFlagValue * names) : std::bitset<N>() {
		mFlagValues=names;
	};
	/**
	* The derived copy constructor will use this
	*/
	Flags(tFlagValue * names, const Flags<N> &t) : std::bitset<N>() {
		mFlagValues=names;
	};
	/**
	* A lazy way to redefine all unary constructors in bitset 
	* by forwarding it.
	* @see std::bitset To obtain the complete set of available 
	* constructors.
	*/
	template <class T> Flags(tFlagValue * names,const T &t) : std::bitset<N>(t) {
		mFlagValues=names;
	};
	/** 
	* A template binary constructor that fordwards to the
	* matching std::bitset<PRE>&lt;N&gt;</PRE> constructor
	* A lazy way to redefine all binary constructors in bitset 
	* by forwarding it.
	* @see std::bitset To obtain the complete set of available 
	* constructors.
	*/
	template <class T1, class T2> Flags(tFlagValue * names,const T1 &t1,const T2 &t2) : 
		std::bitset<N>(t1, t2)
	{
		mFlagValues=names;
	};
public:
	/** The required virtual destructor */
	virtual ~Flags ()  {};

	/** @todo GetClassName for Flags */
	const char * GetClassName() const {return NULL;}

// Operators
public:
	virtual unsigned int GetNFlags () const {
		return N;
	}
protected:
	virtual bool IsSetFlag(unsigned int whichOne) const {
		return std::bitset<N>::test(whichOne);
	}
	virtual void SetFlag(unsigned int whichOne, bool value=true) {
		this->set(whichOne, value);
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

	virtual Component * DeepCopy () const {
		Flags<N> * pe = (Flags<N>*)Species();
		*pe=*this;
		return pe;
	}

	virtual Component * ShallowCopy () const {
		Flags<N> * pe = (Flags<N>*)Species();
		*pe=*this;
		return pe;
	}

};

//MRJ: Why this? There exists a fairly well known issue about
//VC6 support of template base classes, where polymorphism usual assumptions
//don't hold. In this case we introduce two proxy methods that
//just "inform" the compiler about what it has to do next... to promote
//the references to the derived class object into references to the
//base class. Sad but true. In discharge of MS guys this seems to be no
//longer needed in VC++ 7.1. We could have implemented the body of the methods
//inline on the header, but would have prevented us from using the compile-time
//saving usage of the <iosfwd> header.

#ifdef _MSC_VER
template <unsigned int N>
std::istream & operator >> (std::istream & is, Flags<N> & f) {
	return (is >>  static_cast<FlagsBase&>(f) );
}

template <unsigned int N>
std::ostream & operator << (std::ostream & os, const Flags<N> & f){
	return (os << static_cast<const FlagsBase&>(f));
}
#endif //_MSC_VER

}

#endif//_Flags_hxx_

