/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

/* Class for Strings, defined using the Array Template Class  */

#ifndef _String_
#define _String_

#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>



#include "Array.hxx"
#include "Err.hxx"
#include "OSDefines.hxx"
#include <iostream>

namespace CLAM {

class String
{
public:

// Constructors


	String();

	String(const String& originalString);
	String(const  char* charArray);
	String(const  Array<char>& pCharArray);

	String(float number, int nDecimalPos);
	String(int number);

// Attributes & Operations


	int GetLength() const;
	bool IsEmpty() const;
	void Empty();
	char* GetBuffer(){
		char* buffer=new char[mStringBuffer.Size()+1];
		memcpy(buffer,mStringBuffer.GetPtr(),mStringBuffer.Size());
		buffer[mStringBuffer.Size()]=0;
		return buffer;
	};
	char GetAt(int iPosition) const;
	char operator[](int iPosition) const;
	void SetAt(int iPosition, char Character);
	
	// overloaded assignment

	const String& operator=(const String& originalString);
	const String& operator=(char character);

	// string concatenation

	const String& operator+=(const String& newString);
	const String& operator+=(const char* charArray);
	const String& operator+=(char character);

	// string comparison

	bool Compare(const String& stringToCompare) const;
	bool CompareNoCase(const String& stringToCompare) const;
	
	// simple sub-string extraction

	String Mid(int iFirst, int iCount) const;
	String Mid(int iFirst) const;
	String Left(int nCharacters) const;
	String Right(int nCharacters) const;

	// upper/lower/reverse conversion

	void MakeUpper();
	void MakeLower();
	void MakeReverse();

	// trimming whitespace (either side)

	void TrimRight();
	void TrimLeft();

	// trimming anything (either side), true if string is modified

	bool TrimRight(char targetCharacter);
	bool TrimRight(const String& targetString);
	bool TrimLeft(char targetCharacter);
	bool TrimLeft(const String& targetString);

	// advanced manipulation

	int Replace(char oldCharacter, char newCharacter);
	int Replace(const String& oldSubString , const String& newSubString);
	int Replace(const char* oldCharArray, const char* newCharArray);

	int Remove(char characterToRemove);
	int Remove(const String& oldSubString);
	int Remove(const char* charArray);

	const String& operator-=(const String& newString);
	const String& operator-=(const char *charArray);
	const String& operator-=(const char character);
	
	bool Insert(int iPosition, char character);
	bool Insert(int iPosition, const String& newSubString);

	void Delete(int iPosition, int nCharacters= 1);
	void Delete(int nCharacters= 1);


	// searching

	int ReverseFind(char characterToFind,int iPosition) const;
	int Find(char characterToFind, int iPosition) const;
	int FindOneOf(const String& subStringToFind,int which) const;
	int Find(const String& subStringToFind, int iPosition) const;
	
	bool IsCharacter(char targetCharacter, int iPosition) const;
	bool IsCharacter(const String& subString, int iPosition) const;
	bool IsCharacter(const char* targetSubString, int iPosition) const;

	bool ThereIsCharacter(const String& subString) const;
	bool ThereIsCharacter(const char* tokens) const;

	// token operations

	Array<String> Separate(const String &tokens, int &nFoundSubStrings, 
		bool &lastSubString, int nMaxSubStrings, bool modifyOriginalString);
	Array<String> Separate(const String &tokens, int &nFoundSubStrings);

protected:

	//Actual buffer
	Array<char> mStringBuffer;//Change when Array has default construnctor 
	
	//Number of Characters
	int mnCharactersInString;

};

std::istream& operator >> (std::istream& myStream, String& s);
std::ostream& operator << (std::ostream& myStream, String& s);

}

#endif
