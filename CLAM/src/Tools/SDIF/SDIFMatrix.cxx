#include "SDIFMatrix.hxx"

namespace SDIF
{

	Matrix::Matrix(const TypeId& type,DataType dataType,
		CLAM::TInt32 nRows, CLAM::TInt32 nColumns)
	:mHeader(type,dataType,nRows,nColumns)
	{
	}

}

