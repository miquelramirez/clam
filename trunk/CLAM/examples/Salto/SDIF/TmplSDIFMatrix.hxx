#ifndef __TmplSDIFMatrix__
#define __TmplSDIFMatrix__

template <class T> struct TmplSDIFMatrix:public SDIFMatrix
{
	T** mData;
	
	TmplSDIFMatrix(
			const SDIFType& type="\0\0\0\0",
			TInt32 nRows = 0, TInt32 nColumns = 0);

	~TmplSDIFMatrix();
};

#endif
