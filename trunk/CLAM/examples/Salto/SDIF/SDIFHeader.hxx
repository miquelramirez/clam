#ifndef __SDIFHeader__
#define __SDIFHeader__

#include "SDIFType.hxx"

/**
* Base header for all frames. 
* The only frame that uses this header directly is the special 
* SDIFOpeningFrame.
*
* @see SDIFDataFrameHeader
*/
class SDIFFrameHeader
{
public:
	//* Identifier indicating the kind of frame */
	SDIFType mType;
	//* Size in bytes, of the frame, not including the type or size
	TInt32 mSize;
public:
	/** Create a new SDIFFrameHeader
	* @param type Identifier specifying the frame type
	*/
	SDIFFrameHeader(const SDIFType& type);
};

/**
*       An extension of SDIFFrameHeader, that is used for data frames.
*       This is the header that is used for a SDIFFrame.
*/
class SDIFDataFrameHeader:public SDIFFrameHeader 
{
public:
	 //* Timestamp
	TFloat64 mTime;
	//* Stream ID @see SDIFStream
	TInt32 mStreamId;
	//* Number of data matrixes in the frame. @see SDIFMatrix
	TInt32 mnMatrices;
public:
	/** Create a new SDIFDataFrameHeader
	* @param type Identifier specifying the frame type
	* @param time Timestamp of the frame
	* @param id Stream ID
	*/
	SDIFDataFrameHeader(const SDIFType& type,TFloat64 time=0.,TInt32 id=0);
};

/** The header for each SDIFMatrix
*/
class SDIFMatrixHeader
{
public:
	//* Identifier indicating the matrix type
	SDIFType mType;
	//* Identifier indicating the type of data in the matrix
	ECLAMSDIFDataType mDataType;
	//* Number of rows in the matrix
	TInt32 mnRows;
	//* Number of columns in the matrix
	TInt32 mnColumns;
public:
	/** Create a new SDIFMatrixHeader
	* @param type Identifier specifying the matrix type
	* @param dataType Identifier specifying the type of data type in the matrix
	* @param nRows Number of rows in the matrix
	* @param nColumns Number of rows in the matrix
	*/
	SDIFMatrixHeader(
		const SDIFType& type,
		ECLAMSDIFDataType dataType,
		TInt32 nRows, TInt32 nColumns);
};

#endif
