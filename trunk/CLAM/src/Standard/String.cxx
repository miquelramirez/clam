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

#include "String.hxx"

namespace CLAM {

/* CONSTRUCTORS */

/* Constructor empty String */
String::String()
	:mStringBuffer(0)    // 0 length buffer
{
	mnCharactersInString=0;
}


/* Copy constructor from an original String object  */
String::String(const String& originalString)
	:mStringBuffer(originalString.GetLength())
{
	mnCharactersInString=originalString.mStringBuffer.Size();
	*this=originalString;
}

/* Copy constructor from a string  */
String::String(const  char* charArray)
	:mStringBuffer(strlen(charArray))
{
	mnCharactersInString=strlen(charArray);
	mStringBuffer.Resize(strlen(charArray));
	memcpy(mStringBuffer.GetPtr(),charArray,sizeof(char)*strlen(charArray));
	mStringBuffer.SetSize(strlen(charArray));
}

/* Copy constructor from a CLAMTmplArray (of char) object  */
String::String(const  Array<char>& pCharArray)
	:mStringBuffer(pCharArray.Size())
{
	mnCharactersInString=pCharArray.Size();
	mStringBuffer=pCharArray;
}

/* Constructor from a float */
String::String(float number,int nDecimalPos)
	:mStringBuffer(0)              
{ 
	char nDecimalStr[50];
	mnCharactersInString=50;
	snprintf(nDecimalStr,50,"%%.%df",nDecimalPos);
	mStringBuffer.Resize(50);
	mStringBuffer.SetSize(50);
	
	snprintf(mStringBuffer.GetPtr(),50,nDecimalStr,number);
	mnCharactersInString=strlen((char*)GetBuffer());
	mStringBuffer.Resize(mnCharactersInString);
	mStringBuffer.SetSize(mnCharactersInString);
}

/* Constructor from an int */
String::String(int number)
	:mStringBuffer(0)               // Analog to empty constructor
{
	mStringBuffer.Resize(50);
	mStringBuffer.SetSize(50);
	snprintf(mStringBuffer.GetPtr(),50,"%d",number);
	mnCharactersInString=strlen((char*)GetBuffer());
	mStringBuffer.Resize(mnCharactersInString);
	mStringBuffer.SetSize(mnCharactersInString);
}


/* ATTRIBUTES & OPERATIONS */

/* Get data length */
int String::GetLength() const
{
	return mnCharactersInString;
	
}

/* TRUE if zero length */
bool String::IsEmpty() const
{
	if(mnCharactersInString==0) return true;
	else return false;
}

/* Clear String  */
void String::Empty()
{
	if(mStringBuffer.Size())
		{
			mStringBuffer.Resize(0);
			mStringBuffer.SetSize(0);
		}
	mnCharactersInString=0;
	
}


/* Return single character at position index */
char String::GetAt(int iPosition) const
{
	return mStringBuffer[iPosition];
	
}

/* Return single character at position index */
char String::operator[](int iPosition) const  // Operator overloading 
{
	return mStringBuffer[iPosition];
	
}

/* Set a single character at position index */
void String::SetAt(int iPosition, char Character)
{
	mStringBuffer[iPosition]=Character;
}


/* Overloaded assignment */

/* Ref-counted copy from another String */
const String& String::operator=(const String& originalString)
{
	mStringBuffer.Init();
	mStringBuffer=originalString.mStringBuffer;
	mnCharactersInString=mStringBuffer.Size();
	return *this;
}

/* Set string content to single character */
const String& String::operator=(char character)
{
	mStringBuffer.SetSize(1);
	mStringBuffer[0]=character;
	mnCharactersInString=1;
	return *this;
}


/* String concatenation */

/* Concatenate another object String */
const String& String::operator+=(const String& newString)
{
	mStringBuffer+=newString.mStringBuffer;     // Concatenate the buffers 
	mnCharactersInString+=newString.mnCharactersInString; // Add the lengths
	return *this;
}

/* Concatenate a string charArray */
const String& String::operator+=(const char* charArray)
{
	String stringToAdd(charArray);  // Convert to a String object
	*this+=stringToAdd;                 // Concatenate two String objets
	return *this;
}


/* Concatenate a single character */
const String& String::operator+=(char character)
{
	mStringBuffer.SetSize(mnCharactersInString+1); 
	mStringBuffer[mnCharactersInString]=character;  
	mnCharactersInString++;
	return *this;
}


/* String comparison */

/* Compare two CLAMString objects */
bool String::Compare(const String& stringToCompare) const
{
	if(stringToCompare.mStringBuffer==mStringBuffer)  // Compare two Array<char> objects
		return true;                   
	else return false;
	return false;
}

/* Compare ignoring case */
bool String::CompareNoCase(const String& stringToCompare) const
{
	bool isSame;

	// Define two strings
	char *currentBuffer,*bufferToCompare;    
	bufferToCompare=new char[stringToCompare.mnCharactersInString+1];
	memcpy(bufferToCompare,stringToCompare.mStringBuffer.GetPtr(),
	 sizeof(char)*stringToCompare.mnCharactersInString);
	bufferToCompare[stringToCompare.mnCharactersInString]='\0';
	currentBuffer=new char[mnCharactersInString+1];
	memcpy(currentBuffer,mStringBuffer.GetPtr(),sizeof(char)*mnCharactersInString);
	currentBuffer[mnCharactersInString]='\0';

	// Compare the two strings ignoring the case
#if defined _MSC_VER
	if(!stricmp(currentBuffer,bufferToCompare)) isSame=true;

#elif defined __MWERKS__
	isSame = false;
	if (stringToCompare.mnCharactersInString==mnCharactersInString)
	{
		for (int i=0;i<mnCharactersInString;i++)
		{	 	
			 	bufferToCompare[i] = toupper (bufferToCompare[i]);
				currentBuffer  [i] = toupper (currentBuffer  [i]);
				if (bufferToCompare[i] != currentBuffer [i]) 
				{	
					isSame = false;
					break;
				}
				isSame = true;
		}
	}	

#elif defined __GNUC__
	if(!strcasecmp(currentBuffer,bufferToCompare)) isSame=true;
#else
#error("No case insensitive string comparation defined for the compiler");
#endif
	else isSame=false;

	// Delete the strings
	delete[] bufferToCompare;
	delete[] currentBuffer;
	return isSame;
}

/* Simple sub-string extraction */

/* Return String object with nCount characters starting at nFirst */
String String::Mid(int iFirst, int iCount) const
{
	if(iFirst+iCount>mnCharactersInString||iFirst<0||iCount<1)
	{
		throw Err("Out of bounds in String");
	}
	else
	{
		Array<char> tmpArray(iCount);    
		tmpArray.SetSize(iCount);
 
		memcpy(tmpArray.GetPtr(),&(mStringBuffer.GetPtr())[iFirst],iCount*sizeof(char));
		return String(tmpArray);   // Convert to a String
	}
	
}

/* Return all characters starting at zero-based nFirst */
String String::Mid(int iFirst) const
{
	if(iFirst>mnCharactersInString||iFirst<0)
	{
		throw Err("Out of bounds in String");
	}
	else
	{
		Array<char> tmpArray(mnCharactersInString-iFirst);
 
		memcpy(tmpArray.GetPtr(),&(mStringBuffer.GetPtr())[iFirst],tmpArray.Size()*sizeof(char));
		return String(tmpArray);
	} 
}

/* Return first nCount characters in string, NFirst = 0 */
String String::Left(int nCharacters) const
{
	if(nCharacters>mnCharactersInString||nCharacters<0)
	{
		throw Err("Out of bounds in String");
	}
	else
	{
		Array<char> tmpArray(nCharacters);
		
		memcpy(tmpArray.GetPtr(),mStringBuffer.GetPtr(),nCharacters*sizeof(char));
		return String(tmpArray);
	}

}

/* Return nCount characters from end of string */
String String::Right(int nCharacters) const
{
	if(nCharacters>mnCharactersInString||nCharacters<0)
	{
		throw Err("Out of bounds in String");
	}
	else
	{
		Array<char> tmpArray(nCharacters);
		
		memcpy(tmpArray.GetPtr(),&(mStringBuffer.GetPtr())[mStringBuffer.Size()-nCharacters],
		 nCharacters*sizeof(char));
		return String(tmpArray);
	}
}


/* upper/lower/reverse conversion */

/* Conversion to uppercase */
void String::MakeUpper()
{
	for(int i=0;i<mnCharactersInString;i++)
	{
		mStringBuffer[i]=toupper(mStringBuffer[i]);
	}
}

/* Conversion to lowercase */
void String::MakeLower()
{
	for(int i=0;i<mnCharactersInString;i++)
	{
		mStringBuffer[i]=tolower(mStringBuffer[i]);
	}
}

/* Reverse string right-to-left */
void String::MakeReverse()
{
	if(mnCharactersInString>0)    // Not an empty string
	{
		Array<char> tmpBuffer(mnCharactersInString); // Temporary buffer
		for(int i=0;i<mnCharactersInString;i++)
		{
			tmpBuffer[i]=mStringBuffer[mnCharactersInString-i];
		}
		mStringBuffer=tmpBuffer;
	}
}


/* Trimming whitespace (either side) */

/* Remove whitespaces starting from right edge */
void String::TrimRight()
{
	if(mnCharactersInString>0) // Not an empty string
	{
		for(int i=mnCharactersInString-1;i>=0;i--)
		{
			if(mStringBuffer[i]==' ')
			{
				mStringBuffer.DeleteElem(i);
			}
			else break;
		}
	}
}

/* Remove whitespace starting from left side */
void String::TrimLeft()
{
	if(mnCharactersInString>0) // Not an empty string
	{
		for(int i=0;i<mnCharactersInString;i++)	
		{
			if(mStringBuffer[i]==' ')
			{
				mStringBuffer.DeleteElem(i);
			}
			else break;
		}
	}
}



/* Trimming anything (either side) */
/* Return TRUE if something found  */

/* Remove continuous occurrences of targetCharacter starting from right */
bool String::TrimRight(char targetCharacter)
{
	bool found=false;
	if(mnCharactersInString>0)  // Not an empty string
	{
		for(int i=mnCharactersInString-1;i>=0;i--)
		{
			if(mStringBuffer[i]==targetCharacter)
			{
				mStringBuffer.DeleteElem(i); // Delete character
				found=true;                 // Character found
			}
			else break;
		}
	}
	return found;
}


/* Remove a substring from the original one, starting from right */
bool String::TrimRight(const String& targetString)
{
	bool found=true,somethingFound=false;
	int i;
	
	if(targetString.GetLength()<mnCharactersInString&&mnCharactersInString>0) 
		// String length >= substring length
	{
		while(found) // Search the targetstring 
		{
			for(i=targetString.mnCharactersInString-1;i>=0;i--)
			{
				if(mStringBuffer[i]!=targetString.mStringBuffer[i])
				{
					found=false;
					break;
				}
			}
			if(found) // Remove it
			{
				for(i=targetString.mnCharactersInString-1;i>=0;i--)
				{
					if (somethingFound==false) somethingFound=true;
					mStringBuffer.DeleteElem(i);
				}
			found=TrimRight(targetString);
			}
		}
	}
	return somethingFound;
}


/* Remove continuous occurrences of chTarget starting from left */
bool String::TrimLeft(char targetCharacter)
{
	bool found=false;
	if(mnCharactersInString>0)
	{
		for(int i=0;i<mnCharactersInString;i++)
		{
			if(mStringBuffer[i]==targetCharacter)
			{
				mStringBuffer.DeleteElem(i);
				found=true;
			}
			else break;
		}
	}
	return found;
}


/* Remove occcurrences of a substring in passed string, starting from	left */
bool String::TrimLeft(const String& targetString)
{
	bool found=true,somethingFound=false;
	int i;
	
	if(targetString.GetLength()<mnCharactersInString&&mnCharactersInString>0)
	{
		while(found)  // Searching for the substring
		{
			for(i=0;i<targetString.mnCharactersInString;i++)
			{
				if(mStringBuffer[i]!=targetString.mStringBuffer[i])
				{
					found=false;
					break;
				}
			}
			if(found) // Delete it
			{
				for(i=targetString.mnCharactersInString-1;i>=0;i--)
				{
					if (somethingFound==false) somethingFound=true;
					mStringBuffer.DeleteElem(i);
				}
				found=TrimRight(targetString);
			}
		}
	}
	return somethingFound;
}



/* Advanced manipulation */

/* Replace occurrences of OldCharacter with NewCharacter, 
	 returning number of ocurrences found									 */
int String::Replace(char oldCharacter, char newCharacter)
{
	int nReplacedCharacters=0;
	if(mnCharactersInString>0)
	{
		for(int i=0;i<mnCharactersInString;i++)
		{
			if(mStringBuffer[i]==oldCharacter)
			{
				nReplacedCharacters++;
				mStringBuffer[i]=newCharacter;
			}
		}
	}
	return nReplacedCharacters;
}


/* Replace occurrences of substring OldSubString with NewSubString;
	 empty newSubString removes instances of oldSubString						 
	 return number of ocurrences found or -1 if 0										 */
int String::Replace(const String& oldSubString , const String& newSubString)
{
	int nReplacedStrings=0,iSubStringPosition=0;
	if(oldSubString.GetLength()>0&&mnCharactersInString>0&&
		 oldSubString.mnCharactersInString<=mnCharactersInString)
	{
		while(iSubStringPosition+oldSubString.mnCharactersInString<mnCharactersInString&&
		iSubStringPosition>-1)
		{
			iSubStringPosition=Find(oldSubString,iSubStringPosition);
			Delete(iSubStringPosition,oldSubString.mnCharactersInString);
			if(!newSubString.IsEmpty())
				Insert(iSubStringPosition,newSubString);
			nReplacedStrings++;
		}
		return nReplacedStrings;
	}
	else return -1;
}

/* Replace occurrences of substring oldCharArray with newCharArray	*/
int String::Replace(const char* oldCharArray, const char* newCharArray)
{
	const String oldString(oldCharArray),newString(newCharArray);
	return Replace(oldString,newString);
}

/* Remove occurrences of substring OldSubString	*/
int String::Remove(const String& oldSubString)
{
	int nReplacedStrings=0,iSubStringPosition=0;
	if(oldSubString.GetLength()>0&&mnCharactersInString>0&&
		 oldSubString.mnCharactersInString<=mnCharactersInString)
	{
		while(iSubStringPosition+oldSubString.mnCharactersInString<mnCharactersInString&&
		iSubStringPosition>-1)
		{
			iSubStringPosition=Find(oldSubString,iSubStringPosition);
			if(iSubStringPosition>0)
			{
				Delete(iSubStringPosition,oldSubString.mnCharactersInString);
				nReplacedStrings++;
			}
		}
		return nReplacedStrings;
	}
	else return -1;
}

/* Remove occurrences of charArray	 */
int String::Remove(const char* charArray)
{
	const String stringToRemove(charArray);	// Define String from charArray
	int nReplacedStrings=Remove(stringToRemove);
	return nReplacedStrings;
}

/* Remove occurrences of CharacterToRemove */
int String::Remove(char characterToRemove)
{
	int nRemovedCharacters=0;
	for(int i=0;i<mnCharactersInString;i++)
	{
		if(mStringBuffer[i]==characterToRemove)
		{
			mStringBuffer.DeleteElem(i);
			i--;
			nRemovedCharacters++;
			mnCharactersInString--;
		}
	}
	return nRemovedCharacters;
}

/* Operator Overloading */

/* -= analog to Remove from string */
const String& String::operator-=(const String& newString)
{
	Remove(newString);
	return *this;
}

/* Remove string from String */
const String& String::operator-=(const char *charArray)
{
	Remove(charArray);
	return *this;
}

/* Remove char from String */
const String& String::operator-=(const char character)
{
	Remove(character);
	return *this;
}

/* Insert character at iPosition index; concatenates
	 if index is past end of string (then returns false)	*/
bool String::Insert(int iPosition, char character)
{
	bool isInserted=true;
	if(iPosition<0) return false;
	else if(iPosition>=mnCharactersInString)
	{
		isInserted=false;
		mStringBuffer.AddElem(character);
	}
	else mStringBuffer.InsertElem(iPosition,character);
	mnCharactersInString++;
	return isInserted;
}

/* Insert substring at iPosition index; concatenates
	 if index is past end of string (then returns false) */
bool String::Insert(int iPosition, const String& newSubString)
{
	bool isInserted=true;
	if(iPosition<0) return false;
	else if(iPosition<mnCharactersInString)
	{
		for(int i=0;i<newSubString.mnCharactersInString;i++)
		{
			mStringBuffer.InsertElem(i+iPosition,newSubString[i]);
			mnCharactersInString++;
		}
	}
	else
	{
		isInserted=false;
		for(int i=0;i<newSubString.mnCharactersInString;i++)
		{
			mStringBuffer.AddElem(newSubString[i]);
			mnCharactersInString++;
		}
	}
	return isInserted;
}

/* Delete nCharacters characters starting at iPosition index */
void String::Delete(int iPosition, int nCharacters)
{
	for(int i=iPosition;i<iPosition+nCharacters;i++)
	{
		if(iPosition<mnCharactersInString)
		{
			mStringBuffer.DeleteElem(iPosition);
			mnCharactersInString--;
		}
	}
}

/* Delete nCharacters characters at the end of string */
void String::Delete(int nCharacters)
{
	if(nCharacters<mnCharactersInString)
	{
		for(int i=mnCharactersInString-1;i>=mnCharactersInString-nCharacters;i--)
		{
			mStringBuffer.DeleteElem(i);
			mnCharactersInString--;
		}
	}
}


/* Searching functions */

/* Search characterToFind starting at the end of the string */
int String::ReverseFind(char characterToFind,int iPosition=-1) const
{
	int lastCharacterPosition=-1;
	if(iPosition==-1) iPosition=mnCharactersInString-1;
	if(iPosition>0&&iPosition<mnCharactersInString&&mnCharactersInString>0)
	{
		for(int i=iPosition;i>=0;i--)
		{
			if(mStringBuffer[i]==characterToFind)
			{
				lastCharacterPosition=i;
				break;
			}
		}
	}
	return lastCharacterPosition;
}


/* Search characterToFind starting at zero-based (iPosition) index and going right */
int String::Find(char characterToFind, int iPosition=0) const
{
	int firstCharacterPosition=-1;
	if(iPosition<mnCharactersInString&&mnCharactersInString>0&&iPosition>=0)
	{
		for(int i=iPosition;i<mnCharactersInString;i++)
		{
			if(mStringBuffer[i]==characterToFind)
			{
				firstCharacterPosition=i;
				break;
			}
		}
	}
	return firstCharacterPosition;
}


/* Search nth(which, default to 1st) instance of subStringToFind,
	 returning the position																				 */
int String::FindOneOf(const String& subStringToFind, int which=1) const
{
	int iPosition=-1;
	if(mnCharactersInString>0&&subStringToFind.mnCharactersInString>0)
		// No empty strings
	{
		for(int i=0;i<mnCharactersInString;i++)
		{
			for(int n=0;n<subStringToFind.mnCharactersInString;n++)
			{
				if(mStringBuffer[i]==subStringToFind[n])
				{
					which--;
					if(which==0)
					{
						iPosition=i;
						break;
					}
				}
			}
			if(iPosition>0) break;
		}
	}
	return iPosition;
}

/* Search first instance of substringToFind starting at iPosition */
int String::Find(const String& subStringToFind,int iPosition=0) const
{
	int iSubStringPosition=iPosition,iLastCharacter=0;
	bool found=false;
	if(iPosition>=0&&mnCharactersInString>0&&
		iPosition+subStringToFind.mnCharactersInString<=mnCharactersInString)
	{
		while(iSubStringPosition+subStringToFind.mnCharactersInString<mnCharactersInString&&
		iSubStringPosition>-1)
		{
			if(found) break;
			found=true;
			iSubStringPosition=Find(subStringToFind[0],iLastCharacter);
			iLastCharacter=iSubStringPosition+1;
			if(iSubStringPosition>=0)
			{
				for(int i=1;i<subStringToFind.mnCharactersInString;i++)
				{
					if(Find(subStringToFind[i],iLastCharacter)!=iSubStringPosition+i)
					{
						found=false;
						break;
					}
				}
			}
		}
		if(!found) return -1;
		else return iSubStringPosition;
	}
	else return -1;
}


/* TRUE if targetCharacter found at iPosition */
bool String::IsCharacter(char targetCharacter, int iPosition) const
{
	bool isSame=false;
	if(iPosition<0||iPosition>mnCharactersInString-1)
		throw Err("Out of bounds in String");
	else
		if(mStringBuffer[iPosition]==targetCharacter) isSame=true;	
	return isSame;
}

/* TRUE if String substring found at iPosition	*/
bool String::IsCharacter(const String& targetSubString, int iPosition) const
{
	bool isIncluded=false;
	if(iPosition<0||iPosition>mnCharactersInString-1)
		throw Err("Out of bounds in String");
	else
	{
		for(int i=0;i<targetSubString.mnCharactersInString;i++)
		{
			if(IsCharacter(targetSubString[i],iPosition))
			{
				isIncluded=true;
				break;
			}
		}
	}
	return isIncluded;
}

/* TRUE if a substring found at iPosition */
bool String::IsCharacter(const char* targetSubString, int iPosition) const
{
	bool isIncluded=false;
	if(iPosition<0||iPosition>mnCharactersInString-1)
		throw Err("Out of bounds in String");
	else
	{
		for(int i=0;i<(int)strlen(targetSubString);i++)
		{
			if(IsCharacter(targetSubString[i],iPosition)) // Searching for a single char
			{
				isIncluded=true;
				break;
			}
		}
	}
	return isIncluded;
}


/* TRUE	if String contains a character included in TargetSubstring */
bool String::ThereIsCharacter(const String& subString) const
{
	bool isIncluded=false;
	for(int i=0;i<mnCharactersInString;i++)
		if(IsCharacter(subString,i))
			isIncluded=true;
	return isIncluded;
}

/* TRUE if tokens contains a character included in TargetSubstring	*/
bool String::ThereIsCharacter(const char* tokens) const
{
	bool isIncluded=false;
	for(int i=0;i<mnCharactersInString;i++)
	{
		if(IsCharacter(tokens,i))
			isIncluded=true;
	}
	return isIncluded;
}


/* TOKEN OPERATIONS */

/* Separate OriginalString into nMaxSubStrings delimited by Tokens, the function returns an array 
	 of Strings, the number of SubStrings found and a boolean indicating if it is the last substring. 
	 The Original String is modified (if ModifyOriginalString=true or omitted)													 */
Array<String> String::Separate(const String &tokens, int &nFoundSubStrings, 
	 bool &lastSubString, int nMaxSubStrings=-1, bool modifyOriginalString=true)
{
	Array<String> stringArray;
	nFoundSubStrings=0;
	if(mnCharactersInString>0&&tokens.mnCharactersInString>0)
	{
		int tokenPosition,i=0,lastCharacter=0;
		bool finished=false;
		while(!finished)
		{
			if(nMaxSubStrings>0)
			{
				if(nFoundSubStrings==nMaxSubStrings)
					finished=true;
			}
			tokenPosition=FindOneOf(tokens,i);
			if(tokenPosition>0)
			{
				stringArray.AddElem(Mid(lastCharacter,tokenPosition));
				nFoundSubStrings++;
				if(modifyOriginalString)
					{
						Delete(lastCharacter,tokenPosition);
					}
				else i++;
				lastCharacter=tokenPosition;
					
			}
			else finished=true;
		}
	}
	return stringArray;
}

Array<String> String::Separate(const String &tokens, int &nFoundSubStrings)
{
	Array<String> stringArray;
	nFoundSubStrings=0;
	if(mnCharactersInString>0&&tokens.mnCharactersInString>0)
	{
		int tokenPosition,i=1,lastCharacter=-1;
		bool finished=false;
		while(!finished)
		{
			tokenPosition=FindOneOf(tokens,i);
			if(tokenPosition>0)
			{
				String tmpString;
				if(tokenPosition-lastCharacter>1)
				{
					tmpString=Mid(lastCharacter+1,tokenPosition-lastCharacter-1);
					stringArray.Resize(nFoundSubStrings+1);
					stringArray.SetSize(nFoundSubStrings+1);
					stringArray[nFoundSubStrings]=tmpString;
						//stringArray.AddElem(tmpString);
					nFoundSubStrings++;
				}
			i++;
			lastCharacter=tokenPosition;
				
			}
			else finished=true;
		}
	}
	return stringArray;
}

/** @todo: Not Implemented */
std::istream& operator >> (std::istream& myStream, String& s)
{
	return myStream;

}
std::ostream& operator << (std::ostream& myStream, String& s)
{
	myStream << s.GetBuffer();
	return myStream;

}

}
