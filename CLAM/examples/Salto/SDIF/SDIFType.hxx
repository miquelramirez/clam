#ifndef __SDIFType__
#define __SDIFType__

#include "DataFileIO.hxx"

#ifndef LINUX
	#include <string.h>
#endif

using namespace CLAM;

/**
* Used to store the SDIF frame and matrix types, a 4-byte
* identifier
*/
class SDIFType
{
friend class SDIFFile;
protected:
	/** The 4-byte identifier */
	TByte mData[4];
public:
	/** Create a new SDIFType
	* @param str String containing the 4-byte identifier
	*/
	SDIFType(const char* str = "\0\0\0\0")
	{
		memcpy(mData,str,4);	
	}
	/** Compare this SDIFType
	* @param cmp The SDIFType to compare with
	* @return true when equal
	*/
	bool operator == (const SDIFType& cmp) { return !memcmp(mData,cmp.mData,4); }

	/** Convert the SDIFType to a character string
	* @return The 4-byte identifier in a null-terminator const char string>         */
	const char* String(void) const
	{
		static char str[5];
		memcpy(str,mData,4);
		str[4]='\0';
		return str;
	}
	
	static SDIFType sDefault;
};

enum  ECLAMSDIFDataType{
	eSDIFDataTypeUnknown = 0
};

#endif
