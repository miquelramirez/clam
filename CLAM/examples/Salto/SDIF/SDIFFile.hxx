#ifndef __SDIFFile__
#define __SDIFFile__

#include "SDIFType.hxx"
#include "DataFileIO.hxx"
#include "SDIFHeader.hxx"
#include "SDIFFrame.hxx"
#include "SDIFMatrix.hxx"
#include "SDIFCollection.hxx"
#include "ByteOrder.hxx"

/** Used to read or write an SDIF file. When reading, the SDIFFile
* parses the whole files and passes the read SDIFFrames to a 
* SDIFStorage, typically a SDIFCollection to store all in memory.
*       
*       Note that the SDIFStorage is responsable for the 
*       freeing memory of the read frames, though SDIFFile
*       creates them.
*       
*       SDIF files use BIG ENDIAN byte order for the data storage.
*       SDIFFile takes care of fixing the byte order in case
*       the operating system is not BIG ENDIANm, @see CLAMByteOrder
*/

using namespace CLAM;

class SDIFFile:public DataFileIO
{
public:
	bool mSkipData;
public:
	SDIFFile(const char* filename,EIOMode mode);
	~SDIFFile();

	inline void FixByteOrder(TByte* ptr,
		TUInt32 nElems,TUInt32 elemSize);

	void Read(TInt32& t);
	void Write(const TInt32& t);

	void Read(TFloat64& t);
	void Write(const TFloat64& t);

	void Read(SDIFStorage& storage);
	void Write(const SDIFCollection& collection);

	void Read(SDIFType& header);
	void Write(const SDIFType& header);

	void Read(SDIFDataFrameHeader& header);
	void Write(const SDIFDataFrameHeader& header);

	void Read(SDIFFrameHeader& header);
	void Write(const SDIFFrameHeader& header);

	void Read(SDIFMatrixHeader& header);
	void Write(const SDIFMatrixHeader& header);

	void Read(SDIFFrame& frame);
	void Write(const SDIFFrame& frame);

	void Read(SDIFOpeningsFrame& frame);
	void Write(const SDIFOpeningsFrame& frame);

	void Read(SDIFMatrix& matrix);
	void Write(const SDIFMatrix& matrix);

	void SkipMatrixData(const SDIFMatrix& matrix);
	void ReadMatrixData(SDIFMatrix& matrix);
private:
	void _FixByteOrder(
	TByte* ptr,TUInt32 nElems,TUInt32 elemSize);
};

inline void SDIFFile::Read(TInt32& t)
{
	DataFileIO::Read(t);
	FixByteOrder((TByte*) &t,1,sizeof(t));
}

inline void SDIFFile::Read(TFloat64& t)
{
	DataFileIO::Read(t);
	FixByteOrder((TByte*) &t,1,sizeof(t));
}

inline void SDIFFile::FixByteOrder(TByte* ptr,
	TUInt32 nElems,TUInt32 elemSize)
{
#ifdef CLAM_LITTLE_ENDIAN
	_FixByteOrder(ptr,nElems,elemSize);
#else
#ifndef CLAM_BIG_ENDIAN
#pragma message ("BYTE ORDER NOT DEFINED!")
#endif
#endif
}

#endif
