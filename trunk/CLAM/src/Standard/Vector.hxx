/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* Simple Vector Class                                     
   This class provides functionality for vector operations 
   It's derivated from the Matrix class (1,N)               

   By default, it stores a buffer. 
   If we want to have a vector envelopping an array, use
   the SetVectorBuffer() function

   The vector can be a row or a column vector depending on a flag 
  
   em
*/

#ifndef _Vector_
#define _Vector_

#include "CLAM_Math.hxx"

#include "Matrix.hxx"

namespace CLAM {

template <class T> 
class Vector : public MatrixTmpl<T>
{
#define NROWS(dim,isARow) isARow?1:dim
#define NCOLUMNS(dim,isARow) isARow?dim:1


public:
	/* CONSTRUCTORS */

	// Default Constructor 
	Vector(unsigned int dimension=0, bool isARow=true)
		: 
		 MatrixTmpl<T>( NROWS(dimension,isARow), NCOLUMNS(dimension,isARow))
	{ mOwnsBuffer=true;} 

	//Copy from another Vector object
	Vector(const Vector<T>& originalVector)
		:
		MatrixTmpl<T>(originalVector)
	{ mOwnsBuffer=true;}

	/* // Create a vector from an DataArray (of T) object
	Vector(const DataArray<T>& originalArray, bool isARow=true) // The array information is copied into the vector
		:
		Matrix<T>(NROWS(originalArray.Size(),isARow),NCOLUMNS(originalArray.Size(),isARow))
	{
		MatrixBuffer() = originalArray;
		mOwnsBuffer=true;
	}
	*/

	// Create a vector from an Array (of T)  object
	Vector(const Array<T>& originalArray, bool isARow=true) 
		// The array information is copied into the vector
		:MatrixTmpl<T>(NROWS(originalArray.Size(),isARow),NCOLUMNS(originalArray.Size(),isARow))
	{
		Array<T> aux(originalArray);
		MatrixBuffer() = aux;
		mOwnsBuffer=true;
	}

	// Create a vector from an array of T
	Vector(const T *array, int dimension, bool isARow=true)
		:
		MatrixTmpl<T>(NROWS(dimension,isARow), NCOLUMNS(dimension, isARow)) 
	{
		memcpy(MatrixBuffer().Buffer(), array, sizeof(T)*dimension);
		MatrixBuffer().SetSize(dimension);
		mOwnsBuffer=true;
	} 

	// Constructor from a matrix (with either one single row or one single column)
	Vector(const MatrixTmpl<T>& originalMatrix)
		:MatrixTmpl<T>(originalMatrix.GetNumRows(), originalMatrix.GetNumColumns())
	{
		if ( (originalMatrix.GetNumRows() != 1) && (originalMatrix.GetNumColumns() !=1))
			throw Err("conversion from matrix to columns not allowed");
		
		mOwnsBuffer = true;
		MatrixBuffer() = originalMatrix.MatrixBuffer();
		mNumRows = originalMatrix.GetNumRows(); // either mNumRows or mNumColumns will be 1
		mNumColumns = originalMatrix.GetNumColumns();
	}

	//Destructor
	~Vector()
	{
		if(mpMatrixBuffer && mOwnsBuffer)
			delete mpMatrixBuffer;

		// For avoid the matrix to delete the buffer
		mpMatrixBuffer=NULL;
	}

	/* Operations */
	// Get vector dimension
	int GetDimension() const { return ((mNumRows==1)?mNumColumns:mNumRows); }
	
	// See if it's a row or a column vector
	bool IsARow() const {return ((mNumRows==1)?true:false);}
	bool IsAColumn() const {return ((mNumRows==1)?false:true);}

	// Euclidean Norm
	float Module() const 
	{ 
		T sum = 0;
		for (int i=0; i<GetDimension(); i++)
			sum+= pow ( (*this)[i], (T)2);
		return sqrt(sum); 
	}
	
