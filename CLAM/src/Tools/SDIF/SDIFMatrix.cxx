#include "SDIFMatrix.hxx"

namespace SDIF
{

	Matrix::Matrix(const TypeId& type,DataType dataType,
		TInt32 nRows, TInt32 nColumns)
	:mHeader(type,dataType,nRows,nColumns)
	{
	}

}

