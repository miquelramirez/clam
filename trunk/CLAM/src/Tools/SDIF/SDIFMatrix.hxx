#ifndef __Matrix__
#define __Matrix__

#include "SDIFHeader.hxx"

#undef CLAM_USE_XML
#include "Array.hxx"

#include "SDIFType.hxx"

namespace SDIF
{
	/** The data in a Frames are stored in 2D matrices.
	* Each column corresponds to a parameter like frequency or amplitude and 
	* each row represents an object like a filter, sinusoid, or noise band.
	* Each Matrix starts with a MatrixHeader, that contains a
	*       type id, and the dimensions of the matrix.
	*
	*       As Matrixes are stored in a linked list in the Frames,> *       each Matrix has a pointer to the next matrix inside the
	*       frame.
	*/

	class Matrix
	{
	friend class File;
	protected:
		MatrixHeader mHeader;

		Matrix(const MatrixHeader& header)
		:mHeader(header)
		{
		}

		Matrix(
				const TypeId& type = TypeId::sDefault,DataType dataType = eUnknown,
				TInt32 nRows = 0, TInt32 nColumns = 0);
		virtual char* GetPtr(void) = 0;
		virtual void Resize(int nElems) = 0;
		virtual void SetSize(int nElems) = 0;
	public:
		virtual ~Matrix() { }

		TypeId Type(void) {return mHeader.mType;}
		
		TInt32 Rows(void) { return mHeader.mnRows; }
		TInt32 Columns(void) { return mHeader.mnColumns; }
		TInt32 SizeInFile(void)
		{
			TUInt32 nElems = mHeader.mnColumns*mHeader.mnRows;
			TUInt32 elemSize = mHeader.mDataType&0xFF;
			TUInt32 size = nElems*elemSize;
			TUInt32 padding = 8-size&7;
			
			return mHeader.SizeInFile()+size+padding;
		}
	};

	template <class T=TFloat32> class ConcreteMatrix:public Matrix
	{
	friend class File;
	private:
		CLAM::Array<T> mpData;
	public:
		/** Create a new Matrix.
		* @param type Identifier specifying the matrix type
		* @param dataType Identifier specifying the type of data type in the matrix
		* @param nRows Number of rows in the matrix
		* @param nColumns Number of rows in the matrix
		*/
		ConcreteMatrix(
				const TypeId& type = TypeId::sDefault,
				TInt32 nRows = 0, TInt32 nColumns = 0)
			:Matrix(type,GetType<T>::Get(),nRows,nColumns)
		{
			TInt32 nElems = Rows()*Columns();
			Resize(nElems);
			SetSize(nElems);
		}

		ConcreteMatrix(const MatrixHeader& header)
		:Matrix(header)
		{
			TInt32 nElems = Rows()*Columns();
			Resize(nElems);
			SetSize(nElems);
		}

		char* GetPtr(void) { return (char*)mpData.GetPtr(); }
		void Resize(int nElems) { mpData.Resize(nElems); }
		void SetSize(int nElems) { mpData.SetSize(nElems); }


		/** Lookup a value in the matrix
		* @param row,col coordinates of the value
		* @return The indicated value
		*/
		T GetValue(TInt32 row,TInt32 col)
		{
			return mpData[row*mHeader.mnColumns + col];
		}
		
		/** Set a value in the matrix
		* @param row,col coordinates of the value
		* @param val The new value
		*/
		void SetValue(TInt32 row,TInt32 col,const T& val)
		{
			mpData[row*mHeader.mnColumns + col] = val;
		}
	};
}

#endif