	//Set a single T element at position index
	void SetAt (int iPosition, T element) 
	{ 
		if (mNumRows == 1) // row vector 
			Matrix<T>::SetAt(0, iPosition, element);
		else // column vector
			Matrix<T>::SetAt(iPosition, 0, element);
	}
	
	// Assign External Buffer to vector, set the pointer to the external buffer
	void SetVectorBuffer(Array<T>& newBuffer, bool isARow = true) // a row or a column vector depending on the flag
	{
		if(mpMatrixBuffer && mOwnsBuffer)
		{
			delete mpMatrixBuffer;
			mpMatrixBuffer=NULL;
		}

		mpMatrixBuffer=&newBuffer;
		mOwnsBuffer=false;

		if (isARow) {
			mNumColumns=newBuffer.Size();
			mNumRows=1;
		}
		else { // column vector
			mNumRows=newBuffer.Size();
			mNumColumns=1; 
		}
	}

	/*
	 void SetVectorBuffer(Array<T>& newBuffer, bool isARow = true) // a row or a column vector depending on the flag
	 {
		 if(mpMatrixBuffer && mOwnsBuffer)
		 {
			 delete mpMatrixBuffer;
			 mpMatrixBuffer=NULL;
			 mOwnsBuffer=false;
		 }
		 dynamic_cast<Array<T>*>(mpMatrixBuffer)=&newBuffer;
		 if (isARow) {
			 mNumColumns=newBuffer.Size();
			 mNumRows=1;
		 }
		 else { // column vector
			 mNumRows=newBuffer.Size();
			 mNumColumns=1; 
		 }
	 }
	*/

	//  Return single T element at position index
	T GetAt (int iPosition) const 
	{ 
		if (mNumRows == 1) // row vector
			return Matrix<T>::GetAt (0, iPosition);
		else
			return Matrix<T>::GetAt (iPosition, 0);
	}

	// Transpose funcionality overloaded from the matrix
	// Get Transposed Vector
	Vector<T> GetTrans()
	{
		Vector<T> ret(GetDimension(), GetScale(), !(IsARow()));
		for (unsigned int i=0; i<mNumColumns; i++)
			for(unsigned int j=0; j<mNumRows; j++)
				ret(i,j) = (*this)(j,i);
		return ret;
	}

	// Get subvector 
	Vector<T> Subvector(TIndex begin, TSize size)
	{
		if( (size>GetDimension()) | (begin<0) | (begin+size-1 >= GetDimension()) )
			throw Err("Vector::Subvector: not the correct begin and size");
		
		Vector<T> ret(size);
		for (int i=0; i<size; i++)
			ret[i]=(*this)[begin+i];
		return ret;
	}

	// Transpose the vector
	void Trans()
	{
		Vector<T> ret(GetDimension(), GetScale(), !(IsARow()));
		for (unsigned int i=0; i<mNumColumns; i++)
			for(unsigned int j=0; j<mNumRows; j++)
				ret(i,j) = (*this)(j,i);
		(*this) = ret;
	}
	
	// Assignement operator
	const Vector<T>& operator = (const MatrixTmpl<T>& matrix)
	{
		if ( (matrix.GetNumRows() != 1) && (matrix.GetNumColumns() !=1))
			throw Err("conversion from matrix to columns not allowed");
 
		MatrixBuffer().Init();
		MatrixBuffer() = matrix.MatrixBuffer();
		mNumRows = matrix.GetNumRows(); // either mNumRows or mNumColumns will be 1
		mNumColumns = matrix.GetNumColumns();
		return *this;
	 }
	
	// Operator []  Added to matrix operators
	T& operator [] (int iPosition) const 
	{
		if ( (iPosition >= GetDimension() ) | (iPosition < 0) )
			throw Err("Index exceed vector dimension");
		if (mNumRows ==1)
			return (*this)(0, iPosition);
		else
			return (*this)(iPosition, 0);
	}
	 
