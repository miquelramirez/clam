#include "SDIFFile.hxx"

using namespace CLAM;

SDIFFile::SDIFFile(const char* filename,EIOMode mode)
:DataFileIO(filename,mode)
{
	mSkipData = false;
}

SDIFFile::~SDIFFile()
{
}

void SDIFFile::Read(SDIFStorage& storage)
{
	SDIFOpeningsFrame opening;
	Read(opening);
#ifdef FEW_MEM


#else
	while (!Done()) {
		SDIFFrame* frame = new SDIFFrame;
		Read(*frame);
		storage.Add(frame);
	}
#endif
}

void SDIFFile::Read(SDIFType& type)
{
	DataFileIO::Read(type.mData,4);
}

void SDIFFile::Read(SDIFFrameHeader& header)
{
	Read(header.mType);
	Read(header.mSize);
}

void SDIFFile::Write(const SDIFFrameHeader& header) {}

void SDIFFile::Read(SDIFDataFrameHeader& header)
{
	Read((SDIFFrameHeader&)header);
	Read(header.mTime);
	Read(header.mStreamId);
	Read(header.mnMatrices);
}

void SDIFFile::Write(const SDIFDataFrameHeader& header) {}

void SDIFFile::Read(SDIFFrame& frame)
{
	Read(frame.mHeader);
	for (int i=0;i<frame.mHeader.mnMatrices;i++)
	{
		SDIFMatrix* pMatrix = new SDIFMatrix;
		SDIFFile::Read(*pMatrix);
		frame.Add(pMatrix);
	}
}

void SDIFFile::Write(const SDIFFrame& frame) {}

void SDIFFile::Read(SDIFOpeningsFrame& frame)
{
	Read(frame.mHeader);
	Read(frame.mSpecVersion);
	Read(frame.mStandardTypesVersion);
}

void SDIFFile::Write(const SDIFOpeningsFrame& frame) {}

void SDIFFile::Read(SDIFMatrixHeader& header)
{
	Read(header.mType);
	TInt32 tmp;
	Read(tmp);
	header.mDataType = (ECLAMSDIFDataType) tmp;
	Read(header.mnRows);
	Read(header.mnColumns);
}

void SDIFFile::Write(const SDIFMatrixHeader& header) {}

void SDIFFile::Read(SDIFMatrix& matrix)
{
	Read(matrix.mHeader);
	if (mSkipData)
	{
		SkipMatrixData(matrix);
	} else {
		ReadMatrixData(matrix);
	}
}

void SDIFFile::Write(const SDIFMatrix& matrix)  {}

void SDIFFile::SkipMatrixData(const SDIFMatrix& matrix)
{
	TUInt32 size = matrix.mHeader.mnColumns*matrix.mHeader.mnRows*
		((matrix.mHeader.mDataType)&0xFF);
	TUInt32 padding = 8-size&7;
	Pos(Pos()+size+padding);
}

#ifdef LINUX
#include <byteswap.h>
#endif

TUInt16 Swap(const TUInt16& val)
{
#if defined LINUX 
	return bswap_16(val);
#else
	return (val>>8)|(val<<8);
#endif
}

TUInt32 Swap(const TUInt32& val)
{
#if defined LINUX 
	return bswap_32(val);
#else
  TUInt32 cp = val;
	TByte* ptr=(TByte*) &cp;
	static TByte tmp;
	tmp=ptr[0]; ptr[0]=ptr[3]; ptr[3]=tmp;
	tmp=ptr[1]; ptr[1]=ptr[2]; ptr[2]=tmp;
  return cp;
#endif
}

TUInt64 Swap(const TUInt64& val)
{
#if defined LINUX 
	return bswap_64(val);
#else
  TUInt64 cp = val;
	TByte* ptr=(TByte*) &cp;
	static TByte tmp;
	tmp=ptr[0]; ptr[0]=ptr[7]; ptr[7]=tmp;
	tmp=ptr[1]; ptr[1]=ptr[6]; ptr[6]=tmp;
	tmp=ptr[2]; ptr[2]=ptr[5]; ptr[5]=tmp;
	tmp=ptr[3]; ptr[3]=ptr[4]; ptr[4]=tmp;
  return cp;
#endif
}

void SDIFFile::_FixByteOrder(
	TByte* ptr,TUInt32 nElems,TUInt32 elemSize)
{
	switch (elemSize)
	{
		case 1: return;
		case 2:
		{
			TUInt16* fptr = (TUInt16*) ptr;
			for (TUInt32 i=0;i<nElems;i++)
			{
				*fptr = Swap(*fptr);
				fptr++;
			}
			return;
		}
		case 4:
		{
			TUInt32* fptr = (TUInt32*) ptr;
			for (TUInt32 i=0;i<nElems;i++)
			{
				*fptr = Swap(*fptr);
				fptr++;
			}
			return;
		}
		case 8:
		{
			TUInt64* fptr = (TUInt64*) ptr;
			for (TUInt32 i=0;i<nElems;i++)
			{
				*fptr = Swap(*fptr);
				fptr++;
			}
			return;
		}
		default: throw;
	}
}

void SDIFFile::ReadMatrixData(SDIFMatrix& matrix)
{
  TByte dum[8];
	TUInt32 nElems = matrix.mHeader.mnColumns*matrix.mHeader.mnRows;
	TUInt32 elemSize = matrix.mHeader.mDataType&0xFF;
	TUInt32 size = nElems*elemSize;
		
	TUInt32 padding = 8-size&7;

// Testing XA	matrix.mpData = new TByte[size];
	matrix.mpFloat32Data.Resize(size);
	matrix.mpFloat32Data.SetSize(size);

// Testing XA	DataFileIO::Read((TByte*) matrix.mpData,size);
	DataFileIO::Read((TByte*) matrix.mpFloat32Data.GetPtr(),size);
// Testing XA	FixByteOrder((TByte*) matrix.mpData,nElems,elemSize);
	FixByteOrder((TByte*) matrix.mpFloat32Data.GetPtr(),nElems,elemSize);

//  Pos(Pos()+padding);
	DataFileIO::Read(dum,padding);  
}

