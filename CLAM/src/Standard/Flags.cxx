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

#include "Flags.hxx"

#include <bitset>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Component.hxx"
#include "XMLAdapter.hxx"

// TODO: This is provisional to share the IllegalValue exception
#include "Enum.hxx"

using namespace CLAM;

/** 
* Retrieves the name of the flag at a position
* @param whichOne The position of the selected flag
* @returns The symbolic name of the selected flag as stream
*/
std::string FlagsBase::GetFlagString(unsigned int whichOne) const throw (IllegalValue) {
	for (unsigned int i = 0; mFlagValues[i].name; i++) {
		if (whichOne==mFlagValues[i].value) 
			return mFlagValues[i].name;
	}
	throw IllegalValue("Name not defined for an active flag");
	return "UnnamedFlag";
}
/** 
* Retrieves the position of the named flag
* @param whichOne The string containing the simbolic name
* @returns The symbolic name of the selected flag as stream
*/
unsigned int FlagsBase::GetFlagPosition(const std::string & whichOne) const throw (IllegalValue) {
	for (int i = 0; mFlagValues[i].name; i++) {
		if (whichOne==mFlagValues[i].name) 
			return mFlagValues[i].value;
	}
	throw IllegalValue("Name not defined for an active flag");
	return this->GetNFlags();
}

/* 
 * Stores component's subitems on the given Storage
 * @param store The given storage where the subitem will be stored
 * @see Storage
 * @todo TODO: This method can throw and IllegalValue exception
 */
void FlagsBase::StoreOn (Storage & storage) const {
	unsigned int N=GetNFlags();
	for (unsigned int i=0; i<N; i++) {
		if (!IsSetFlag(i)) continue;
		std::string s = GetFlagString(i);
		XMLAdapter<std::string> adapter(s);
		storage.Store(adapter);
	}
}
/* 
 * Loads component's subitems from the given Storage
 * @param store The given storage where the subitem will be stored
 * @see Storage
 * @todo TODO: This method can throw and IllegalValue exception
 */
void FlagsBase::LoadFrom (Storage & storage) {
	unsigned int N=GetNFlags();
	for (unsigned int i=0; i<N; i++) {
		SetFlag(i,false);
	}
	do {
		std::string flagName;
		XMLAdapter<std::string> adapter(flagName);
		if (!storage.Load(adapter)) break;
		unsigned int i = GetFlagPosition(flagName);
		SetFlag(i,true);
	}
	while (true);
}

std::istream & CLAM::operator >> (std::istream & is, FlagsBase & f) {
	const unsigned int N = f.GetNFlags();
	bool * bs = new bool[N];
	for (unsigned int i=N; i--; ) {
		bs[i]=false;
	}
	if (is.flags() & std::ios::skipws) {
		char c = '\0';
		do
			is.get(c);
		while (is && isspace(c));
		if (is) is.putback(c);
	}
	char c = '\0';
	is >> c;
	if (c!='{') {
		if (is)	is.putback(c);
		std::cerr << "A flag starting with '" << c << "'" << std::endl;
		delete [] bs;
		return is;
	}
	std::string flagContent;
	std::getline(is,flagContent,'}');
	std::stringstream ss(flagContent);
	std::string flagName;
	while (ss>>flagName) {
		try {
			unsigned i = f.GetFlagPosition(flagName);
			bs[i]=true;
		}
		catch (IllegalValue) {
			delete [] bs;
			throw IllegalValue(std::string("Invalid flag name: '")+
				flagName+"'");
		}
	}
	for (unsigned i=N; i--;) f.SetFlag(i, bs[i]);
	delete [] bs;
	return is;
}

std::ostream & CLAM::operator << (std::ostream & os, const FlagsBase & f) {
	const unsigned int N = f.GetNFlags();
	bool first = true;
	os << "{";
	for (unsigned int i=0; i<N; i++) {
		if (f.IsSetFlag(i)) {
			if (!first) os << " ";
			else first=false;
			os << f.GetFlagString(i);
		}
	}
	os << "}";
	return os;
}

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

bool FlagsBase::CheckInvariant() {
	// Test that the names array is not a NULL pointer (it still could be invalid)
	if (!mFlagValues) {
		std::cerr << "Name definitions are a NULL pointer" << std::endl;
		return false;
	}
	// Test that a NULL name is present at the end of the names array
	unsigned int top;
	for (top=0; top<=GetNFlags() && mFlagValues[top].name; top++) {
		if (top==GetNFlags() && mFlagValues[top].name) {
			std::cerr << "There are more names than flags or there is no NULL name "
				"at the end of the name array list" << std::endl;
		return false;
		}
	}
	// Test that the names array has no values replications and no names replications
	for (unsigned int i=0; i<top; i++) {
		for (unsigned int j=i+1; j<top; j++) {
			if (std::string(mFlagValues[i].name)==std::string(mFlagValues[j].name)) {
				std::cerr << "Warning: flag names '"<< mFlagValues[i].name 
					<< "' and '" << mFlagValues[j].name 
					<< "' are replicated" << std::endl;
			}
			if (mFlagValues[i].value==mFlagValues[j].value) {
				std::cerr << "Warning: flag values '"<< mFlagValues[i].value 
					<< "' and '" << mFlagValues[j].value 
					<< "' are replicated" << std::endl;
			}
		}
	}
	return true;
}

