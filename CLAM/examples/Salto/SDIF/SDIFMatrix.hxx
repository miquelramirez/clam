#ifndef __SDIFMatrix__
#define __SDIFMatrix__

#include "SDIFHeader.hxx"

/** The data in a SDIF-frames are stored in 2D matrices.
* Each column corresponds to a parameter like frequency or amplitude and 
* each row represents an object like a filter, sinusoid, or noise band.
* Each SDIFMatrix starts with a SDIFMatrixHeader, that contains a
*       type id, and the dimensions of the matrix.
*
*       As SDIFMatrixes are stored in a linked list in the SDIFFrames,> *       each SDIFMatrix has a pointer to the next matrix inside the
*       frame.
*/
using namespace CLAM;

class SDIFMatrix
{
public:
	SDIFMatrixHeader mHeader;
	SDIFMatrix* mpNext;

	// Testing XA union {
		// Testing XAvoid* mpData;
		// Testing XA TFloat32* mpFloat32Data;
		Array<TFloat32> mpFloat32Data;
	// Testing XA};
	
public:
	/** Create a new SDIFMatrix.
	* @param type Identifier specifying the matrix type
	* @param dataType Identifier specifying the type of data type in the matrix
	* @param nRows Number of rows in the matrix
	* @param nColumns Number of rows in the matrix
	*/
	SDIFMatrix(
			const SDIFType& type = SDIFType::sDefault,
			ECLAMSDIFDataType dataType = eSDIFDataTypeUnknown,
			TInt32 nRows = 0, TInt32 nColumns = 0);

	/** Lookup a value in the matrix
	* @param row,col coordinates of the value
	* @return The indicated value
	*/
	TFloat32 GetValue(TInt32 row,TInt32 col)
	{
		return mpFloat32Data[row*mHeader.mnColumns + col];
	}
};

#endif
