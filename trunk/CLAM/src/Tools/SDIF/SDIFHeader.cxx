#include "SDIFHeader.hxx"

namespace SDIF
{

	FrameHeader::FrameHeader(
		const TypeId& type, CLAM::TInt32 size)
	{
		mType = type;
		mSize = size;
	}

	DataFrameHeader::DataFrameHeader(const TypeId& type,CLAM::TFloat64 time,CLAM::TInt32 id)
	:FrameHeader(type,SizeInFile()) 
	{
		mTime = time;
		mStreamId = id;
		mnMatrices = 0;	
	}

	MatrixHeader::MatrixHeader(const TypeId& type = "\0\0\0\0",DataType dataType = eUnknown,
		CLAM::TInt32 nRows = 0, CLAM::TInt32 nColumns = 0)
	{
		mType = type;
		mDataType = dataType;
		mnRows = nRows;
		mnColumns = nColumns;
	}

}
