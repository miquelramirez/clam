#include "SDIFMatrix.hxx"

SDIFMatrix::SDIFMatrix(const SDIFType& type,ECLAMSDIFDataType dataType,
	TInt32 nRows, TInt32 nColumns)
:mHeader(type,dataType,nRows,nColumns)
{
	mpNext = NULL;
// Testing XA	mpData = NULL;
}

