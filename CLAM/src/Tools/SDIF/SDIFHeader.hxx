#ifndef __SDIFHeader__
#define __SDIFHeader__

#include "SDIFType.hxx"

namespace SDIF
{

	/**
	* Base header for all frames. 
	* The only frame that uses this header directly is the special 
	* OpeningFrame.
	*
	* @see DataFrameHeader
	*/

	class FrameHeader
	{
	public:
		//* Identifier indicating the kind of frame */
		TypeId mType;
		//* Size in bytes, of the frame, not including the type or size
		CLAM::TInt32 mSize;
	public:
		/** Create a new FrameHeader
		* @param type Identifier specifying the frame type
		*/
		FrameHeader(const TypeId& type, CLAM::TInt32 size);

		static CLAM::TInt32 SizeInFile(void)
		{
			return 
				sizeof(TypeId)+ // mType
				sizeof(CLAM::TInt32); // mSize
		}
	};

	/**
	*       An extension of FrameHeader, that is used for data frames.
	*       This is the header that is used for a Frame.
	*/
	class DataFrameHeader:public FrameHeader 
	{
	public:
		 //* Timestamp
		CLAM::TFloat64 mTime;
		//* Stream ID @see Stream
		CLAM::TInt32 mStreamId;
		//* Number of data matrixes in the frame. @see Matrix
		CLAM::TInt32 mnMatrices;
	public:
		/** Create a new DataFrameHeader
		* @param type Identifier specifying the frame type
		* @param time Timestamp of the frame
		* @param id Stream ID
		*/
		DataFrameHeader(const TypeId& type,CLAM::TFloat64 time=0.,CLAM::TInt32 id=0);

		static CLAM::TInt32 SizeInFile(void)
		{
			return 
				sizeof(CLAM::TFloat64)+ // mTime
				sizeof(CLAM::TInt32)+   // mStreamId
				sizeof(CLAM::TInt32);   // mnMatrices
		}
	};

	/** The header for each Matrix
	*/
	class MatrixHeader
	{
	public:
	
		//* Identifier indicating the matrix type
		TypeId mType;
		//* Identifier indicating the type of data in the matrix
		DataType mDataType;
		//* Number of rows in the matrix
		CLAM::TInt32 mnRows;
		//* Number of columns in the matrix
		CLAM::TInt32 mnColumns;

	public:
		/** Create a new MatrixHeader
		* @param type Identifier specifying the matrix type
		* @param dataType Identifier specifying the type of data type in the matrix
		* @param nRows Number of rows in the matrix
		* @param nColumns Number of rows in the matrix
		*/
		
		static CLAM::TInt32 SizeInFile(void)
		{
			return 
				sizeof(TypeId)+
				sizeof(CLAM::TInt32)+ // mDataType
				sizeof(CLAM::TInt32)+ // mnRows
				sizeof(CLAM::TInt32); // mnColumns
		}
		
		MatrixHeader()
		{
		}
		
		MatrixHeader(
			const TypeId& type,
			DataType dataType,
			CLAM::TInt32 nRows, CLAM::TInt32 nColumns);
	};

}

#endif
