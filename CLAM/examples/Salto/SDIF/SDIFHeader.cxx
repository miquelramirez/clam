#include "SDIFHeader.hxx"

SDIFFrameHeader::SDIFFrameHeader(
	const SDIFType& type)
{
	mType = type;
	mSize = 0;
}

SDIFDataFrameHeader::SDIFDataFrameHeader(const SDIFType& type,TFloat64 time,TInt32 id)
:SDIFFrameHeader(type) 
{
	mTime = time;
	mStreamId = id;
	mnMatrices = 0;	
}

SDIFMatrixHeader::SDIFMatrixHeader(const SDIFType& type = "\0\0\0\0",ECLAMSDIFDataType dataType = eSDIFDataTypeUnknown,
	TInt32 nRows = 0, TInt32 nColumns = 0)
{
	mType = type;
	mDataType = dataType;
	mnRows = nRows;
	mnColumns = nColumns;
}