	//    Find an unitary vector on the same direction
	friend Vector<T> UnitVector(Vector<T>& originalVector)
	{
		Vector<T> ret(originalVector.GetDimension());
		ret = originalVector / originalVector.Module();
		return ret;
	}
	
 // Concatenation of two vectors
	friend Vector<T> Concatenate(const Vector<T> &v1, const Vector<T>& v2)
	{
		Vector<T> ret(v1.GetDimension()+v2.GetDimension());
		int i;
		for(i=0; i<v1.GetDimension(); i++)
			ret[i] = v1[i];
		for(i=0; i<v2.GetDimension(); i++)
			ret[i+v1.GetDimension()] = v2[i];

		return ret;
	}

	// Concatenation of a vector and an element
	friend Vector<T> Concatenate(const Vector<T> &v, T element)
	{
		Vector<T> ret(v.GetDimension()+1);
		int i;
		for(i=0; i<v.GetDimension(); i++)
			ret[i] = v[i];
		ret[v.GetDimension()] = element;

		return ret;
	}

	 // Concatenation of a vector and an element
	friend Vector<T> Concatenate(T element, const Vector<T> &v)
	{
		Vector<T> ret(v.GetDimension()+1);
		int i;
		ret[0] = element;
		for(i=1; i<(v.GetDimension()+1); i++)
			ret[i] = v[i-1];

		return ret;
	}

	
	//    Scalar Product of two vectors. The vectors can be either row or column vectors 
	friend T ScalarProduct(const Vector<T>& v1, const Vector<T>& v2) 
	{
		unsigned int i;
		T ret;
		if ( v1.GetDimension() != v2.GetDimension() ) 
			throw Err("Scalar Product of vectors of different dimension not permitted"); 
		else {
			ret = 0;
			for (i=0; i<v1.GetDimension(); i++)
				ret += v1[i]*v2[i];
			return ret;
		}
	}
	 
	//    Vector Product of two vectors (only for 3-dimensional vectors) that can be either row or column vectors
	friend Vector<T> VectorProduct(const Vector<T>& v1, const Vector<T>& v2)
	{
		if ( (v1.GetDimension() != 3) | (v2.GetDimension() != 3) )
			throw Err("Vector Product only allowed for 3-dimensional vectors");
		else
			{
				Vector<T> ret(3);
				ret[0] = v1[1]*v2[2] - v1[2]*v2[1];
				ret[1] = v1[2]*v2[0] - v1[0]*v2[2];
				ret[2] = v1[0]*v2[1] - v1[1]*v2[0]; 
				return ret;
			}
	}

	//     Triple Scalar Product of three vectors (a x b).c that can be either row or column vectors
	friend T TripleScalarProduct(const Vector<T>& v1, const Vector<T>& v2, const Vector<T>&
v3)
	{
		Vector<T> aux( VectorProduct(v1,v2)) ;
		return (aux*v3);
	}
	
	 
	//    Triple Vector Product of three vectors (a x b) x c that can be either row or column vectors
	friend Vector<T> TripleVectorProduct(const Vector<T>& v1, const Vector<T>& v2, const
Vector<T>& v3)
	{
		Vector<T> ret(3);
		ret = VectorProduct(v1, v2);
		return(VectorProduct(ret,v3));
	}
	
	// Angle Between two vectors (only for 3-dimensional vectors) (in radians). The vectors can be either row or column vectors
		friend float Angle(const Vector<T>& v1, const Vector<T>& v2)
	{
		if ((v1.GetDimension() != 3) | (v2.GetDimension() != 3))
			throw Err("Allowed only for 3-dimensional vectors");
		else
			return acos( (v1*v2)/( v1.Module()*v2.Module() ) );
	}
protected:
	
	bool mOwnsBuffer;//has buffer been created in vector or outside the class...

};

template <class T> inline EDataFormat DataFormat(Vector<T> &)
{
	return eFmtDefault;
}

template <class T> inline EDataFormat DataFormat(Vector<T>* &)
{
	return eFmtDefault;
}

}

#endif
